#include "QueuedFile.hpp"

// GAME - 0x43CAA0
bool QueuedFile::GetAllChildrenFinished() const {
    return ThisCall<bool>(0x43CAA0, this);
}
