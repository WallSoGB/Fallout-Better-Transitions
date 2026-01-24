#pragma once

#include "IOTask.hpp"
#include "BSSimpleArray.hpp"
#include "Gamebryo/NiSmartPointer.hpp"

class QueuedReference;
class QueuedChildren;
class QueuedParents;
class NiObject;

NiSmartPointer(QueuedFile);

class QueuedFile : public IOTask {
public:
	QueuedFile();
	virtual ~QueuedFile();

	virtual void QueueMe();
	virtual void NotifyChildFinished(QueuedFile* apParent);
	virtual void CheckFinished();

	uint32_t					eContext;
	NiPointer<QueuedReference>	spParent;
	QueuedChildren*				pChildren;
	QueuedParents*				pParents;

	bool GetAllChildrenFinished() const;
};

ASSERT_SIZE(QueuedFile, 0x28);

class QueuedChildren : public BSSimpleArray<NiPointer<QueuedFile>> {
public:
	uint32_t	uiCounter;
};

ASSERT_SIZE(QueuedChildren, 0x14);