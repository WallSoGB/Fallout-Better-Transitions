#pragma once

#include "BSMemObject.hpp"


template <class T_Data>
class BSSimpleList {
public:
	BSSimpleList();
	BSSimpleList(const T_Data& item);
	BSSimpleList(const T_Data& item, BSSimpleList<T_Data>* apNext);
	BSSimpleList(const BSSimpleList<T_Data>& arEntry);
	~BSSimpleList();

	T_Data					m_item;
	BSSimpleList<T_Data>*	m_pkNext;

	const T_Data GetItem() const;
	T_Data GetItem();
	void SetItem(const T_Data& aItem);

	const BSSimpleList<T_Data>* GetNext() const;
	BSSimpleList<T_Data>* GetNext();
	void SetNext(BSSimpleList<T_Data>* apNext);

	bool IsEmpty() const;

	void AddHead(const T_Data& aItem);

	void AddTail(const T_Data& aItem);

	bool IsInList(const T_Data& aItem) const;

	BSSimpleList<T_Data>* GetPos(const T_Data& aItem) const;

	uint32_t ItemsInList() const;

	void RemoveAll();

	void RemoveHead();

	void RemoveTail();

	void Remove(const T_Data& aItem);

	void InsertSorted(const T_Data& aItem, int32_t(__cdecl* apCompare)(const T_Data& aItem1, const T_Data& aItem2));

	// Custom methods

	const BSSimpleList<T_Data>* GetHead() const { return this; }
	BSSimpleList<T_Data>* GetHead() { return this; }

	BSSimpleList<T_Data>* GetTail() const {
		BSSimpleList<T_Data>* kIter;
		for (kIter = const_cast<BSSimpleList<T_Data>*>(this); kIter->GetNext(); kIter = kIter->GetNext()) {}
		return kIter;
	}

	class Iterator {
	public:
		Iterator(BSSimpleList<T_Data>* node) : m_node(node) {}

		T_Data& operator*() { return m_node->m_item; }
		const T_Data& operator*() const { return m_node->m_item; }

		Iterator& operator++() {
			if (m_node)
				m_node = m_node->m_pkNext;
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return m_node != other.m_node;
		}

	private:
		BSSimpleList<T_Data>* m_node;
	};

	Iterator begin() { return Iterator(this); }
	Iterator end() { return Iterator(nullptr); }

	BSSimpleList<T_Data>* GetAt(uint32_t auiIndex) const {
		uint32_t i = 0;
		BSSimpleList<T_Data>* pIter = const_cast<BSSimpleList<T_Data>*>(this);
		while (pIter) {
			if (i == auiIndex)
				return pIter;

			pIter = pIter->GetNext();
			++i;
		}
		return nullptr;
	}

	int32_t GetIndexOf(const T_Data& aItem) const {
		int32_t iIndex = 0;
		const BSSimpleList<T_Data>* pIter = this;
		while (pIter) {
			if (pIter->GetItem() == aItem)
				return iIndex;

			pIter = pIter->GetNext();
			++iIndex;
		}
		return -1;
	}

	template <typename FUNC>
	int32_t GetIndexOf(const FUNC&& func) const {
		int32_t iIndex = 0;
		const BSSimpleList<T_Data>* pIter = this;
		while (pIter) {
			if (func(pIter))
				return iIndex;

			pIter = pIter->GetNext();
			++iIndex;
		}
		return -1;
	}

	BSSimpleList<T_Data>* Find(const T_Data& aItem) const {
		BSSimpleList<T_Data>* pIter = const_cast<BSSimpleList<T_Data>*>(this);
		while (pIter) {
			if (pIter->GetItem() == aItem)
				return pIter;

			pIter = pIter->GetNext();
		}
		return nullptr;
	}

	template <typename FUNC>
	BSSimpleList<T_Data>* Find(const FUNC&& func) const {
		BSSimpleList<T_Data>* pIter = const_cast<BSSimpleList<T_Data>*>(this);
		while (pIter) {
			if (func(pIter))
				return pIter;

			pIter = pIter->GetNext();
		}
		return nullptr;
	}

	template <typename FUNC>
	bool IsInList(FUNC&& func) const {
		return Find(func) != nullptr;
	}

	template <typename FUNC, typename... ARGS>
	void ForEach(FUNC&& func, ARGS... args) {
		BSSimpleList<T_Data>* pIter = const_cast<BSSimpleList<T_Data>*>(this);
		while (pIter) {
			func(pIter, args...);
			pIter = pIter->GetNext();
		}
	}

	[[nodiscard]] BSSimpleList<T_Data>* ReplaceAt(uint32_t auiIndex, const T_Data& aItem) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		BSSimpleList<T_Data>* pReplaced = nullptr;
		if (pIter) {
			pReplaced = new BSSimpleList<T_Data>(pIter->GetItem());
			pIter->SetItem(aItem);
		}
		return pReplaced;
	}

	[[nodiscard]] BSSimpleList<T_Data>* SetAt(uint32_t auiIndex, const T_Data& aItem) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		if (pIter)
			pIter->SetItem(aItem);
		else
			AddTail(aItem);
		return pIter;
	}

	BSSimpleList<T_Data>* AddAt(uint32_t auiIndex, const T_Data& aItem) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		if (pIter) {
			if (pIter->GetItem()) {
				BSSimpleList<T_Data>* pNewNode = new BSSimpleList<T_Data>(aItem);
				pNewNode->SetNext(pIter->GetNext());
				pIter->SetNext(pNewNode);
			}
			else {
				pIter->SetItem(aItem);
			}
		}
		else {
			AddTail(aItem);
		}
		return pIter;
	}

	BSSimpleList<T_Data>* RemoveAt(uint32_t auiIndex) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		if (pIter)
			Remove(pIter->GetItem());
		return pIter;
	}
};

#include "BSSimpleList.inl"

ASSERT_SIZE(BSSimpleList<uint32_t>, 0x8);