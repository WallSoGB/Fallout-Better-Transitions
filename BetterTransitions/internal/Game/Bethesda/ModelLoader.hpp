#pragma once

#include "IOTask.hpp"

class TESBoundObject;
class TESObjectREFR;
class QueuedFile;
class TESModel;

class ModelLoader {
public:
	static ModelLoader* GetSingleton();

	TESModel* GetModelForBoundObject(TESBoundObject* apBoundObject, TESObjectREFR* apRef);

	void QueueBoundObject(TESBoundObject* apObj, IO_TASK_PRIORITY aePriority, QueuedFile* pParent, TESObjectREFR* apRefr);
};