#include "ProcessLists.hpp"

ProcessLists* ProcessLists::GetSingleton() {
	return reinterpret_cast<ProcessLists*>(0x11E0E80);
}