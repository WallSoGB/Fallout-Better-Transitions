#pragma once

#include "BSMemObject.hpp"

// Clone of NiTMapBase lmao

typedef void* BSMapIterator;

template <class T_Key, class T_Data>
class BSMapItem {
public:
    BSMapItem*  m_pkNext;
    T_Key       m_key;
    T_Data      m_val;
};

template <class T_Key, class T_Data>
class BSMapBase {
public:
    typedef BSMapItem<T_Key, T_Data> Entry;

    BSMapBase(uint32_t uiHashSize = 37);
	BSMapBase(const BSMapBase&) = delete;
	BSMapBase& operator=(const BSMapBase&) = delete;

    virtual                             ~BSMapBase();
    virtual uint32_t                    KeyToHashIndex(T_Key key) const;
    virtual bool                        IsKeysEqual(T_Key key1, T_Key key2) const;
    virtual void                        SetValue(BSMapItem<T_Key, T_Data>* apItem, T_Key key, T_Data data);
    virtual void                        ClearValue(BSMapItem<T_Key, T_Data>* apItem);
    virtual BSMapItem<T_Key, T_Data>*   NewItem() = 0;
    virtual void                        DeleteItem(BSMapItem<T_Key, T_Data>* apItem) = 0;

    uint32_t                    m_uiHashSize;
    BSMapItem<T_Key, T_Data>**  m_ppkHashTable;
    uint32_t                    m_uiCount;

    uint32_t GetCount() const { return m_uiCount; }

    bool GetAt(T_Key key, T_Data& dataOut) {
        uint32_t hashIndex = KeyToHashIndex(key);
        BSMapItem<T_Key, T_Data>* item = m_ppkHashTable[hashIndex];
        while (item) {
            if (IsKeysEqual(item->m_key, key)) {
                dataOut = item->m_val;
                return true;
            }
            item = item->m_pkNext;
        }
        return false;
    }

    void SetAt(T_Key key, T_Data data) {
        uint32_t hashIndex = KeyToHashIndex(key);
        BSMapItem<T_Key, T_Data>* item = m_ppkHashTable[hashIndex];
        while (item) {
            if (IsKeysEqual(item->m_key, key)) {
                item->m_val = data;
                return;
            }
            item = item->m_pkNext;
        }
        BSMapItem<T_Key, T_Data>* nuEntry = NewItem();
        SetValue(nuEntry, key, data);
        nuEntry->m_pkNext = m_ppkHashTable[hashIndex];
        m_ppkHashTable[hashIndex] = nuEntry;
        m_uiCount++;
    }

    bool Insert(BSMapItem<T_Key, T_Data>* nuEntry) {
        // game code does not appear to care about ordering of entries in buckets
        uint32_t bucket = nuEntry->m_key % m_uiHashSize;
        BSMapItem<T_Key, T_Data>* prev = nullptr;
        for (BSMapItem<T_Key, T_Data>* cur = m_ppkHashTable[bucket]; cur; cur = cur->m_pkNext) {
            if (cur->m_key == nuEntry->m_key) {
                return false;
            }
            else if (!cur->m_pkNext) {
                prev = cur;
                break;
            }
        }

        if (prev) {
            prev->m_pkNext = nuEntry;
        }
        else {
            m_ppkHashTable[bucket] = nuEntry;
        }

        m_uiHashSize++;
        return true;
    }

    void RemoveAll() {
        for (uint32_t i = 0; i < m_uiHashSize; i++) {
            while (m_ppkHashTable[i]) {
                BSMapItem<T_Key, T_Data>* item = m_ppkHashTable[i];
                m_ppkHashTable[i] = m_ppkHashTable[i]->m_pkNext;
                ClearValue(item);
                DeleteItem(item);
            }
        }

        m_uiCount = 0;
    }

    BSMapIterator GetFirstPos() const {
        for (uint32_t i = 0; i < m_uiHashSize; i++) {
            if (m_ppkHashTable[i])
                return m_ppkHashTable[i];
        }
        return 0;
    }

    void GetNext(BSMapIterator& pos, T_Key& key, T_Data& val) {
        BSMapItem<T_Key, T_Data>* item = (BSMapItem<T_Key, T_Data>*) pos;

        key = item->m_key;
        val = item->m_val;

        if (item->m_pkNext) {
            pos = item->m_pkNext;
            return;
        }

        uint32_t i = KeyToHashIndex(item->m_key);
        for (++i; i < m_uiHashSize; i++) {
            item = m_ppkHashTable[i];
            if (item) {
                pos = item;
                return;
            }
        }

        pos = 0;
    }

    bool RemoveAt(const T_Key key) {
        uint32_t uiIndex = KeyToHashIndex(key);
        BSMapItem<T_Key, T_Data>* pItem = m_ppkHashTable[uiIndex];

        if (!pItem)
            return false;

        if (IsKeysEqual(key, pItem->m_key)) {
            m_ppkHashTable[uiIndex] = pItem->m_pkNext;
            ClearValue(pItem);
            DeleteItem(pItem);
            --m_uiCount;
            return true;
        }

        BSMapItem<T_Key, T_Data>* pPrev = pItem;
        BSMapItem<T_Key, T_Data>* i = pItem->m_pkNext;

        for (i; i && !IsKeysEqual(key, i->m_key); i = i->m_pkNext)
            pPrev = i;

        if (!i)
            return false;

        pPrev->m_pkNext = i->m_pkNext;
        ClearValue(i);
        DeleteItem(i);
        --m_uiCount;
        return true;
    }
};

template<class T_Key, class T_Data>
inline BSMapBase<T_Key, T_Data>::BSMapBase(uint32_t uiHashSize) {
    m_uiHashSize = uiHashSize;
    m_uiCount = 0;
    m_ppkHashTable = nullptr;

    if (!m_uiHashSize)
        return;

    uint32_t uiSize = sizeof(BSMapItem<T_Key, T_Data>*) * m_uiHashSize;
    m_ppkHashTable = new BSMapItem<T_Key, T_Data>* [uiSize];
    memset(m_ppkHashTable, 0, m_uiHashSize * sizeof(BSMapItem<T_Key, T_Data>*));
};

template<class T_Key, class T_Data>
inline BSMapBase<T_Key, T_Data>::~BSMapBase() {
   BSFree(m_ppkHashTable);
}

template<class T_Key, class T_Data>
inline uint32_t BSMapBase<T_Key, T_Data>::KeyToHashIndex(T_Key key) const {
    return (uint32_t)(((size_t)key) % m_uiHashSize);
}

template<class T_Key, class T_Data>
inline bool BSMapBase<T_Key, T_Data>::IsKeysEqual(T_Key key1, T_Key key2) const {
    return key1 == key2;
}

template<class T_Key, class T_Data>
inline void BSMapBase<T_Key, T_Data>::SetValue(BSMapItem<T_Key, T_Data>* apItem, T_Key key, T_Data data) {
    apItem->m_key = key;
    apItem->m_val = data;
}

template<class T_Key, class T_Data>
inline void BSMapBase<T_Key, T_Data>::ClearValue(BSMapItem<T_Key, T_Data>* apItem) {
}
