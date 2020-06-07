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
#define SELECT 10
#define UPDATE 11
#define DELETE 12

#define GAA_TOKEN '#'
#define AEA_TOKEN '&'

#define EQUAL 100
#define LESS 101
#define GREATER 201


// PATHS
#define META_TABLES_PATH "metadata/db_tables.gaa"
#define META_INDEXES_PATH "metadata/db_indexes.gaa"
#define TABLES_INFO_PATH "metadata/tables/"
#define TABLES_DATA_PATH "data/tables/"


// DB VARIABLES
static string __NULL = "nill";
static vector<string> data_types = {"int", "text", "date"};
static vector<char> reserved_characters = {'{','}','(',')','$','\'','#','/','&','~',',','^'};


//DB ACTIONS
#define THROW_(ERR) query_type = ERR; delete current_args; return
#define _DONE delete valid_args; return

// DB TYPES
typedef pair<int, void*> query_info;
typedef pair<string, string> str_duo;
typedef vector<vector<string> > table_ram;

struct meta_table{
  string name;
  string path_info;
  string path_data;
  vector<string> cast_vec(){ return {name, path_info, path_data}; }
  meta_table(vector<string> inf){
    name = inf[0];
    path_info = inf[1];
    path_data = inf[2];
  }
};

struct table_colum{
  string name;
  string type;
  vector<string> cast_vec(){ return {name, type}; }
  table_colum(vector<string> inf){
    name = inf[0];
    type = inf[1];
  }
};

struct args_table{
  string name;
  vector<str_duo> data; //colum - type
};

struct args_insert{
  string table;
  bool default_cols;
  vector<string> col_data;
  vector<string> val_data;
};

struct args_where{
  bool ok;
  string colum;
  string value;
  int opt;
};

struct args_select{
  string table;
  args_where *condition;
  vector<string> col_data;
};

struct args_update{
  string table;
  args_where *condition;
  vector<str_duo> data; // colum - value
};

struct args_delete{
  string table;
  args_where *condition;
};
