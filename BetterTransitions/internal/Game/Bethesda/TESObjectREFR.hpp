#pragma once

#include "BSSoundHandle.hpp"
#include "ExtraDataList.hpp"
#include "Gamebryo/NiNode.hpp"
#include "TESChildCell.hpp"
#include "TESForm.hpp"

class TESBoundObject;
class LockData;
class DoorTeleportData;
class ExtraScript;
class Actor;
class ActorCause;
class ScriptLocals;
class TESContainer;
class TESSound;
class Animation;
class BipedAnim;
class NiGeometry;
class ExtraDroppedItemList;
class BSFaceGenNiNode;
class BSFaceGenAnimationData;
class BSAnimNoteReceiver;
class TESWorldSpace;
class MagicCaster;
class MagicTarget;
class TrapEntry;
class TargetEntry;
class TESActorBase;
class TESObjectWEAP;
class TESTopic;
class bhkPhantom;
class BGSPrimitive;
class BGSWorldLocation;
class BGSLoadGameBuffer;

struct LoadedRefData;

struct ReferenceData {
	TESBoundObject* pObjectReference;
	NiPoint3		kRotation;
	NiPoint3		kPosition;
};

ASSERT_SIZE(ReferenceData, 0x1C);

enum SitSleepStates {
	NORMAL					= 0,
	LOAD_SIT_IDLE			= 1,
	WANT_TO_SIT				= 2,
	WAITING_FOR_SIT_ANIM	= 3,
	SITTING					= 4,
	WANT_TO_STAND			= 5,
	LOADING_SLEEP_IDLE		= 6,
	WANT_TO_SLEEP			= 7,
	WAITING_FOR_SLEEP_ANIM	= 8,
	SLEEPING				= 9,
	WANT_TO_WAKE			= 10,
};

class TESObjectREFR : public TESForm, public TESChildCell {
public:
	TESObjectREFR();
	virtual ~TESObjectREFR();

