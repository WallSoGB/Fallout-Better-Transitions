#pragma once

#include "Character.hpp"
#include "BSSimpleList.hpp"
#include "TeleportPath.hpp"
#include "BSSoundHandle.hpp"
#include "TESRegionList.hpp"
#include "Gamebryo/NiTMap.hpp"

class TESRegion;
class TESRegionSound;
class AlchemyItem;
class BGSNote;
class BGSQuestObjective;
class bhkMouseSpringAction;
class CameraCaster;
class CombatGroup;
class DialoguePackage;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;
class MagicItem;
class MagicShaderHitEffect;
class MagicTarget;
class MusicMarker;
class QuestObjectiveTargets;
class TESCaravanCard;
class TESClass;
class TESEnchantableForm;
class TESObjectCELL;
class TESObjectREFR;
class TESObjectWEAP;
class TESQuest;
class TESReputation;
class TESTopic;
class Animation;
class BipedAnim;
class NiPointLight;
class TESQuestStageItem;
class TESQuestTarget;
class MapMarkerData;

struct MAP_MARKER_STRUCT {
	MapMarkerData* pData;
	TESObjectREFR* pRef;
};

class PlayerCharacter : public Character {
public:
	PlayerCharacter();
	virtual ~PlayerCharacter();

	struct WobbleNodes
	{
		NiNode* pWobbleAnimNodes[12];
		NiNode* pWobbleAnimNodes2[12];
	};

	struct LevelManager {
		struct Data {
			bool		bShouldLevelUp;
			uint32_t	uiXPForNextLevel;
			bool		bIsChangingMaxXP;

			void Reset();
		};

		Data* pData;

		void Reset();
	};

	struct CompassTarget {
		CompassTarget() {};
		~CompassTarget() {};

		Actor*	pTarget			= nullptr;
		bool	bIsHostile		= false;
		bool	bIsUndetected	= false;
	};

	struct PositionRequest {
		TESWorldSpace*	pWorldSpace;
		TESObjectCELL*	pCell;
		NiPoint3		kPos;
		NiPoint3		kRot;
		bool			bResetWeather;
		void(__cdecl*	pfnCallbackFunc)(void*);
		void*			pCallbackFuncArg;
		TESObjectREFR*	pDestRef;
		TESObjectREFR*	pFastTravelRef;
	};

	enum ModifierGroup : uint32_t {
		MG_MAGIC	= 0,
		MG_SCRIPTS	= 1,
		MG_GAME		= 2,
	};

