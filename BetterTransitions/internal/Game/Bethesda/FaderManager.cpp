#include "FaderManager.hpp"
#include "Fader.hpp"

FaderManager* FaderManager::GetSingleton() {
    return *reinterpret_cast<FaderManager**>(0x11D8804);
}

Fader* FaderManager::GetFader(FADER_TYPE aeFader) {
    return &reinterpret_cast<Fader*>(0x11D8828)[aeFader];
}

// GAME - 0x7014E0
float FaderManager::GetFaderAlpha(FADER_TYPE aeFader) const {
	return GetFader(aeFader)->fAlpha;
}

// GAME - 0x7014A0
bool FaderManager::IsFaderActive(FADER_TYPE aeFader) const {
    const Fader* pFader = GetFader(aeFader);
    return pFader->pRoot && pFader->bFadingIn;
}

// GAME - 0x701450
bool FaderManager::IsFaderVisible(FADER_TYPE aeFader) const {
    const Fader* pFader = GetFader(aeFader);
    return pFader->pRoot && pFader->bFadingIn && pFader->fAlpha == 1.f;
}

// GAME - 0x701400
bool FaderManager::IsFadingIn(FADER_TYPE aeFader) const {
    const Fader* pFader = GetFader(aeFader);
    return pFader->pRoot && pFader->bFadingIn && pFader->fAlpha < 1.f;
}

// GAME - 0x700960
void FaderManager::CreateFader(FADER_TYPE aeFader, float afFadeSpeed, bool abUnk) {
    ThisCall(0x700960, this, aeFader, afFadeSpeed, abUnk);
}

// GAME - 0x7010E0
void FaderManager::RemoveFader(FADER_TYPE aeFader, bool abForce) {
    ThisCall(0x7010E0, this, aeFader, abForce);
}
