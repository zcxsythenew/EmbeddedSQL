#pragma once
#include <string>
#include "Database.h"

class Course
{
	Database* database;
	static const Tuple cno;
	static const Tuple cname;
	static const Tuple cpno;
	static const Tuple ccredit;
	static const string tableName;
	
public:
	Course(Database* database);

	void Insert(const string& cno, const string& cname, const string& cpno, bool cpnoNull, long ccredit);
};