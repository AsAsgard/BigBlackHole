#include <iostream>
#include <string>
#include <queue>

using namespace std;

struct Operation {
    char operation;
    int operand;
    bool need_brackets;
};

void PrintExpression(queue<Operation>& qu_ovfl)
{
    while(!qu_ovfl.empty())
    {
        if (qu_ovfl.front().need_brackets) cout << ")";
        cout << " " << qu_ovfl.front().operation <<  " " << qu_ovfl.front().operand;
        qu_ovfl.pop();
    }
}


int main()
{
    int first_value = 0;
    int operation_count = 0;
    cin >> first_value >> operation_count;
    queue<Operation> qu_ovfl;
    char prev_operation = ' ';
    int brackets_count = 0;
    for (; operation_count > 0; --operation_count)
    {
        char operation;
        int operand;
        cin >> operation >> operand;
        bool nb = ((operation == '/' || operation == '*') && (prev_operation == '-' || prev_operation == '+')) && (prev_operation != ' ');
        qu_ovfl.push({operation,operand,nb});
        if (nb) ++brackets_count;
        prev_operation = operation;
    }
    for (; brackets_count > 0; --brackets_count) cout << "(";
    cout << first_value;
    PrintExpression(qu_ovfl);
    return 0;
}
