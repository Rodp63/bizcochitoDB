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
  /*
    Code made to test database!
   */
  
  running = true;
  srand (time(NULL));
  string a = "create table 500k_1 (id int, edad int, nombre text, apellido text, fecha date);";
  string b = "create table 500k_2 (id int, edad int, nombre text, apellido text, fecha date);";
  par->get_query(a);
  res->solve(par->parse(), running);
  par->get_query(b);
  res->solve(par->parse(), running);
  
  int n = 500000;
  ofstream data1("data/tables/500k_1.aea");
  ofstream data2("data/tables/500k_2.aea");
  for(int i = 0; i < n; i++){
    string id = to_string(i) + AEA_TOKEN;
    string ed = to_string(rand() % 200) + AEA_TOKEN;
    string nom = "\'nombre" + to_string(i) + "\'" + AEA_TOKEN;
    string ap = "\'apellido" + to_string(i) + "\'" + AEA_TOKEN;
    string year = to_string(2018 + (rand() % 3));
    string mon = to_string(1 + (rand() % 12));
    string day = to_string(1 + (rand() % 28));
    string dat = "\'" + day + "-" + mon + "-" + year + "\'" + AEA_TOKEN;
    string push = id + ed + nom + ap + dat + '\n';
    data1 << push;
    data2 << push;
  }
  data1.close();
  data2.close();
  
}
