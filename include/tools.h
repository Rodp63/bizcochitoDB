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
    if(data.empty())
      return false;
    if(type == "int")
      {
	int ind = 0;
	if(data[0] == '-' || data[0] == '+') ind++;
	for(int i = ind; i < data.size(); i++){
	  if(!isdigit(data[i])) return false;
	}}
    if(type == "text" && (data[0] != '\'' || data.back() != '\''))
      {
	return false;
      }
    if(type == "date")
      {
	if(data[0] != '\'' || data.back() != '\'')
	  return false;
	string tmp = data.substr(1,data.size()-2);
	struct tm cur;
	return strptime(tmp.c_str(), "%d-%m-%Y", &cur);
      }
    return true;
  }

  static bool compare_values(const string &a, const string &b, string type, int opt)
  {
    if(type == "date")
      {
	string A1 = a.substr(1, a.size()-2);
	string A2 = b.substr(1, b.size()-2);
	struct tm T1 = {0,0,0,0,0,0,0,0,0};
	struct tm T2 = {0,0,0,0,0,0,0,0,0};
	strptime(A1.c_str(), "%d-%m-%Y", &T1);
	strptime(A2.c_str(), "%d-%m-%Y", &T2);
	time_t ut1 = mktime(&T1);
	time_t ut2 = mktime(&T2);
	if(opt == EQUAL) return ut1 == ut2;
	if(opt == LESS) return ut1 < ut2;
	if(opt == GREATER) return ut1 > ut2;
      }
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
