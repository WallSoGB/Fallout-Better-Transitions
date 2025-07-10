#include "nvse/PluginAPI.h"

#include "BetterTransitions.hpp"

constexpr char const* PLUGIN_NAME = "Better Transitions";
constexpr uint32_t PLUGIN_VERSION = 110;

#include "Bethesda/BSMemory.hpp"
BS_ALLOCATORS


EXTERN_DLL_EXPORT bool NVSEPlugin_Query(const NVSEInterface* apNVSE, PluginInfo* apInfo) {
	apInfo->infoVersion = PluginInfo::kInfoVersion;
	apInfo->name = PLUGIN_NAME;
	apInfo->version = PLUGIN_VERSION;

	return !apNVSE->isEditor;
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Load(NVSEInterface* apNVSE) {
	BetterTransitions::InitHooks();
	return true;
}

BOOL WINAPI DllMain(
	HANDLE  hDllHandle,
	DWORD   dwReason,
	LPVOID  lpreserved
)
{
	return TRUE;
}