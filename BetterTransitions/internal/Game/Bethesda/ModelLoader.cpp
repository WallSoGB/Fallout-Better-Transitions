#include "ModelLoader.hpp"

ModelLoader* ModelLoader::GetSingleton() {
    return *reinterpret_cast<ModelLoader**>(0x11C3B3C);
}

// GAME - 0x446A60
TESModel* ModelLoader::GetModelForBoundObject(TESBoundObject* apBoundObject, TESObjectREFR* apRef) {
	return ThisCall<TESModel*>(0x446A60, this, apBoundObject, apRef);
}

// GAME - 0x444D40
void ModelLoader::QueueBoundObject(TESBoundObject* apObj, IO_TASK_PRIORITY aePriority, QueuedFile* pParent, TESObjectREFR* apRefr) {
	ThisCall<void>(0x444D40, this, apObj, aePriority, pParent, apRefr);
}