#pragma once

#include "ActorsScriptTaskData.hpp"
#include "ActorUpdateTaskData.hpp"
#include "AnimationTaskData.hpp"
#include "BSSemaphore.hpp"
#include "BSSimpleList.hpp"
#include "BSTStaticFreeList.hpp"
#include "DetectionTaskData.hpp"
#include "Gamebryo/NiSmartPointer.hpp"
#include "LipTask.hpp"
#include "LockFreeMap.hpp"
#include "MobileObjectMessage.hpp"
#include "MovementTaskData.hpp"
#include "PackageUpdateTaskData.hpp"
#include "ProcessArray.hpp"

class MobileObject;
class BSTempEffect;
class MagicHitEffect;
class MuzzleFlash;
class Crime;
class Actor;

class ProcessLists {
public:
	struct TaskData {
		DetectionTaskData							kDetectionTaskData;
		AnimationTaskData							kAnimationTaskData;
		PackageUpdateTaskData						kPackageUpdateTaskData;
		ActorUpdateTaskData							kActorUpdateTaskData;
		ActorsScriptTaskData						kActorsScriptTaskData;
		MovementTaskData							kMovementTaskData;
		BSTStaticFreeList<MobileObjectMessage,4096> kMobileObjectMessageList;
		bool										byte10180;
	};

	float											fTime000;
	ProcessArray									kAllProcessArrays;
	BSSimpleList<Crime*>*							pCrimes[5];
	BSSimpleList<NiPointer<BSTempEffect>>				kTempEffects;
	BSSimpleList<NiPointer<MagicHitEffect>>			kHitEffects;
	BSSimpleList<MuzzleFlash*>						kMuzzleFlashList;
	BSSimpleList<void*>								kArrowProjectileList;
	BSSimpleList<MobileObject*>						kTempChangeList;
	BSSimpleList<Actor*>							kAliveActors;
	Actor*											pNearbyActors[50];
	uint32_t										uiNearbyActorCount;
	float											fCommentOnPlayerActionsTimer;
	float											fCommentOnPlayerKnockingThingsTimer;
	bool											bPlayerInRadiation1;
	bool											bPlayerInRadiation2;
	TaskData										kTaskData;
	BSSimpleList<uint32_t>							kList102E4;
	uint32_t										unk102EC;
	uint32_t										unk102F0;
	uint32_t										unk102F4;
	uint32_t										unk102F8;
	uint32_t										unk102FC;
	BSSpinLock										kNearbyActorLock;
	BSSpinLock										kActorLock_10320;
	BSSpinLock										kLock_10340;
	LockFreeMap<MobileObject*, NiPointer<LipTask>>	kLipTaskMap;
	bool											bToggleAI;
	bool											bToggleDetection;
	bool											bToggleDetectionStats;
	uint32_t										uiDetectionStats;
	bool											bToggleHighProcess;
	bool											bToggleLowProcess;
	bool											bToggleMidHighProcess;
	bool											bToggleMidLowProcess;
	bool											bToggleAISchedules;
	bool											bShowSubtitle;
	uint32_t										uiNumberHighActors;
	float											fCrimeUpdateTimer;
	uint32_t										uiCrimeNumber;
	float											fRemoveExcessDeadTimer;
	BSSemaphore										semaphore103C0;

	static ProcessLists* GetSingleton();
};

ASSERT_SIZE(ProcessLists, 0x103CC);