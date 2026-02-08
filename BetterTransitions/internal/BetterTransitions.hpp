#pragma once

struct NVSEInterface;

namespace BetterTransitions {
	void InitSettings();
	void InitHooks();
	void InitMessageHandler(NVSEInterface* apNVSE);
}