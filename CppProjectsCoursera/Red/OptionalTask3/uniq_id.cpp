#include <string>
#include <vector>
using namespace std;

#define LINE(a) var##a
#define ADDLINE(a) LINE(a)
#define UNIQ_ID ADDLINE(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}
