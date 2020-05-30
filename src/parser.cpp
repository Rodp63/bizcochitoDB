#include "../include/parser.h"

using namespace std;

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


void parser::get_type()
{
  string tipo = get_word();
  if(keys.find(tipo) == keys.end())
    {
      query_type = SYNTAX_ERROR;
      return;
    }
  query_type = keys[tipo];
  if((keys[tipo] == EXIT ||
      keys[tipo] == HELP ||
      keys[tipo] == DT) && current_pos < query_size)
    {
      query_type = SYNTAX_ERROR;
      return;
    }
}


void parser::_create_table()
{
  string table_name = get_word('{');
  if(!table_name.empty() && current_pos < query_size && query[current_pos++] == '{')
    {
      args_new_table *current_args = new args_new_table;
      current_args->name = table_name;
      while(current_pos < query_size && query[current_pos] != '}')
	{
	  string name_arg = get_word(',', '}');
	  string type_arg = get_word(',', '}');
	  if(name_arg.empty() || type_arg.empty())
	    {
	      query_type = SYNTAX_ERROR;
	      delete current_args;
	      return;
	    }
	  if(find(arg_types.begin(), arg_types.end(), type_arg) == arg_types.end())
	    {
	      query_type = TYPE_ERROR;
	      delete current_args;
	      return;
	    }
	  if(name_arg == type_arg)
	    {
	      query_type = NAME_ERROR;
	      delete current_args;
	      return; 
	    }
	  if(current_pos < query_size)
	    {
	      if(query[current_pos] == ',')
		{
		  current_pos++;
		}
	      else if(query[current_pos] != '}')
		{
		  query_type = SYNTAX_ERROR;
		  delete current_args;
		  return;
		}
	    }
	  current_args->data.emplace_back(name_arg, type_arg);
	}
      string last = get_word();
      if(current_pos != query_size || last != "}")
	{
	  query_type = SYNTAX_ERROR;
	  delete current_args;
	  return;
	}
      if(current_args->data.empty())
	{
	  query_type = EMPTY_ERROR;
	  delete current_args;
	  return;
	}
      query_args = (void*)current_args; // todo ok!
    }
  else
    {
      query_type = SYNTAX_ERROR;
      return;
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
