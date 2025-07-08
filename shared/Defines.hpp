#pragma once

#pragma warning(disable: 4100 4201 4244 4324 4389 5054)

#include <d3d9.h>
#include <d3dx9math.h>

#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <stdint.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <cassert>

#define JIP_CHANGES 1
#define USE_DXMATH 1
#define USE_STB_SPRINTF 0

#if USE_STB_SPRINTF
#include <stb/stb_sprintf.h>

static const auto our_snprintf	= stbsp_snprintf;
static const auto our_sprintf	= stbsp_sprintf;
static const auto our_vsprintf	= stbsp_vsprintf;
static const auto our_vsnprintf	= stbsp_vsnprintf;
#else
static const auto our_snprintf	= snprintf;
static const auto our_sprintf	= sprintf;
static const auto our_vsprintf	= vsprintf;
static const auto our_vsnprintf = vsnprintf;
#endif

#include "Utils/Bitfield.hpp"

#include "SafeWrite/SafeWrite.hpp"
#include "Utils/AddressPtr.hpp"

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

#define DEBUG_MESSAGES false

constexpr double M_TAU = 6.28318530717958647692;   // tau;

inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr))
		throw std::exception();
}

#ifdef _DEBUG
class DebugString : public std::string {
public:
	__forceinline std::string get() {
		return *this;
	}
};
#else
class DebugString {
public:
	DebugString() : string() {}
	DebugString(const char* str) : string(str) {}

	void* fakeProxy = nullptr;
	std::string string;

	__forceinline std::string get() const {
		return string;
	}
};
#endif

constexpr float fHK2NI = 6.999125671386719f;
constexpr float fNI2HK = 1.f / fHK2NI;

template <typename T_Ret = uint32_t, typename ...Args>
__forceinline T_Ret ThisCall(uint32_t _addr, const void* _this, Args ...args) {
	return ((T_Ret(__thiscall*)(const void*, Args...))_addr)(_this, std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret StdCall(uint32_t _addr, Args ...args) {
	return ((T_Ret(__stdcall*)(Args...))_addr)(std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret CdeclCall(uint32_t _addr, Args ...args) {
	return ((T_Ret(__cdecl*)(Args...))_addr)(std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret FastCall(uint32_t _addr, Args ...args) {
	return ((T_Ret(__fastcall*)(Args...))_addr)(std::forward<Args>(args)...);
}

#pragma region Macros
#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

#define ALIGN1	alignas(1)
#define ALIGN2	alignas(2)
#define ALIGN4	alignas(4)
#define ALIGN8	alignas(8)
#define ALIGN16 alignas(16)

// KiB from bytes
#define B_KiB(x) (x##u << 10u)

// MiB from bytes
#define B_MiB(x) (x##u << 20u)

// GiB from bytes
#define B_GiB(x) (x##u << 30u)

#ifdef NDEBUG
#define ASSUME_ASSERT(x) __assume(x)
#else
#define ASSUME_ASSERT(x) assert(x)
#endif

#define ASSERT_SIZE(a, b) static_assert(sizeof(a) == b, "Wrong structure size!");
#define ASSERT_OFFSET(a, b, c) static_assert(offsetof(a, b) == c, "Wrong member offset!");

#define CREATE_OBJECT(CLASS, ADDRESS) static CLASS* CreateObject() { return CdeclCall<CLASS*>(ADDRESS); };

#ifdef GAME 
#define FUNCTION_UNKOWN static_assert(false, "Find \"" __FUNCSIG__ "\" in the game" );
#define FUNCTION_NOT_IMPLEMENTED static_assert(false, "Implement \"" __FUNCSIG__ "\" in the game" );
#else
#define FUNCTION_UNKOWN static_assert(false, "Find \"" __FUNCSIG__ "\" in GECK" );
#define FUNCTION_NOT_IMPLEMENTED static_assert(false, "Implement \"" __FUNCSIG__ "\" in GECK" );
#endif
#pragma endregion