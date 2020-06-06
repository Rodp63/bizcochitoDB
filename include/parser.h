#pragma once
#include "db_env.h"

using namespace std;

class parser{

  typedef void (parser::*ptr_par)();

  string query;
  int current_pos;
  int query_size;
  int query_type;
  void* query_args;
  
  map<string,int> keys;
  map<int,ptr_par> decode;
  vector<meta_table>* db_tables;

  void get_type();
  string get_word(char,char);
  string get_phrase(char);
  bool check_word(string &);

  void _d_table();
  void _create_table();
  void _insert_into();
  void _select();
  
public:
  
  parser();
  void get_query();
  query_info parse();
  
};
