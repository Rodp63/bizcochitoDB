#pragma once
#include "db_env.h"

using namespace std;

struct tools{
  template<typename T>
  static vector<T> read_file(string path, char token)
  {
    vector<T> data;
    ifstream file;
    file.open(path);
    string A, current_row;
    while(getline(file, current_row))
      {
	int pos = 0, partition;
	vector<string> info;
	while(pos < current_row.size())
	  {
	    partition = current_row.find(token, pos);
	    A = current_row.substr(pos, partition - pos);
	    info.push_back(A);
	    pos = partition + 1;
	  }
	data.emplace_back(info);
      }
    file.close();
    return data;
  }

  template<typename T>
  static table_ram cast_table(vector<T> &cur_vec)
  {
    table_ram ans;
    for(T &reg : cur_vec){
      ans.push_back(reg.cast_vec());
    }
    return ans;
  }

  static bool check_type(string &data, string type)
  {
    if(type == "int")
      {
	
      }
    if(type == "text" || type == "date")
      {
	if(data[0] != '\'' || data.back() != '\'')
	  return false;
	if(type == "date")
	  {
	    
	  }
      }
    return true;
  }
  
};
