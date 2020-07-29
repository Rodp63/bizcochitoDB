#include "../include/bizcochitoDB.h"

using namespace std;

bizcochitoDB::bizcochitoDB()
{
  _DB_tables_metadata = tools::read_file<meta_table>(META_TABLES_PATH, GAA_TOKEN);
  _DB_indexes_metadata = tools::read_file<meta_index>(META_INDEXES_PATH, GAA_TOKEN);
  par = new parser();
  res = new response(&_DB_tables_metadata, &_DB_indexes_metadata);
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
      running = true;
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

void bizcochitoDB::runcode()
{
  running = true;
  srand (time(NULL));
  string c = "create table test (id int, cnt int);";
  par->get_query(c);
  res->solve(par->parse(), running);
  
  int n = 1000000;
  for(int i = 0; i < n; i++){
    int val = rand() % 10000;
    string id = to_string(i);
    string cnt = to_string(val);
    string q = "insert into test values(" + id + "," + cnt + ");";
    par->get_query(q);
    res->solve(par->parse(), running);
  }
  
}
