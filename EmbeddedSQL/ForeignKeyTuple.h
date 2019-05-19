#pragma once
#include <string>
#include "FoerignKeyRestrictType.h"

using namespace std;

struct ForeignKeyTuple
{
	string KeyName;
	string ReferencedTable;
	string ReferencedColumn;
	FoerignKeyRestrictType OnUpdate;
	FoerignKeyRestrictType OnDelete;
};