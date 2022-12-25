#include "ini.h"
#include <string_view>

using namespace std;

namespace Ini{
    Section& Document::AddSection(string name){
        return sections[name];
    }
    const Section& Document::GetSection(const string& name) const{
        return sections.at(name);
    }
    size_t Document::SectionCount() const{
        return sections.size();
    }

    Document Load(istream& input){
        Document doc;
        Section *sect;
        for(string line; getline(input, line); ){
            if(!line.empty()){
                if(line[0] == '['){
                    line.erase(line.begin());
                    line.pop_back();
                    sect = &doc.AddSection(move(line));
                }
                else{
                    string_view view = line;
                    size_t pos = view.find('=');
                    (*sect)[string(view.substr(0, pos))] = string(view.substr(pos + 1));
                }
            }
        }
        return doc;
    }
}