#include "simple_vector.h"
#include "test_runner.h"

#include <numeric>
#include <vector>
#include <tuple>

using namespace std;

void TestCopyAssignment() {
  SimpleVector<int> numbers(10);
  iota(numbers.begin(), numbers.end(), 1);

  SimpleVector<int> dest;
  ASSERT_EQUAL(dest.Size(), 0u);

  dest = numbers;
  ASSERT_EQUAL(dest.Size(), numbers.Size());
  ASSERT(dest.Capacity() >= dest.Size());
  ASSERT(equal(dest.begin(), dest.end(), numbers.begin()));

  SimpleVector<int> source;
  SimpleVector<int> light = source;
  dest = source;

  ASSERT_EQUAL(dest.Size(), 0);
  ASSERT_EQUAL(dest.Capacity(), 0);
  ASSERT_EQUAL(light.Size(), 0);
  ASSERT_EQUAL(light.Capacity(), 0);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCopyAssignment);
  return 0;
}
