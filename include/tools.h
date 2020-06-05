#pragma once
#include "db_env.h"

using namespace std;

struct tools{
  template<typename T>
  static vector<T> read_gaa_file(string path)
  {
    vector<T> data;
    ifstream file;
    file.open(path);
    string A, B, current_row;
    while(file >> current_row)
      {
	int partition = current_row.find('#');
	A = current_row.substr(0, partition);
	B = current_row.substr(partition+1);
	data.emplace_back(A, B);
      }
    file.close();
    return data;
  }

  template<typename T>
  static vector_gaa cast_vec_gaa(vector<T> &cur_vec)
  {
    vector_gaa ans;
    for(T &reg : cur_vec){
      ans.push_back(reg.cast_pair());
    }
    return ans;
  }

  static bool check_type(string &data, string type)
  {
    if(type == "int")
      {
	// 
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
