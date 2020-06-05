#include "../include/parser.h"

using namespace std;

parser::parser()
{
  keys["exit"] = EXIT;
  keys["help"] = HELP;
  keys["\\dt"] = DT;
  keys["\\d"] = D;
  keys["create_table"] = CREATE_TABLE;
  keys["insert_into"] = INSERT_INTO;

  decode[D] = &parser::_d_table;
  decode[CREATE_TABLE] = &parser::_create_table;
  decode[INSERT_INTO] = &parser::_insert_into;
}

void parser::get_query()
{
  getline(cin, query);
  query_size = query.size();
  current_pos = 0;
}

string parser::get_word(char token1 = ' ', char token2 = ' ')
{
  string word;
  for(; current_pos < query_size && query[current_pos] == ' '; current_pos++);
  for(; current_pos < query_size
	&& query[current_pos] != ' '
	&& query[current_pos] != token1
	&& query[current_pos] != token2;)
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
  if(keys.find(tipo) == keys.end())
    {
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
  for(char c : reserved_characters)
    {
      if(find(text.begin(), text.end(), c) != text.end())
	return false;
    }
  return true;
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
      THROW_(EMPTY_ERROR);
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
  if(!table_name.empty())
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
	      if(col.empty())
		{
		  THROW_(SYNTAX_ERROR);
		}
	      if(!check_word(col))
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
	THROW_(SYNTAX_ERROR);
      }
    }
  else{
    THROW_(SYNTAX_ERROR);
  }
}


query_info parser::parse()
{
  get_type();
  query_args = nullptr;
  if(query.empty())
    {
      return make_pair(NOTHING,nullptr);
    }
  if(query_type > DT)
    {
      (this->*decode[query_type])();
    }
  return make_pair(query_type, query_args);
}
