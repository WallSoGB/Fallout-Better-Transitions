#pragma once

#include "NiTMap.hpp"
#include "NiFixedString.hpp"

template <class Allocator, class T_Data>
class NiTFixedStringMapBase : public NiMemObject {
public:
	NiTFixedStringMapBase(uint32_t uiHashSize = 37) {
		m_uiHashSize = uiHashSize;
		m_kAllocator.m_uiCount = 0;

		uint32_t uiSize = sizeof(NiTMapItem<NiFixedString, T_Data>*) * uiHashSize;
		m_ppkHashTable = (NiTMapItem<NiFixedString, T_Data>**)NiAlloc(uiSize);
		memset(m_ppkHashTable, 0, uiHashSize * sizeof(NiTMapItem<NiFixedString, T_Data>*));
	}
	NiTFixedStringMapBase(const NiTFixedStringMapBase&) = delete;
	NiTFixedStringMapBase& operator=(const NiTFixedStringMapBase&) = delete;

	virtual ~NiTFixedStringMapBase();
	virtual NiTMapItem<NiFixedString, T_Data>* NewItem() = 0;
	virtual void DeleteItem(NiTMapItem<NiFixedString, T_Data>* apItem) = 0;

	struct AntiBloatAllocator : public Allocator {
		uint32_t m_uiCount;
	};

	uint32_t							m_uiHashSize;
	NiTMapItem<NiFixedString, T_Data>** m_ppkHashTable;
	AntiBloatAllocator					m_kAllocator;

	uint32_t GetCount() const { return m_kAllocator.m_uiCount; }
	bool IsEmpty() const { return m_kAllocator.m_uiCount == 0; }

	uint32_t KeyToHashIndex(const NiFixedString& arKey) const {
		const char* pcKey = arKey;
		return (uint32_t)(((size_t)pcKey) % m_uiHashSize);
	}

	bool IsKeysEqual(const NiFixedString& arKey1, const NiFixedString& arKey2) const {
		return arKey1 == arKey2;
	}

	void SetValue(NiTMapItem<NiFixedString, T_Data>* pItem, NiFixedString& arKey, T_Data data) {
		pItem->m_key = arKey;
		pItem->m_val = data;
	}

	bool GetAt(const NiFixedString& arKey, T_Data& dataOut) const {
		uint32_t uiHashIndex = KeyToHashIndex(arKey);
		NiTMapItem<NiFixedString, T_Data>* pItem = m_ppkHashTable[uiHashIndex];
		while (pItem) {
			if (IsKeysEqual(pItem->m_key, arKey)) {
				dataOut = pItem->m_val;
				return true;
			}
			pItem = pItem->m_pkNext;
		}
		return false;
	}

	void SetAt(NiFixedString& arKey, T_Data data) {
		uint32_t uiHashIndex = KeyToHashIndex(arKey);
		NiTMapItem<NiFixedString, T_Data>* pItem = m_ppkHashTable[uiHashIndex];
		while (pItem) {
			if (IsKeysEqual(pItem->m_key, arKey)) {
				SetValue(pItem, arKey, data);
				return;
			}
			pItem = pItem->m_pkNext;
		}
		NiTMapItem<NiFixedString, T_Data>* pNewEntry = NewItem();
		SetValue(pNewEntry, arKey, data);
		pNewEntry->m_pkNext = m_ppkHashTable[uiHashIndex];
		m_ppkHashTable[uiHashIndex] = pNewEntry;
		m_kAllocator.m_uiCount++;
	}

	NiTMapIterator GetFirstPos() const {
		for (uint32_t i = 0; i < m_uiHashSize; i++) {
			if (m_ppkHashTable[i])
				return m_ppkHashTable[i];
		}
		return nullptr;
	}

	void GetNext(NiTMapIterator& pos, NiFixedString& key, T_Data& val) {
		NiTMapItem<NiFixedString, T_Data>* pItem = (NiTMapItem<NiFixedString, T_Data>*) pos;

		key = pItem->m_key;
		val = pItem->m_val;

		if (pItem->m_pkNext) {
			pos = pItem->m_pkNext;
			return;
		}

		uint32_t i = KeyToHashIndex(pItem->m_key);
		for (++i; i < m_uiHashSize; i++) {
			pItem = m_ppkHashTable[i];
			if (pItem) {
				pos = pItem;
				return;
			}
		}

		pos = 0;
	}

	void RemoveAll() {
		for (uint32_t i = 0; i < m_uiHashSize; i++) {
			while (m_ppkHashTable[i]) {
				NiTMapItem<NiFixedString, T_Data>* pkSave = m_ppkHashTable[i];
				m_ppkHashTable[i] = m_ppkHashTable[i]->m_pkNext;
				DeleteItem(pkSave);
			}
		}

		m_kAllocator.m_uiCount = 0;
	}
};

template <class T_Data>
class NiTFixedStringMap : public NiTFixedStringMapBase<NiTDefaultAllocator<T_Data>, T_Data> {
public:
	NiTFixedStringMap(uint32_t uiHashSize = 37) :
		NiTFixedStringMapBase<NiTDefaultAllocator<T_Data>, T_Data>(uiHashSize) {}
	~NiTFixedStringMap() {
		NiTFixedStringMap<T_Data>::RemoveAll();
	}

	NiTMapItem<NiFixedString, T_Data>* NewItem() override {
		NiTMapItem<NiFixedString, T_Data>* pItem = (NiTMapItem<NiFixedString, T_Data>*)NiTFixedStringMapBase<NiTDefaultAllocator<T_Data>, T_Data>::m_kAllocator.Allocate();
		memset(&pItem->m_key, 0, sizeof(pItem->m_key));
		return pItem;
	}
	void DeleteItem(NiTMapItem<NiFixedString, T_Data>* apItem) override {
		apItem->m_key = 0;
		apItem->m_val = 0;
		NiTFixedStringMapBase<NiTDefaultAllocator<T_Data>, T_Data>::m_kAllocator.Deallocate(apItem);
	}
};

ASSERT_SIZE(NiTFixedStringMap<uint32_t>, 0x10);

template<class Allocator, class T_Data>
inline NiTFixedStringMapBase<Allocator, T_Data>::~NiTFixedStringMapBase() {
	NiTFixedStringMapBase<Allocator, T_Data>::RemoveAll();
	NiFree(m_ppkHashTable);
}