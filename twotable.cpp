#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <list>
#include <time.h>
#include <string.h>

using namespace std;

void bfs(int s)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	string cmd;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	if( rc ){
		 fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		 exit(0);
	}else{
		 fprintf(stderr, "Opened database successfully\n");
	}

	vector<bool> check;
	check.resize(1);
	sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
	sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &zErrMsg);
	sqlite3_exec(db, "visited", NULL, NULL, &zErrMsg);
	sqlite3_exec(db, "edge", NULL, NULL, &zErrMsg);
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		 fprintf(stderr, "SQL error: %s\n", zErrMsg);
		 sqlite3_free(zErrMsg);
	}

	sqlite3_exec(db, "update visited set vs = 0 where vs = 1;", NULL, NULL, &zErrMsg);
	//cmd = "update visited set vs = 1 where id= "+ to_string(s);
	//strcpy(sql, cmd.c_str());
	sqlite3_exec(db, ("update visited set vs = 1 where id= " + to_string(s)).c_str(), NULL, NULL, &zErrMsg);

	list<int> queue;
	check[s] = true;
	queue.push_back(s);
	clock_t startclock;
	startclock = clock();
	while (!queue.empty())
	{

		s = queue.front();
		//cout << s << " ";
		queue.pop_front();

		sqlite3_stmt* stmt = 0;
    cmd = "select edge.end from edge left join visited on edge.end = visited.id where visited.vs = 0 and edge.start = " + to_string(s);
		strcpy(sql, cmd.c_str());
		rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
		while (sqlite3_step (stmt) == SQLITE_ROW)
		{
			int end = sqlite3_column_int (stmt, 0);
			if (end % 100000 == 0) {
				cout << end << " ";
				//cout << (clock() - startclock) / (double)CLOCKS_PER_SEC;
				//sqlite3_close(db);
				//return;
			}
			queue.push_back(end);
		//update table visited
			sqlite3_exec(db, ("update visited set vs = 1 where id = "+ to_string(end)).c_str(), NULL, NULL, &zErrMsg);
		}
		//update table visited



	}
	rc = sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		 fprintf(stderr, "SQL error: %s\n", zErrMsg);
		 sqlite3_free(zErrMsg);
	}
	cout << (clock() - startclock) / (double)CLOCKS_PER_SEC;
	sqlite3_close(db);
}

int main(int argc, char* argv[])
{
   bfs(1);
   return 0;
}
