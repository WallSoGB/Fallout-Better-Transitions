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
#include "Bethesda/TESBoundObject.hpp"
#include "Bethesda/GameSettingCollection.hpp"
#include "Bethesda/TimeGlobal.hpp"
#include "Bethesda/ProcessLists.hpp"
#include "Bethesda/ModelLoader.hpp"

#include "Gamebryo/NiControllerManager.hpp"
#include "Gamebryo/NiControllerSequence.hpp"

#include "nvse/PluginAPI.h"

namespace BetterTransitions {

#define PRELOAD_CELLS 1
#define PLAYER_ANIMS 1

	CallDetour kOrgRequestPositionPlayer[2];
	CallDetour kOrgHandlePositionPlayer;
	CallDetour kOrgIsMenuMode;

	namespace Settings {
		class CustomGameSetting {
		public:
			union Info {
				const char* str;
				int				i;
				unsigned int	u;
				float			f;
				bool			b;
				char			c;
				char			h;
			};

			CustomGameSetting() {
				memset(this, 0, sizeof(CustomGameSetting));
			}

			~CustomGameSetting() {
			}

			void* __vtable;
			Info		uValue;
			const char* pKey;

			void Initialize(const char* apName, float afValue) {
				ThisCall(0x40E0B0, this, apName, afValue);
			}

			float Float() const {
				return uValue.f;
			}
		};

		bool bAnimateDoors = true;
		CustomGameSetting fDoorFadeToBlackFadeSeconds;
		CustomGameSetting fFastTravelFadeToBlackFadeSeconds;
	}

	// TODO: check if we can play anims through Bethesda's animation system, without breaking anything
	namespace Animation {
		static constexpr char cDoorFadeTextKey[] = "DoorFadeStart";
		float fDoorFadeKeyTime = 0.0f;
		NiControllerSequence* pCurrentOpenSequence = nullptr;

		void __fastcall FixSequenceCycle(NiControllerSequence* apSequence) {
			// If this happens, the sequence is broken and should be fixed by a meshoid...
			// TODO: print a message
			if (apSequence->m_eCycleType == NiTimeController::CycleType::LOOP) [[unlikely]] {
				apSequence->m_eCycleType = NiTimeController::CycleType::CLAMP;
			}
		}

		bool __fastcall HasDoorKeyframes(NiControllerSequence* apSequence) {
			fDoorFadeKeyTime = 0.f;

			if (!apSequence || !apSequence->m_spTextKeys || !apSequence->m_spTextKeys->m_pKeys)
				return false;

			const uint32_t uiCount = apSequence->m_spTextKeys->m_uiNumKeys;
			if (!uiCount)
				return false;

			for (uint32_t i = 0; i < uiCount; ++i) {
				const NiTextKey& rKey = apSequence->m_spTextKeys->m_pKeys[i];
				if (!rKey.m_kText)
					continue;

				if (!_strnicmp(cDoorFadeTextKey, rKey.m_kText, sizeof(cDoorFadeTextKey))) {
					fDoorFadeKeyTime = rKey.m_fTime;
					return true;
				}
			}
			return false;
		}

		bool __fastcall HandleDoorKeyframes(NiControllerSequence* apSequence) {
			const float fStartTime = apSequence->m_fStartTime;
			const float fLastScaledTime = apSequence->m_fLastScaledTime;

			if (fStartTime == -FLT_MAX || fLastScaledTime == -FLT_MAX || fLastScaledTime == fStartTime)
				return false;

			if (fStartTime > fLastScaledTime && (fStartTime < fDoorFadeKeyTime || fLastScaledTime >= fDoorFadeKeyTime))
				return true;
			else if (fStartTime <= fDoorFadeKeyTime && fLastScaledTime > fDoorFadeKeyTime)
				return true;

			return false;
		}

