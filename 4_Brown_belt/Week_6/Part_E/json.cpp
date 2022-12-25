#include "json.h"

using namespace std;

namespace Json {

    Document::Document(Node root) : root(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root;
    }

    Node LoadNode(istream& input);

    Node LoadArray(istream& input) {
        vector<Node> result;

        for (char c; input >> c && c != ']'; ) {
            if (c != ',') {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }

        return Node(move(result));
    }

    Node LoadTrue(istream& input) {
        input.get();
        input.get();
        input.get();
        return Node(true);
    }
    Node LoadFalse(istream& input) {
        input.get();
        input.get();
        input.get();
        input.get();
        return Node(false);
    }

    Node LoadNumber(istream& input) {
        int sign = 1;
        if (input.peek() == '-') {
            sign = -1;
            input.get();
        }

        double result = 0.0;
        while (isdigit(input.peek())) {
            result *= 10.0;
            result += input.get() - '0';
        }

        if(input.peek() != '.')
            return Node(result * sign);
        input.get();

        double decimal_order = 1.0;
        while (isdigit(input.peek())) {
            decimal_order /= 10.0;
            result += (input.get() - '0')*decimal_order;
        }
        return Node(result * sign);
    }

    Node LoadString(istream& input) {
        string line;
        getline(input, line, '"');
        return Node(move(line));
    }

    Node LoadDict(istream& input) {
        map<string, Node> result;

        for (char c; input >> c && c != '}'; ) {
            if (c == ',') {
                input >> c;
            }

            string key = LoadString(input).AsString();
            input >> c;
            result.emplace(move(key), LoadNode(input));
        }

        return Node(move(result));
    }

    Node LoadNode(istream& input) {
        char c;
        input >> c;

        if (c == '[') {
            return LoadArray(input);
        } else if (c == '{') {
            return LoadDict(input);
        } else if (c == '"') {
            return LoadString(input);
        } else if (c == 't') {
            return LoadTrue(input);
        } else if (c == 'f') {
            return LoadFalse(input);
        } else {
            input.putback(c);
            return LoadNumber(input);
        }
    }

    Document Load(istream& input) {
        return Document{LoadNode(input)};
    }

    template<typename T>
    void Print(const T& val, ostream &os) {
        os << val;
    }

    void Print(const string& val, ostream& os) {
        os << '"' << val << '"';
    }

    void Print(const vector<Node>& vec, ostream& os) {
        os << '[';
        bool first = true;
        for (const Node& node : vec) {
            if (!first) {
                os << ", ";
            }
            os << '\n';
            first = false;
            PrintNode(node, os);
        }
        os << ']';
    }

    void Print(const map<string, Node>& map, ostream& os) {
        os << '{' << '\n';
        bool first = true;
        for (const auto& [key, node]: map) {
            if (!first) {
                os << ", " << '\n';
            }
            first = false;
            Print(key, os);
            os << ": ";
            PrintNode(node, os);
            os;
        }
        os << '\n' << '}';
    }

    void PrintNode(const Node& node, ostream& os) {
        visit([&os](const auto& val) { Print(val, os); },
          node.GetСontent());
    }

}
