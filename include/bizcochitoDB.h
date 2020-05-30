#pragma once
#include "parser.h"
#include "response.h"

using namespace std;

class bizcochitoDB{

  parser* par;
  response* res;
  bool running;
  vector<meta_table> db_tables;

  void load_tables();

public:
  
  bizcochitoDB();
  ~bizcochitoDB();
  void run();

};
