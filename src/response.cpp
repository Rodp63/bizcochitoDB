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

void response::print_table(table_ram &table, vector<string> cols)
{
  unsigned long n_cols = cols.size();
  vector<unsigned long> max_size(n_cols);
  for(int i = 0; i < n_cols; i++){
    max_size[i] = cols[i].size();
  }
  for(auto &reg : table){
    for(int i = 0; i < n_cols; i++)
      max_size[i] = max(max_size[i], reg[i].size());
  }
  cout<<"| ";
  for(int i = 0; i < n_cols; i++){
    cout<<cols[i];
    for(int j = 0; j < max_size[i] - cols[i].size(); j++) cout<<' ';
    cout<<" | ";
  }
  cout<<"\n|";
  for(int i = 0; i < n_cols; i++){
    for(int j = 0; j < max_size[i] + 2; j++) cout<<'-';
    cout<<(i + 1 == n_cols ? "|\n" : "+");
  }
  for(vector<string> &row : table){
    cout<<"| ";
    for(int i = 0; i < n_cols; i++){
      cout<<row[i];
      for(int j = 0; j < max_size[i] - row[i].size(); j++) cout<<' ';
      cout<<" | ";
    }
    cout<<"\n";
  }
  cout<<"("<<table.size()<<" filas)\n"<<endl;
}

void response::_dt(void *args)
{
  table_ram _table= tools::cast_table<meta_table>(*db_tables);
  cout<<"Tablas actuales\n";
  print_table(_table, {"tabla", "path_info", "path_data"});
}

void response::_d_table(void *args)
{
  string* valid_args = (string*)args;
  string _table_name = *valid_args;
  for(meta_table &db_table : *db_tables)
    if(db_table.name == _table_name)
      {
        vector<table_colum> table_info = tools::read_file<table_colum>(db_table.path_info, GAA_TOKEN);
	table_ram _vec = tools::cast_table<table_colum>(table_info);
	cout<<"Tabla \'"<<_table_name<<"\'\n";
        print_table(_vec, {"columna", "tipo"});
	_DONE;
      }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
}

void response::_create_table(void *args)
{
  args_table *valid_args = (args_table*) args;
  string _table_name = valid_args->name;
  string _path_info = TABLES_INFO_PATH + _table_name + ".gaa";
  string _path_data = TABLES_DATA_PATH + _table_name + ".aea";
  for(meta_table &db_table : *db_tables)
    if(db_table.name == _table_name){
      cout<<"ERROR: La tabla \'"<<_table_name<<"\' ya existe\n"<<endl;
      _DONE;
    }
  ofstream tables_file(META_TABLES_PATH, ofstream::app);
  ofstream table_info(_path_info);
  tables_file << _table_name << GAA_TOKEN << _path_info << GAA_TOKEN << _path_data << GAA_TOKEN << '\n';
  vector<string> __info = {_table_name, _path_info, _path_data};
  db_tables->emplace_back(__info);
  for(auto &colum : valid_args->data)
    {
      string &colum_name = colum.first;
      string &colum_type = colum.second;
      table_info << colum_name << GAA_TOKEN << colum_type << GAA_TOKEN << '\n';
    }
  cout<<"Tabla \'"<<_table_name<<"\' creada exitosamente\n"<<endl; // Todo OK!
  table_info.close();
  tables_file.close();
  _DONE;
}

