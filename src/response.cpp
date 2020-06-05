#include "../include/response.h"

using namespace std;

void response::_syntax_error(void *args)
{
  cout<<"ERROR: <code "<<SYNTAX_ERROR<<"> ";
  cout<<"Error de sintaxis\n"<<endl;
}

void response::_type_error(void *args)
{
  cout<<"ERROR: <code "<<TYPE_ERROR<<"> ";
  cout<<"Error de tipo, el tipo de dato especificado no es valido\n"<<endl;
}

void response::_name_error(void *args)
{
  cout<<"ERROR: <code "<<NAME_ERROR<<"> ";
  cout<<"Error de nombre, el campo ingresado utiliza caracteres reservados\n"<<endl;
}

void response::_empty_error(void *args)
{
  cout<<"ERROR: <code "<<EMPTY_ERROR<<"> ";
  cout<<"Error de campo vacio, los campos de la consulta no pueden ser nulos\n"<<endl;
}

void response::_help(void *args)
{
  cout<<"Bienvenido a cake, la interfaz de linea de comandos de bizcochitoDB\n";
  cout<<"Los siguientes comandos están disponibles:\n";
  cout<<"\t exit \t\t Salir de cake\n";  
  cout<<"\t help \t\t Lista los comandos disponibles\n";
  cout<<"\t \\d [TABLE] \t Describe las columnas de una tabla\n";
  cout<<"\t \\dt \t\t Lista las tablas existentes\n"<<endl;
}

void response::print_table(vector_gaa &table, string col1, string col2)
{
  unsigned long max_1 = col1.size(), max_2 = col2.size();
  for(auto &reg : table)
    {
      max_1 = max(max_1, reg.first.size());
      max_2 = max(max_2, reg.second.size());
    }
  cout<<"| "<<col1;
  for(int i = 0; i < max_1 - col1.size(); i++) cout<<' '; cout<<" | "<<col2;
  for(int i = 0; i < max_2 - col2.size(); i++) cout<<' '; cout<<" |\n|";
  for(int i = 0; i < max_1 + 2; i++) cout<<'-'; cout<<'+';
  for(int i = 0; i < max_2 + 2; i++) cout<<'-'; cout<<"|\n";
  for(auto &reg : table)
    {
      cout<<"| "<<reg.first;
      for(int i = 0; i < max_1 - reg.first.size(); i++) cout<<' ';
      cout<<" | "<<reg.second;
      for(int i = 0; i < max_2 - reg.second.size(); i++) cout<<' ';
      cout<<" |\n";
    }
  cout<<"("<<table.size()<<" filas)\n"<<endl;
}

void response::_dt(void *args)
{
  vector_gaa _vec = tools::cast_vec_gaa<meta_table>(*db_tables);
  cout<<"Tablas actuales\n";
  print_table(_vec, "tabla", "path");
}

void response::_d_table(void *args)
{
  string* valid_args = (string*)args;
  string _table_name = *valid_args;
  for(meta_table &db_table : *db_tables){
    if(db_table.name == _table_name)
      {
        vector<table_colum> table_info = tools::read_gaa_file<table_colum>(db_table.path);
	vector_gaa _vec = tools::cast_vec_gaa<table_colum>(table_info);
	cout<<"Tabla \'"<<_table_name<<"\'\n";
        print_table(_vec, "columna", "tipo");
	_DONE;
      }
  }
  cout<<"No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
}

void response::_create_table(void *args)
{
  args_table *valid_args = (args_table*) args;
  string _table_name = valid_args->name;
  string _table_path = TABLES_INFO_PATH + _table_name + ".gaa";
  for(meta_table &db_table : *db_tables)
    {
      if(db_table.name == _table_name){
	cout<<"ERROR: La tabla \'"<<_table_name<<"\' ya existe\n"<<endl;
	_DONE;
      }
    }
  ofstream tables_file(META_TABLES_PATH, ofstream::app);
  ofstream table_info(_table_path);
  tables_file << _table_name << '#' << _table_path << '\n';
  db_tables->emplace_back(_table_name, _table_path);
  for(auto &colum : valid_args->data)
    {
      string colum_name = colum.first;
      string colum_type = colum.second;
      table_info << colum_name << '#' << colum_type << '\n';
    }
  cout<<"Tabla \'"<<_table_name<<"\' creada exitosamente\n"<<endl;
  table_info.close();
  tables_file.close();
  _DONE;
}

void response::_insert_into(void *args)
{
  args_insert* valid_args = (args_insert*) args;
  string _table_name = valid_args->table;
  vector<string> &q_cols = valid_args->col_data;
  vector<string> &q_vals = valid_args->val_data;
  for(const meta_table &db_table : *db_tables){
    if(db_table.name == _table_name)
      {
	vector<table_colum> table_info = tools::read_gaa_file<table_colum>(db_table.path);
	for(string &col : q_cols){
	  bool ok = false;
	  for(table_colum &true_col : table_info){
	    if(col == true_col.name) ok = true;
	  }
	  if(!ok){
	    cout<<"ERROR: La columna \'"<<col<<"\' no existe en la tabla\n"<<endl;
	    _DONE;
	  }
	}
	if(valid_args->default_cols){
	  for(table_colum &colum : table_info)
	    q_cols.push_back(colum.name);
	}
	if(q_cols.size() != q_vals.size()){
	  cout<<"ERROR: El numero de expresiones no es igual al numero de columnas\n"<<endl;
	  _DONE;
	}
	string util_info;
	for(table_colum &colum : table_info)
	  {
	    int pos = find(q_cols.begin(), q_cols.end(), colum.name) - q_cols.begin();
	    if(pos == q_cols.size()){
	      util_info += (__NULL + "&");
	    }
	    else{
	      if(tools::check_type(q_vals[pos], colum.type)){
		util_info += (q_vals[pos] + "&");
		q_vals.erase(q_vals.begin()+pos);
		q_cols.erase(q_cols.begin()+pos);
	      }
	      else{
		cout<<"ERROR: El tipo de la entrada y el tipo de la columna difieren\n"<<endl;
		_DONE;
	      }
	    }}
	string data_path = TABLES_DATA_PATH + _table_name + ".aea";
	ofstream table_data(data_path, ofstream::app);
	table_data << util_info << '\n';
	table_data.close();
        _DONE;
      }
  }
  cout<<"No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
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

  keys[D] = &response::_d_table;
  keys[CREATE_TABLE] = &response::_create_table;
  keys[INSERT_INTO] = &response::_insert_into;

  ios_base::sync_with_stdio(false);
}
