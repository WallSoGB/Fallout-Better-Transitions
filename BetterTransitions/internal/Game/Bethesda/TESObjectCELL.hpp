#pragma once

#include "BSExtraData.hpp"
#include "ExtraDataList.hpp"
#include "Gamebryo/NiSmartPointer.hpp"
#include "Gamebryo/NiTMap.hpp"
#include "Gamebryo/NiNode.hpp"
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESTexture.hpp"

class NiNode;
class NavMeshArray;
class TESObjectLAND;
class TESWorldSpace;
class TESObjectREFR;
class BGSLightingTemplate;
class DECAL_CREATION_DATA;
class BGSAcousticSpace;
class CellMopp;
class BSMultiBoundNode;
class BSPortalGraph;

class TESObjectCELL : public TESForm, public TESFullName {
public:
	TESObjectCELL();
	~TESObjectCELL();

	enum CELL_STATE {
		CS_NOTLOADED	= 0,
		CS_UNLOADING	= 1,
		CS_LOADING		= 2,
		CS_LOADED		= 3,
		CS_DETACHING	= 4,
		CS_ATTACHING	= 5,
		CS_ATTACHED		= 6,
	};

	struct LoadedData;

	struct ExteriorData {
		int32_t		iCellX;
		int32_t		iCellY;
		Bitfield8	ucLandHideFlags;
	};

	struct InteriorData {
		uint32_t	uiAmbient;
		uint32_t	uiDirectional;
		uint32_t	uiFogColorNear;
		float		fFogNear;
		float		fFogFar;
		int32_t		iDirectionalXY;
		int32_t		iDirectionalZ;
		float		fDirectionalFade;
		float		fClipDist;
		float		fFogPower;
		uint32_t	uiInteriorOffset;
	};

	union CellData {
		ExteriorData* __restrict pCellDataExterior;
		InteriorData* __restrict pCellDataInterior;
	};

	enum CELLNODE {
		CN_ACTOR = 0x0,
		CN_MARKER = 0x1,
		CN_LAND = 0x2,
		CN_STATIC = 0x3,
		CN_DYNAMIC = 0x4,
		CN_OCCLUSION_PLANE = 0x5,
		CN_PORTAL = 0x6,
		CN_MULTIBOUND = 0x7,
		CN_COLLISION = 0x8,
		CN_LIGHT_MARKER = 0x9,
		CN_SOUND_MARKER = 0xA,
		CN_WATER = 0xB,
		CN_MAX = 0xC,
	};


	enum CELL_INHERIT_FLAGS {
		CIF_AMBIENT_COLOR = 0x1,
		CIF_DIRECTIONAL_COLOR = 0x2,
		CIF_FOG_COLOR = 0x4,
		CIF_FOG_NEAR = 0x8,
		CIF_FOG_FAR = 0x10,
		CIF_DIRECTIONAL_ROT = 0x20,
		CIF_DIRECTIONAL_FADE = 0x40,
		CIF_CLIP_DIST = 0x80,
		CIF_FOG_POWER = 0x100,
	};

	enum Flags {
		INTERIOR				= 1 << 0,
		HAS_WATER				= 1 << 1,
		CANT_FAST_TRAVEL		= 1 << 2,
		NO_LOD_WATER			= 1 << 3,
		HAS_TEMP_DATA			= 1 << 4,
		PUBLIC					= 1 << 5,
		TEMP_PUBLIC				= 1 << 6,
		BEHAVE_LIKE_EXTERIOR	= 1 << 7,
	};

	Bitfield8								ucCellFlags;
	Bitfield8								ucCellGameFlags;
	uint8_t									cCellState;
	ExtraDataList							kExtraList;
	CellData								pCellData;
	TESObjectLAND*							pCellLand;
	float									fWaterHeight;
	bool									bAutoWaterLoaded;
	bool									bAllRefsLoaded; // Added
	TESTexture								kNoiseTexture;
	NavMeshArray*							pNavMeshes;
	uint32_t								unk68[6];
	BSSpinLock								kSpinLock;
	int32_t									iCriticalQueuedRefCount;
	int32_t									iQueuedRefCount;
	uint16_t								usDistantRefCount;
	uint16_t								usLoadedDistantRefCount;
	BSSimpleList<TESObjectREFR*>			kReferences;
	NiNodePtr								spLightMarkerNode;
	NiNodePtr								spSoundMarkerNode;
	NiAVObjectPtr							spUnkC8;
	union {
		TESWorldSpace*			 __restrict pWorldSpace;		// Exteriors
		uint32_t							uiTempDataOffset;	// Interiors
	};
	LoadedData*								pLoadedData;
	float									fLODFadeOut;
	bool									byteCC;
	bool									byteCD;
	bool									bTriggeredLODHide;
	bool									bAlwaysFalse_CF;
	bool									bCanHideLOD;
	bool									bCellDetached;
	bool									bSkippedFade;
	bool									byteD3;
	NiPointer<BSPortalGraph>				spPortalGraph;
	BGSLightingTemplate*					pLightingTemplate;
	Bitfield32								uiInheritFlags;

	bool IsInterior() const { return ucCellFlags.GetBit(INTERIOR); }

	bool LoadAllTempData();
};

ASSERT_SIZE(TESObjectCELL, 0xE0);