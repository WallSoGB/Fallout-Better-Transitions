#pragma once

#include "QueuedFile.hpp"

class BSFileEntry;

class QueuedFileEntry : public QueuedFile {
public:
	QueuedFileEntry();
	~QueuedFileEntry();

	virtual uint8_t GetFileIndex() const;

	char*			pFileName;
	BSFileEntry*	pData;
};

ASSERT_SIZE(QueuedFileEntry, 0x30);