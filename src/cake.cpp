#include "../include/bizcochitoDB.h"

using namespace std;

int main()
{
  ios_base::sync_with_stdio(false);
  bizcochitoDB* DB = new bizcochitoDB();

  string version = "1.2", tmp;
  cout << "cake (" << version << ")" << endl;

  cout << "Load file ? (y/n) ";
  char ans;
  cin >> ans;
  getline(cin, tmp);
  
  if(ans == 'y')
    DB->runscript();
  else
    DB->run();
}
