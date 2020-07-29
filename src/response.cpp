#include "../include/response.h"
#define CLI

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
  cout<<"Error de nombre, el nombre del campo ingresado no es valido\n"<<endl;
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
  cout<<"\t \\d [TABLE] \t Describe la tabla especificada\n";
  cout<<"\t \\di \t\t Lista los indices existentes\n";
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
  printf("| ");
  for(int i = 0; i < n_cols; i++){
    cout<<cols[i];
    for(int j = 0; j < max_size[i] - cols[i].size(); j++) printf(" ");
    printf(" | ");
  }
  printf("\n|");
  for(int i = 0; i < n_cols; i++){
    for(int j = 0; j < max_size[i] + 2; j++) printf("-");
    printf(i + 1 == n_cols ? "|\n" : "+");
  }
  for(vector<string> &row : table){
    printf("| ");
    for(int i = 0; i < n_cols; i++){
      cout<<row[i];
      for(int j = 0; j < max_size[i] - row[i].size(); j++) printf(" ");
      printf(" | ");
    }
    printf("\n");
  }
  cout<<"("<<table.size()<<" filas)"<<endl;
}

void response::_dt(void *args)
{
  table_ram _table= tools::cast_table<meta_table>(*db_tables);
  cout<<"Tablas actuales\n";
  print_table(_table, {"tabla", "path_info", "path_data"});
  cout<<endl;
}

void response::_di(void *args)
{
  table_ram _table= tools::cast_table<meta_index>(*db_indexes);
  cout<<"Indices actuales\n";
  print_table(_table, {"tabla", "columna", "path_data"});
  cout<<endl;
}

void response::_d_table(void *args)
{
  string* valid_args = (string*)args;
  string _table_name = *valid_args;
  for(meta_table &db_table : *db_tables)
    if(db_table.name == _table_name)
      {
        table_ram table_info = tools::read_file<vector<string> >(db_table.path_info, GAA_TOKEN);
	cout<<"Tabla \'"<<_table_name<<"\'\n";
        print_table(table_info, {"columna", "tipo"});
	for(meta_index &db_index : *db_indexes)
	  if(db_index.table == _table_name)
	    cout << "INDEX: indice secundario sobre (" << db_index.colum << ")\n";
	cout<<endl;
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

void response::_create_index(void *args)
{
  str_duo* valid_args = (str_duo*) args;
  string &_table_name = valid_args->first;
  string &_colum = valid_args->second;
  for(meta_index &db_index : *db_indexes)
    if(db_index.table == _table_name && db_index.colum == _colum){
      cout<<"ERROR: La tabla \'"<<_table_name<<"\' ya posee un indice sobre \'"<<_colum<<"\'\n"<<endl;
      _DONE;
    }
  for(const meta_table &db_table : *db_tables)
    if(db_table.name == _table_name)
      {
	vector<table_colum> table_info = tools::read_file<table_colum>(db_table.path_info, GAA_TOKEN);
	int ind = -1;
	for(int i = 0; i < table_info.size(); i++){
	  if(table_info[i].name == _colum) ind = i;
	}
	if(ind == -1){
	  cout<<"ERROR: La columna \'"<<_colum<<"\' no existe en la tabla\n"<<endl;
	  _DONE;
	}
	string _path_data = INDEXES_DATA_PATH + _table_name + INDEX_TOKEN + _colum + ".aea";
	ofstream index_file(META_INDEXES_PATH, ofstream::app);
	index_file <<_table_name << GAA_TOKEN << _colum << GAA_TOKEN << _path_data << GAA_TOKEN << ind << GAA_TOKEN << '\n';
	vector<string> _info = {_table_name, _colum, _path_data, to_string(ind)};
	db_indexes->emplace_back(_info);
	cout<<"Indice sobre \'"<<_table_name<<"\' creado exitosamente\n"<<endl;
	index_file.close();
	_DONE;
      }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
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
	  }}
	if(valid_args->default_cols){
	  for(const table_colum &colum : table_info)
	    q_cols.push_back(colum.name);
	}
	if(q_cols.size() != q_vals.size()){
	  cout<<"ERROR: El numero de expresiones no es igual al numero de columnas\n"<<endl;
	  _DONE;
	}
	string util_info;
	vector<string> _data;
	for(const table_colum &colum : table_info)
	  {
	    int pos = find(q_cols.begin(), q_cols.end(), colum.name) - q_cols.begin();
	    if(pos == q_cols.size())
	      {
		util_info += (__NULL + AEA_TOKEN);
		_data.push_back(__NULL);
	      }
	    else{
	      if(tools::check_type(q_vals[pos], colum.type))
		{
		  util_info += (q_vals[pos] + AEA_TOKEN);
		  _data.push_back(q_vals[pos]);
		  q_vals.erase(q_vals.begin()+pos);
		  q_cols.erase(q_cols.begin()+pos);
		}
	      else{
		cout<<"ERROR: El tipo de entrada y el tipo de columna difieren\n"<<endl;
		_DONE;
	      }
	    }}
        if(active_indexes.count(_table_name))
	  {
	    for(auto &p : active_indexes[_table_name]){
	      p.second->insert(_data[p.second->col_ind]);
	    }
	  }
	if(active_tables.count(_table_name))
	  {
	    active_tables[_table_name].push_back(_data);
	  }
	ofstream table_data(db_table.path_data, ofstream::app);
	table_data << util_info << '\n';
	table_data.close();	
	#ifdef CLI
	cout<<"Insercion exitosa\n"<<endl; // Todo Ok!
	#endif	
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
	if(active_tables.count(_table_name) == 0)
	  {
	    active_tables[_table_name] = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
	  }
	bool index_active = false;
	if(where_active){
	  if(active_indexes.count(_table_name) && active_indexes[_table_name].count(_condition->colum)) index_active = true; 
	  else{
	    for(meta_index &db_index : *db_indexes)
	      if(db_index.table == _table_name && db_index.colum == _condition->colum){
		active_indexes[_table_name][_condition->colum] = new index_ram(&active_tables[_table_name], db_index.col_idx);
		index_active = true;
	      }}
	}
	vector<int> affected_rows;
	if(index_active){
	  active_indexes[_table_name][_condition->colum]->search(affected_rows, _condition);
	}
	else{
	  for(int i = 0; i < active_tables[_table_name].size(); i++) affected_rows.push_back(i);
	}
	table_ram table_query;
	for(const int i : affected_rows){
	  vector<string> &row = active_tables[_table_name][i];
	  if(!where_active || (where_active && tools::compare_values(row[where_idx], _condition->value, table_info[where_idx].type, _condition->opt))){
	    vector<string> tmp;
	    for(int idx : idx_col)
	      tmp.push_back(row[idx]);
	    table_query.push_back(tmp);
	  }}
	print_table(table_query, q_cols);
	cout<<endl;
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
	if(active_tables.count(_table_name) == 0)
	  {
	    active_tables[_table_name] = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
	  }
        ofstream new_table(db_table.path_data);
	for(int j = 0; j < active_tables[_table_name].size(); j++){
	  vector<string> &row = active_tables[_table_name][j];
	  if(where_active && !tools::compare_values(row[idx], _condition->value, table_info[idx].type, _condition->opt))
	    for(int i = 0; i < row.size(); i++)
	      new_table << row[i] << AEA_TOKEN;
	  else{
	    if(active_indexes.count(_table_name))
	      for(auto &p : active_indexes[_table_name]){
		if(change[p.second->col_ind]) p.second->update(j, new_data[p.second->col_ind]);
	      }
	    for(int i = 0; i < row.size(); i++){
	      if(change[i]){
		new_table << new_data[i];
		row[i] = new_data[i];
	      }
	      else new_table << row[i];
	      new_table << AEA_TOKEN;
	    }
	  }
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
	if(active_tables.count(_table_name) == 0)
	  {
	    active_tables[_table_name] = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
	  }
        ofstream new_table(db_table.path_data);
	bool change = false;
	for(const vector<string> &row : active_tables[_table_name]){
	  if(where_active && !tools::compare_values(row[idx], _condition->value, table_info[idx].type, _condition->opt)){
	    for(int i = 0; i < row.size(); i++)
	      new_table << row[i] << AEA_TOKEN;
	    new_table << '\n';
	  }
	  else change = true;
	}
	new_table.close();
	if(change){
	  active_tables[_table_name] = tools::read_file<vector<string> >(db_table.path_data, AEA_TOKEN);
	  if(active_indexes.count(_table_name))
	    {
	      for(auto &p : active_indexes[_table_name]){
		p.second->build(&active_tables[_table_name]);
	      }
	    }}
	cout<<"Borrado exitoso\n"<<endl;
	_DONE;
      }
  }
  cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
  _DONE;
}

