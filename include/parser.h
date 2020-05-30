#pragma once
#include "args.h"

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
  vector<string> arg_types = {"int", "text", "date"};

  void get_type();
  string get_word(char,char);

  void _create_table();
  
public:
  
  parser()
  {
    keys["exit"] = EXIT;
    keys["help"] = HELP;
    keys["\\dt"] = DT;
    keys["create_table"] = CREATE_TABLE;

    decode[CREATE_TABLE] = &parser::_create_table;
  }
  
  void get_query();
  query_info parse();
  
};
