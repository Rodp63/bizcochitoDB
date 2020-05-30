#include "../include/bizcochitoDB.h"

using namespace std;

bizcochitoDB:: bizcochitoDB()
{
  load_tables();
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
      cout<<"|> ";
      par->get_query();
      res->solve(par->parse(), running);
    }
}

void bizcochitoDB::load_tables()
{
  ifstream meta_tables;
  meta_tables.open(META_TABLES_PATH);
  string table_name, table_path, current_text;
  while(meta_tables >> current_text)
    {
      int partition = current_text.find('#');
      table_name = current_text.substr(0, partition);
      table_path = current_text.substr(partition+1);
      db_tables.emplace_back(table_name, table_path);
    }
  meta_tables.close();
}
