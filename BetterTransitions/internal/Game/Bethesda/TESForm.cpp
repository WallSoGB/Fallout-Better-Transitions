#include "TESForm.hpp"

// GAME - 0x84E3A0
uint32_t TESForm::GetFormID() const{
	return uiFormID;
}

// GAME - 0x401170
FORM_TYPE TESForm::GetFormType() const {
	return static_cast<FORM_TYPE>(eFormType);
}

// GAME - 0x4013E0
bool TESForm::GetInitialized() const {
	return uiFormFlags.IsSet(FormFlags::INITIALIZED);
}

// GAME - 0x440D80
bool TESForm::GetDeleted() const {
	return uiFormFlags.IsSet(FormFlags::DELETED);
}

// GAME - 0x440DA0
bool TESForm::GetDisabled() const {
	return uiFormFlags.IsSet(FormFlags::DISABLED);
}

// GAME - 0x576D30
bool TESForm::IsStillLoading() const {
	return uiFormFlags.IsSet(FormFlags::STILL_LOADING);
}