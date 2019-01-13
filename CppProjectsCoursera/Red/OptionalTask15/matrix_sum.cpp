#include "test_runner.h"
#include "profile.h"
#include <vector>
#include <future>
#include <numeric>
using namespace std;

template <typename Iterator>
class Paginator {
public:
    class Page {
        Iterator Begin;
        Iterator End;
        const size_t Size;
    public:
        Page(Iterator begin_, Iterator end_) : Begin(begin_), End(end_), Size(distance(begin_, end_)) {}

        size_t size() const { return Size;}
        auto& begin() { return Begin;}
        const auto& begin() const { return Begin;}
        auto& end() { return End;}
        const auto& end() const { return End;}
    };

    Paginator(Iterator begin_, Iterator end_, size_t page_size_)
    {
        bool ahead = true;
        if (begin_ == end_) ahead = false;
        while (ahead) {
            if (distance(begin_, end_) > page_size_) {
                auto current_end = next(begin_,page_size_);
                pages.push_back(Page(begin_, current_end));
                begin_ = current_end;
            } else {
                pages.push_back(Page(begin_, end_));
                ahead = false;
            }
        }
    }

    auto begin() {return pages.begin();}
    auto begin() const {return pages.begin();}
    auto end() const {return pages.end();}
    auto end() {return pages.end();}
    size_t size() const {return pages.size();}

private:
    vector<Page> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}

#define NUM_THREADS 4

int64_t CalculateMatrixSumS(const vector<vector<int>>& matrix) {
    const size_t matrix_size = matrix.size();
    if (matrix_size == 0) return 0;
    int64_t result = 0;
    /*auto paginator = Paginate(matrix, matrix_size / NUM_THREADS + 1);*/
    for (auto pag : /*paginator*/ Paginate(matrix, matrix_size / NUM_THREADS + 1))
    {
        result += [&pag] () {
            int64_t single_result = 0;
            for (auto& v : pag)
            {
                single_result = accumulate(v.begin(), v.end(), single_result);
            }
            return single_result;
        }();
    }
    return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    const size_t matrix_size = matrix.size();
    if (matrix_size == 0) return 0;
    vector<future<int64_t>> futures;
    auto paginator = Paginate(matrix, matrix_size / NUM_THREADS + 1);
    for (auto pag : paginator /*Paginate(matrix, matrix_size / NUM_THREADS + 1)*/)
    {
        futures.push_back(
                    async([/*&*/pag] () {
                        int64_t single_result = 0;
                        for (auto& v : pag)
                        {
                            single_result = accumulate(v.begin(), v.end(), single_result);
                        }
                        return single_result;
                    })
        );
    }

    int64_t result = 0;
    for (auto& f : futures) {
        result += f.get();
    }
    return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
  const vector<vector<int>> matrix1 = {
    {1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix1), 1);
  const vector<vector<int>> matrix2;
  ASSERT_EQUAL(CalculateMatrixSum(matrix2), 0);
  const vector<vector<int>> matrix3 = {
    {1, 2, 3, 4, 5, 6},
    {5, 6, 7, 8, 9, 10},
    {9, 10, 11, 12, 13, 14},
    {13, 14, 15, 16, 17, 18}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix3), 228);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
