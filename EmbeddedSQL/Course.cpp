#include "Course.h"

const Tuple Course::cno =
{
	.fieldName = "cno",
	.fieldType = MYSQL_TYPE_STRING,
	.fieldTypeSize = 4,
	.primaryKey = true,
	.notNull = true
};

const Tuple Course::cname =
{
	.fieldName = "cname",
	.fieldType = MYSQL_TYPE_VAR_STRING,
	.fieldTypeSize = 50,
	.primaryKey = false,
	.notNull = true
};

const Tuple Course::cpno =
{
	.fieldName = "cpno",
	.fieldType = MYSQL_TYPE_STRING,
	.fieldTypeSize = 4,
	.primaryKey = false,
	.notNull = false
};

const Tuple Course::ccredit =
{
	.fieldName = "ccredit",
	.fieldType = MYSQL_TYPE_LONG,
	.fieldTypeSize = 1,
	.primaryKey = false,
	.notNull = true
};

const string Course::tableName = "Course";

Course::Course(Database* database) : database(database)
{
	if (!(database->Exists(tableName)))
	{
		database->CreateTable(tableName, false, { cno, cname, cpno, ccredit }, {});
	}
}

void Course::Insert(const string& cno, const string& cname, const string& cpno, bool cpnoNull, long ccredit)
{
	if (cpnoNull)
	{
		database->InsertIntoTable(tableName, { this->cno, this->cname, this->ccredit }, { (void*)cno.c_str(), (void*)cname.c_str(), (void*)&ccredit });
	}
	else
	{
		database->InsertIntoTable(tableName, { this->cno, this->cname, this->cpno, this->ccredit }, { (void*)cno.c_str(), (void*)cname.c_str(), (void*)cpno.c_str(), (void*)& ccredit });
	}
}
