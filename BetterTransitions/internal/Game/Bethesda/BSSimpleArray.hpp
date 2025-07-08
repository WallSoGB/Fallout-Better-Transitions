#pragma once

#include "BSMemObject.hpp"

template <typename T, uint32_t RESIZE_SIZE = 1024>
class BSSimpleArray {
public:
	BSSimpleArray() { InitialSetup(0, 0); }
	BSSimpleArray(uint32_t auiAllocSize, uint32_t auiSize) { InitialSetup(auiAllocSize, auiSize); }
	BSSimpleArray(uint32_t auiSize) { InitialSetup(0, auiSize); }
	BSSimpleArray(const BSSimpleArray&) = delete;
	BSSimpleArray& operator=(const BSSimpleArray&) = delete;

	virtual			~BSSimpleArray() { Clear(); };
	virtual T*		Allocate(uint32_t auiCount) { return BSNew<T>(auiCount); };
	virtual void    Deallocate(T* apData) { BSFree(apData); };
	virtual T*		Reallocate(T* apData, uint32_t auiCount) { return (T*)BSReallocate(apData, sizeof(T) * auiCount); };

	T*			pBuffer;
	uint32_t	uiSize;
	uint32_t	uiAllocSize;

	uint32_t GetSize() const { return uiSize; }
	uint32_t GetAllocSize() const { return uiAllocSize; }
	bool IsEmpty() const { return uiSize == 0; }
	bool IsFull() const { return uiSize == uiAllocSize; }
	T& GetAt(const uint32_t idx) const { return pBuffer[idx]; }
	T& GetLast() { return pBuffer[uiSize - 1]; }

	class Iterator {
	public:
		Iterator(T* node) : m_node(node) {}

		T& operator*() { return *m_node; }
		const T& operator*() const { return *m_node; }

		Iterator& operator++() {
			m_node++;
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return m_node != other.m_node;
		}

	private:
		T* m_node;
	};

	Iterator begin() { return Iterator(&GetAt(0)); }
	Iterator end() { return Iterator(&GetLast()); }

	template <typename FUNC, typename... ARGS>
	void ForEach(FUNC&& func, ARGS... args) {
		for (uint32_t i = 0; i < uiSize; i++)
			func(GetAt(i), args...);
	}

	// 0x822780
	void ConstructItems(T* apData, uint32_t auiCount) {
		for (uint32_t i = 0; i < auiCount; i++)
			new (&apData[i]) T;
	}

	// 0x822820
	void DestructItems(T* apData, uint32_t auiCount) {
		for (uint32_t i = 0; i < auiCount; ++i)
			apData[i].~T();
	}

	void InitialSetup(uint32_t auiAllocSize, uint32_t auiSize) {
		pBuffer = nullptr;
		uiSize = 0;
		uiAllocSize = 0;

		if (auiSize > auiAllocSize)
			auiAllocSize = auiSize;

		if (auiAllocSize) {
			pBuffer = Allocate(auiAllocSize);
			uiAllocSize = auiAllocSize;
		}

		if (auiSize) {
			ConstructItems(pBuffer, auiSize);
			uiSize = auiSize;
		}
	}

	// 0x6A8500
	void FreeArray() {
		Deallocate(pBuffer);
		pBuffer = nullptr;
	}

	// 0x6C6200
	void Clear(bool abFree = true) {
		if (!pBuffer)
			return;

		DestructItems(pBuffer, uiSize);

		if (abFree) {
			FreeArray();
			pBuffer = nullptr;
			uiAllocSize = 0;
		}

		uiSize = 0;
	}

	__declspec(noalias) void CopyItems(T* apNewBuffer, T* apOldBuffer, uint32_t auiSize) {
        if (!auiSize)  
            return;  

        if (apNewBuffer < apOldBuffer) {  
            for (uint32_t i = 0; i < auiSize; ++i)  
                memcpy(&apNewBuffer[i], &apOldBuffer[i], sizeof(T));  
        }  
        else if (apNewBuffer > apOldBuffer) {  
            for (int32_t i = static_cast<int32_t>(auiSize) - 1; i >= 0; --i)  
                memcpy(&apNewBuffer[i], &apOldBuffer[i], sizeof(T));  
        }  
    }

	// 0x42FC00
	void Resize(uint32_t auiNewSize, uint32_t auiSize) {
		if (!pBuffer) {
			pBuffer = Allocate(auiNewSize);
			uiAllocSize = auiNewSize;
		}
		else if (auiSize == uiAllocSize) {
			pBuffer = Reallocate(pBuffer, auiNewSize);
		}
		else {
			T* pNewBuffer = Allocate(auiNewSize);
			CopyItems(pNewBuffer, pBuffer, auiSize);
			FreeArray();
			pBuffer = pNewBuffer;
		}
	}

	// 0x695230
	void SetSize(uint32_t auiSize, bool abFree = true) {
		if (!auiSize) {
			Clear(abFree);
		}
		else if (auiSize > uiAllocSize) {
			if (uiAllocSize)
				Resize(auiSize, uiSize);
			else
				pBuffer = Allocate(auiSize);

			uiAllocSize = auiSize;
			ConstructItems(&pBuffer[uiSize], auiSize - uiSize);
			uiSize = auiSize;
		}
		else if (auiSize >= uiSize) {
			ConstructItems(&pBuffer[uiSize], auiSize - uiSize);
			uiSize = auiSize;
		}
		else {
			DestructItems(&pBuffer[auiSize], uiSize - auiSize);
			uiSize = auiSize;
			if (abFree && auiSize <= uiAllocSize >> 2) {
				Resize(auiSize, auiSize);
				uiAllocSize = auiSize;
			}
		}
	}

