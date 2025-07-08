#pragma once

#include "BSSimpleList.hpp"

template<class T_Data>
inline BSSimpleList<T_Data>::BSSimpleList() : m_item(0), m_pkNext(nullptr) {
}

template<class T_Data>
inline BSSimpleList<T_Data>::BSSimpleList(const T_Data& item) : m_item(item), m_pkNext(nullptr) {
}

template<class T_Data>
inline BSSimpleList<T_Data>::BSSimpleList(const T_Data& item, BSSimpleList<T_Data>* apNext) : m_item(item), m_pkNext(apNext) {
}

template<class T_Data>
inline BSSimpleList<T_Data>::BSSimpleList(const BSSimpleList<T_Data>& arEntry) : m_item(arEntry.m_item), m_pkNext(0) {
}

template<class T_Data>
inline BSSimpleList<T_Data>::~BSSimpleList() {
	RemoveAll();
}

template<class T_Data>
inline const T_Data BSSimpleList<T_Data>::GetItem() const {
	return m_item;
}

template<class T_Data>
inline T_Data BSSimpleList<T_Data>::GetItem() {
	return m_item;
}

template<class T_Data>
inline void BSSimpleList<T_Data>::SetItem(const T_Data& aItem) {
	m_item = aItem;
}

template<class T_Data>
inline const BSSimpleList<T_Data>* BSSimpleList<T_Data>::GetNext() const {
	return m_pkNext;
}

template<class T_Data>
inline BSSimpleList<T_Data>* BSSimpleList<T_Data>::GetNext() {
	return m_pkNext;
}

template<class T_Data>
inline void BSSimpleList<T_Data>::SetNext(BSSimpleList<T_Data>* apNext) {
	m_pkNext = apNext;
}

// GAME - 0x8256D0
template<class T_Data>
inline bool BSSimpleList<T_Data>::IsEmpty() const { 
	return !m_pkNext && !m_item; 
}

// GAME - 0x631540
template<class T_Data>
inline void BSSimpleList<T_Data>::AddHead(const T_Data& aItem) {
	if (!aItem)
		return;

	BSSimpleList<T_Data>* pHead = GetHead();

	if (pHead->GetItem()) {
		BSSimpleList<T_Data>* pNewEntry = new BSSimpleList<T_Data>(*pHead);
		pNewEntry->SetNext(pHead->GetNext());
		pHead->SetNext(pNewEntry);
		pHead->SetItem(aItem);

	}
	else {
		pHead->SetItem(aItem);
	}
}

// GAME - 0xAF25B0, 0x905820, 0xB63BF0
template<class T_Data>
inline void BSSimpleList<T_Data>::AddTail(const T_Data& aItem) {
	if (!aItem)
		return;

	BSSimpleList<T_Data>* pTail = GetTail();

	if (pTail->GetItem()) {
		pTail->SetNext(new BSSimpleList<T_Data>(aItem));
	}
	else {
		pTail->SetItem(aItem);
	}
}

// GAME - 0x5F65D0
template<class T_Data>
inline bool BSSimpleList<T_Data>::IsInList(const T_Data& aItem) const {
	const BSSimpleList<T_Data>* pIter = this;
	while (pIter && pIter->GetItem() != aItem)
		pIter = pIter->GetNext();
	return pIter != 0;
}

template<class T_Data>
inline BSSimpleList<T_Data>* BSSimpleList<T_Data>::GetPos(const T_Data& aItem) const {
	BSSimpleList<T_Data>* pIter = const_cast<BSSimpleList<T_Data>*>(this);
	while (pIter && pIter->GetItem() != aItem)
		pIter = pIter->GetNext();
	return pIter;
}

// GAME - 0x5AE380
template<class T_Data>
inline uint32_t BSSimpleList<T_Data>::ItemsInList() const {
	uint32_t uiCount = 0;
	const BSSimpleList<T_Data>* pIter = this;
	while (pIter) {
		if (pIter->GetItem())
			++uiCount;

		pIter = pIter->GetNext();
	}
	return uiCount;
}

// GAME - 0xB64EC0
template<class T_Data>
inline void BSSimpleList<T_Data>::RemoveAll() {
	if (GetNext()) {
		BSSimpleList<T_Data>* pNext = 0;
		do {
			auto pCurrNode = GetNext();
			pNext = pCurrNode->GetNext();
			pCurrNode->SetNext(0);
			delete m_pkNext;
			SetNext(pNext);
		} while (pNext);
	}
	SetItem(0);
}

// GAME - 0xB99730
template<class T_Data>
inline void BSSimpleList<T_Data>::RemoveHead() {
	auto pNext = GetNext();
	if (pNext) {
		SetNext(pNext->GetNext());
		SetItem(pNext->GetItem());
		pNext->SetNext(nullptr);
		delete pNext;
	}
	else {
		SetItem(0);
	}
}

template<class T_Data>
inline void BSSimpleList<T_Data>::RemoveTail() {
	BSSimpleList<T_Data>* pTail = GetTail();
	if (pTail) {
		BSSimpleList<T_Data>* pIter = this;
		while (pIter->GetNext() != pTail)
			pIter = pIter->GetNext();

		pIter->SetNext(0);
		delete pTail;
	}
	else {
		SetItem(0);
	}
}

// GAME - 0x905330
template<class T_Data>
inline void BSSimpleList<T_Data>::Remove(const T_Data& aItem) {
	if (!aItem || IsEmpty())
		return;

	BSSimpleList<T_Data>* pIter = this;
	BSSimpleList<T_Data>* pPrev = this;

	while (pIter && pIter->GetItem() != aItem) {
		pPrev = pIter;
		pIter = pIter->GetNext();
	}

	if (!pIter)
		return;

	if (pIter == this) {
		if (GetNext()) {
			auto pNext = GetNext();
			SetNext(pNext->GetNext());
			SetItem(pNext->GetItem());
			pNext->SetNext(nullptr);
			delete pNext;
		}
		else {
			SetItem(0);
		}
	}
	else {
		pPrev->SetNext(pIter->GetNext());
		pIter->SetNext(0);
		delete pIter;
	}
}

template<class T_Data>
inline void BSSimpleList<T_Data>::InsertSorted(const T_Data& aItem, int32_t(__cdecl* apCompare)(const T_Data& aItem1, const T_Data& aItem2)) {
	if (!aItem)
		return;

	if (!this)
		return;

	BSSimpleList<T_Data>* pIter = this;
	BSSimpleList<T_Data>* pPrev = nullptr;
	bool bExit = false;

	while (true) {
		if (bExit)
			return;

		if (!pIter->GetItem())
			break;

		if (apCompare(aItem, pIter->GetItem()) <= 0) {
			if (pPrev)
				pPrev->SetNext(new BSSimpleList<T_Data>(aItem, pIter));
			else
				AddHead(aItem);
			goto EXIT_LOOP;
		}

		if (!pIter->GetNext()) {
			pIter->SetNext(new BSSimpleList<T_Data>(aItem));
			goto EXIT_LOOP;
		}

	NEXT:
		pPrev = pIter;
		pIter = pIter->GetNext();

		if (!pIter)
			return;
	}

	pIter->SetItem(aItem);

EXIT_LOOP:
	bExit = true;
	goto NEXT;
}