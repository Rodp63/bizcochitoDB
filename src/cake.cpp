#include "../include/bizcochitoDB.h"
//#define TEST

using namespace std;

int main()
{
  ios_base::sync_with_stdio(false);
  bizcochitoDB* DB = new bizcochitoDB();

  #ifdef TEST

  DB->runcode();

  #else

  string version = "v.1.2", tmp;
  cout << "cake (" << version << ")\n" << endl;

  cout << "|> Load file ? (y/n) ";
  char ans;
  cin >> ans;
  getline(cin, tmp);
  
  if(ans == 'y')
    DB->runscript();
  else
    DB->run();
  
  #endif
  
  delete DB;
}
