#pragma once

#include <intrin.h>

template <typename T>
class BitfieldBase {
public:
	BitfieldBase() { tField = 0; }
	~BitfieldBase() { }

	T		tField;

	void	Clear() { tField = 0; }
	void	RawSet(T data) { tField = data; }

	void	Set(T data) { tField |= data; }
	void	Clear(T data) { tField &= ~data; }
	void	Unset(T data) { Clear(data); }
	void	Mask(T data) { tField &= data; }
	void	Toggle(T data) { tField ^= data; }
	void	SetBit(T data, bool state)
	{
		if (state) Set(data); else Clear(data);
	}

	void	SetField(T data, T mask, T pos) {
		tField = (tField & ~mask) | (data << pos);
	}

	T		GetField(T mask, T pos) const {
		return (tField & mask) >> pos;
	}

	bool	Test(uint32_t bit) const {
		if constexpr (sizeof(T) == 4) {
			return _bittest((const long*)&tField, bit);
		}
		else {
			return (tField & (T(1) << bit)) != 0;
		}
	}

	bool	TestSet(uint32_t bit) {
		if constexpr (sizeof(T) == 4) {
			return _bittestandset((long*)&tField, bit);
		}
		else {
			T mask = T(1) << bit;
			bool bVal = (tField & mask) != 0;
			tField ^= mask;
			return bVal;
		}
	}

	bool	TestClear(uint32_t bit) {
		if constexpr (sizeof(T) == 4) {
			return _bittestandreset((long*)&tField, bit);
		}
		else {
			T mask = T(1) << bit;
			bool bVal = (tField & mask) != 0;
			tField &= ~mask;
			return bVal;
		}
	}

	T		Get(void) const { return tField; }
	T		GetBit(T data) const { return tField & data; }
	T		Extract(uint32_t bit) const { return (tField >> bit) & 1; }
	T		ExtractField(uint32_t shift, uint32_t length)
	{
		return (tField >> shift) & (0xFFFFFFFF >> (32 - length));
	}

	bool	IsSet(T data) const { return GetBit(data) != 0; }
	bool	IsUnSet(T data) const { return GetBit(data) == 0; }
	bool	IsClear(T data) const { return IsUnSet(data); }
};

template <typename STRUCT>
class Bitfield : public BitfieldBase<
	typename std::conditional<sizeof(STRUCT) == 1, uint8_t,
	typename std::conditional<sizeof(STRUCT) == 2, uint16_t,
	typename std::conditional<sizeof(STRUCT) == 4, uint32_t, void>::type>::type>::type> 
{
public:
	Bitfield() {};
	~Bitfield() {};

	const STRUCT& operator()() const {
		return *reinterpret_cast<const STRUCT*>(this);
	}

	STRUCT& operator()() {
		return *reinterpret_cast<STRUCT*>(this);
	}
};

typedef Bitfield<uint8_t>	Bitfield8;
typedef Bitfield<uint16_t>	Bitfield16;
typedef Bitfield<uint32_t>	Bitfield32;	