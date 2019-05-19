#include <cassert>
#include <cstring>
#include <sstream>
#include "Database.h"
#include "FieldTypeString.h"
#include "ForeignKeyRestrictTypeString.h"

string Database::PrepareStringFromLength(size_t length)
{
	string prepare_str;

	for (size_t i = 0; i < length; i++)
	{
		if (i != 0) prepare_str += ",";
		prepare_str += "?";
	}

	return prepare_str;
}

string Database::JoinNamesFromFields(const vector<Tuple>& fields)
{
	string prepare_str;
	size_t length = fields.size();

	for (size_t i = 0; i < length; i++)
	{
		if (i != 0) prepare_str += ",";
		prepare_str += fields[i].fieldName;
	}

	return prepare_str;
}

string Database::CreateTableStringFromFields(const vector<Tuple>& fields)
{
	stringstream ss, pk;

	size_t number_of_primary_key = 0;

	for (size_t i = 0; i < fields.size(); i++)
	{
		ss << fields[i].fieldName;
		ss << " ";
		ss << FieldTypeString[fields[i].fieldType];
		ss << " ";
		if (fields[i].fieldTypeSize > 1)
		{
			ss << "(";
			ss << fields[i].fieldTypeSize;
			ss << ") ";
		}
		if (fields[i].notNull)
		{
			ss << "NOT NULL ";
		}
		if (fields[i].primaryKey)
		{
			if (number_of_primary_key != 0)
			{
				pk << ",";
			}
			pk << fields[i].fieldName;
			number_of_primary_key++;
		}
		ss << ",";
	}

	ss << "PRIMARY KEY (";
	ss << pk.str();
	ss << ")";

	return ss.str();
}

string Database::CreateTableStringFromForeignKeys(const vector<ForeignKeyTuple>& foreignKeys)
{
	stringstream ss;

	for (size_t i = 0; i < foreignKeys.size(); i++)
	{
		if (i != 0)
		{
			ss << ",";
		}
		ss << "FOREIGN KEY (";
		ss << foreignKeys[i].KeyName;
		ss << ") REFERENCES ";
		ss << foreignKeys[i].ReferencedTable;
		ss << "(";
		ss << foreignKeys[i].ReferencedColumn;
		ss << ") ON UPDATE ";
		ss << ForeignKeyRestrictTypeString[foreignKeys[i].OnUpdate];
		ss << " ON DELETE ";
		ss << ForeignKeyRestrictTypeString[foreignKeys[i].OnDelete];
	}

	return ss.str();
}

Database::Database()
{
	if (mysql_init(&mysql) == nullptr)
	{
		throw exception("Error occured when initializing mysql\n"
			"in mysql_init(&mysql)\n"
			"in Database::Database()");
	}

	if (mysql_real_connect(&mysql, "localhost", "z", "", "jxgl", 3306, nullptr, 0) == nullptr)
	{
		throw exception("Error occured when initializing mysql\n"
			"in mysql_real_connect(&mysql, ...)\n"
			"in Database::Database()");
	}
}

void Database::CreateTable(const string& tableName, bool recover, const vector<Tuple>& fields, const vector<ForeignKeyTuple>& foreignKeys)
{
	if (mysql_list_tables(&mysql, tableName.c_str())->row_count != 0)
	{
		if (!recover)
		{
			throw exception("Error occured when creating table\n"
				"Table has existed and recover is not allowed\n"
				"in Database::CreateTable(...)");
		}
	}

	string str = "CREATE TABLE ";
	str += tableName;
	str += " (";
	str += CreateTableStringFromFields(fields);
	if (foreignKeys.size() != 0)
	{
		str += ",";
		str += CreateTableStringFromForeignKeys(foreignKeys);
	}
	str += ")";

	if (mysql_query(&mysql, str.c_str()) != 0)
	{
		throw exception("Error occured when creating table\n"
			"in mysql_query(&mysql, str.c_str())");
	}
}

void Database::InsertIntoTable(const string& tableName, const vector<Tuple>& fields, const vector<void*>& data)
{
	if (fields.size() != data.size())
	{
		throw exception("Parameter error when inserting into table\n"
			"fields.size() != data.size()\n"
			"in Database::InsertIntoTable(...)");
	}

	if (mysql_list_tables(&mysql, tableName.c_str())->row_count == 0)
	{
		throw exception("Error occured when inserting into table\n"
			"Table does not exist\n"
			"in Database::InsertIntoTable(...)");
	}

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (stmt == nullptr)
	{
		throw exception("Error occured when inserting into table\n"
			"Unable to initialize stmt\n"
			"in Database::InsertIntoTable(...)");
	}

	string prepare_str = "INSERT INTO ";
	prepare_str += tableName;
	prepare_str += " (";
	prepare_str += JoinNamesFromFields(fields);
	prepare_str += ") VALUES (";
	prepare_str += PrepareStringFromLength(data.size());
	prepare_str += ")";

	if (mysql_stmt_prepare(stmt, prepare_str.c_str(), prepare_str.length()) != 0)
	{
		throw exception("Error occured when inserting into table\n"
			"Unable to prepare stmt\n"
			"in Database::InsertIntoTable(...)");
	}

	unsigned long param_count = mysql_stmt_param_count(stmt);
	assert(param_count == data.size());

	MYSQL_BIND* bind = new MYSQL_BIND[param_count]();
	memset(bind, 0, sizeof(MYSQL_BIND) * param_count);

	for (size_t i = 0; i < data.size(); i++)
	{
		bind[i].buffer = data[i];
		bind[i].buffer_type = fields[i].fieldType;
		if (fields[i].fieldType == MYSQL_TYPE_STRING || fields[i].fieldType == MYSQL_TYPE_VAR_STRING)
		{
			bind[i].buffer_length = strlen((char*)(data[i]));
		}
		else
		{
			bind[i].buffer_length = fields[i].fieldTypeSize;
		}
	}

	if (mysql_stmt_bind_param(stmt, bind))
	{
		throw exception("Error occured when inserting into table\n"
			"Unable to bind stmt parameters\n"
			"in Database::InsertIntoTable(...)");
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		throw exception("Error occured when inserting into table\n"
			"Unable to execute stmt\n"
			"in Database::InsertIntoTable(...)");
	}

	mysql_stmt_close(stmt);
	delete[] bind;
}

bool Database::Exists(const string& tableName)
{
	auto x = mysql_list_tables(&mysql, tableName.c_str());
	return x->row_count != 0;
}

Database::~Database()
{
	mysql_close(&mysql);
}
