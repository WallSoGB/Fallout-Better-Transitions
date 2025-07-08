#include "BetterTransitions.hpp"

#include "Bethesda/ExteriorCellLoader.hpp"
#include "Bethesda/FaderManager.hpp"
#include "Bethesda/FixedStrings.hpp"
#include "Bethesda/IOManager.hpp"
#include "Bethesda/IOTask.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/TES.hpp"
#include "Bethesda/TESObjectCELL.hpp"
#include "Bethesda/TESWorldSpace.hpp"
#include "Bethesda/GameSettingCollection.hpp"
#include "Bethesda/TimeGlobal.hpp"

#include "Gamebryo/NiControllerManager.hpp"
#include "Gamebryo/NiControllerSequence.hpp"

namespace BetterTransitions {

	bool bRequestFader = false;
	bool bLoadingCell = false;
	bool bLoadingInterior = false;
	bool bLoadingExterior = false;
	volatile bool bCellLoaded = false;

	TESObjectREFR* pCurrentDoor;

	class InteriorCellLoaderTask : public IOTask {
	public:
		InteriorCellLoaderTask(TESObjectCELL* apCell) : IOTask(IO_TASK_PRIORITY_CRITICAL) {
			bCellLoaded = false;
			if (apCell && apCell->IsInterior())
				pCell = apCell;
		}
		~InteriorCellLoaderTask() {};

		TESObjectCELL* pCell = nullptr;

		void Run() override {
			if (pCell)
				pCell->LoadAllTempData();
		}

		void Finish() override {
			AddToPostProcessQueue();
		}

		void PostProcess() override {
			bCellLoaded = true;
		}
	};

	NiPointer<InteriorCellLoaderTask> spLoaderTask;

	static bool IsCellLoaded() {
		if (bLoadingInterior)
			return bCellLoaded;
		else if (bLoadingExterior)
			return ExteriorCellLoader::GetSingleton()->GetCount() == 0;

		return true;
	}

	// Start a bit delayed - that way the fader will start during anim
	bool HandleDoorAnim(NiAVObject* apRoot, bool& abStopped) {
		if (!apRoot)
			return true;

		NiControllerManager* pCtrlMgr = apRoot->GetController<NiControllerManager>();
		if (!pCtrlMgr)
			return true;

		NiControllerSequence* pSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pOpen);
		if (!pSequence)
			return true;

		float fProgress = pSequence->m_fLastScaledTime / pSequence->m_fEndKeyTime;
		if (fProgress >= 0.05f) {
			pCtrlMgr->DeactivateSequence(pSequence, 0.f);
			abStopped = true;
		}

