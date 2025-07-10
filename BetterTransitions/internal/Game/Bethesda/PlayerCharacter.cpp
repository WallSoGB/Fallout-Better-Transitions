#include "PlayerCharacter.hpp"

PlayerCharacter* PlayerCharacter::GetSingleton() {
	return *reinterpret_cast<PlayerCharacter**>(0x011DEA3C);
}

// GAME - 0x950A60
Animation* PlayerCharacter::GetPlayerAnimation(bool abFirstPerson) const {
	return ThisCall<Animation*>(0x950A60, this, abFirstPerson);
}

// GAME - 0x93BE30
void PlayerCharacter::RequestPositionPlayer(PositionRequest* apTargetLoc) {
	ThisCall(0x93BE30, this, apTargetLoc);
}

// GAME - 0x93BEA0
bool PlayerCharacter::HandlePositionPlayerRequest() {
	return ThisCall<bool>(0x93BEA0, this);
}

// GAME - 0x47AAF0
void PlayerCharacter::SetPreventActivate(bool abPrevent) {
	bPreventActivate = abPrevent;
	if (abPrevent)
		fPreventActivateTimer = 5.f;
	else
		fPreventActivateTimer = 0.f;
}
