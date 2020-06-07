#pragma once
#include "db_env.h"
#include "tools.h"

using namespace std;

class response{

  typedef void (response::*fun_res)(void*);

  map<string, vector<string> > info_tables;
  vector<meta_table>* db_tables;
  
  int query_code;
  fun_res keys[20];

  void print_table(table_ram &, vector<string>);
  
  void _syntax_error(void *);
  void _type_error(void *);
  void _name_error(void *);
  void _empty_error(void *);
  
  void _help(void *);
  void _dt(void *);

  void _d_table(void *);
  void _create_table(void *);
  void _insert_into(void *);
  void _select(void *);
  void _update(void *);
  void _delete(void *);
  
public:

  void solve(query_info query, bool &running);
  response(vector<meta_table>* tables);
  
};
