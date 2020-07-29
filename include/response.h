#pragma once
#include "db_env.h"
#include "tools.h"
#include "index.h"

using namespace std;

class response{

  typedef void (response::*fun_res)(void*);

  vector<meta_table> *db_tables;
  vector<meta_index> *db_indexes;
  map<string, table_ram> active_tables;
  map<string, map<string, index_ram*> > active_indexes;
  
  int query_code;
  fun_res keys[30];

  void print_table(table_ram &, vector<string>);
  
  void _syntax_error(void *);
  void _type_error(void *);
  void _name_error(void *);
  void _empty_error(void *);
  
  void _help(void *);
  void _dt(void *);
  void _di(void *);

  void _d_table(void *);
  void _create_table(void *);
  void _create_index(void *);
  void _insert_into(void *);
  void _select(void *);
  void _update(void *);
  void _delete(void *);
  void _drop_table(void *);
  
public:
  
  response(vector<meta_table>*, vector<meta_index> *);
  ~response();
  void solve(query_info query, bool &running);
  
};
