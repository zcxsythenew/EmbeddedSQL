#pragma once
#include <string>
#include <vector>
#include <mysql.h>
#include "ForeignKeyTuple.h"

using namespace std;

struct Tuple
{
	string fieldName;
	enum_field_types fieldType;
	int fieldTypeSize;
	bool primaryKey;
	bool notNull;

	// Not supported
	bool unique;

	// Not supported
	bool binary;

	// Not supported
	bool unsign;

	// Not supported
	bool zeroFill;

	// Not supported
	bool autoIncrement;

	// Not supported
	bool generated;
};