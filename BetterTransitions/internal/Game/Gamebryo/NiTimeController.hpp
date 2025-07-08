#pragma once

#include "NiObject.hpp"
#include "Bethesda/NiUpdateData.hpp"

class NiObjectNET;
class NiAVObject;

class NiTimeController : public NiObject {
public:
	NiTimeController();
	virtual ~NiTimeController();

	virtual void	Start(float afTime = INFINITY);			// 35
	virtual void	Stop();									// 36
	virtual void	Update(NiUpdateData& arUpdateData);		// 37
	virtual void	SetTarget(NiObjectNET* apTarget);		// 38
	virtual bool	IsTransformController() const;			// 39
	virtual bool	IsVertexController() const;				// 40
	virtual float	ComputeScaledTime(float fTime);			// 41
	virtual void	OnPreDisplay() const;					// 42
	virtual bool	IsStreamable() const;					// 43
	virtual bool	TargetIsRequiredType() const;			// 44

	enum AnimType {
		APP_TIME,
		APP_INIT
	};

	enum CycleType : uint32_t {
		LOOP,
		REVERSE,
		CLAMP,
		MAX_CYCLE_TYPES
	};

	enum Flags{
		ANIMTYPE_MASK			= 0x0001,
		ANIMTYPE_POS			= 0,
		CYCLETYPE_MASK			= 0x0006,
		CYCLETYPE_POS			= 1,
		ACTIVE_MASK				= 0x0008,
		DIRECTION_MASK			= 0x0010,
		MANAGERCONTROLLED_MASK	= 0x0020,
		COMPUTESCALEDTIME_MASK	= 0x0040,
		FORCEUDPATE_MASK		= 0x0080
	};

	Bitfield16						m_usFlags;
	float							m_fFrequency;
	float							m_fPhase;
	float							m_fLoKeyTime;
	float							m_fHiKeyTime;
	float							m_fStartTime;
	float							m_fLastTime;
	float							m_fWeightedLastTime;
	float							m_fScaledTime;
	NiObjectNET*					m_pkTarget;
	NiPointer<NiTimeController>		m_spNext;

	NIRTTI_ADDRESS(0x11F49E4);

	void SetActive(bool abActive);
	bool GetActive() const;
};

ASSERT_SIZE(NiTimeController, 0x34);