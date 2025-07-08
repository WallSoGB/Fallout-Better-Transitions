#pragma once

class NiUpdateData {
public:
	explicit NiUpdateData(float afTime = 0.f, bool abUpdateControllers = false, bool abIsMultiThreaded = false, bool abDeferUpdate = false, bool abUpdateGeomorphs = false, bool abUpdateShadowSceneNode = false)
		: fTime(afTime), bUpdateControllers(abUpdateControllers), bIsMultiThreaded(abIsMultiThreaded), bDeferUpdate(abDeferUpdate), bUpdateGeomorphs(abUpdateGeomorphs), bUpdateLighting(abUpdateShadowSceneNode)
	{}
	~NiUpdateData() {};

	float fTime;
	bool bUpdateControllers;
	bool bIsMultiThreaded;
	bool bDeferUpdate;
	bool bUpdateGeomorphs;
	bool bUpdateLighting;
};

ASSERT_SIZE(NiUpdateData, 0xC);