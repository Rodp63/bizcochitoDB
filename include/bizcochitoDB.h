#pragma once
#include "parser.h"
#include "response.h"
#include "tools.h"

using namespace std;

class bizcochitoDB{

  vector<meta_table> _DB_tables_metadata;
  map<string, table_ram> _DB_tables_info;
  parser* par;
  response* res;
  bool running;

public:
  
  bizcochitoDB();
  ~bizcochitoDB();
  void run();
  void runscript();

};
