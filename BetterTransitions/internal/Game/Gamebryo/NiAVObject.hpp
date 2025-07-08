#pragma once

#include "NiObjectNET.hpp"
#include "NiTPointerList.hpp"
#include "NiTransform.hpp"
#include "Bethesda/NiUpdateData.hpp"

class NiCullingProcess;
class NiNode;
class NiCamera;
class bhkNiCollisionObject;
class NiFixedString;
class NiBound;
class NiProperty;
class NiPropertyState;
class NiDX9Renderer;

NiSmartPointer(NiAVObject);
NiSmartPointer(NiProperty);

class NiAVObject : public NiObjectNET {
public:
	NiAVObject();
	virtual ~NiAVObject();

	virtual void			UpdateControllers(NiUpdateData& arData);
	virtual void			ApplyTransform(NiMatrix3& arMat, NiPoint3& arTrn, bool abOnLeft);
	virtual void			Unk_39();
	virtual NiAVObject*		GetObject_(const NiFixedString& arName);
	virtual NiAVObject*		GetObjectByName(const NiFixedString& arName) const;
	virtual void			SetSelectiveUpdateFlags(bool& arSelectiveUpdate, BOOL abSelectiveUpdateTransforms, bool& arRigid);
	virtual void			UpdateDownwardPass(const NiUpdateData& arData, uint32_t auiFlags);
	virtual void			UpdateSelectedDownwardPass(const NiUpdateData& arData, uint32_t auiFlags);
	virtual void			UpdateRigidDownwardPass(const NiUpdateData& arData, uint32_t auiFlags);
	virtual void			UpdatePropertiesDownward(NiPropertyState* apParentState);
	virtual void			Unk_47();
	virtual void			UpdateWorldData(const NiUpdateData& arData);
	virtual void			UpdateWorldBound();
	virtual void			UpdateTransformAndBounds(const NiUpdateData& arData);
	virtual void			PreAttachUpdate(NiNode* apEventualParent, const NiUpdateData& arData);
	virtual void			PreAttachUpdateProperties(NiNode* apEventualParent);
	virtual void			DetachParent();
	virtual void			UpdateUpwardPassParent(void* arg);
	virtual void			OnVisible(NiCullingProcess* apCuller);
	virtual void			PurgeRendererData(NiDX9Renderer* apRenderer);

	struct NiFlags {
		enum {
		    APP_CULLED                              = 1u <<  0, // Forces culled state
			SELECTIVE_UPDATE						= 1u <<  1, // Allows selective updates - see flags below
		    SELECTIVE_UPDATE_TRANSFORMS             = 1u <<  2, // Allows transform controller update
			SELECTIVE_UPDATE_CONTROLLER				= 1u <<  3, // Allows controller update
		    SELECTIVE_UPDATE_RIGID                  = 1u <<  4, // Forces the use of UpdateRigidDownwardPass
		    DISPLAY_OBJECT                          = 1u <<  5, // Used only by sky objects
		    DISABLE_SORTING                         = 1u <<  6,	// Unused, just like Gamebryo's sorter
			SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE	= 1u <<  7, // Forces UPDATE_TRANSFORMS even if node has no transform controllers
			UNK_8									= 1u <<  8, // ??
		    TRANS_IS_IDENTITY						= 1u <<  9, // Assumes local transform is an identity one, skipping transformations
			NO_DECALS								= 1u << 10,	// Disables decals for this object
		    ALWAYS_DRAW                             = 1u << 11, // Forces light inclusion, and skips culling
			PREPROCESSED_NODE						= 1u << 12, // Used to mark actor nodes, for actor culling
		    FIXED_BOUND                             = 1u << 13, // Prevents bound updates
			FADED_IN								= 1u << 14, // BSFadeNode only; Marks the fade state
			IGNORE_FADE								= 1u << 15, // BSFadeNode only; Disables fading
			LOD_FADING_OUT							= 1u << 16, // BSFadeNode only; Looks unused
			HAS_MOVING_SOUND						= 1u << 17, // Used for sound updates
			HAS_PROPERTY_CONTROLLER					= 1u << 18, // Marks the presence of a property controller
			HAS_BOUND								= 1u << 19, // Marks the presence of a bound
		    NOT_VISIBLE                             = 1u << 20, // Used for actor culling
			IGNORES_PICKING							= 1u << 21, // Disables picking for this object
			WORLD_BOUND_CHANGE						= 1u << 22,	// Forces multibound update/attachment
			NO_SHADOWS								= 1u << 23, // Set if bActorSelfShadowing == false, checks for it look broken (maybe why the setting doesn't work?)
		    HIGH_DETAIL	                            = 1u << 24, // BSFadeNode only; Unused, meant for actors 0x936F75
			UNK_25									= 1u << 25, // ??
			UNK_26									= 1u << 26, // ??
		    PLAYER_BONE                             = 1u << 27,	// Marks player's bones, not read anywhere?
			IMPOSTER_LOADED							= 1u << 28, // BSFadeNode only; Marks the imposter state to override fading
		    IS_POINTLIGHT                           = 1u << 29, // Added by JIP
		    DONE_INIT_LIGHTS                        = 1u << 30, // Added by JIP
		    IS_INSERTED                             = 1u << 31  // Added by JIP
		};

