#pragma once

#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class TESFile;
class TESForm;

class TESFullName : public BaseFormComponent {
public:
	TESFullName();
	~TESFullName();

	BSString	strFullName;

	const char* c_str() const;

	const char* GetFullName() const;
	static const char* GetFullName(const TESForm* apForm);

	void Save();
	static void LoadFullNameChunk(TESFullName* apFullName, TESFile* apFile);
};

ASSERT_SIZE(TESFullName, 0xC);