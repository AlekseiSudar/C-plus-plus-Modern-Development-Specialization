#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

  class Node : std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            bool, int, double,
                            std::string> {
  public:
    using variant::variant;

    const variant &GetСontent() const { return *this; }

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }
    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }
    bool AsBool() const {
      return std::get<bool>(*this);
    }
    double AsDouble() const {
      return std::get<double>(*this);
    }
    const auto& AsString() const {
      return std::get<std::string>(*this);
    }
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);

  void PrintNode(const Node& node, std::ostream& os);
}