void response::_drop_table(void *args)
{
  string* valid_args = (string*)args;
  string _table_name = *valid_args;
  ofstream new_file(META_TABLES_PATH);
  auto rem = db_tables->end();
  for(auto cur_table = db_tables->begin(); cur_table != db_tables->end(); cur_table++){
    if(cur_table->name == _table_name)
      rem = cur_table;
    else
      new_file << cur_table->name << GAA_TOKEN << cur_table->path_info << GAA_TOKEN << cur_table->path_data << GAA_TOKEN << '\n';
  }
  if(rem == db_tables->end()){
    cout<<"ERROR: No se encontro ninguna tabla llamada \'"<<_table_name<<"\'\n"<<endl;
    _DONE;
  }
  //drop index
  cout<<"Tabla \'"<<rem->name<<"\' eliminada correctamente\n"<<endl; // Todo OK!
  remove(rem->path_info.c_str());
  remove(rem->path_data.c_str());
  db_tables->erase(rem);
  new_file.close();
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
  #ifdef CLI
  
  t_start = high_resolution_clock::now();
  (this->*keys[query_code])(query.second);
  t_end = high_resolution_clock::now();
  t_total = duration_cast<chrono::nanoseconds>(t_end - t_start).count() * 1e-6;
  if(query_code > D)
    cout << fixed << setprecision(5) << "Time: " << t_total << " ms" <<endl;
  
  #else
  
  (this->*keys[query_code])(query.second);
  
  #endif
}

response::response(vector<meta_table>* a, vector<meta_index>* b) : db_tables(a), db_indexes(b)
{
  keys[SYNTAX_ERROR] = &response::_syntax_error;
  keys[TYPE_ERROR] = &response::_type_error;
  keys[NAME_ERROR] = &response::_name_error;
  keys[EMPTY_ERROR] = &response::_empty_error;
  
  keys[HELP] = &response::_help;
  keys[DT] = &response::_dt;
  keys[DI] = &response::_di;

  keys[D] = &response::_d_table;
  keys[CREATE_TABLE] = &response::_create_table;
  keys[CREATE_INDEX] = &response::_create_index;
  keys[INSERT_INTO] = &response::_insert_into;
  keys[SELECT] = &response::_select;
  keys[UPDATE] = &response::_update;
  keys[DELETE] = &response::_delete;
  keys[DROP_TABLE] = &response::_drop_table;
}

response::~response()
{
  for(auto p : active_indexes)
    for(auto q: p.second)
      delete q.second;
}
