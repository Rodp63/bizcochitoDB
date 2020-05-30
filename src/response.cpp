#include "../include/response.h"

using namespace std;

void response::_syntax_error(void *args)
{
  cout<<"ERROR: <code "<<SYNTAX_ERROR<<"> Error de sintaxis\n"<<endl;
}

void response::_type_error(void *args)
{
  cout<<"ERROR: <code "<<TYPE_ERROR<<"> Error de tipo, el tipo de dato especificado no es valido\n"<<endl;
}

void response::_name_error(void *args)
{
  cout<<"ERROR: <code "<<NAME_ERROR<<"> El nombre del campo ingresado no es valido\n"<<endl;
}

void response::_empty_error(void *args)
{
  cout<<"ERROR: <code "<<EMPTY_ERROR<<"> Los campos no pueden ser vacios\n"<<endl;
}

void response::_help(void *args)
{
  cout<<"\nBienvenido a cake, la interfaz de linea de comandos de bizcochitoDB\n";
  cout<<"Los siguientes comandos están disponibles:\n";
  cout<<"\t exit \t\t Salir de cake\n";  
  cout<<"\t help \t\t Lista los comandos disponibles\n";
  cout<<"\t \\dt \t\t Lista las tablas existentes\n"<<endl;
}

void response::_dt(void *args)
{
  unsigned long max_name = 5, max_path = 4;
  for(auto &table : *db_tables)
    {
      max_name = max(max_name, table.name.size());
      max_path = max(max_path, table.path.size());
    }
  cout<<"Lista de tablas existentes:\n";
  cout<<"| tabla";
  for(int i = 0; i < max_name - 5; i++) cout<<' ';
  cout<<" | path";
  for(int i = 0; i < max_path - 4; i++) cout<<' ';
  cout<<" |\n|";
  for(int i = 0; i < max_name + max_path + 5; i++) cout<<'-';
  cout<<"|\n";
  for(auto &table : *db_tables)
    {
      cout<<"| "<<table.name;
      for(int i = 0; i < max_name - table.name.size(); i++) cout<<' ';
      cout<<" | "<<table.path;
      for(int i = 0; i < max_path - table.path.size(); i++) cout<<' ';
      cout<<" |\n";
    }
  cout<<"("<<db_tables->size()<<" filas)\n"<<endl;
}

void response::_create_table(void *args)
{
  args_new_table *valid_args = (args_new_table*) args;
  string _table_path = "metadata/tables/" + valid_args->name + ".gaa";
  string _table_name = valid_args->name;
  for(meta_table &db_table : *db_tables){
    if(db_table.name == _table_name)
      {
	cout<<"ERROR: La tabla \'"<<_table_name<<"\' ya existe\n"<<endl;
	delete valid_args;
	return;
      }
  }
  ofstream tables_file(META_TABLES_PATH, ofstream::app);
  tables_file << _table_name << '#' << _table_path << '\n';
  tables_file.close();
  db_tables->emplace_back(_table_name, _table_path);
  ofstream table_info(_table_path);
  for(auto colum : valid_args->data)
    {
      string colum_name = colum.first, colum_type = colum.second;
      table_info << colum_name << '#' << colum_type << '\n';
    }
  table_info.close();
  cout<<"Tabla \'"<<_table_name<<"\' creada exitosamente\n"<<endl;
  delete valid_args;
}

void response::solve(query_info query, bool &running)
{
  query_code = query.first;
  if(query_code == NOTHING)
    return;
  if(query_code == EXIT)
    {
      running = false;
      return;
    }
  (this->*keys[query_code])(query.second);
}

response::response(vector<meta_table>* tables) : db_tables(tables)
{
  keys[SYNTAX_ERROR] = &response::_syntax_error;
  keys[TYPE_ERROR] = &response::_type_error;
  keys[NAME_ERROR] = &response::_name_error;
  keys[EMPTY_ERROR] = &response::_empty_error;
  
  keys[HELP] = &response::_help;
  keys[DT] = &response::_dt;
  
  keys[CREATE_TABLE] = &response::_create_table;

  ios_base::sync_with_stdio(false);
  
}
