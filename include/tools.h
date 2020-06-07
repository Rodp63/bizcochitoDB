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

  static bool check_type(const string &data, string type)
  {
    if(data.empty()) return false;
    if(type == "int"){
      int ind = 0;
      if(data[0] == '-' || data[0] == '+') ind++;
      for(int i = ind; i < data.size(); i++){
	if(!isdigit(data[i])) return false;
      }
    }
    if(type == "text" && (data[0] != '\'' || data.back() != '\''))
      return false;
    if(type == "date"){
      if(data[0] != '\'' || data.back() != '\'')
	return false;
      
    }
    return true;
  }

  static bool compare_values(const string &a, const string &b, string type, int opt)
  {
    if(opt == EQUAL){
      if(type == "int")
	return stoi(a) == stoi(b);
      return a == b;
    }
    if(opt == LESS){
      if(type == "int")
	return stoi(a) < stoi(b);
      return a < b;
    }
    if(opt == GREATER){
      if(type == "int")
	return stoi(a) > stoi(b);
      return a > b;
    }
    return true;
  }
  
};
