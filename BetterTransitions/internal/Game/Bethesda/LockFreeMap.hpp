#pragma once

#include "BSSpinLock.hpp"
#include "InterfacedClass.hpp"
#include "ThreadSpecificInterfaceManager.hpp"

template<typename K, typename T>
class LockFreeMap : public InterfacedClass {
public:
	LockFreeMap(uint32_t auiMaxThreads, uint32_t auiInitialSize, uint32_t auiDeleteBatchSize);
	~LockFreeMap() override;

	virtual bool		GetAt(K aKey, T& aData) const;
	virtual bool		SetAt(uint32_t auiBucket, K aKey, T& aData, bool abReplaceExisting);
	virtual bool		SetAtAlt(K aKey, T& aData, bool abReplaceExisting);
	virtual bool		RemoveAt(K aKey);
	virtual void		Remove(uint32_t auiBucket, T& aData);
	virtual void		RemoveAlt(T& aData);
	virtual bool		GetAtAlt(K aKey, T& aData); // Same as GetAt, but no lock
	virtual uint32_t	GetBucketForKey(K key);
	virtual void		DeleteKey(K aKey);
	virtual K			GenerateKey(K aKey);
	virtual K			CopyKey(K aKey, K& aData);
	virtual bool		IsKeyGreaterOrEqual(K aKey1, K aKey2) const;
	virtual bool		IsKeyEqual(K aKey1, K aKey2) const;
	virtual uint32_t	IncrementCount();
	virtual uint32_t	DecrementCount();
	virtual uint32_t	GetCount() const;

	template<typename PTR>
	struct MarkedPointer {
		MarkedPointer() {};
		MarkedPointer(bool abMark, PTR* apValue) : pPointer(apValue) { SetMark(abMark); }

		PTR* pPointer = nullptr;

		bool GetMark() const {
			return reinterpret_cast<size_t>(pPointer) & 1;
		}

        void SetMark(bool abMark) {  
            if (abMark)  
                pPointer = reinterpret_cast<PTR*>(reinterpret_cast<size_t>(pPointer) | 1);  
            else  
                pPointer = reinterpret_cast<PTR*>(reinterpret_cast<size_t>(pPointer) & ~1);  
        }

		void Clear() {
			pPointer = nullptr;
		}

		PTR* GetPointer() {
			return reinterpret_cast<PTR*>(reinterpret_cast<size_t>(pPointer) & ~1u);
		}

		PTR* operator->() {
			return GetPointer();
		}
	};

	struct HashListNode {
		K							Key;
		T							Data;
		MarkedPointer<HashListNode>	pNext;
	};

	class LockFreeMapInterface {
	public:
		LockFreeMapInterface(class LockFreeMap* apOwner, HashListNode** apNode0, HashListNode** apNode1, HashListNode** apNode2);

		LockFreeMap*							pOwner				= nullptr;
		HashListNode**							pReferencedNodes[3] = {};
		MarkedPointer<HashListNode>*			pPrev				= nullptr;
		MarkedPointer<HashListNode>				kCurrent;
		MarkedPointer<HashListNode>				kNext;
		HashListNode*							pDeleteHead			= nullptr;
		uint32_t								uiDeleteCount		= 0;
	};

	typedef ThreadSpecificInterfaceManager<LockFreeMapInterface> Manager;

	HashListNode**					pReferencedNodes;
	uint32_t						uiNumBuckets;
	MarkedPointer<HashListNode>*	pBuckets;
	uint32_t						uiDeleteBatchSize;
	Manager*						pManager;
	uint32_t						uiCount;
	uint32_t						unk01C;
	mutable BSSpinLock				kLock;

};

template<class T>
class LockFreeStringMap : public LockFreeMap<const char*, T> {
public:
};

template<class T>
class LockFreeCaseInsensitiveStringMap : public LockFreeStringMap<T> {};

ASSERT_SIZE(LockFreeCaseInsensitiveStringMap<char>, 0x40);
ASSERT_SIZE(LockFreeCaseInsensitiveStringMap<char>::LockFreeMapInterface, 0x24);