		// Handles animation's length and state
		// Return true if fader should activate
		bool __fastcall HandleDoorAnim(NiAVObject* apRoot) {
			if (!Settings::bAnimateDoors)
				return true;

			if (!apRoot) [[unlikely]]
				return true;

			NiControllerManager* pCtrlMgr = apRoot->GetController<NiControllerManager>();
			if (!pCtrlMgr) [[unlikely]]
				return true;

			NiControllerSequence* pCloseSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pClose);
			if (pCloseSequence && pCloseSequence->m_eState != NiControllerSequence::AnimState::INACTIVE) [[unlikely]] {
				return true;
			}

			NiControllerSequence* pOpenSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pOpen);
			if (pOpenSequence && pOpenSequence->m_eState != NiControllerSequence::AnimState::INACTIVE) [[likely]] {
				if (fDoorFadeKeyTime > 0.f) [[unlikely]] {
					return HandleDoorKeyframes(pOpenSequence);
				}
				else [[likely]] {
					float fProgress = pOpenSequence->m_fLastScaledTime / pOpenSequence->m_fEndKeyTime;

					// Stop the anim before it hits a wall, or shows void. Hopefully.
					if (fProgress >= pOpenSequence->m_fEndKeyTime * 0.05f) {
						pCtrlMgr->DeactivateSequence(pOpenSequence, 0.f);
						NiUpdateData kData(0.16f, true);
						pCtrlMgr->Update(kData);
						pOpenSequence->m_fFrequency = 1.f;
					}

					return fProgress >= pOpenSequence->m_fEndKeyTime * 0.025f;
				}
			}

