#include "../include/parser.h"

using namespace std;

void parser::get_query()
{
  getline(cin, query);
  query_size = query.size();
  current_pos = 0;
}

string parser::get_word(char token1 = ' ', char token2 = ' ', char token3 = ' ')
{
  string word;
  for(; current_pos < query_size && query[current_pos] == ' '; current_pos++);
  for(; current_pos < query_size
	&& query[current_pos] != ' '
	&& query[current_pos] != token1
	&& query[current_pos] != token2
	&& query[current_pos] != token3;)
    {
      word += query[current_pos];
      current_pos++;
    }
  for(; current_pos < query_size && query[current_pos] == ' '; current_pos++);
  return word;
}

string parser::get_phrase(char token)
{
  string phrase;
  for(current_pos++; current_pos < query_size && query[current_pos] != token; current_pos++)
    {
      phrase += query[current_pos];
    }
  for(current_pos++; current_pos < query_size && query[current_pos] == ' '; current_pos++);
  return token + phrase + token;
}

void parser::get_type()
{
  string tipo = get_word();
  if(tipo.empty()){
    query_type = NOTHING;
    return;
  }
  if(keys.find(tipo) == keys.end()){
    query_type = SYNTAX_ERROR;
    return;
  }
  query_type = keys[tipo];
  if((query_type == EXIT ||
      query_type == HELP ||
      query_type == DT) && current_pos < query_size)
    {
      query_type = SYNTAX_ERROR;
      return;
    }
}

bool parser::check_word(string &text)
{
  if(tools::check_type(text, "int"))
    return false;
  for(char c : reserved_characters){
    if(text.find(c) != string::npos)
      return false;
  }
  return true;
}

args_where* parser::parse_condition()
{
  args_where *args = new args_where;
  string col = get_word('=', '<', '>');
  string op = current_pos < query_size ? string(1, query[current_pos]) : __NULL;
  for(current_pos++; current_pos < query_size && query[current_pos] == ' '; current_pos++);
  string val = (current_pos < query_size && query[current_pos] == '\'') ? get_phrase('\'') : get_word();
  if(keys.find(op) == keys.end())
    args->ok = false;
  else{
    int opp = keys[op];
    if(col.empty() || val.empty() || current_pos < query_size)
      args->ok = false;
    else{
      args->ok = true;
      args->colum = col;
      args->value = val;
      args->opt = opp;
    }}
  return args;
}

void parser::_d_table()
{
  string *current_args = new string(get_word());
  if(current_pos < query_size)
    {
      THROW_(SYNTAX_ERROR);
    }
  if(current_args->empty())
    {
      THROW_(EMPTY_ERROR);
    }
  query_args = (void*) current_args;
}

void parser::_create_table()
{
  args_table *current_args = new args_table;
  string table_name = get_word('(');
  if(!check_word(table_name))
    {
      THROW_(NAME_ERROR);
    }
  else if(!table_name.empty() && current_pos < query_size && query[current_pos++] == '(')
    {
      for(; current_pos < query_size && query[current_pos] == ' '; current_pos++);
      while(current_pos < query_size && query[current_pos] != ')')
	{
	  string name_arg = get_word(',', ')');
	  string type_arg = get_word(',', ')');
	  if(name_arg.empty() || type_arg.empty())
	    {
	      THROW_(SYNTAX_ERROR);
	    }
	  if(find(data_types.begin(), data_types.end(), type_arg) == data_types.end())
	    {
	      THROW_(TYPE_ERROR);
	    }
	  if(name_arg == type_arg || !check_word(name_arg)) 
	    {
	      THROW_(NAME_ERROR);
	    }
	  if(current_pos < query_size)
	    {
	      if(query[current_pos] == ',') current_pos++;
	      else if(query[current_pos] != ')')
		{
		  THROW_(SYNTAX_ERROR);
		}
	    } 
	  current_args->data.emplace_back(name_arg, type_arg);
	}
      string last = get_word();
      if(current_pos != query_size || last != ")")
	{
	  THROW_(SYNTAX_ERROR);
	}
      if(current_args->data.empty())
	{
	  THROW_(EMPTY_ERROR);
	}
      current_args->name = table_name;
      query_args = (void*)current_args; // todo ok!
    }
  else{
    THROW_(SYNTAX_ERROR);
  }
}

void parser::_insert_into()
{
  args_insert* current_args = new args_insert;
  string table_name = get_word('(');
  if(!table_name.empty() || !check_word(table_name))
    {
      current_args->table = table_name;
      current_args->default_cols = true;
      if(current_pos < query_size && query[current_pos] == '(')
	{
	  current_pos++;
	  current_args->default_cols = false;
	  while(current_pos < query_size && query[current_pos] != ')')
	    {
	      string col = get_word(',',')');
	      if(col.empty() || !check_word(col))
		{
		  THROW_(SYNTAX_ERROR);
		}
	      if(current_pos < query_size)
		{
		  if(query[current_pos] == ',') current_pos++;
		  else if(query[current_pos] != ')')
		    {
		      THROW_(SYNTAX_ERROR);
		    }
		}
	      current_args->col_data.push_back(col);
	    }
	  current_pos++;
	}
      string tag = get_word('(');
      if(tag == "values" && current_pos < query_size && query[current_pos++] == '(')
	{
	  for(; current_pos < query_size && query[current_pos] == ' '; current_pos++);
	  while(current_pos < query_size && query[current_pos] != ')')
	    {
	      string val = query[current_pos] == '\'' ? get_phrase('\'') : get_word(',',')');
	      if(val.empty())
		{
		  THROW_(SYNTAX_ERROR);
		}
	      if(current_pos < query_size)
		{
		  if(query[current_pos] == ',') current_pos++;
		  else if(query[current_pos] != ')')
		    {
		      THROW_(SYNTAX_ERROR);
		    }
		} 
	      current_args->val_data.push_back(val);
	      for(; current_pos < query_size && query[current_pos] == ' '; current_pos++);
	    }
	  string last = get_word();
	  if(current_pos != query_size || last != ")")
	    {
	      THROW_(SYNTAX_ERROR);
	    }
	  if(current_args->val_data.empty())
	    {
	      THROW_(EMPTY_ERROR);
	    }
	  query_args = (void*) current_args; // todo ok!
	}
      else{
	THROW_(SYNTAX_ERROR);}
    }
  else{
    THROW_(SYNTAX_ERROR);}
}

