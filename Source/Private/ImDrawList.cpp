#include "ImDrawList.h"

FImGuiDrawList::FImGuiDrawList()
{
}

FImGuiDrawList::FImGuiDrawList(ImDrawList* InDrawList)
{
	DrawListImpl = InDrawList;
}