			return true;
		}

		void __fastcall PlayDoorAnim(TESObjectREFR* apRef, bool abOpen) {
			if (!Settings::bAnimateDoors)
				return;

			if (!apRef) [[unlikely]]
				return;

			if (apRef->GetDeleted() || apRef->GetDisabled())
				return;

			if (!apRef->GetInitialized() || apRef->IsStillLoading())
				return;

			NiAVObject* pRoot = apRef->DirectGet3D();
			if (!pRoot) [[unlikely]]
				return;

			NiControllerManager* pCtrlMgr = pRoot->GetController<NiControllerManager>();
			if (!pCtrlMgr) [[unlikely]]
				return;

			NiControllerSequence* pOpenSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pOpen);
			if (!pOpenSequence) [[unlikely]]
				return;

			NiControllerSequence* pCloseSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pClose);

			FixSequenceCycle(pOpenSequence);
			FixSequenceCycle(pCloseSequence);

			if (abOpen) {
				// If the close anim is still playing, don't even bother playing the open one
				if (pCloseSequence && pCloseSequence->m_eState != NiControllerSequence::AnimState::INACTIVE) [[unlikely]]
					return;

				pCtrlMgr->SetActive(true);
				pCtrlMgr->ActivateSequence(pOpenSequence, 0, true, 1.f, 0.f, nullptr);

				// Highly advanced logic to determine the speed of the animation
				// Long animations usually actually open the door far later, so we don't want to run them in slow motion
				if (pOpenSequence->m_fEndKeyTime >= 2.f) [[unlikely]] {
					pOpenSequence->m_fFrequency = 1.f;
				}
				else [[likely]] {
					pOpenSequence->m_fFrequency = 0.25f;
				}

				HasDoorKeyframes(pOpenSequence);

				pCurrentOpenSequence = pOpenSequence;
			}
			else {
				pCurrentOpenSequence = nullptr;

				pOpenSequence->m_fFrequency = 1.f;
				pCtrlMgr->SetActive(true);
				pCtrlMgr->DeactivateSequence(pCloseSequence, 0.f);
				pCtrlMgr->ActivateSequence(pOpenSequence, 0, false, 1.f, 0.f, nullptr);
				pOpenSequence->m_fOffset = -FLT_MAX;
				NiUpdateData kUpdateData(pOpenSequence->m_fBeginKeyTime, true, false);
				pRoot->Update(kUpdateData);
				pCtrlMgr->DeactivateSequence(pOpenSequence, 0.f);
				CdeclCall(0xC6BD00, pRoot, true);
			}
		}

		class TextKeyStealer {
		private:
			uint32_t				uiTextKeyCount = 0;
			NiControllerSequence*	pSequence = nullptr;
		public:
			TextKeyStealer(NiControllerSequence* apSequence) {
				if (apSequence == pCurrentOpenSequence) {
					pSequence = apSequence;
					if (apSequence->m_spTextKeys)
						uiTextKeyCount = apSequence->m_spTextKeys->m_uiNumKeys;
				}
				else {
					pSequence = nullptr;
				}
			}
			~TextKeyStealer() {
				if (pSequence && pSequence->m_spTextKeys)
					pSequence->m_spTextKeys->m_uiNumKeys = uiTextKeyCount;
			}
		};
	}

	namespace Tasks {
		class InteriorCellLoaderTask : public IOTask {
		public:
			InteriorCellLoaderTask(TESObjectCELL* apCell) : IOTask(IO_TASK_PRIORITY_CRITICAL) {
				if (apCell && apCell->IsInterior()) {
					pCell = apCell;
				}
			}
			~InteriorCellLoaderTask() {};

			TESObjectCELL*			pCell = nullptr;
			NiPointer<QueuedFile>	spFiles;
			bool					bLoaded = false;

			void Run() override {
				if (pCell && pCell->LoadAllTempData()) [[likely]] {
					bLoaded = true;
					spFiles = BSMemory::create<QueuedFile, 0xC3C590>(IO_TASK_PRIORITY_VERY_HIGH);
					QueueModels();
				}
				else {
					bLoaded = true;
				}
			}

			void Cancel(BS_TASK_STATE aeState, void* apParent) override {
				if (spFiles)
					spFiles->Cancel(aeState, this);
			}

			void Finish() override {
				if (AreModelsLoaded() && spFiles)
					spFiles->CheckFinished();
			}

			bool GetDescription(char* apDescription, size_t aiBufferSize) override {
				sprintf_s(apDescription, aiBufferSize, "ExteriorCellLoaderTask for cell %s", pCell ? pCell->GetFormEditorID() : "None");
				return true;
			}

			bool AreModelsLoaded() const {
				return spFiles ? spFiles->GetAllChildrenFinished() : bLoaded;
			}

		private:
			void QueueModels() {
				auto pIter = pCell->kReferences.GetHead();
				while (pIter && !pIter->IsEmpty()) {
					TESObjectREFR* pRef = pIter->GetItem();
					pIter = pIter->GetNext();
					if (pRef && !pRef->Get3D()) {
						TESBoundObject* pBase = pRef->GetObjectReference();
						// Need to double-check the model here, because Bethesda doesn't, and crashes like a dumb bitch
						if (pBase && ModelLoader::GetSingleton()->GetModelForBoundObject(pBase, pRef))
							ModelLoader::GetSingleton()->QueueBoundObject(pBase, IO_TASK_PRIORITY_HIGH, spFiles, pRef);
					}
				}
			}
		};

		NiPointer<InteriorCellLoaderTask> spLoaderTask;

		enum SpaceType {
			NONE	 = 0,
			INTERIOR = 1,
			EXTERIOR = 2,
		};

		SpaceType Start(PlayerCharacter::PositionRequest* apTargetLoc) {
#if PRELOAD_CELLS
			// Load cell on an async thread
			if (apTargetLoc->pCell && apTargetLoc->pCell->IsInterior()) {
				if (!TES::GetSingleton()->IsCellLoaded(apTargetLoc->pCell, false)) {
					Tasks::spLoaderTask = new Tasks::InteriorCellLoaderTask(apTargetLoc->pCell);
					IOManager::GetSingleton()->AddTask(Tasks::spLoaderTask);
					return INTERIOR;
				}
			}
			else if (apTargetLoc->pFastTravelRef) {
				ExteriorCellLoader* pLoader = ExteriorCellLoader::GetSingleton();
				const uint32_t uiQueuedCellsOrg = pLoader->QueuedCellCount();
				TESObjectREFR* pRef = apTargetLoc->pFastTravelRef->GetLinkedRef();
				if (!pRef)
					pRef = apTargetLoc->pFastTravelRef;

				TESWorldSpace* pWorldSpace = pRef->GetWorldSpace();
				const NiPoint3 kRefPos = pRef->GetPos();

				if (pWorldSpace) {
					const uint32_t uiGridSize = *reinterpret_cast<uint32_t*>(0x11C63CC + 4);
					const uint32_t uiGridHalfSize = uiGridSize >> 1;
					TESObjectCELL* pCell = pWorldSpace->GetCellAtPos(kRefPos);
					if (!TES::GetSingleton()->IsCellLoaded(pCell, false)) {
						int32_t iCellX = int32_t(kRefPos.x) >> 12;
						int32_t iCellY = int32_t(kRefPos.y) >> 12;
						for (int32_t iX = iCellX - uiGridHalfSize; iX < iCellX + uiGridHalfSize; iX++) {
							for (int32_t iY = iCellY - uiGridHalfSize; iY < iCellY + uiGridHalfSize; iY++) {
								pCell = pWorldSpace->GetCellFromCellCoord(iX, iY);
								if (!TES::GetSingleton()->IsCellLoaded(pCell, false))
									ExteriorCellLoader::GetSingleton()->QueueCellLoad(pWorldSpace, iX, iY);
							}
						}
					}

					if (uiQueuedCellsOrg != pLoader->QueuedCellCount())
						return EXTERIOR;
				}
			}
#endif
			return NONE;
		}

		void Finish() {
			if (spLoaderTask) {
				if (IOManager::GetSingleton()->TryCancelTask(spLoaderTask, nullptr))
					IOManager::GetSingleton()->WaitForTask(spLoaderTask);
				spLoaderTask = nullptr;
			}
			else {
				ExteriorCellLoader::GetSingleton()->PostProcessCompletedTasks();
				if (ExteriorCellLoader::GetSingleton()->TryCancelTasks())
					ExteriorCellLoader::GetSingleton()->WaitForTasks();
			}
		}
	}

	namespace WorldState {
		struct _State {
			bool bPlayerLocked	   : 1 = false;
			bool bAIEnabled		   : 1 = true;
			bool bCollisionEnabled : 1 = true;
		};
		_State kState;

		void Pause() {
			{
				kState.bAIEnabled = ProcessLists::GetSingleton()->bToggleAI;

				// Do not run AI while the door is opening, can break the game
				ProcessLists::GetSingleton()->bToggleAI = false;
			}

			{
				kState.bPlayerLocked = PlayerCharacter::GetSingleton()->bPreventActivate;

				// Prevent player from activating anything while the door is opening
				PlayerCharacter::GetSingleton()->SetPreventActivate(true);
			}
		}

		void Resume() {
			ProcessLists::GetSingleton()->bToggleAI = kState.bAIEnabled;

			PlayerCharacter::GetSingleton()->SetPreventActivate(kState.bPlayerLocked);
		}
	}

	namespace CellLoadState {
		struct _State {
			TESObjectREFR*	pCurrentDoor;
			bool			bRequestFader	 : 1 = false;
			bool			bLoadingCell	 : 1 = false;
			bool			bIsFastTravel	 : 1 = false;
			bool			bLoadingInterior : 1 = false;
			bool			bLoadingExterior : 1 = false;
		};

		_State kState;

		bool __fastcall IsCellLoaded(bool abFaderFinished) {
			if (kState.bLoadingInterior) {
				if (Tasks::spLoaderTask) {
					if (Tasks::spLoaderTask->bLoaded && abFaderFinished)
						return true;

					return Tasks::spLoaderTask->eState == BS_TASK_STATE_COMPLETED;
				}
			}
			else if (kState.bLoadingExterior) {
				if (abFaderFinished)
					return true;

				if (ExteriorCellLoader::GetSingleton()->GetCount())
					return false;
			}

			return true;
		}

		void __fastcall Start(TESObjectREFR* apDoor, PlayerCharacter::PositionRequest* apTargetLoc, bool abFastTravel) {
			WorldState::Pause();

			kState.bLoadingCell = true;
			kState.bRequestFader = true;
			kState.bIsFastTravel = abFastTravel;
			kState.pCurrentDoor = nullptr;

			Animation::pCurrentOpenSequence = nullptr;

			if (apDoor && apDoor->IsReference() && apDoor->GetObjectReference()) {
				TESBoundObject* pBase = apDoor->GetObjectReference();
				if (pBase->GetFormType() == FORM_TYPE::TESObjectDOOR)
					kState.pCurrentDoor = apDoor;
			}

			if (kState.pCurrentDoor) [[likely]] {
				// We are going to enter a new cell, let's open the current door
				Animation::PlayDoorAnim(kState.pCurrentDoor, true);
			}

			auto eSpace = Tasks::Start(apTargetLoc);
			switch (eSpace) {
				case Tasks::INTERIOR:
					kState.bLoadingInterior = true;
					break;
				case Tasks::EXTERIOR:
					kState.bLoadingExterior = true;
					break;
				case Tasks::NONE:
					break;
				default:
					__assume(0);
					break;
			}
		}

		bool __fastcall Finish(PlayerCharacter* apPlayer) {
			Tasks::Finish();
			
			FaderManager::GetSingleton()->RemoveFader(FADER_TYPE_ABOVE_MENU, false);

			TESObjectREFR* pTargetDoor = nullptr;

			// Close door we just used
			if (kState.pCurrentDoor) [[likely]] {
				Animation::PlayDoorAnim(kState.pCurrentDoor, false);

				DoorTeleportData* pTeleport = kState.pCurrentDoor->GetTeleport();

				if (pTeleport)
					pTargetDoor = pTeleport->pLinkedDoor;
			}

			kState.pCurrentDoor = nullptr;

			WorldState::Resume();

			// Load the cell
			bool bResult = ThisCall<bool>(kOrgHandlePositionPlayer.GetOverwrittenAddr(), apPlayer);

			// Close the door we just teleported to
			if (pTargetDoor) [[likely]] {
				Animation::PlayDoorAnim(pTargetDoor, false);
			}

			TES::GetSingleton()->UpdateFadeNodesForAttachedCells();

			kState.bLoadingInterior = false;
			kState.bLoadingExterior = false;
			kState.bLoadingCell = false;

			return bResult;
		}

		bool __fastcall Update(PlayerCharacter* apPlayer) {
			if (kState.bLoadingCell) {
				bool bStartFaderReally = kState.pCurrentDoor && Animation::HandleDoorAnim(kState.pCurrentDoor->DirectGet3D());
				if (!kState.pCurrentDoor) [[unlikely]]
					bStartFaderReally = true;

				if (kState.bRequestFader && bStartFaderReally) {
					float fFadeSeconds = kState.bIsFastTravel ? Settings::fFastTravelFadeToBlackFadeSeconds.Float() : Settings::fDoorFadeToBlackFadeSeconds.Float();
					FaderManager::GetSingleton()->CreateFader(FADER_TYPE_ABOVE_MENU, fFadeSeconds, false);
					kState.bRequestFader = false;
				}

				const bool bFaderFinished = FaderManager::GetSingleton()->IsFaderVisible(FADER_TYPE_ABOVE_MENU);
				if (IsCellLoaded(bFaderFinished) && bFaderFinished) {
					return Finish(apPlayer);
				}
				else {
#if PLAYER_ANIMS
					// Fader is ongoing, update player anims
					const float fDelta = TimeGlobal::GetSingleton()->fDelta;
					apPlayer->UpdateAnimation();
					apPlayer->b3rdPerson = !apPlayer->b3rdPerson;
					ThisCall(0x8D3550, apPlayer, fDelta); // Character::Update
					apPlayer->UpdateAnimationMovement(apPlayer->GetPlayerAnimation(!apPlayer->b3rdPerson), 0.f);
					apPlayer->b3rdPerson = !apPlayer->b3rdPerson;
					ThisCall(0x8D3550, apPlayer, fDelta); // Character::Update
					apPlayer->UpdateAnimationMovement(apPlayer->GetPlayerAnimation(!apPlayer->b3rdPerson), 0.f);
#endif
					return true;
				}
			}
			else {
				return ThisCall<bool>(kOrgHandlePositionPlayer.GetOverwrittenAddr(), apPlayer);
			}
		}
	}

	class Hook {
	public:
		// Runs on demand
		template<uint32_t INDEX>
		void RequestPositionPlayer(PlayerCharacter::PositionRequest* apTargetLoc) {
			TESObjectREFR* pUsedDoor = static_cast<TESObjectREFR*>(apTargetLoc->pCallbackFuncArg);

			// Should not happen, but just in case
			if (pUsedDoor && pUsedDoor == CellLoadState::kState.pCurrentDoor)
				return;

			CellLoadState::Start(pUsedDoor, apTargetLoc, INDEX == 0);

			ThisCall(kOrgRequestPositionPlayer[INDEX].GetOverwrittenAddr(), this, apTargetLoc);
		}

		// Runs every frame
		bool HandlePositionPlayerRequest() {
			return CellLoadState::Update(reinterpret_cast<PlayerCharacter*>(this));
		}

		static bool CantBreathe() {
			if (CellLoadState::kState.bLoadingCell)
				return true;

			return CdeclCall<bool>(kOrgIsMenuMode.GetOverwrittenAddr());
		}
	};

	float GetFloatSetting(const char* apSettingName, float afDefaultValue, const char* apFilename) {
		char cFloatBuffer[256];
		sprintf_s(cFloatBuffer, "%f", afDefaultValue);
		if (GetPrivateProfileString("Main", apSettingName, "0.0", cFloatBuffer, sizeof(cFloatBuffer), apFilename)) {
			float fValue = 0.f;
			if (sscanf_s(cFloatBuffer, "%f", &fValue))
				return fValue;
		}

		return afDefaultValue;
	}

	void InitSettings() {
		char cFilename[MAX_PATH];
		GetModuleFileNameA(nullptr, cFilename, MAX_PATH);
		char* pLastSlash = strrchr(cFilename, '\\') + 1;
		uint32_t uiLength = MAX_PATH - (pLastSlash - cFilename);
		strcpy_s(pLastSlash, uiLength, "Data\\nvse\\plugins\\BetterTransitions.ini");
		
		Settings::bAnimateDoors = GetPrivateProfileInt("Main", "bAnimateDoors", 1, cFilename);
		Settings::fDoorFadeToBlackFadeSeconds.Initialize("fDoorFadeToBlackFadeSeconds", GetFloatSetting("fDoorFadeToBlackFadeSeconds", 0.2f, cFilename));
		Settings::fFastTravelFadeToBlackFadeSeconds.Initialize("fFastTravelFadeToBlackFadeSeconds", GetFloatSetting("fFastTravelFadeToBlackFadeSeconds", 0.5f, cFilename));
	}

	void InitHooks() {
		kOrgRequestPositionPlayer[0].ReplaceCallEx(0x798A9D, &Hook::RequestPositionPlayer<0>);
		kOrgRequestPositionPlayer[1].ReplaceCallEx(0x518CA7, &Hook::RequestPositionPlayer<1>);
		kOrgHandlePositionPlayer.ReplaceCallEx(0x86F94F, &Hook::HandlePositionPlayerRequest);
	}

	bool bHooked = false;
	void MessageHandler(NVSEMessagingInterface::Message* apMsg) {
		if (bHooked) [[likely]]
			return;

		switch (apMsg->type) {
			case NVSEMessagingInterface::kMessage_MainGameLoop:
				kOrgIsMenuMode.ReplaceCall(0x8898BD, Hook::CantBreathe);
				bHooked = true;
				break;
		}
	}

	void InitMessageHandler(NVSEInterface* apNVSE) {
		uint32_t uiPluginHandle = apNVSE->GetPluginHandle();
		NVSEMessagingInterface* pMessaging = static_cast<NVSEMessagingInterface*>(apNVSE->QueryInterface(kInterface_Messaging));
		pMessaging->RegisterListener(uiPluginHandle, "NVSE", MessageHandler);
	}
}