void parser::_select()
{
  args_select* current_args = new args_select;
  int pos_from = query.find(" from ");
  if(pos_from++ == string::npos)
    {
      THROW_(SYNTAX_ERROR);
    }
  while(current_pos < pos_from)
    {
      string col = get_word(',');
      if(col.empty() || !check_word(col))
	{
	  THROW_(SYNTAX_ERROR);
	}
      if(query[current_pos] == ',') current_pos++;
      else if(current_pos != pos_from)
	{
	  THROW_(SYNTAX_ERROR);
	}
      current_args->col_data.push_back(col);
    }
  if(current_pos != pos_from || current_args->col_data.empty())
    {
      THROW_(SYNTAX_ERROR);
    }
  current_pos += 4;
  string table_name = get_word();
  if(table_name.empty() || !check_word(table_name))
    {
      THROW_(SYNTAX_ERROR);
    }
  current_args->table = table_name;
  string tag = get_word();
  if(tag.empty())
    current_args->condition = nullptr;
  else{
    if(tag == "where")
      current_args->condition = parse_condition();
    else{
      THROW_(SYNTAX_ERROR);}
  }
  query_args = (void*) current_args; // Todo OK!! 
}

void parser::_update()
{
  args_update* current_args = new args_update;
  string table_name = get_word();
  if(table_name.empty() || !check_word(table_name))
    {
      THROW_(SYNTAX_ERROR);
    }
  current_args->table = table_name;
  string tag = get_word();
  if(tag == "set")
    {
      int limit = query.find(" where ");
      limit = (limit == string::npos ? query_size : limit + 1);
      while(current_pos < limit)
	{
	  string col = get_word('=');
	  if(current_pos >= query_size || query[current_pos] != '=')
	    {
	      THROW_(SYNTAX_ERROR);
	    }
	  for(current_pos++; current_pos < query_size && query[current_pos] == ' '; current_pos++);
	  string val = (current_pos < query_size && query[current_pos] == '\'') ? get_phrase('\'') : get_word(',');
	  if(query[current_pos] == ',') current_pos++;
	  else if(current_pos != limit)
	    {
	      THROW_(SYNTAX_ERROR);
	    }
	  current_args->data.emplace_back(col, val);
	}
      if(current_pos != limit)
	{
	  THROW_(SYNTAX_ERROR);
	}
      if(current_args->data.empty())
	{
	  THROW_(EMPTY_ERROR);
	}
      current_args->condition = nullptr;
      if(current_pos < query_size)
	{
	  current_pos += 5;
	  current_args->condition = parse_condition();
	}
      query_args = (void*) current_args; // Todo OK!! 
    }
  else{
    THROW_(SYNTAX_ERROR);}
}

void parser::_delete()
{
  args_delete* current_args = new args_delete;
  string tag = get_word();
  if(tag == "from"){
    string table_name = get_word();
    if(table_name.empty() || !check_word(table_name))
      {
	THROW_(SYNTAX_ERROR);
      }
    current_args->table = table_name;
    string w = get_word();
    if(w.empty())
      current_args->condition = nullptr;
    else{
      if(w == "where")
	current_args->condition = parse_condition();	
      else{
	THROW_(SYNTAX_ERROR);}
    }
    query_args = (void*) current_args; // Todo OK!! 
  }
  else{
    THROW_(SYNTAX_ERROR);}
}

query_info parser::parse()
{
  get_type();
  query_args = nullptr;
  if(query_type >= EQUAL)
    {
      query_type = SYNTAX_ERROR;
    }
  if(query_type > DT)
    {
      (this->*decode[query_type])();
    }
  return make_pair(query_type, query_args);
}

parser::parser()
{
  keys["exit"] = EXIT;
  keys["help"] = HELP;
  keys["\\dt"] = DT;
  keys["\\d"] = D;
  keys["create_table"] = CREATE_TABLE;
  keys["insert_into"] = INSERT_INTO;
  keys["select"] = SELECT;
  keys["update"] = UPDATE;
  keys["delete"] = DELETE;

  keys["="] = EQUAL;
  keys["<"] = LESS;
  keys[">"] = GREATER;
  
  decode[D] = &parser::_d_table;
  decode[CREATE_TABLE] = &parser::_create_table;
  decode[INSERT_INTO] = &parser::_insert_into;
  decode[SELECT] = &parser::_select;
  decode[UPDATE] = &parser::_update;
  decode[DELETE] = &parser::_delete;
}
