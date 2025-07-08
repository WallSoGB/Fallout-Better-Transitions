#pragma once

#include "NiMatrix3.hpp"
#include "NiPoint3.hpp"

class NiTransform {
public:
	BS_ALLOCATORS

	NiTransform();

	NiMatrix3	m_Rotate;
	NiPoint3	m_Translate;
	float		m_fScale;

	NiTransform operator*(const NiTransform& arOther) const noexcept;

	// 0x524C40
	inline NiPoint3 operator*(const NiPoint3& arPoint) const noexcept {
		return(((m_Rotate * arPoint) * m_fScale) + m_Translate);
	};

	inline bool operator==(const NiTransform& arTransform) const noexcept {
		return m_Rotate == arTransform.m_Rotate && m_Translate == arTransform.m_Translate && m_fScale == arTransform.m_fScale;
	}

	void MakeIdentity() noexcept;
	void Invert(NiTransform& arDest) const noexcept;
};

ASSERT_SIZE(NiTransform, 0x34);