	virtual bool					GetStartingPosition(NiPoint3& arPos, NiPoint3& arRot, TESForm* apParentLocation, TESObjectCELL* apCell);
	virtual BSSoundHandle			VoiceSoundFunction(TESTopic* apTopic, Actor* apTarget, bool ab2DSound, bool abVoice, bool, bool, bool abForceSubtitles);
	virtual void					UpdateSoundCallBack();
	virtual void					DamageObject(float afDamage, bool abForce);
	virtual bool					GetCastShadows() const;
	virtual void					SetCastShadows(bool abVal);
	virtual bool					GetMotionBlur() const;
	virtual void					SetMotionBlur(bool abVal);
	virtual void					Unk_56();
	virtual bool					IsObstacle() const;
	virtual bool					IsQuestObject() const;
	virtual void					SetActorCause(ActorCause* apActorCause);
	virtual ActorCause*				GetActorCause() const;
	virtual NiPoint3				GetStartingAngle() const;
	virtual NiPoint3				GetStartingLocation() const;
	virtual void					SetStartingPosition(NiPoint3 akPos);
	virtual void					UpdateRefLight();
	virtual TESObjectREFR*			RemoveItem(TESBoundObject* apObject, ExtraDataList* apExtraList, uint32_t auiCount, bool abStealing, bool abDropWorld, TESObjectREFR* apContainerObj, NiPoint3* apPoint, NiPoint3* apRotate, bool abDelete, bool abPreferStolen);
	virtual void					RemoveItemType(uint32_t auiTypeID, bool abStealing, uint32_t auiCount);
	virtual bool					AddWornItem(TESBoundObject* apObject, uint32_t auiCount, ExtraDataList* apExtraList, bool abForceEquip = false);
	virtual bool					RemoveWornItem(TESBoundObject* apObject, uint32_t auiCount, ExtraDataList* apExtraList);
	virtual void					DoTrap(TrapEntry*, TargetEntry*);
	virtual void					AddBoundObjectItem(TESBoundObject* apObject, ExtraDataList* apExtraList, uint32_t auiCount);
	virtual NiPoint3				GetLookingAtLocation() const;
	virtual MagicCaster*			GetMagicCaster() const;
	virtual MagicTarget*			GetMagicTarget() const;
	virtual bool					IsChild(bool abCheckHeight);
	virtual TESActorBase*			GetTemplateActorBase() const;
	virtual void					SetTemplateActorBase(TESActorBase* apBase);
	virtual NiNode*					GetFaceNodeBiped(NiNode* apNode = nullptr) const;
	virtual BSFaceGenNiNode*		GetFaceNodeSkinned(NiNode* apNode = nullptr) const;
	virtual BSFaceGenNiNode*		GetFaceNode(NiNode* apNode = nullptr) const;
	virtual BSFaceGenAnimationData* GetFaceAnimationData(NiNode* apNode = nullptr) const;
	virtual bool					ClampToGround();
	virtual bool					DetachHavok();
	virtual void					InitHavok();
	virtual NiAVObject*				Load3D(bool abBackgroundLoading);
	virtual void					Set3D(NiAVObject* apObject, bool abDebugPrint);
	virtual NiAVObject*				Get3D() const;
	virtual void					HasRandomAnim() const;
	virtual NiPoint3				GetBoundMin() const;
	virtual NiPoint3				GetBoundMax() const;
	virtual void					UpdateAnimation();
	virtual Animation*				GetAnimation() const;
	virtual BipedAnim*				GetBiped() const;
	virtual BipedAnim*				CallGetBiped() const;
	virtual void					SetBiped(BipedAnim* apBiped);
	virtual const NiPoint3&			GetPos() const;
	virtual void					SetHasLowLevelProcessing(bool abVal);
	virtual void					ResetHavokSimulation(bool abRecursive);
	virtual void					SetActionComplete(bool abVal);
	virtual void					SetMovementComplete(bool abVal);
	virtual void					ResetInventory(bool abLeveledOnly);
	virtual NiNode*					GetProjectileNode() const;
	virtual void					SetProjectileNode(NiNode* apNode);
	virtual uint32_t				GetSitSleepState() const;
	virtual bool					IsCharacter() const;
	virtual bool					IsCreature() const;
	virtual bool					IsExplosion() const;
	virtual bool					IsProjectile() const;
	virtual void					SetParentCell(TESObjectCELL* apCell);
	virtual bool					IsDead(bool abNotEssential) const;
	virtual bool					IsKnockedOut() const;
	virtual bool					IsParalyzed() const;
	virtual BSAnimNoteReceiver*		CreateAnimNoteReceiver();
	virtual BSAnimNoteReceiver*		GetAnimNoteReceiever() const;

	enum Flags : uint32_t {
		TARGETED				= 1u << 18, // TESObjectREFR
		HAS_TEMP_3D				= 1u << 19, // TESObjectREFR
		DISABLE_FADE			= 1u << 27, // TESObjectREFR
		REFRACTED_BY_AUTO_WATER = 1u << 29, // TESObjectREFR
	};

	TESSound*		pLoopSound;
	ReferenceData	kData;
	float			fRefScale;
	TESObjectCELL*	pParentCell;
	ExtraDataList	kExtraList;
	LoadedRefData*	pLoadedData;

	NiAVObject* DirectGet3D() const;

	TESObjectCELL* GetParentCell() const;

	TESWorldSpace* GetWorldSpace() const;

	TESObjectREFR* GetLinkedRef() const;

	TESBoundObject* GetObjectReference() const;
};

ASSERT_SIZE(TESObjectREFR, 0x68);

struct LoadedRefData {
	enum Flags {
		HAS_HAVOK		= 1 << 0,
		HAS_ADDONS		= 1 << 1,
		HAS_PRIMITIVE	= 1 << 2,
	};

	TESObjectREFR*			pCurrentWaterType;
	uint32_t				uiUnderwaterCount;
	float					fRelevantWaterHeight;
	float					fCachedRadius;
	Bitfield32				uiFlags;
	NiNodePtr				spNode;
	NiPointer<bhkPhantom>	spPhantom;
};

ASSERT_SIZE(LoadedRefData, 0x1C);