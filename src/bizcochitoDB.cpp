#include "../include/bizcochitoDB.h"

using namespace std;

bizcochitoDB::bizcochitoDB()
{
  db_tables = tools::read_gaa_file<meta_table>(META_TABLES_PATH);
  par = new parser();
  res = new response(&db_tables);
}

bizcochitoDB::~bizcochitoDB()
{
  delete par;
  delete res;
}

void bizcochitoDB::run()
{
  cout<<"cake (v.1.0)"<<endl;
  cout<<"Escriba \"help\" para ayuda.\n"<<endl;
  running = true;
  while(running)
    {
      cout<<"root#> ";
      par->get_query();
      res->solve(par->parse(), running);
    }
}
