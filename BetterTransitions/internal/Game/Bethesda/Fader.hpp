#pragma once

#include "Gamebryo/NiSmartPointer.hpp"

class NiNode;
class TileShaderProperty;

class Fader {
public:
	bool							bBelowMenus;
	const char*						pFileName;
	bool							bFadingIn;
	float							fFadeSpeed;
	NiNode*							pRoot;
	float							fAlpha;
	NiPointer<TileShaderProperty>	spShaderProp;
};

ASSERT_SIZE(Fader, 0x1C);