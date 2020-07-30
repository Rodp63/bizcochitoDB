#pragma once
#include "db_env.h"
#include "tools.h"
#include "index.h"

using namespace std;
using namespace std::chrono;

class response{

  typedef void (response::*fun_res)(void*);
  typedef high_resolution_clock::time_point cclock;

  vector<meta_table> *db_tables;
  vector<meta_index> *db_indexes;
  map<string, table_ram> active_tables;
  map<string, map<string, index_ram*> > active_indexes;
  
  int query_code;
  fun_res keys[30];
  cclock t_start;
  cclock t_end;
  double t_total;

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
  void _drop_index(void *);
  void _drop_table(void *);
  
public:
  
  response(vector<meta_table>*, vector<meta_index> *);
  ~response();
  void solve(query_info query, bool &running);
  
};