	// 0x6F9790, 0x84B5C0
	void SetAllocSize(uint32_t auiAllocSize) {
		if (auiAllocSize == uiAllocSize)
			return;

		if (auiAllocSize < uiSize) {
			uiSize = auiAllocSize;
		}
		Resize(auiAllocSize, uiSize);
		uiAllocSize = auiAllocSize;
	}

	// 0x9A3910
	uint32_t GetResizeSize() {
		if (uiAllocSize <= RESIZE_SIZE)
			return 2 * uiAllocSize;
		else
			return uiAllocSize + RESIZE_SIZE;
	}

	// GAME - 0x761540, 0xB5C360
	uint32_t GetNextIndex() {
		if (!IsFull())
			return uiSize++;

		if (uiAllocSize) {
			uint32_t uiResizeSize = GetResizeSize();
			Resize(uiResizeSize, uiSize);
			uiAllocSize = uiResizeSize;
		}
		else {
			constexpr uint32_t DEFAULT_ALLOC_SIZE = 4;
			pBuffer = Allocate(DEFAULT_ALLOC_SIZE);
			uiAllocSize = DEFAULT_ALLOC_SIZE;
		}

		return uiSize++;
	}

	// GAME - 0x7CB2E0
	uint32_t Add(const T& aItem) {
		uint32_t uiNext = GetNextIndex();
		ConstructItems(&pBuffer[uiNext], 1);
		pBuffer[uiNext] = aItem;
		return uiNext;
	}

	// GAME - 0x6DB540
	void SetAt(uint32_t auiIndex, const T& aItem) {
		if (auiIndex == uiSize) {
			Add(aItem);
		}
		else if (IsFull()) {
			uint32_t uiResizeSize = GetResizeSize();
			T* pNewBuffer = Allocate(uiResizeSize);
			CopyItems(pNewBuffer, pBuffer, auiIndex);
			ConstructItems(&pBuffer[auiIndex], 1);
			CopyItems(&pNewBuffer[auiIndex + 1], &pBuffer[auiIndex], uiSize - auiIndex);
			FreeArray();
			pBuffer = pNewBuffer;
			uiAllocSize = uiResizeSize;
		}
		else {
			CopyItems(&pBuffer[auiIndex + 1], &pBuffer[auiIndex], uiSize - auiIndex);
			ConstructItems(&pBuffer[auiIndex], 1);
		}
		++uiSize;
		pBuffer[auiIndex] = aItem;
	}

	bool HasSufficientCapacity() {
		if (uiAllocSize <= RESIZE_SIZE) {
			if (uiSize <= uiAllocSize >> 2)
				return true;
		}
		else if (uiSize <= (uiAllocSize - RESIZE_SIZE * 2))
			return true;

		return false;
	}

	uint32_t GetOptimalCapacity() {
		if (uiAllocSize <= RESIZE_SIZE * 2)
			return uiAllocSize >> 1;
		else
			return uiAllocSize - RESIZE_SIZE;
	}

	void MoveItems(T* apNewBuffer, const T* apOldBuffer, const uint32_t auiSize) {
		if (!auiSize)
			return;

		if (apNewBuffer < apOldBuffer) {
			for (uint32_t j = 0; j < auiSize; ++j)
				memcpy(&apNewBuffer[j], &apOldBuffer[j], sizeof(T));
		}
		else if (apNewBuffer > apOldBuffer) {
			for (uint32_t i = auiSize - 1; i >= 0; --i)
				memcpy(&apNewBuffer[i], &apOldBuffer[i], sizeof(T));
		}
	}

	void RemoveAt(uint32_t auiIndex, bool abResize) {
		if (abResize && BSSimpleArray::HasSufficientCapacity()) {
			uint32_t uiOptimalCapacity = GetOptimalCapacity();
			T* pNewBuffer = Allocate(uiOptimalCapacity);
			MoveItems(pNewBuffer, pBuffer, auiIndex);
			DestructItems(&pBuffer[auiIndex], 1);
			MoveItems(&pNewBuffer[auiIndex], &pBuffer[auiIndex + 1], uiSize - 1);
			FreeArray();
			pBuffer = pNewBuffer;
			uiAllocSize = uiOptimalCapacity;
		}
		else
		{
			DestructItems(&pBuffer[auiIndex], 1);
			MoveItems(&pBuffer[auiIndex], &pBuffer[auiIndex + 1], uiSize - auiIndex - 1);
		}
		--uiSize;
	}

	// GAME - 0x98F1F0
	void RemoveFast(uint32_t auiIndex, uint32_t auiCount) {
		uint32_t uiToMove = std::min(auiCount, uiSize - auiIndex - auiCount);
		DestructItems(&pBuffer[auiIndex], auiCount);
		MoveItems(&pBuffer[auiIndex], &pBuffer[uiSize - uiToMove], uiToMove);
		uiSize -= auiCount;
	}

	// GAME - 0x719B20
	int32_t Find(const T& aItem, uint32_t auiStartPosition, bool(__cdecl* apCompareFunc)(const T&, const T&)) const {
		while (auiStartPosition < uiSize) {
			if (pBuffer && apCompareFunc(pBuffer[auiStartPosition], aItem))
				return auiStartPosition;
			++auiStartPosition;
		}
		return -1;
	}

	// GAME - 0x9962F0
	bool IsInArray(const T& aItem) const {
		return Find(aItem, 0, [](const T& left, const T& right) -> bool { return left == right; }) != -1;
	}

	// GAME - 0x6C8980
	void Sort(_CoreCrtNonSecureSearchSortCompareFunction apCompareFunc) {
		std::qsort(pBuffer, uiSize, sizeof(T), apCompareFunc);
	}
};

ASSERT_SIZE(BSSimpleArray<uint32_t>, 0x10);