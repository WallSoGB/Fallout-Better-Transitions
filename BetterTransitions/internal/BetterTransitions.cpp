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
#include "Bethesda/ProcessLists.hpp"

#include "Gamebryo/NiControllerManager.hpp"
#include "Gamebryo/NiControllerSequence.hpp"

namespace BetterTransitions {

#define DOOR_ANIMS 1
#define PRELOAD_CELLS 1
#define PLAYER_ANIMS 1

	bool bRequestFader = false;
	bool bLoadingCell = false;
	bool bLoadingInterior = false;
	bool bLoadingExterior = false;
	volatile bool bCellLoaded = false;

	TESObjectREFR* pCurrentDoor;

	class FakeGameSetting {
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

		FakeGameSetting() {
			memset(this, 0, sizeof(FakeGameSetting));
		}

		~FakeGameSetting() {
		}

		void*		__vtable;
		Info		uValue;
		const char* pKey;

		void Initialize(const char* apName, float value) {
			ThisCall(0x40C150, this, apName, value);
		}

		float Float() const {
			return uValue.f;
		}
	};

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
			if (pCell) [[likely]] {
				pCell->LoadAllTempData();
				pCell->QueueReferences(false);
			}
		}

		void Finish() override {
			AddToPostProcessQueue();
		}

		void PostProcess() override {
			bCellLoaded = true;
		}
	};

	NiPointer<InteriorCellLoaderTask> spLoaderTask;

	FakeGameSetting fDoorFadeToBlackFadeSeconds;

	static bool IsCellLoaded() {
		if (bLoadingInterior)
			return bCellLoaded;
		else if (bLoadingExterior)
			return ExteriorCellLoader::GetSingleton()->GetCount() == 0;

		return true;
	}

	// TODO: check if we can play anims through Bethesda's animation system, without breaking anything
	namespace Animation {
		static constexpr char cDoorFadeTextKey[] = "DoorFadeStart";
		float fDoorFadeKeyTime = 0.0f;

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
#if !DOOR_ANIMS
			return true;
#endif
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

		void __fastcall PlayDoorAnim(NiAVObject* apRoot, bool abOpen, bool abUpdateControllers = true) {
#if !DOOR_ANIMS
			return;
#endif
			if (!apRoot) [[unlikely]]
				return;

			NiControllerManager* pCtrlMgr = apRoot->GetController<NiControllerManager>();
			if (!pCtrlMgr) [[unlikely]]
				return;

			NiControllerSequence* pOpenSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pOpen);
			if (!pOpenSequence) [[unlikely]]
				return;

			NiControllerSequence* pCloseSequence = pCtrlMgr->GetSequenceByName(*FixedStrings::pClose);

			if (abOpen) {
				// If the close anim is still playing, don't even bother playing the open one
				if (pCloseSequence && pCloseSequence->m_eState != NiControllerSequence::AnimState::INACTIVE) [[unlikely]]
					return;

				FixSequenceCycle(pOpenSequence);

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
			}
			else {
				NiUpdateData kData(0.16f, abUpdateControllers);

				if (pOpenSequence->m_eState == NiControllerSequence::AnimState::ANIMATING) {
					pCtrlMgr->DeactivateSequence(pOpenSequence, 0.f);
					pOpenSequence->m_fFrequency = 1.f;

					apRoot->Update(kData);
				}

				if (!pCloseSequence) [[unlikely]]
					return;

				FixSequenceCycle(pCloseSequence);

				pCtrlMgr->SetActive(true);
				pCtrlMgr->ActivateSequence(pCloseSequence, 0, true, 1.f, 0.f, nullptr);

				apRoot->Update(kData);
			}
		}
	}

	namespace WorldState {
		bool bPlayerLocked	= false;
		bool bAIEnabled		= true;

		void Pause() {
			{
				bAIEnabled = ProcessLists::GetSingleton()->bToggleAI;

				// Do not run AI while the door is opening, can break the game
				ProcessLists::GetSingleton()->bToggleAI = false;
			}

			{
				bPlayerLocked = PlayerCharacter::GetSingleton()->bPreventActivate;

				// Prevent player from activating anything while the door is opening
				PlayerCharacter::GetSingleton()->SetPreventActivate(true);
			}
		}

		void Resume() {
			ProcessLists::GetSingleton()->bToggleAI = bAIEnabled;

			PlayerCharacter::GetSingleton()->SetPreventActivate(bPlayerLocked);
		}
	}

	class PlayerCharacterEx : public PlayerCharacter {
	public:
		// Runs on demand
		void RequestPositionPlayer(PositionRequest* apTargetLoc) {
			TESObjectREFR* pUsedDoor = static_cast<TESObjectREFR*>(apTargetLoc->pCallbackFuncArg);

			// Should not happen, but just in case
			if (pUsedDoor && pUsedDoor == pCurrentDoor)
				return;

			WorldState::Pause();

			bLoadingCell  = true;
			bRequestFader = true;

			pCurrentDoor = pUsedDoor;

			// We are going to enter a new cell, let's open the current door
			if (pCurrentDoor) [[likely]] {
				Animation::PlayDoorAnim(pCurrentDoor->DirectGet3D(), true);
			}

#if PRELOAD_CELLS
			// Load cell on an async thread
			if (apTargetLoc->pCell && apTargetLoc->pCell->IsInterior()) {
				if (!TES::GetSingleton()->IsCellLoaded(apTargetLoc->pCell, false)) {
					bLoadingInterior = true;
					spLoaderTask = new InteriorCellLoaderTask(apTargetLoc->pCell);
					IOManager::GetSingleton()->AddTask(spLoaderTask);
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
#endif

			PlayerCharacter::RequestPositionPlayer(apTargetLoc);
		}

		// Runs every frame
		bool HandlePositionPlayerRequest() {
			bool bResult = false;
			if (bLoadingCell) [[unlikely]] {
				bool bStartFaderReally = pCurrentDoor && Animation::HandleDoorAnim(pCurrentDoor->DirectGet3D());
				if (!pCurrentDoor) [[unlikely]]
					bStartFaderReally = true;

				if (bRequestFader && bStartFaderReally) {
					FaderManager::GetSingleton()->CreateFader(FADER_TYPE_ABOVE_MENU, fDoorFadeToBlackFadeSeconds.Float(), false);
					bRequestFader = false;
				}

				bool bFaderFinished = FaderManager::GetSingleton()->IsFaderVisible(FADER_TYPE_ABOVE_MENU);
				if (bFaderFinished && IsCellLoaded()) {
					ExteriorCellLoader::GetSingleton()->WaitForTasks();

					spLoaderTask = nullptr;

					TESObjectREFR* pTargetDoor = nullptr;

					// Close door we just used
					if (pCurrentDoor) [[likely]] {
						Animation::PlayDoorAnim(pCurrentDoor->DirectGet3D(), false);
					
						DoorTeleportData* pTeleport = pCurrentDoor->GetTeleport();
					
						if (pTeleport) [[likely]] {
							pTargetDoor = pTeleport->pLinkedDoor;
						}
					}

					pCurrentDoor = nullptr;

					WorldState::Resume();

					// Load the cell
					bResult = PlayerCharacter::HandlePositionPlayerRequest();

					TES::GetSingleton()->UpdateFadeNodesForAttachedCells();

					// Close the door we just teleported to
					if (pTargetDoor) [[likely]] {
						Animation::PlayDoorAnim(pTargetDoor->Get3D(), false);
					}

					bLoadingInterior = false;
					bLoadingExterior = false;
					bLoadingCell = false;
				}
				else {
#if PLAYER_ANIMS
					// Fader is ongoing, update player anims
					const float fDelta = TimeGlobal::GetSingleton()->fDelta;
					UpdateAnimation();
					b3rdPerson = !b3rdPerson;
					ThisCall(0x8D3550, this, fDelta); // Character::Update
					UpdateAnimationMovement(GetPlayerAnimation(!b3rdPerson), 0.f);
					b3rdPerson = !b3rdPerson;
					ThisCall(0x8D3550, this, fDelta); // Character::Update
					UpdateAnimationMovement(GetPlayerAnimation(!b3rdPerson), 0.f);
#endif
					bResult = true;
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

		fDoorFadeToBlackFadeSeconds.Initialize("fDoorFadeToBlackFadeSeconds", 0.2f);
	}
}
