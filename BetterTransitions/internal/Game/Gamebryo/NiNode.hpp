#pragma once

#include "NiAVObject.hpp"
#include "NiTArray.hpp"

NiSmartPointer(NiNode);

class NiFixedString;

class NiNode : public NiAVObject {
public:
	NiNode();
	virtual ~NiNode();

	virtual void	AttachChild(NiAVObject* apChild, bool abFirstAvail);
	virtual void	InsertChildAt(uint32_t i, NiAVObject* apChild);
	virtual void	DetachChild(NiAVObject* apChild, NiAVObjectPtr& arResult);
	virtual void	DetachChildAlt(NiAVObject* apChild);
	virtual void	DetachChildAt(uint32_t i, NiAVObjectPtr& arResult);
	virtual void	DetachChildAtAlt(uint32_t i);
	virtual void	SetAt(uint32_t i, NiAVObject* apChild, NiAVObjectPtr& arResult);
	virtual void	SetAtAlt(uint32_t i, NiAVObject* apChild);
	virtual void	UpdateUpwardPass();

	NiTObjectArray<NiAVObjectPtr> m_kChildren;

	CREATE_OBJECT(NiNode, 0xA5F030);
	NIRTTI_ADDRESS(0x11F4428);
};

ASSERT_SIZE(NiNode, 0xAC);