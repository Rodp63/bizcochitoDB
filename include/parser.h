#pragma once
#include "db_env.h"
#include "tools.h"

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
  string get_word(char,char,char);
  string get_phrase(char);
  bool check_word(string &);
  bool check_coma();
  args_where* parse_condition();
  
  void _d_table();
  void _create_table();
  void _insert_into();
  void _select();
  void _update();
  void _delete();
  
public:
  
  parser();
  void get_query();
  query_info parse();
  
};