void response::_insert_into(void *args)
{
  args_insert* valid_args = (args_insert*) args;
  string &_table_name = valid_args->table;
  vector<string> &q_cols = valid_args->col_data;
  vector<string> &q_vals = valid_args->val_data;
  for(const meta_table &db_table : *db_tables)
    if(db_table.name == _table_name)
      {
	vector<table_colum> table_info = tools::read_file<table_colum>(db_table.path_info, GAA_TOKEN);
	for(const string &col : q_cols){
	  bool ok = false;
	  for(const table_colum &true_col : table_info)
	    if(col == true_col.name) ok = true;
	  if(!ok){
	    cout<<"ERROR: La columna \'"<<col<<"\' no existe en la tabla\n"<<endl;
	    _DONE;
	  }
	}
	if(valid_args->default_cols){
	  for(const table_colum &colum : table_info)
	    q_cols.push_back(colum.name);
	}
	if(q_cols.size() != q_vals.size()){
	  cout<<"ERROR: El numero de expresiones no es igual al numero de columnas\n"<<endl;
	  _DONE;
	}
	string util_info;
	for(const table_colum &colum : table_info)
	  {
	    int pos = find(q_cols.begin(), q_cols.end(), colum.name) - q_cols.begin();
	    if(pos == q_cols.size())
	      util_info += (__NULL + AEA_TOKEN);
	    else{
	      if(tools::check_type(q_vals[pos], colum.type)){
		util_info += (q_vals[pos] + AEA_TOKEN);
		q_vals.erase(q_vals.begin()+pos);
		q_cols.erase(q_cols.begin()+pos);
	      }
	      else{
		cout<<"ERROR: El tipo de entrada y el tipo de columna difieren\n"<<endl;
		_DONE;
	      }
	    }}
	ofstream table_data(db_table.path_data, ofstream::app);
	table_data << util_info << '\n';
	cout<<"Insercion exitosa\n"<<endl; // Todo Ok!
	table_data.close();
        _DONE;
      }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
}

void response::_select(void *args)
{
  args_select* valid_args = (args_select*) args;
  string _table_name = valid_args->table;
  vector<string> &q_cols = valid_args->col_data;
  for(const meta_table &db_table : *db_tables){
    if(db_table.name == _table_name)
      {
	vector<table_colum> table_info = tools::read_file<table_colum>(db_table.path_info, GAA_TOKEN);
	if(q_cols.size() == 1 && q_cols[0] == "*"){
	  q_cols.pop_back();
	  for(const table_colum &colum : table_info)
	    q_cols.push_back(colum.name);
	}
	bool where_active = false;
	args_where* _condition = valid_args->condition;
	if(_condition){
	  if(_condition->ok){
	    where_active = true;
	    q_cols.push_back(_condition->colum);
	  }
	  else{
	    cout<<"ERROR: Condicion mal formulada. Utilice el formato [columna operador valor]\n"<<endl;
	    _DONE;
	  }}
	vector<int> idx_col;
	for(const string &col : q_cols){
	  bool ok = false;
	  for(int i = 0; i < table_info.size(); i++)
	    if(col == table_info[i].name){
	      ok = true;
	      idx_col.push_back(i);
	    }
	  if(!ok){
	    cout<<"ERROR: La columna \'"<<col<<"\' no existe en la tabla\n"<<endl;
	    _DONE;
	  }}
	int where_idx;
	if(where_active){
	  where_idx = idx_col.back();
	  if(!tools::check_type(_condition->value, table_info[where_idx].type)){
	    cout<<"ERROR: El tipo de la condicion y el tipo de la columna son diferentes\n"<<endl;
	    _DONE;
	  }
	  idx_col.pop_back();
	  q_cols.pop_back();
	}
	table_ram table_data = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
	table_ram table_query;
	for(const vector<string> &row : table_data){
	  if(!where_active || (where_active && tools::compare_values(row[where_idx], _condition->value, table_info[where_idx].type, _condition->opt))){
	    vector<string> tmp;
	    for(int idx : idx_col)
	      tmp.push_back(row[idx]);
	    table_query.push_back(tmp);
	  }}
	print_table(table_query, q_cols);
	_DONE;
      }
  }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
}

