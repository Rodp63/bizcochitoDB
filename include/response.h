#pragma once
#include "db_env.h"

using namespace std;

class response{

  typedef void (response::*fun_res)(void*);

  map<string, vector<string> > info_tables;
  vector<meta_table>* db_tables;
  
  int query_code;
  fun_res keys[10];
  
  void _syntax_error(void *args);
  void _type_error(void *arg);
  void _name_error(void *args);
  void _empty_error(void *args);
  
  void _help(void *args);
  void _dt(void *args);
  
  void _create_table(void *args);
  
public:

  void solve(query_info query, bool &running);
  response(vector<meta_table>* tables);
  
};