	enum GrabbingType : uint32_t {
		GRAB_NONE			= 0,
		GRAB_NORMAL			= 1,
		GRAB_INVENTORY_DROP = 2,
		GRAB_TELEKINESIS	= 3,
	};

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement = 1 << 0,
		kControlFlag_Look = 1 << 1,
		kControlFlag_Pipboy = 1 << 2,
		kControlFlag_Fight = 1 << 3,
		kControlFlag_POVSwitch = 1 << 4,
		kControlFlag_RolloverText = 1 << 5,
		kControlFlag_Sneak = 1 << 6,
	};

	enum KillcamMode {
		kKillcamMode_None = 0x0,
		kKillcamMode_PlayerView = 0x1,
		kKillcamMode_Cinematic = 0x2,
	};

	virtual bool						Unk_139(int, int, int, int, int, int, int);
	virtual BSSimpleList<uint32_t*>*	GetPerkRanks(bool);

	uint32_t							unk1C8;
	uint32_t							unk1CC;
	uint32_t							unk1D0;
	uint32_t							unk1D4;
	uint32_t							unk1D8;
	uint32_t							unk1DC;
	uint32_t							unk1E0;
	uint32_t							unk1E4;
	uint32_t							unk1E8;
	PositionRequest*					pPositionRequest;
	TESObjectWEAP*						pWeap1F0;
	float								fTime1F4;
	uint8_t								byte1F8;
	uint32_t							iSandmanDetectionValue;
	uint32_t							iCombatPersue;
	uint8_t								byte204;
	uint8_t								byte205;
	bool								bQuestTargetsNeedRecalculated;
	uint8_t								byte207;
	DialoguePackage*					pClosestConversation;
	bool								bIsPlayerMovingIntoNewSpace;
	BSSimpleList<ActiveEffect*>*		pActiveEffects;
	MagicItem*							pCurrentSpell;
	MagicTarget*						pDesiredTarget;
	CameraCaster*						pCameraCaster;
	uint32_t							unk220;
	uint32_t							unk224;
	uint32_t							unk228;
	float								fTime22C;
	uint32_t							unk230;
	float								time234;
	BSSimpleList<MagicItem*>*			pEatDrinkItems;
	BSSimpleList<TESEnchantableForm*>*  pQueuedEnchantments;
	bool								bShowQuestItemsInInventory;
	float								fMagicActorValueModifiers[77];
	float								fScriptActorValueModifiers[77];
	float								fHealthModifier;
	float								fGameActorValueModifiers[77];
	BSSimpleList<uint32_t>				kNotes;
	ImageSpaceModifierInstanceDOF*		pIronSightsDOFInstance;
	ImageSpaceModifierInstanceDOF*		pVATSDOFInstance;
	ImageSpaceModifierInstanceDRB*		pVATSDRBInstance;
	bool								bIsDetected;
	bool								bPreventRegionSoundUpdates;
	BSSimpleList<TESObjectREFR*>		kTeammates;
	TESObjectREFR*						pLastDoorActivated;
	bool								bIsPlayingCombatAttackSound;
	void*								pActionList;
	BSSimpleList<uint32_t>*				pCasinoDataList;
	BSSimpleList<TESCaravanCard*>*		pCaravanCards1;
	BSSimpleList<TESCaravanCard*>*		pCaravanCards2;
	uint32_t							uiCaravanCapsEarned;
	uint32_t							uiCaravanCapsLost;
	uint32_t							uiNumCaravanWins;
	uint32_t							uiNumCaravanLosses;
	uint32_t							uiLargestCaravanWin;
	uint32_t							unk630;
	bhkMouseSpringAction*				pGrabSpringAction;
	TESObjectREFR*						pGrabbedObject;
	GrabbingType						eGrabType;
	float								fCurrentGrabbedItemWeight;
	float								fGrabDistance;
	uint8_t								byte648;
	uint8_t								byte649;
	bool								b3rdPerson;
	bool								bIs3rdPerson;
	bool								bThirdPerson;
	bool								bTemp3rdPerson;
	bool								bTemp3rdPersonSwitchBack;
	bool								bIsForceFirstPerson;
	bool								byte650;
	bool								bAlwaysRun;
	bool								bAutoMove;
	bool								byte653;
	uint32_t							uiSleepHours;
	bool								bIsSleeping;
	bool								byte659;
	bool								byte65A;
	bool								byte65B;
	float								fFOV;
	float								fSecondsRunning;
	float								fSecondsSwimming;
	float								fSecondsSneaking;
	bool								bActorinSneakRange;
	uint8_t								flag66D;
	bool								bCanSleepWait;
	uint8_t								byte66F;
	float								fWorldFOV;
	float								f1stPersonFOV;
	float								fOverShoulderFOV;
	uint32_t							uiNumberTraining;
	Bitfield8							cControlFlags;
	bool								bPreventActivate;
	bool								byte682;
	bool								byte683;
	float								fPreventActivateTimer;
	TESObjectREFR*						pForceActivateRef;
	BipedAnim*							p1stPersonBipedAnim;
	Animation*							p1stPersonAnimation;
	NiNodePtr							sp1stPerson3D;
	float								fEyeHeight;
	NiNodePtr							spInventoryPC;
	Animation*							pInventoryAnimation;
	MagicShaderHitEffect*				pMagicShaderHitEffect;
	BSSimpleList<TESTopic*>				kTopics;
	BSSimpleList<TESQuestStageItem*>	kQuestLog;
	TESQuest*							pActiveQuest;
	BSSimpleList<uint32_t>				kQuestObjectiveList;
	BSSimpleList<TESQuestTarget*>		kQuestTargetList;
	bool								bPlayerGreetFlag;
	float								fPlayerGreetTimer;
	uint32_t							uiHoursToSleep;
	bool								bIsAMurderer;
	uint32_t							uiAmountStolenSold;
	float								fSortActorDistanceListTimer;
	float								fSitHeadingDelta;
	bool								bBeenAttacked;
	MagicItem*							pSelectedSpell;
	TESObjectBOOK*						pSelectedScroll;
	TESObjectREFR*						pPlacedMarker;
	TeleportPath						kPlacedMarkerTarget;
	float								fTimeGrenadeHeld;
	uint32_t							uiNumAdvance;
	uint32_t							eSkillAdvance;
	TESClass*							pDefaultClass;
	TESClass*							pClassBasedOn;
	uint32_t							uiCrimeCounts[5];
	AlchemyItem*						pPendingPoison;
	bool								bInCharGen;
	uint8_t								byte75D;
	uint8_t								bTelekinesisSelected;
	uint8_t								byte75F;
	TESRegion*							pCurrentRegion;
	TESRegionList						kRegionsList;
	BSSimpleList<TESRegionSound*>		kRegionSounds;
	BSSoundHandle						kHeartBeatSound;
	void*								pInitialStateBuffer;
	uint32_t							uiLastPlayingTimeUpdate;
	uint32_t							uiTotalPlayingTime;
	uint32_t							uiCharacterSeed;
	bool								bAiControlledToPos;
	bool								bAiControlledFromPos;
	bool								bIsDrinkingPlacedWater;
	bool								bIsPackagePlaying;
	bool								bInBorderContainedCell;
	bool								bReturnToLastKnownGoodPosition;
	NiPoint3							kLastKnownGoodPosition;
	TESForm*							pLastKnownGoodLocation;
	NiTPrimitiveArray<TESRegion>*		pBorderRegions;
	void*								pMusicType;
	uint32_t							uiGameDifficulty;
	bool								bIsHardcore;
	KillcamMode							eKillCamMode;
	bool								bBeingChased;
	bool								bIsYoung;
	bool								bIsToddler;
	bool								bCanUsePA;
	BSSimpleList<MAP_MARKER_STRUCT*>	kMapMarkers;
	TESWorldSpace*						pMapWorld;
	BSSimpleList<MusicMarker*>			kMusicMarkers;
	MusicMarker*						pCurrMusicMarker;
	float								fFlyCameraHeading;
	float								fFlyCameraPitch;
	NiPoint3							kFlyCameraPos;
	uint32_t							uiSelectedSpellCastSoundID;
	BSSoundHandle						kSelectedSpellCastSound;
	BSSoundHandle						kMagicFailureSounds[6];
	BSSimpleList<TESObjectREFR*>		kDroppedRefs;
	NiTMap<uint32_t, uint8_t>			kRandomDoorSpaces;
	NiPointer<NiPointLight>				spThirdPersonLight;
	NiPointer<NiPointLight>				spFirstPersonLight;
	bool								bInsufficientChargeMessageShown;
	float								fDropAngleMod;
	float								fLastDropAngleMod;
	LevelManager						kLevelManager;
	BSSimpleList<uint32_t>				kPerkRanksPC;
	BSSimpleList<uint32_t>				kPerkEntriesPC[74];
	BSSimpleList<uint32_t>				kPerkRanksTM;
	BSSimpleList<uint32_t>				kPerkEntriesTM[74];
	Actor*								pAutoAimActor;
	NiPoint3							kBulletAutoAim;
	NiNodePtr							spAutoAimNode;
	bool								bIsActorWithinInterfaceManagerPickDistance;
	Actor*								pReticleActor;
	BSSimpleList<CompassTarget*>*		pCompassTargets;
	float								fPipboyLightHeldTime;
	float								fAmmoSwapTimer;
	bool								bShouldOpenPipboy;
	char								byteD55;
	char								byteD56;
	char								byteD57;
	NiPoint3							kPtD58;
	CombatGroup*						pCombatGroup;
	uint32_t							uiTeammateCount;
	float								fCombatYieldTimer;
	float								fCombatYieldRetryTimer;
	WobbleNodes							kWobbleAnims;
	NiPoint3							kVectorDD4;
	NiPoint3							kCameraPos;
	bhkRigidBody*						pRigidBody;
	bool								bInCombat;
	bool								bUnseen;
	bool								bIsLODApocalypse;
	bool								byteDF3;
	BSSimpleArray<uint32_t>				kRockitLauncherContainer;
	float								fRockItLauncherWeight;
	bool								bHasNightVisionApplied;
	char								byteE09;
	char								byteE0A;
	char								byteE0B;
	TESReputation*						pModifiedReputation;
	int									unkE10;
	int									unkE14;
	float								fKillCamTimer;
	float								fKillCamCooldown;
	char								byteE20;
	char								bIsUsingTurbo;
	char								byteE22;
	char								byteE23;
	float								fLastHelloTime;
	float								fCounterAttackTimer;
	char								byteE2C;
	bool								bIsCateyeEnabled;
	bool								bIsSpottingImprovedActive;
	char								byteE2F;
	float								fItemDetectionTimer;
	NiNode*								pIronSightNode;
	bool								bNoHardcoreTracking;
	bool								bSkipHCNeedsUpdate;
	char								byteE3A;
	char								byteE3B;
	BSSimpleArray<uint32_t>				kHotkeyedWeaponAmmos;
	TESQuest*							pPatch04DebugQuest;

	static PlayerCharacter* GetSingleton();

	Animation* GetPlayerAnimation(bool abFirstPerson) const;

	void SetPreventActivate(bool abPrevent);
};

ASSERT_OFFSET(PlayerCharacter, fLastDropAngleMod, 0x874);
ASSERT_OFFSET(PlayerCharacter, fKillCamTimer, 0xE18);
ASSERT_SIZE(PlayerCharacter, 0xE50);