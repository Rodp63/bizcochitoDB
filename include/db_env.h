#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
#include <ctime>
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
#define D 7
#define CREATE_TABLE 8
#define INSERT_INTO 9


// PATHS
#define META_TABLES_PATH "metadata/db_tables.gaa"
#define META_INDEXES_PATH "metadata/db_indexes.gaa"
#define TABLES_INFO_PATH "metadata/tables/"
#define TABLES_DATA_PATH "data/tables/"


// DB VARIABLES
enum {INT, TEXT, DATE};
static string __NULL = "o_o/";
static vector<string> data_types = {"int", "text", "date"};
static vector<char> reserved_characters = {'{','}','(',')','$','\'','#','/','&','*',',','^'};


//DB ACTIONS
#define THROW_(ERR) query_type = ERR; delete current_args; return
#define _DONE delete valid_args; return


// DB TYPES
typedef pair<int, void*> query_info;
typedef vector<pair<string, string> > vector_gaa;

struct meta_table{
  string name;
  string path;
  pair<string,string> cast_pair(){ return make_pair(name, path);}
  meta_table(string &a, string &b) : name(a), path(b) {}
};

struct table_colum{
  string name;
  string type;
  pair<string,string> cast_pair(){ return make_pair(name, type);}
  table_colum(string &a, string &b) : name(a), type(b) {}
};

struct args_table{
  string name;
  vector_gaa data; //colum - type
};

struct args_insert{
  string table;
  bool default_cols;
  vector<string> col_data;
  vector<string> val_data;
};