		return fProgress >= 0.02f;
	}

	void PlayDoorAnim(NiAVObject* apRoot, bool abOpen) {
		if (!apRoot)
			return;

		NiControllerManager* pCtrlMgr = apRoot->GetController<NiControllerManager>();
		if (!pCtrlMgr)
			return;

		NiControllerSequence* pOpenSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pOpen);
		if (!pOpenSequence)
			return;


		if (abOpen) {
			pCtrlMgr->SetActive(true);
			pCtrlMgr->ActivateSequence(pOpenSequence, 0, 0, 1.f, 0.f, nullptr);
			pOpenSequence->m_fOffset = -FLT_MAX;
			pOpenSequence->m_fFrequency = 0.25f;
		}
		else {
			pCtrlMgr->DeactivateSequence(pOpenSequence, 0.f);
			pCtrlMgr->SetActive(false);
			pOpenSequence->m_fFrequency = 1.f;
			NiUpdateData kData(0.16f);
			apRoot->Update(kData);

			NiControllerSequence* pCloseSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pClose);
			if (!pCloseSequence)
				return;

			pCtrlMgr->SetActive(true);
			pCtrlMgr->ActivateSequence(pCloseSequence, 0, 0, 1.f, 0.f, nullptr);
			pCloseSequence->m_fOffset = -FLT_MAX;

			pCloseSequence->m_fFrequency = 100.f;
			apRoot->Update(kData);
		}
	}

	class PlayerCharacterEx : public PlayerCharacter {
	public:
		void RequestPositionPlayer(PositionRequest* apTargetLoc) {
			bLoadingCell = true;
			bRequestFader = true;

			pCurrentDoor = static_cast<TESObjectREFR*>(apTargetLoc->pCallbackFuncArg);

			// We are going to enter a new cell, let's open the current door
			if (pCurrentDoor)
				PlayDoorAnim(pCurrentDoor->DirectGet3D(), true);

			// Load cell on an async thread
			if (apTargetLoc->pCell && apTargetLoc->pCell->IsInterior()) {
				spLoaderTask = new InteriorCellLoaderTask(apTargetLoc->pCell);
				IOManager::GetSingleton()->AddTask(spLoaderTask);
			}
			else {
				ExteriorCellLoader* pLoader = ExteriorCellLoader::GetSingleton();
				uint32_t uiQueuedCellsOrg = pLoader->QueuedCellCount();
				TESWorldSpace* pWorldSpace = nullptr;
				NiPoint3 kRefPos;

				if (apTargetLoc->pFastTravelRef) {
					TESObjectREFR* pRef = apTargetLoc->pFastTravelRef->GetLinkedRef();
					if (!pRef)
						pRef = apTargetLoc->pFastTravelRef;

					pWorldSpace = pRef->GetWorldSpace();
					kRefPos = pRef->GetPos();
				}

				if (pWorldSpace) {
					TESObjectCELL* pCell = pWorldSpace->GetCellAtPos(kRefPos);
					if (!pCell || !TES::GetSingleton()->IsCellLoaded(pCell, false)) {
						int32_t iCellX = int32_t(kRefPos.x) >> 12;
						int32_t iCellY = int32_t(kRefPos.y) >> 12;
						for (int32_t iX = iCellX - 2; iX < iCellX + 2; iX++) {
							for (int32_t iY = iCellY - 2; iY < iCellY + 2; iY++) {
								ExteriorCellLoader::GetSingleton()->QueueCellLoad(pWorldSpace, iX, iY);
							}
						}
					}

					bLoadingExterior = uiQueuedCellsOrg != pLoader->QueuedCellCount();
				}
			}

			PlayerCharacter::RequestPositionPlayer(apTargetLoc);
		}

		bool HandlePositionPlayerRequest() {
			bool bResult = false;
			if (bLoadingCell) {
				bool bStopped = false;
				bool bStartFaderReally = pCurrentDoor && HandleDoorAnim(pCurrentDoor->DirectGet3D(), bStopped);
				if (!pCurrentDoor)
					bStartFaderReally = true;
				if (bRequestFader && bStartFaderReally) {
					FaderManager::GetSingleton()->CreateFader(FADER_TYPE_ABOVE_MENU, GameSettingCollection::fFadeToBlackFadeSeconds->Float(), false);
					bRequestFader = false;
				}
				bResult = FaderManager::GetSingleton()->IsFaderVisible(FADER_TYPE_ABOVE_MENU);
				if (bResult && IsCellLoaded()) {
					ExteriorCellLoader::GetSingleton()->PostProcessCompletedTasks();
					ExteriorCellLoader::GetSingleton()->WaitForTasks();

					spLoaderTask = nullptr;
					// Fader has finished

					// Close door
					if (pCurrentDoor)
						PlayDoorAnim(pCurrentDoor->DirectGet3D(), false);

					pCurrentDoor = nullptr;

					// Load the cell
					bResult = PlayerCharacter::HandlePositionPlayerRequest();

					TES::GetSingleton()->UpdateFadeNodesForAttachedCells();

					bLoadingExterior = false;
					bLoadingCell = false;
				}
				else {
					// Fader is ongoing, update player anims
					float fDelta = TimeGlobal::GetSingleton()->fDelta;
					UpdateAnimation();
					b3rdPerson = !b3rdPerson;
					ThisCall(0x8D3550, this, fDelta); // Character::Update
					UpdateAnimationMovement(GetPlayerAnimation(!b3rdPerson), 0.f);
					b3rdPerson = !b3rdPerson;
					ThisCall(0x8D3550, this, fDelta); // Character::Update
					UpdateAnimationMovement(GetPlayerAnimation(!b3rdPerson), 0.f);
					bResult = false;
				}
			}
			else {
				bResult = PlayerCharacter::HandlePositionPlayerRequest();
			}

			return bResult;
		}
	};

	void InitHooks() {
		ReplaceCallEx(0x798A9D, &PlayerCharacterEx::RequestPositionPlayer);
		ReplaceCallEx(0x518CA7, &PlayerCharacterEx::RequestPositionPlayer);
		ReplaceCallEx(0x86F94F, &PlayerCharacterEx::HandlePositionPlayerRequest);
	}
}