        bool bAppCulled                     : 1;
		bool bSelUpdate						: 1;
        bool bSelUpdateTransforms           : 1;
		bool bSelUpdateController			: 1;
        bool bSelUpdateRigid                : 1;
        bool bDisplayObject                 : 1;
        bool bDisableSorting                : 1;
		bool bSelUpdateTransformsOverride	: 1;
		bool bUnk8							: 1;
        bool bTransIsIdentity				: 1;
		bool bNoDecals						: 1;
        bool bAlwaysDraw                    : 1;
		bool bPreprocessedNode				: 1;
        bool bFixedBound                    : 1;
		bool bFadedIn						: 1;
		bool bIgnoreFade					: 1;
		bool bLODFadingOut					: 1;
		bool bHasMovingSound				: 1;
		bool bHasPropertyController			: 1;
		bool bHasBound						: 1;
        bool bNotVisible                    : 1;
		bool bIgnoresPicking				: 1;
		bool bWorldBoundChange				: 1;
		bool bNoShadows						: 1;
        bool bHighDetail	                : 1;
		bool bUnk25							: 1;
		bool bUnk26							: 1;
        bool bPlayerBone                    : 1;
		bool bImposterLoaded				: 1;
        bool bIsPointlight                  : 1;
        bool bDoneInitLights                : 1;
        bool bIsInserted                    : 1;
	};

	NiNode*							m_pkParent;
	NiPointer<bhkNiCollisionObject> m_spCollisionObject;
	NiBound*						m_pWorldBound;
	NiTPointerList<NiPropertyPtr>	m_kPropertyList;
	Bitfield<NiFlags>				m_uiFlags;
	NiTransform						m_kLocal;
	NiTransform						m_kWorld;

	NIRTTI_ADDRESS(0x11F4280);

	void Update(const NiUpdateData& arData = NiUpdateData());
};

ASSERT_OFFSET(NiAVObject, m_pkParent, 0x18);
ASSERT_OFFSET(NiAVObject, m_spCollisionObject, 0x1C);
ASSERT_OFFSET(NiAVObject, m_pWorldBound, 0x20);
ASSERT_OFFSET(NiAVObject, m_kPropertyList, 0x24);
ASSERT_SIZE(NiTPointerList<NiPointer<NiProperty>>, 0xC);
ASSERT_OFFSET(NiAVObject, m_uiFlags, 0x30);
ASSERT_OFFSET(NiAVObject, m_kLocal, 0x34);
ASSERT_OFFSET(NiAVObject, m_kWorld, 0x68);
ASSERT_SIZE(NiAVObject, 0x9C);