#include "../include/index.h"

using namespace std;

void index_ram::build(table_ram *table)
{
  main_table = table;
  data.clear();
  for(int i = 0; i < main_table->size(); i++)
    {
      int key = stoi((*main_table)[i][col_ind]);
      data[key].push_back(i);
    }
  table_size = main_table->size();
}

void index_ram::insert(string value)
{
  int util_value = stoi(value);
  data[util_value].push_back(table_size);
  table_size++;
}

void index_ram::search(vector<int> &rows, args_where* args)
{
  int val = stoi(args->value);
  if(args->opt == EQUAL)
    {
      rows = data[val];
    }
  if(args->opt == LESS)
    {
      auto it = data.begin();
      while(it != data.end() && it->first < val){
	for(const int &i : it->second) rows.push_back(i);
	it++;
      }
    }
  if(args->opt == GREATER)
    {
      auto it = data.rbegin();
      while(it != data.rend() && it->first > val){
	for(const int &i : it->second) rows.push_back(i);
	it++;
      }
    }
}

void index_ram::update(int row, string new_val)
{
  int n_value = stoi(new_val);
  int p_value = stoi((*main_table)[row][col_ind]);
  data[p_value].erase(find(data[p_value].begin(), data[p_value].end(), row));
  if(data[p_value].empty()){
    data.erase(p_value);
  }
  data[n_value].push_back(row);
}

index_ram::index_ram(table_ram* table, int ind) : col_ind(ind)
{
  build(table);
}
