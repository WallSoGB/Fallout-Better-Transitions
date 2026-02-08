#pragma once

class NiNode;
class Fader;

enum FADER_TYPE {
	FADER_TYPE_BELOW_MENU		= 0,
	FADER_TYPE_ABOVE_MENU		= 1,
	FADER_TYPE_ABOVE_MENU_WHITE	= 2,
};

class FaderManager {
public:
	NiNode* pRootBelowMenus;
	NiNode* pRootAboveMenus;

	static FaderManager* GetSingleton();

	static Fader* GetFader(FADER_TYPE aeFader);

	float GetFaderAlpha(FADER_TYPE aeFader) const;

	bool IsFaderActive(FADER_TYPE aeFader) const;

	bool IsFaderVisible(FADER_TYPE aeFader) const;

	bool IsFadingIn(FADER_TYPE aeFader) const;

	void CreateFader(FADER_TYPE aeFader, float afFadeSpeed, bool abBeginBlack);

	void RemoveFader(FADER_TYPE aeFader, bool abForce);
};

ASSERT_SIZE(FaderManager, 0x8);