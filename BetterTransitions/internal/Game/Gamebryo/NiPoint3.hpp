#pragma once

#include "Bethesda/BSMemObject.hpp"

class NiStream;

class NiPoint3 {
public:
	float x;
	float y;
	float z;
	
	NiPoint3() : x(0.f), y(0.f), z(0.f) {};
	NiPoint3(float x, float y, float z) : x(x), y(y), z(z) {};
	NiPoint3(float f) : x(f), y(f), z(f) {};
	NiPoint3(int32_t x, int32_t y, int32_t z) : x(float(x)), y(float(y)), z(float(z)) {};
	NiPoint3(int32_t f) : x(float(f)), y(float(f)), z(float(f)) {};
	NiPoint3(const NiPoint3& src) : x(src.x), y(src.y), z(src.z) {};
	NiPoint3(const NiPoint3* src) : x(src->x), y(src->y), z(src->z) {};
	
	auto operator<=>(const NiPoint3&) const = default;

	bool operator==(const NiPoint3& arOther) const {
		return x == arOther.x && y == arOther.y && z == arOther.z;
	}

#ifdef __D3DX9MATH_INL__
	D3DXVECTOR3 AsD3DXVECTOR3() const { return (D3DXVECTOR3)*this; };
#endif

	inline float operator[] (uint32_t i) const { return ((float*)&x)[i]; };

	operator float* () const { return (float*)this; };

	NiPoint3& operator= (const NiPoint3& arOther) {
		x = arOther.x;
		y = arOther.y;
		z = arOther.z;
		return *this;
	};

	NiPoint3& operator= (const NiPoint3* arOther) {
		x = arOther->x;
		y = arOther->y;
		z = arOther->z;
		return *this;
	};

	NiPoint3 operator+ (const NiPoint3& arOther) const { return NiPoint3(x + arOther.x, y + arOther.y, z + arOther.z); };
	NiPoint3& operator+= (const NiPoint3& arOther) {
		x += arOther.x;
		y += arOther.y;
		z += arOther.z;
		return *this;
	};

	NiPoint3 operator- (const NiPoint3& arOther) const { return NiPoint3(x - arOther.x, y - arOther.y, z - arOther.z); };
	NiPoint3 operator- () const { return NiPoint3(-x, -y, -z); };
	NiPoint3& operator-= (const NiPoint3& arOther) {
		x -= arOther.x;
		y -= arOther.y;
		z -= arOther.z;
		return *this;
	};

	float operator* (const NiPoint3& arOther) const { return x * arOther.x + y * arOther.y + z * arOther.z; };

	NiPoint3 operator* (float fScalar) const { return NiPoint3(fScalar * x, fScalar * y, fScalar * z); };
	friend NiPoint3 operator* (float fScalar, const NiPoint3& arOther) { return NiPoint3(fScalar * arOther.x, fScalar * arOther.y, fScalar * arOther.z); };
	NiPoint3& operator*= (float fScalar) {
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		return *this;
	};



	NiPoint3 operator/ (float fScalar) const {
		float fInvScalar = 1.0f / fScalar;
		return NiPoint3(fInvScalar * x, fInvScalar * y, fInvScalar * z);
	};

	NiPoint3& operator/= (float fScalar) {
		x /= fScalar;
		y /= fScalar;
		z /= fScalar;
		return *this;
	};

	float Length() const;

	float SqrLength() const;

	float Dot(const NiPoint3& arOther) const;

	NiPoint3 Cross(const NiPoint3& arOther) const;

	void Unitize();

	NiPoint3 UnitCross(const NiPoint3& arOther) const;

	float Distance(const NiPoint3& arOther) const;

	static void PointsPlusEqualFloatTimesPoints(NiPoint3* __restrict apDst, float afMult, const NiPoint3* __restrict apSrc, uint32_t auiVerts);

	void Lerp(const NiPoint3& to, const NiPoint3& from, float by) {
		x = to.x * (1 - by) + from.x * by;
		y = to.y * (1 - by) + from.y * by;
		z = to.z * (1 - by) + from.z * by;
	}

	NiPoint3& Lerp(const NiPoint3& to, const NiPoint3& from, const NiPoint3& by) {
		x = to.x * (1 - by.x) + from.x * by.x;
		y = to.y * (1 - by.y) + from.y * by.y;
		z = to.z * (1 - by.z) + from.z * by.z;
		return *this;
	}

	float GetLargest() const {
		float largest = x;
		if (y > largest)
			largest = y;
		if (z > largest)
			largest = z;
		return largest;
	}

	static float Sign(const NiPoint3& p1, const NiPoint3& p2, const NiPoint3& p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	static bool PointInTriangle(const NiPoint3& arOther, const NiPoint3& v1, const NiPoint3& v2, const NiPoint3& v3) {
		bool b1 = Sign(arOther, v1, v2) < 0.0;
		bool b2 = Sign(arOther, v2, v3) < 0.0;
		bool b3 = Sign(arOther, v3, v1) < 0.0;

		return (b1 == b2) && (b2 == b3);
	}

	static NiPoint3 GetTriangleCenter(const NiPoint3& v1, const NiPoint3& v2, const NiPoint3& v3) {
		return NiPoint3((v1.x + v2.x + v3.x) / 3.0f, (v1.y + v2.y + v3.y) / 3.0f, (v1.z + v2.z + v3.z) / 3.0f);
	}

	static float GetZAngleFromVector(const NiPoint3& arVector);

	void LoadBinary(NiStream& arStream);
	void SaveBinary(NiStream& arStream) const;

	static const NiPoint3 UNIT_X;
	static const NiPoint3 UNIT_Y;
	static const NiPoint3 UNIT_Z;
	static const NiPoint3 UNIT_ALL;
	static const NiPoint3 ZERO;
};

ASSERT_SIZE(NiPoint3, 0xC);