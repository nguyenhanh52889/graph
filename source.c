#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <math.h>


void createEdge(int scale, int edgefactor)
{
  int n;
  n = (int)pow(2, scale);
  int e;
  e = n * edgefactor;
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;


  /* Open database */
  rc = sqlite3_open("test.db", &db);
  if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     exit(0);
  }else{
     fprintf(stderr, "Opened database successfully\n");
  }



  rc = sqlite3_exec(db, "CREATE TABLE edge(START INT NOT NULL, END INT NOT NULL, WEIGHT INT NOT NULL , UNIQUE (START, END))", 0, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
     fprintf(stderr, "SQL error: %s\n", zErrMsg);
     sqlite3_free(zErrMsg);
  }else{
     fprintf(stdout, "Operation done successfully\n");
  }
  int i;
  int start;
  int end;
  int weight;

 //create table EDGE
 sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
 sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &zErrMsg);
 sqlite3_exec(db, "edge", NULL, NULL, &zErrMsg);


  rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);
  if( rc != SQLITE_OK ){
     fprintf(stderr, "SQL error: %s\n", zErrMsg);
     sqlite3_free(zErrMsg);
  }
  for (i = 0; i < e; i++) {
       start = rand() % n;
       end = rand() % n;
       weight = rand() % 1000;
       char *sql2;
       asprintf(&sql2, "insert into edge (START, END, WEIGHT) values (%d, %d, %d);", start, end, weight);
        rc = sqlite3_exec(db, sql2, 0, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
           sqlite3_free(zErrMsg);
        }
    }
  rc = sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
}

void createVisited(int scale)
{
  int n = pow(2, scale);
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;


  /* Open database */
  rc = sqlite3_open("test.db", &db);
  if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     exit(0);
  }else{
     fprintf(stderr, "Opened database successfully\n");
  }



  rc = sqlite3_exec(db, "CREATE TABLE VISITED(ID INT NOT NULL, VS INT, UNIQUE (ID))", 0, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
     fprintf(stderr, "SQL error: %s\n", zErrMsg);
     sqlite3_free(zErrMsg);
  }else{
     fprintf(stdout, "Operation done successfully\n");
  }
  int i;
  sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
  sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &zErrMsg);
  sqlite3_exec(db, "visited", NULL, NULL, &zErrMsg);
   /* Create SQL statement */
   rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   for (i = 0; i < n; i++) {
       char *sql2;
        asprintf(&sql2, "insert into visited (ID) values (%d);", i);
        rc = sqlite3_exec(db, sql2, 0, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
           sqlite3_free(zErrMsg);
        }
    }
     rc = sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &zErrMsg);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
     }


sqlite3_close(db);
}

int main(int argc, char* argv[])
{
  int scale = 20;
  int edgefactor = 16;
  createEdge(scale, edgefactor);
  //createVisited(scale);
  return 0;
}
