#include "../include/bizcochitoDB.h"

using namespace std;

bizcochitoDB::bizcochitoDB()
{
  db_tables = tools::read_file<meta_table>(META_TABLES_PATH, GAA_TOKEN);
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
  cout<<"\nEscriba \"help\" para ayuda.\n"<<endl;
  running = true;
  while(running)
    {
      cout<<"root#> ";
      par->get_query();
      res->solve(par->parse(), running);
    }
}

void bizcochitoDB::runscript()
{
  cout << "\n|> Ingrese la ruta del archivo: ";
  string dir;
  getline(cin, dir);
  ifstream data(dir);
  if(data.is_open())
    {
      string current_q;
      while(getline(data, current_q))
	{
	  par->get_query(current_q);
	  res->solve(par->parse(), running);
	}
      data.close();
    }
  else{
    cout << "ERROR: Archivo no encontrado" << endl;
  }
}
