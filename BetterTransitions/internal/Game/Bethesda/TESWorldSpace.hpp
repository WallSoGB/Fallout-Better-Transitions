#pragma once

#include "BSMap.hpp"
#include "BSSimpleArray.hpp"
#include "Gamebryo/NiPoint2.hpp"
#include "TESForm.hpp"
#include "TESObjectCELL.hpp"
#include "TESTexture.hpp"

class BGSTerrainManager;
class TESClimate;
class TESImageSpace;
class TESWaterForm;
class BGSMusicType;
class BGSEncounterZone;
class BGSImpactData;
class TESGrassAreaParam;
class BSPortalGraph;

class TESWorldSpace : public TESForm, public TESFullName, public TESTexture {
public:
	TESWorldSpace();
	virtual ~TESWorldSpace();
	virtual bool GetMapNameForLocation(BSString& arName, NiPoint3 akLocation);
	virtual void GetGrassForLocation(NiPoint2, NiPoint2, TESGrassAreaParam*, uint32_t);

	struct DCoordXY {
		int32_t	X;
		int32_t	Y;
	};

	struct WCoordXY {
		int16_t	X;
		int16_t	Y;
	};

	struct OFFSET_DATA {
		uint32_t*	pCellFileOffsets;
		NiPoint2	kOffsetMinCoords;
		NiPoint2	kOffsetMaxCoords;
		uint32_t	uiFileOffset;
	};

	struct MapData {
		DCoordXY	usableDimensions;
		WCoordXY	cellNWCoordinates;
		WCoordXY	cellSECoordinates;
	};	// 010

	struct ImpactData {
		typedef BSMap<BGSImpactData*, BGSImpactData*> ImpactImpactMap;
		enum MaterialType
		{
			eMT_Stone = 0,
			eMT_Dirt,
			eMT_Grass,
			eMT_Glass,
			eMT_Metal,
			eMT_Wood,
			eMT_Organic,
			eMT_Cloth,
			eMT_Water,
			eMT_HollowMetal,
			eMT_OrganicBug,
			eMT_OrganicGlow,

			eMT_Max
		};

		ImpactImpactMap	impactImpactMap[eMT_Max];	// 000
		char			footstepMaterials[0x12C];	// 030
	};

	enum Flags {
		SMALL_WORLD				= 1 << 0,
		NO_FAST_TRAVEL			= 1 << 1,
		UNK_2					= 1 << 2,
		UNK_3					= 1 << 3,
		NO_LOD_WATER			= 1 << 4,
		NO_LOD_NOISE			= 1 << 5,
		NO_NPC_FALL_DAMAGE		= 1 << 6,
		NEEDS_WATER_ADJUSTMENT	= 1 << 7,
	};

	enum ParentUseFlags {
		PU_LAND		= 0,
		PU_LOD		= 1,
		PU_MAP		= 2,
		PU_WATER	= 3,
		PU_CLIMATE	= 4,
		PU_UNUSED	= 5,
		PU_SKYCELL	= 6,
		PU_COUNT	= 7,
	};

	typedef NiTPointerMap<uint32_t, BSSimpleList<TESObjectREFR*>*>	RefListPointerMap;
	typedef NiTPointerMap<int32_t, TESObjectCELL*>					CellPointerMap;
	typedef NiTMap<TESFile*, TESWorldSpace::OFFSET_DATA*>			OffsetDataMap;

	CellPointerMap*						pCellMap;
	TESObjectCELL*						pPersistentCell;
	uint32_t							kTerrainLODManager; // Unused
	BGSTerrainManager*					pTerrainManager;
	TESClimate*							pClimate;
	TESImageSpace*						pImageSpace;
	ImpactData*							pImpactSwap;
	Bitfield8							cFlags;
	Bitfield16							sParentUseFlags;
	RefListPointerMap					kFixedPersistentRefMap;
	BSSimpleList<TESObjectREFR*>		kMobilePersistentRefs;
	NiTMap<uint32_t, TESObjectREFR*>*	pOverlappedMultiBoundMap;
	NiPointer<BSPortalGraph>			spPortalGraph;
	TESWorldSpace*						pParentWorld;
	TESWaterForm*						pWorldWater;
	TESWaterForm*						pLODWater;
	float								fWaterLODHeight;
	MapData								kMapData;
	float								fWorldMapScale;
	float								fWorldMapCellX;
	float								fWorldMapCellY;
	BGSMusicType*						pMusic;
	NiPoint2							kMin;
	NiPoint2							kMax;
	OffsetDataMap						kOffsetMap;
	BSString							strEditorID;
	float								fDefaultLandHeight;
	float								fDefaultWaterHeight;
	BGSEncounterZone*					pEncounterZone;
	TESTexture							kCanopyShadow;
	TESTexture							kWaterNoiseTexture;

	TESObjectCELL* GetCellFromCellCoord(int32_t aiX, int32_t aiY) const;
	TESObjectCELL* GetCellAtPos(const NiPoint3& arPos) const;
};

ASSERT_SIZE(TESWorldSpace, 0xEC);