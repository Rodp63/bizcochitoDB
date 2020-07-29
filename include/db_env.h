#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <iomanip>
using namespace std;

// CODES
enum{
     NOTHING,
     SYNTAX_ERROR,
     TYPE_ERROR,
     NAME_ERROR,
     EMPTY_ERROR,
     EXIT,
     HELP,
     DT,
     DI,
     D,
     CREATE,
     CREATE_TABLE,
     CREATE_INDEX,
     INSERT_INTO,
     SELECT,
     UPDATE,
     DELETE,
     DROP,
     DROP_TABLE
};

#define GAA_TOKEN '#'
#define AEA_TOKEN '&'
#define INDEX_TOKEN '$'

#define EQUAL 100
#define LESS 101
#define GREATER 201


// PATHS
#define META_TABLES_PATH "metadata/db_tables.gaa"
#define META_INDEXES_PATH "metadata/db_indexes.gaa"
#define TABLES_INFO_PATH "metadata/tables/"
#define TABLES_DATA_PATH "data/tables/"
#define INDEXES_DATA_PATH "data/indexes/"


// DB VARIABLES
static string __NULL = "nill";
static vector<string> data_types = {"int", "text", "date"};
static vector<char> reserved_characters = {'{','}','(',')','$','\'','#','/','&','~',',','^'};


// DB ACTIONS
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

struct meta_index{
  string table;
  string colum;
  string path_data;
  int col_idx;
  vector<string> cast_vec(){ return {table, colum, path_data}; }
  meta_index(vector<string> inf){
    table = inf[0];
    colum = inf[1];
    path_data = inf[2];
    col_idx = stoi(inf[3]);
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
