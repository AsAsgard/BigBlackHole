#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y)  out << (x) << std::endl << (y) << std::endl

int main() {
  TestRunner tr;
  tr.RunTest([] {
    ostringstream output;
    PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  }, "PRINT_VALUES usage example");

  tr.RunTest([] {
    ostringstream output;
    if (false)
        PRINT_VALUES(output, 5+1, "red belt");
    else
        cout << "false way" << endl;
    ASSERT_EQUAL(output.str(), "");
  }, "PRINT_VALUES1 usage example");

  tr.RunTest([] {
    ostringstream output;
    if (true)
        PRINT_VALUES(output, 5+1, "red belt");
    else
        cout << "false way" << endl;
    ASSERT_EQUAL(output.str(), "6\nred belt\n");
  }, "PRINT_VALUES2 usage example");
}
