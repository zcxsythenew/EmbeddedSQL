#pragma once
#include <iostream>
#include <exception>
#include <vector>
#include <mysql.h>
#include "Tuple.h"

using namespace std;

class Database
{
protected:
	MYSQL mysql;
	static string PrepareStringFromLength(size_t length);
	static string JoinNamesFromFields(const vector<Tuple>& fields);
	static string CreateTableStringFromFields(const vector<Tuple>& fields);
	static string CreateTableStringFromForeignKeys(const vector<ForeignKeyTuple>& foreignKeys);

public:
	Database();

	void CreateTable(const string& tableName, bool recover, const vector<Tuple>& fields, const vector<ForeignKeyTuple>& foreignKeys);
	void InsertIntoTable(const string& tableName, const vector<Tuple>& fields, const vector<void*>& data);
	bool Exists(const string& tableName);

	~Database();
};