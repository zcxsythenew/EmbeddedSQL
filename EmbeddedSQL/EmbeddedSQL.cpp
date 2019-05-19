// EmbeddedSQL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <mysql.h>
#include "FieldTypeString.h"
#include "ForeignKeyRestrictTypeString.h"
#include "Course.h"

using namespace std;

void initialize()
{
	FieldTypeString[245] = "JSON";
	FieldTypeString[246] = "DEMICAL";
	FieldTypeString[247] = "ENUM";
	FieldTypeString[248] = "SET";
	FieldTypeString[249] = "TINY BLOB";
	FieldTypeString[250] = "MEDIUM BLOB";
	FieldTypeString[251] = "LONG BLOB";
	FieldTypeString[252] = "BLOB";
	FieldTypeString[253] = "VARCHAR";
	FieldTypeString[254] = "CHAR";
	FieldTypeString[255] = "GEOMETRY";
}

int main()
{
	initialize();
	Database database;
	Course course(&database);

#if 1
	string cno, cname, cpno;
	int ccredit;

	while (cin >> cno)
	{
		cin >> cname >> cpno >> ccredit;
		course.Insert(cno, cname, cpno, cpno == "NULL", ccredit);
	}
#else
	course.Insert("1", "数据库系统", "5", false, 4);
	course.Insert("2", "数学分析", "", true, 2);
	course.Insert("3", "信息系统导论", "1", false, 3);
	course.Insert("4", "操作系统原理", "6", false, 3);
	course.Insert("5", "数据结构", "7", false, 4);
	course.Insert("6", "数据处理基础", "", true, 4);
	course.Insert("7", "C语言", "6", false, 3);
#endif

	cout << "Insert succeeded!" << endl;

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
