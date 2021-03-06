#ifndef NODE_H
#define NODE_H

#include "date.h"
#include <memory>

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    And,
    Or
};


class Node
{
public:
    Node() {}
    virtual ~Node() {}
    virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
};


class EmptyNode : public Node
{
public:
    EmptyNode() {}

    bool Evaluate(const Date& date, const std::string& event) const override;
};


class DateComparisonNode : public Node
{
public:
    DateComparisonNode(const Comparison& operation_, const Date& date_) :
    operation(operation_),
    desiredDate(date_) {}

    bool Evaluate(const Date& date, const std::string& event) const override;
private:
    Comparison operation;
    Date desiredDate;
};


class EventComparisonNode : public Node
{
public:
    EventComparisonNode(const Comparison& operation_, const std::string& event_) :
    operation(operation_),
    desiredEvent(event_) {}

    bool Evaluate(const Date& date, const std::string& event) const override;
private:
    Comparison operation;
    std::string desiredEvent;
};


class LogicalOperationNode : public Node
{
public:
    LogicalOperationNode(const LogicalOperation& operation_,
                         const std::shared_ptr<Node>& left_,
                         const std::shared_ptr<Node>& right_) :
    operation(operation_),
    left(left_),
    right(right_) {}

    bool Evaluate(const Date& date, const std::string& event) const override;
private:
    LogicalOperation operation;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

#endif // NODE_H
