#include <iostream>
#include <exception>
#include <system_error>
#include <string>
using namespace std;

string AskTimeServer() {
     throw system_error(error_code());
}

class TimeServer {
public:
  string GetCurrentTime()
  {
      try {
          string tmp = AskTimeServer();
          LastFetchedTime = tmp;
      } catch(const system_error&) {}
      return LastFetchedTime;
  }
private:
  string LastFetchedTime = "00:00:00";
};


int main()
{
    TimeServer ts;
    try {
        cout << ts.GetCurrentTime() << endl;
    } catch (exception& e) {
        cout << "Exception got: " << e.what() << endl;
    }
    return 0;
}
