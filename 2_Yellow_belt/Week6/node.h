#pragma once
#include <sstream>
#include <memory>
#include "date.h"
#include "token.h"
using namespace std;

enum class LogicalOperation {
	Or,
	And
};

enum class Comparison {
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};

class Node {
public:
	virtual bool Evaluate(const Date& date, const string& event) const { return true; };
};

class EmptyNode : public Node {
public:
	EmptyNode() {}

	bool Evaluate(const Date& date, const string& event) const override
	{
		return true;
	}
};

class DateComparisonNode : public Node {
public:
	DateComparisonNode(const Comparison& op, const Date& date) : op_(op), date_(date) {}

	bool Evaluate(const Date& date, const string& event) const override {
		if (op_ == Comparison::Equal)
			return date == date_;
		else if (op_ == Comparison::NotEqual)
			return date != date_;
		else if (op_ == Comparison::LessOrEqual)
			return date <= date_;
		else if (op_ == Comparison::GreaterOrEqual)
			return date >= date_;
		else if (op_ == Comparison::Less)
			return date < date_;
		else if (op_ == Comparison::Greater)
			return date > date_;
		else
			throw invalid_argument("Unknown comparison operator");
	}

private:
	Comparison op_;
	Date date_;
};

class EventComparisonNode : public Node {
public:
	EventComparisonNode(const Comparison& op, const string& event)
		: op_(op), event_(event) {}

	bool Evaluate(const Date& date, const string& event) const override {
		if (op_ == Comparison::Equal)
			return event_ == event;
		else if (op_ == Comparison::NotEqual)
			return event_ != event;
		throw invalid_argument("Unknown comparison operator");
	}

private:
	Comparison op_;
	string event_;
};

class LogicalOperationNode : public Node {
public:
	LogicalOperationNode(const LogicalOperation& op, shared_ptr<Node> left, shared_ptr<Node> right)
		: op_(op), left_(left), right_(right) {}

	bool Evaluate(const Date& date, const string& event) const override {
		if (op_ == LogicalOperation::And)
			return left_->Evaluate(date, event) && right_->Evaluate( date, event);
		else if (op_ == LogicalOperation::Or)
			return left_->Evaluate(date, event) || right_->Evaluate( date, event);
		else
			throw invalid_argument("Unknown comparison operator");
	}

private:
	LogicalOperation op_;
	shared_ptr<Node> left_, right_;
};