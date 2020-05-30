#pragma once
#include "db_env.h"

using namespace std;

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
