#pragma once

#include "BSSimpleList.hpp"

template<class T>
inline BSSimpleList<T>::BSSimpleList() : m_item(0), m_pkNext(nullptr) {
}

template<class T>
inline BSSimpleList<T>::BSSimpleList(const T& arItem) : m_item(arItem), m_pkNext(nullptr) {
}

template<class T>
inline BSSimpleList<T>::BSSimpleList(const T& arItem, BSSimpleList<T>* apNext) : m_item(arItem), m_pkNext(apNext) {
}

template<class T>
inline BSSimpleList<T>::BSSimpleList(const BSSimpleList<T>& arEntry) : m_item(arEntry.m_item), m_pkNext(0) {
}

template<class T>
inline BSSimpleList<T>::~BSSimpleList() {
	RemoveAll();
}

template<class T>
inline const T& BSSimpleList<T>::GetItem() const {
	return m_item;
}

template<class T>
inline T& BSSimpleList<T>::GetItem() {
	return m_item;
}

template<class T>
inline void BSSimpleList<T>::SetItem(const T& arItem) {
	m_item = arItem;
}

template<class T>
inline const BSSimpleList<T>* BSSimpleList<T>::GetNext() const {
	return m_pkNext;
}

template<class T>
inline BSSimpleList<T>* BSSimpleList<T>::GetNext() {
	return m_pkNext;
}

template<class T>
inline void BSSimpleList<T>::SetNext(BSSimpleList<T>* apNext) {
	m_pkNext = apNext;
}

// GAME - 0x8256D0
template<class T>
inline bool BSSimpleList<T>::IsEmpty() const { 
	return !m_pkNext && !m_item; 
}

// GAME - 0x631540
template<class T>
inline void BSSimpleList<T>::AddHead(const T& arItem) {
	if (!arItem)
		return;

	BSSimpleList<T>* pHead = GetHead();

	if (pHead->GetItem()) {
		BSSimpleList<T>* pNewEntry = new BSSimpleList<T>(*pHead);
		pNewEntry->SetNext(pHead->GetNext());
		pHead->SetNext(pNewEntry);
		pHead->SetItem(arItem);

	}
	else {
		pHead->SetItem(arItem);
	}
}

// GAME - 0xAF25B0, 0x905820, 0xB63BF0
template<class T>
inline void BSSimpleList<T>::AddTail(const T& arItem) {
	if (!arItem)
		return;

	BSSimpleList<T>* pTail = GetTail();

	if (pTail->GetItem()) {
		pTail->SetNext(new BSSimpleList<T>(arItem));
	}
	else {
		pTail->SetItem(arItem);
	}
}

// GAME - 0x5F65D0
template<class T>
inline bool BSSimpleList<T>::IsInList(const T& arItem) const {
	const BSSimpleList<T>* pIter = this;
	while (pIter && pIter->GetItem() != arItem)
		pIter = pIter->GetNext();
	return pIter != 0;
}

template<class T>
inline BSSimpleList<T>* BSSimpleList<T>::GetPos(const T& arItem) const {
	BSSimpleList<T>* pIter = const_cast<BSSimpleList<T>*>(this);
	while (pIter && pIter->GetItem() != arItem)
		pIter = pIter->GetNext();
	return pIter;
}

// GAME - 0x5AE380
template<class T>
inline uint32_t BSSimpleList<T>::ItemsInList() const {
	uint32_t uiCount = 0;
	const BSSimpleList<T>* pIter = this;
	while (pIter) {
		if (pIter->GetItem())
			++uiCount;

		pIter = pIter->GetNext();
	}
	return uiCount;
}

// GAME - 0xB64EC0
template<class T>
inline void BSSimpleList<T>::RemoveAll() {
	if (GetNext()) {
		BSSimpleList<T>* pNext = 0;
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
template<class T>
inline void BSSimpleList<T>::RemoveHead() {
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

template<class T>
inline void BSSimpleList<T>::RemoveTail() {
	BSSimpleList<T>* pTail = GetTail();
	if (pTail) {
		BSSimpleList<T>* pIter = this;
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
template<class T>
inline void BSSimpleList<T>::Remove(const T& arItem) {
	if (!arItem || IsEmpty())
		return;

	BSSimpleList<T>* pIter = this;
	BSSimpleList<T>* pPrev = this;

	while (pIter && pIter->GetItem() != arItem) {
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

template<class T>
inline void BSSimpleList<T>::InsertSorted(const T& arItem, int32_t(__cdecl* apCompare)(const T& arItem1, const T& arItem2)) {
	if (!arItem)
		return;

	if (!this)
		return;

	BSSimpleList<T>* pIter = this;
	BSSimpleList<T>* pPrev = nullptr;
	bool bExit = false;

	while (true) {
		if (bExit)
			return;

		if (!pIter->GetItem())
			break;

		if (apCompare(arItem, pIter->GetItem()) <= 0) {
			if (pPrev)
				pPrev->SetNext(new BSSimpleList<T>(arItem, pIter));
			else
				AddHead(arItem);
			goto EXIT_LOOP;
		}

		if (!pIter->GetNext()) {
			pIter->SetNext(new BSSimpleList<T>(arItem));
			goto EXIT_LOOP;
		}

	NEXT:
		pPrev = pIter;
		pIter = pIter->GetNext();

		if (!pIter)
			return;
	}

	pIter->SetItem(arItem);

EXIT_LOOP:
	bExit = true;
	goto NEXT;
}