void response::_update(void *args)
{
  args_update* valid_args = (args_update*) args;
  string _table_name = valid_args->table;
  for(const meta_table &db_table : *db_tables){
    if(db_table.name == _table_name)
      {
	vector<table_colum> table_info = tools::read_file<table_colum>(db_table.path_info, GAA_TOKEN);
	vector<str_duo> &u_data = valid_args->data;
	vector<string> new_data(table_info.size());
	vector<bool> change(table_info.size(), false);
	for(const str_duo &col : u_data){
	  bool ok = false;
	  for(int i = 0; i < table_info.size(); i++)
	    if(col.first == table_info[i].name){
	      ok = true;
	      if(!tools::check_type(col.second, table_info[i].type)){
		cout<<"ERROR: El tipo del nuevo valor y el tipo de la columna son diferentes\n"<<endl;
		_DONE;
	      }
	      new_data[i] = col.second;
	      change[i] = true;
	    }
	  if(!ok){
	    cout<<"ERROR: La columna \'"<<col.first<<"\' no existe en la tabla\n"<<endl;
	    _DONE;
	  }}
	bool where_active = false;
	args_where* _condition = valid_args->condition;
	if(_condition){
	  if(_condition->ok)
	    where_active = true;
	  else{
	    cout<<"ERROR: Condicion mal formulada. Utilice  el formato [columna operador valor]\n"<<endl;
	    _DONE;
	  }}
	int idx = -1;
	if(where_active){
	  for(int i = 0; i < table_info.size(); i++)
	    if(_condition->colum == table_info[i].name)
	      idx = i;
	  if(idx == -1){
	    cout<<"ERROR: La columna \'"<<_condition->colum<<"\' no existe en la tabla\n"<<endl;
	    _DONE;
	  }
	  if(!tools::check_type(_condition->value, table_info[idx].type)){
	    cout<<"ERROR: El tipo de la condicion y el tipo de la columna son diferentes\n"<<endl;
	    _DONE;
	  }
	}
	table_ram table_data = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
        ofstream new_table(db_table.path_data);
	for(const vector<string> &row : table_data){
	  if(where_active && !tools::compare_values(row[idx], _condition->value, table_info[idx].type, _condition->opt))
	    for(int i = 0; i < row.size(); i++)
	      new_table << row[i] << AEA_TOKEN;
	  else{
	    for(int i = 0; i < row.size(); i++){
	      if(change[i]) new_table << new_data[i];
	      else new_table << row[i];
	      new_table << AEA_TOKEN;
	    }}
	  new_table << '\n';
	}
	cout<<"Modificacion exitosa\n"<<endl;
	new_table.close();
	_DONE;
      }
  }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
}

void response::_delete(void *args)
{
  args_delete* valid_args = (args_delete*) args;
  string _table_name = valid_args->table;
  for(const meta_table &db_table : *db_tables){
    if(db_table.name == _table_name)
      {
	vector<table_colum> table_info = tools::read_file<table_colum>(db_table.path_info, GAA_TOKEN);
	bool where_active = false;
	args_where* _condition = valid_args->condition;
	if(_condition){
	  if(_condition->ok)
	    where_active = true;
	  else{
	    cout<<"ERROR: Condicion mal formulada. Utilice  el formato [columna operador valor]\n"<<endl;
	    _DONE;
	  }}
	int idx = -1;
	if(where_active){
	  for(int i = 0; i < table_info.size(); i++)
	    if(_condition->colum == table_info[i].name)
	      idx = i;
	  if(idx == -1){
	    cout<<"ERROR: La columna \'"<<_condition->colum<<"\' no existe en la tabla\n"<<endl;
	    _DONE;
	  }
	  if(!tools::check_type(_condition->value, table_info[idx].type)){
	    cout<<"ERROR: El tipo de la condicion y el tipo de la columna son diferentes\n"<<endl;
	    _DONE;
	  }
	}
	table_ram table_data = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
        ofstream new_table(db_table.path_data);
	for(const vector<string> &row : table_data){
	  if(where_active && !tools::compare_values(row[idx], _condition->value, table_info[idx].type, _condition->opt)){
	    for(int i = 0; i < row.size(); i++)
	      new_table << row[i] << AEA_TOKEN;
	    new_table << '\n';
	  }}
	cout<<"Borrado exitoso\n"<<endl;
	new_table.close();
	_DONE;
      }
  }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
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
  keys[SELECT] = &response::_select;
  keys[UPDATE] = &response::_update;
  keys[DELETE] = &response::_delete;
}
