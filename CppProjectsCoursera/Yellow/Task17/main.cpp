#include <iostream>
#include <string>
#include <stack>
#include <utility>

using namespace std;

void PrintExpression(const int& first_value, stack<pair<char, int>>& st_ovfl)
{
    if (!st_ovfl.empty())
    {
        pair<char, int> curr = st_ovfl.top();
        st_ovfl.pop();
        cout << "(";
        PrintExpression(first_value, st_ovfl);
        cout << ") " << curr.first <<  " " << curr.second;
    } else {
        cout << first_value;
    }
}

int main()
{
    int first_value = 0;
    int operation_count = 0;
    cin >> first_value >> operation_count;
    stack<pair<char, int>> st_ovfl;
    for (; operation_count > 0; --operation_count)
    {
        char operation;
        int operand;
        cin >> operation >> operand;
        st_ovfl.push({operation,operand});
    }
    PrintExpression(first_value, st_ovfl);
    return 0;
}
