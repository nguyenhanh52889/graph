#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <list>
#include <time.h>
#include <string.h>
#include <math.h>

using namespace std;

void bfs(int s, int scale)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	string cmd;

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	if( rc ){
		 fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		 exit(0);
	}else{
		 fprintf(stderr, "Opened database successfully\n");
	}

	vector<bool> visited;
	visited.resize((int)pow(2, scale));
	sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
	sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &zErrMsg);
	sqlite3_exec(db, "edge", NULL, NULL, &zErrMsg);
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		 fprintf(stderr, "SQL error: %s\n", zErrMsg);
		 sqlite3_free(zErrMsg);
	}

	list<int> queue;
	visited[s] = true;
	queue.push_back(s);
	clock_t startclock;
	startclock = clock();
	while (!queue.empty())
	{

		s = queue.front();
		//cout << s << " ";
		queue.pop_front();

		sqlite3_stmt* stmt = 0;
    //cmd = "select end from edge where end in (select id from visited where vs = 0) and start = " + to_string(s);
		cmd = "select end from edge where start = " + to_string(s);
		strcpy(sql, cmd.c_str());
		rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
		while (sqlite3_step (stmt) == SQLITE_ROW)
		{
			int end = sqlite3_column_int (stmt, 0);

			if(!visited[end]) {
				if (end % 100000 == 0) {
					cout << end << " ";
				}
				queue.push_back(end);
        visited[end] = true;
			}

		}
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
    int scale = 20;
   bfs(1, scale);
   return 0;
}
