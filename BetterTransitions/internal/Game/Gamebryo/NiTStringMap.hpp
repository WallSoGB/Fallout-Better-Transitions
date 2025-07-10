#pragma once

#include "NiTMap.hpp"
#include "NiTPointerMap.hpp"

template <class T_Parent, class T_Data>
class NiTStringTemplateMap : public T_Parent {
public:
	NiTStringTemplateMap(uint32_t auiHashSize, bool abCopy) : T_Parent(auiHashSize), m_bCopy(abCopy) {};
	virtual ~NiTStringTemplateMap() {
		if (!m_bCopy)
			return;

		for (uint32_t i = 0; i < T_Parent::m_uiHashSize; i++) {
			NiTMapItem<const char*, T_Data>* pItem = T_Parent::m_ppkHashTable[i];
			while (pItem) {
				NiTMapItem<const char*, T_Data>* pSave = pItem;
				pItem = pItem->m_pkNext;
				NiFree((char*)pSave->m_key);
			}
		}
	}

	virtual uint32_t	KeyToHashIndex(const char* key) const override;
	virtual bool		IsKeysEqual(const char* key1, const char* key2) const override;
	virtual void		SetValue(NiTMapItem<const char*, T_Data>* apItem, const char* key, T_Data data) override;
	virtual void		ClearValue(NiTMapItem<const char*, T_Data>* apItem) override;

	bool m_bCopy;
};

template<class T_Parent, class T_Data>
inline uint32_t NiTStringTemplateMap<T_Parent, T_Data>::KeyToHashIndex(const char* apKey) const {
	uint32_t uiHash = 0;

	while (*apKey)
		uiHash = (uiHash << 5) + uiHash + *apKey++;

	return uiHash % T_Parent::m_uiHashSize;
}

template<class T_Parent, class T_Data>
inline bool NiTStringTemplateMap<T_Parent, T_Data>::IsKeysEqual(const char* apKey1, const char* apKey2) const {
	return strcmp(apKey1, apKey2) == 0;
}

template<class T_Parent, class T_Data>
inline void NiTStringTemplateMap<T_Parent, T_Data>::SetValue(NiTMapItem<const char*, T_Data>* apItem, const char* apKey, T_Data val) {
	if (m_bCopy) {
		uint32_t uiLen = strlen(apKey) + 1;
		apItem->m_key = NiAlloc<char>(uiLen);
		ASSUME_ASSERT(apItem->m_key);
		strcpy_s((char*)apItem->m_key, uiLen, apKey);
	}
	else {
		apItem->m_key = apKey;
	}
	apItem->m_val = val;
}

template<class T_Parent, class T_Data>
inline void NiTStringTemplateMap<T_Parent, T_Data>::ClearValue(NiTMapItem<const char*, T_Data>* apItem) {
	if (m_bCopy) {
		NiFree((char*)apItem->m_key);
	}
}

template <class T_Data>
class NiTStringMap : public NiTStringTemplateMap<NiTMap<const char*, T_Data>, T_Data> {
public:
	NiTStringMap(uint32_t auiHashSize = 37, bool abCopy = true) : NiTStringTemplateMap<NiTMap<const char*, T_Data>, T_Data>(auiHashSize, abCopy) {};
};
static_assert(sizeof(NiTStringMap<uint32_t>) == 0x14);

template <class T_Data>
class NiTStringPointerMap : public NiTStringTemplateMap<NiTPointerMap<const char*, T_Data>, T_Data> {
public:
	NiTStringPointerMap(uint32_t auiHashSize = 37, bool abCopy = true) : NiTStringTemplateMap<NiTPointerMap<const char*, T_Data>, T_Data>(auiHashSize, abCopy) {};
};
static_assert(sizeof(NiTStringPointerMap<uint32_t>) == 0x14);