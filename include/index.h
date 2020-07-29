#pragma once
#include "db_env.h"
#include "tools.h"

using namespace std;

class index_ram{

  map<int, vector<int> > data;
  table_ram* main_table;
  int table_size;

public:

  int col_ind;
  
  void build(table_ram*);
  void insert(string);
  void search(vector<int>&, args_where*);
  void update(int, string);
  index_ram(table_ram*, int);
  
};
