#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
using namespace std;

// CODES
#define NOTHING -1
#define SYNTAX_ERROR 0
#define TYPE_ERROR 1
#define NAME_ERROR 2
#define EMPTY_ERROR 3
#define EXIT 4
#define HELP 5
#define DT 6
#define CREATE_TABLE 7

// PATHS
#define META_TABLES_PATH "metadata/db_tables.gaa"
#define META_INDEXES_PATH "metadata/db_indexes.gaa"
#define TABLES_INFO_PATH "metadata/tables/"

// DB VARIABLES
static vector<string> data_types = {"int", "text", "date"};

// DB TYPES
typedef pair<int, void*> query_info;

struct meta_table{
  string name;
  string path;
  meta_table(string &a, string &b) : name(a), path(b) {}
};

struct table_colum{
  string name;
  string type;
  table_colum(string &a, string &b) : name(a), type(b) {}
};

struct args_table{
  string name;
  vector<pair<string, string> > data;
};
