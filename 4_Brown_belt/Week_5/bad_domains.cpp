#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

using namespace std;

string ParserDomain(string_view& domain){
    size_t pos = domain.rfind('.');
    
    string elem;
    if (pos == domain.npos) {
        elem = string(domain.substr(0));
        domain = domain.substr(0, 0);
    }
    else {
        elem = string(domain.substr(pos + 1));
        domain = domain.substr(0, pos);
    }
    return string(elem);
}


class TreeNode{
public:
    TreeNode() {}

    void Add(string_view domain){
        string elem = ParserDomain(domain);

        if (domain.size() == 0)
            elems_[elem] = nullptr;
        else{
            if (!elems_.count(elem))
                elems_[elem] = make_shared<TreeNode>();
            if (elems_[elem] != nullptr)
                elems_[elem]->Add(domain);
        }
    }

    bool Check(string_view domain) const{
        string elem =  ParserDomain(domain);

        auto it = elems_.find(elem);
        if (it == elems_.end())
            return false;
        if (it->second == nullptr)
            return true;
        if (domain.size() == 0)
            return false;
        return it->second->Check(domain);
    }
private:
    unordered_map<string, shared_ptr<TreeNode>> elems_;
};



int main(){
    TreeNode banned_domains;

    size_t count_domains_to_ban;
    cin >> count_domains_to_ban;
    string domain_to_ban;
    for (size_t i = 0; i < count_domains_to_ban; i++){
        cin >> domain_to_ban;
        banned_domains.Add(domain_to_ban);
    }
    
    size_t count_domains_to_check;
    cin >> count_domains_to_check;
    string domain_to_check;
    for (size_t i = 0; i < count_domains_to_check; i++){
        cin >> domain_to_check;
        if (banned_domains.Check(domain_to_check))
            cout << "Bad" << endl;
        else
            cout << "Good" << endl;
    }
    
    return 0;
}


