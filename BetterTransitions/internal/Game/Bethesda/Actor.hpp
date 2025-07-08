#pragma once

#include "ActorValueOwner.hpp"
#include "CachedValuesOwner.hpp"
#include "MagicCaster.hpp"
#include "MagicTarget.hpp"
#include "MobileObject.hpp"
#include "ModifierList.hpp"
#include "BSSimpleArray.hpp"

class FurnitureMark;
class HitData;
class ActorMover;
class BGSPerk;
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
class TESActorBase;
class ContinuousBeamProjectile;
class TESPackage;
class TESRace;
class CombatGroup;
class CombatController;
class BGSEntryPointPerkEntry;
class SpellItem;
class ArrowProjectile;
class BSSoundHandle;
class ItemChange;
class TESObjectBOOK;
class TrespassPackage;
class TESFaction;
class TESSkill;
class FACTION_RANK;
class TESNPC;
class AlchemyItem;

struct DispositionModifier;
struct DispositionOverride;


class Actor : public MobileObject, public MagicCaster, public MagicTarget, public ActorValueOwner, public CachedValuesOwner {
public:
	Actor();
	virtual ~Actor();

	virtual bool				IsGuard() const;
	virtual void				SetGuard(bool abVal);
	virtual void				SetFlyInventoryWeight(float afVal);
	virtual float				GetFlyInventoryWeight() const;
	virtual void				Unk_C5();
	virtual void				Unk_C6();
	virtual void				SetIgnoreCrime(bool abVal);
	virtual bool				GetIgnoreCrime() const;
	virtual void				Resurrect(bool, bool, bool);
	virtual void				Unk_CA(uint32_t);
	virtual uint32_t			Unk_CB() const;
	virtual void				Unk_CC(float);
	virtual uint32_t			GetMaxActorValues();
	virtual void				DamageHealthAndFatigue(float afHealth, float afFatigue, Actor* apSource);
	virtual void				DamageActionPoints(float afAmount);
	virtual void				Unk_D0();
	virtual uint32_t			GetDisposition(Actor* apTarget, DispositionOverride* apOverride = nullptr);
	virtual void				UpdateMovement(float afTimeDelta, bool);
	virtual void				Unk_D3();
	virtual void				UpdateAnimMovement();
	virtual float				GetTurningSpeed() const;
	virtual bool				IsOverEncumbered() const;
	virtual void				Unk_D7();
	virtual bool				IsPc() const;
	virtual bool				GetIsInCannibalAction() const;
	virtual void				Unk_DA();
	virtual bool				GetIsInSandmanAction() const;
	virtual void				Unk_DC();
	virtual void				InitiateSandManPackage(Actor* apTarget, TESObjectREFR* apFurnitureRef, FurnitureMark* apFurnitureMark, uint8_t aucFurnitureMarkerIndex);
	virtual void				InitiateCannibalPackage(Actor* apTarget);
	virtual TESRace*			GetRace() const;
	virtual float				GetReach() const;
	virtual void				SetRefraction(bool, float);
	virtual void				SetHasRagdoll(bool);
	virtual bool				GetHasRagdoll() const;
	virtual uint32_t			GetActorType() const;	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void				SetActorValueF(uint32_t auiAVCode, float afValue);
	virtual void				SetActorValueI(uint32_t auiAVCode, int32_t aiValue);
	virtual void				MagicModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker); // SANITY CHECK ALL THESE!!!
	virtual void				MagicModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void				ScriptModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void				ScriptModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void				GameModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void				GameModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void				ModActorBaseValueF(uint32_t auiAVCode, float afModifier);
	virtual void				ModActorBaseValueI(uint32_t auiAVCode, int32_t aiModifier);
	virtual ItemChange*			GetBestWeapon(uint32_t aeWeaponType) const;
	virtual ItemChange*			GetBestAmmo() const;
	virtual void				ResetArmorRating();
	virtual bool				DamageEquipment(ItemChange* apEquipment, float afDamage, bool abIgnoreArmorSkill = false);
	virtual TESObjectREFR*		DropObject(TESForm* apItem, ExtraDataList* apExtraList, int32_t aiCount, const NiPoint3* apPoint, const NiPoint3* apRotate);
	virtual void				PickUpObject(TESObjectREFR* apObject, int32_t aiCount, bool abPlayPickUpSounds);
	virtual void				CastScroll(TESObjectBOOK* apBook, MagicTarget* apTarget);
	virtual void				CheckCastWhenStrikesEnchantment(ItemChange* apWeapon, Actor* apTarget, ArrowProjectile* apArrow, bool& abInsufficientCharge);
	virtual void				Unk_F7();
	virtual bool				AddSpell(SpellItem* apSpell);
	virtual bool				RemoveSpell(SpellItem* apSpell);
	virtual bool				Reload(TESObjectWEAP* apWeapon, uint32_t, bool);
	virtual bool				ReloadAlt(TESObjectWEAP* apWeapon, uint32_t, bool, bool);
	virtual void				DecreaseClipAmmo();
	virtual void				CleanOrphanedMagicModifiers();
	virtual CombatGroup*		GetCombatGroup() const;
	virtual void				SetCombatGroup(CombatGroup* apGroup);
	virtual bool				InitiateTresPassPackage(TrespassPackage* apPackage);
	virtual void				Unk_101(float afDelta); // RunUpdates()?
	virtual BSSoundHandle		VoiceSoundFunctionEx(TESTopic* apTopic, Actor* apTarget, bool abIs2D, bool abNeverSubtitle, int unused, bool abSayTo);
	virtual void				InitiateSpectator();
	virtual void				InitiateFlee(TESObjectREFR* apFleeRef, bool abRunOnce, bool abKnowsTarget, bool abCombatMode, TESObjectCELL* apCell, TESObjectREFR* apRef, float afFleeFromDist, float afFleeToDist);
	virtual void				InitiateSearch(TESObjectREFR* apAttacker, NiPoint3);
	virtual void				InitiateGetUpPackage();
	virtual void				SetAlpha(float afValue);
	virtual float				GetAlpha();
	virtual void				StartCombat(Actor* apTarget, CombatGroup* apCombatGroup, bool abIgnoreActorLimit, bool abIsAggressor, bool, int, bool, TESPackage* apPackage);
	virtual CombatController*	GetCombatController() const;
	virtual Actor*				GetCombatTarget() const;
	virtual void				UpdateCombat(float afElapsedTime);
	virtual void				StopCombat(Actor* apAttacker);
	virtual void				PerformOneRoundofCombat();
	virtual float				GetTotalArmorDR();
	virtual float				GetTotalArmorDT();
	virtual float				GetAttackDamage() const;
	virtual bool				IsTrespassing() const;
	virtual void				SetTrespassing();
	virtual void				Unk_114();
	virtual void				Unk_115();
	virtual float				CalculateWalkSpeed();
	virtual float				CalculateRunSpeed();
	virtual void				ModifyDispositionTowardActor(Actor* apActor, float afValue);
	virtual float				GetDispositionModifierTowardActor(Actor* apActor) const;
	virtual void				RemoveDispositionModifierForActor(Actor* apActor);
	virtual void				SetActorStartingPosition();
	virtual bool				HasBeenAttacked() const;
	virtual void				SetBeenAttacked(bool abVal);
	virtual void				HitMe(HitData* apHitData, float afHealthDmg, float afBlockDTMod, NiPoint3& unk, NiPoint3& arWeaponPos);
	virtual void				Unk_11F(void*, void*);
	virtual void				UseSkill(ActorValue::Index aeSkill, uint32_t aeAction, float afModifier);
	virtual void				UseSkillAlt(ActorValue::Index aeSkill, float afUsage, TESSkill* apSkill);
	virtual void				RewardXP();
	virtual float				InternalGetActorValue(ActorValue::Index aeIndex, bool& arFound);
	virtual bool				InternalSetActorValue(ActorValue::Index aeIndex, float afValue);
	virtual void				UpdateWeaponWobble();
	virtual void				SetPerkRank(BGSPerk* apPerk, uint8_t rank, bool alt);
	virtual void				RemovePerk(BGSPerk* apPerk, bool alt);
	virtual uint8_t				GetPerkRank(BGSPerk* apPerk, bool alt);
	virtual void				AddPerkEntryPoint(BGSEntryPointPerkEntry* perkEntry, bool alt);
	virtual void				RemovePerkEntry(BGSEntryPointPerkEntry* perkEntry, bool alt);
	virtual void*				GetPerkEntryListByID(uint8_t entryPointID, bool alt);
	virtual void				StartAnimOn1stPerson(uint16_t aeAnimGroup, uint32_t aeActionFlags);
	virtual bool				IsImmobileCreature() const;
	virtual void				HandleHealthDamage(Actor* apAttacker, float afDamage);
	virtual void				AttackedBy(Actor* apAttacker, ActiveEffect* apEffect = nullptr);
	virtual void				HandleBlockedAttack(float afFullDamage, float afPercentageBlocked, Actor* apBlockingActor, ArrowProjectile* apArrow);
	virtual void				Unk_131();
	virtual void				HandleHeadTracking(float afTime);
	virtual void				UpdateHeadTrackingEmotions(int empty = 0);
	virtual void				CreateActorMover();
	virtual void				DestroyActorMover();
	virtual NiPoint3			GetAnticipatedLocation(float afTime) const;

	enum VisbilityMask {
		FRUSTUM		= 1,
		OCCLUSION	= 2,
		FADED		= 4,
		ALL			= 7
	};

	bhkRagdollController*				pRagdollController;
	bhkRagdollPenetrationUtil*			pRagdollPenetrationUtil;
	uint32_t							unk0B4;
	float								flt0B8;
	bool								bIsAIProcessing;
	Actor*								pKiller;
	bool								bMurderAlarm;
	float								fCheckMyDeadBodyTimer;
	float								fCC;
	ModifierList						kScriptModifiers;
	ModifierList						kSetActorValues;
	bool								bBlocking;
	uint8_t								byte0F1;
	uint8_t								byte0F2;
	uint8_t								byte0F3;
	BSSimpleList<uint32_t>				kCastPowers;
	BSSimpleList<DispositionModifier*>	kDispositionModifiers;
	bool								bIsInCombat;
	Bitfield8							ucJIPActorFlags1;
	Bitfield8							ucJIPActorFlags2;
	Bitfield8							ucJIPActorFlags3;
	uint32_t							uiLifeState;
	uint32_t							uiCriticalStage;
	uint32_t							uiQueuedAttackAnim;
	float								fLastUpdate;
	uint8_t								byte118;
	uint8_t								byte119;
	uint16_t							usJIP11A;
	Bitfield32							uiVisibilityMask;
	uint32_t							hoursPassed120;
	bool								bForceRun;
	bool								bForceSneak;
	bool								bForceUpdateQuestTarget;
	bool								bHasSearch;
	Actor*								pCombatTarget;
	BSSimpleArray<Actor*>*				pCombatTargets;
	BSSimpleArray<Actor*>*				pCombatAllies;
	bool								bCaughtPickpocketing;
	uint8_t								byte135;
	uint16_t							usJIP136;
	uint32_t							uiPickPocketCaughtDate;
	uint32_t							uiMinorCrimeCount;
	Bitfield32							uiActorFlags;
	bool								bIgnoreCrime;
	bool								bBufferedPackageEvaluation;
	bool								bIsResetAI;
	bool								byte147;
	TESActorBase*						pTemplateActorBase;
	bool								bInWater;
	bool								bIsSwimming;
	bool								bAddToShadowSceneNode;
	uint32_t							iActionValue;
	float								fTimerOnAction;
	float								fHeadTrackTimer;
	uint8_t								byte15C;
	uint8_t								bShouldRotateToTrack;
	uint16_t							usJIP15E;
	NiPoint3							kStartingPos;
	float								fStartingZRot;
	TESForm*							pStartingWorldOrCell;
	bool								bOnDeath;
	uint8_t								byte175;
	uint16_t							usJIP176;
	float								fCachedSpread_Projectile;
	float								fCachedSpread_Scoped;
	float								fCachedSpread_WeaponModel;
	float								fCachedSpread_VATS;
	float								flt188;
	bool								bFootIKInRange;
	bool								bIsTeammate;
	bool								bLightingUpdatedNonMoving;
	bool								byte18F;
	ActorMover*							pActorMover;
	uint32_t							unk194;
	TESPackage*							pPackage198;
	float								fOneMinusRadResist;
	ContinuousBeamProjectile*			pContinuousBeamProjectile;
	uint32_t							eEmotion;
	uint32_t							uiEmotionValue;
	uint32_t							eSitSleepState;
	bool								bIsTurret;
	bool								bForceHit;
	bool								byte1B2;
	bool								byte1B3;

	void UpdateAnimationMovement(Animation* apAnimation, float afDelta);
};

ASSERT_SIZE(Actor, 0x1B4);
ASSERT_OFFSET(Actor, pRagdollController, 0xAC);
ASSERT_OFFSET(Actor, pActorMover, 0x190);