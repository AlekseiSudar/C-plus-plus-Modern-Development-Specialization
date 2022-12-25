#include "stop.h"
#include "route.h"
#include "database.h"
#include "manager.h"
#include "parser.h"
#include <optional>
#include <string>

using namespace std;

// --------------------------------------------------------------------------------------------------

class UpdateRequest {
public:
    enum class Type {
        ADD_STOP,
        ADD_ROUTE,
    };
    UpdateRequest(Type type) : type(type) {}
    virtual ~UpdateRequest() = default;
    static UpdateRequestHolder Create(Type type);
    virtual void ParseFrom(string_view input) = 0;
    virtual void Process(DataBase& db) const = 0;
    const Type type;
};

class ReadRequest {
public:
    enum class Type {
        READ_STOP,
        READ_ROUTE,
    };
    ReadRequest(Type type) : type(type) {}
    virtual ~ReadRequest() = default;
    static ReadRequestHolder Create(Type type);
    virtual void ParseFrom(string_view input) = 0;
    virtual void Process(DataBase& db, ostream& out_stream) const = 0;
    const Type type;
};

// --------------------------------------------------------------------------------------------------

class AddStopRequest : public UpdateRequest {
public:
    AddStopRequest() : UpdateRequest(Type::ADD_STOP) {}
    void ParseFrom(string_view input) override {
        name =  ReadToken(input, ": ");
        lat = stod(string(ReadToken(input, ", ")));
        lon = stod(string(input));
    }
    void Process(DataBase& db) const override {
        db.AddStop(name, lat, lon);
    }
    string name;
    double lat, lon;
};

class AddRouteRequest : public UpdateRequest {
public:
    AddRouteRequest() : UpdateRequest(Type::ADD_ROUTE) {}
    void ParseFrom(string_view input) override {
            name = ReadToken(input, ": ");
        string_view delimiter = input.find(" - ") != input.npos ? " - " : " > ";
        while (input.length() > 0)
            stops.push_back(string(ReadToken(input, delimiter)));
        is_cycle = delimiter == " > " ? true : false;
    }
    void Process(DataBase& db) const override {
        db.AddRoute(is_cycle, name, stops);
    }
    bool is_cycle = false;
    string name;
    vector<string> stops;
};

class ReadStopRequest : public ReadRequest {
public:
    ReadStopRequest() : ReadRequest(Type::READ_STOP) {}
    void ParseFrom(string_view input) override {
        name = input;
    }
    void Process(DataBase& db, ostream& os) const override {
        shared_ptr<Stop> stop = db.GetStop(name);
        if (stop == nullptr){
            os << "Stop " << name << ": not found\n";
        }
        else{
            const auto& routes = stop->GetRoutes();
            if (routes.empty()){
                os << "Stop " << name << ": no buses\n";
            }
            else{
                os << "Stop " << name << ": buses";
                for (auto route : routes)
                    os << " " << route->GetName();
                os << "\n";
            }
        }
    }
    string name;
};

class ReadRouteRequest : public ReadRequest {
public:
    ReadRouteRequest() : ReadRequest(Type::READ_ROUTE) {}
    void ParseFrom(string_view input) override {
        name = input;
    }
    void Process(DataBase& db, ostream& os) const override {
        shared_ptr<Route> route = db.GetRoute(name);
        if (route == nullptr){
            os << "Bus " << name << ": not found\n";
        }
        else{
            os << "Bus " << name << ": "
                << route->GetStopsNumber()       << " stops on route, "
                << route->GetUniqueStopsNumber() << " unique stops, "
                << route->GetRouteLength() << " route length\n";
        }
    }
    string name;
};

// --------------------------------------------------------------------------------------------------

UpdateRequestHolder UpdateRequest::Create(UpdateRequest::Type type) {
    switch (type) {
        case UpdateRequest::Type::ADD_STOP:
            return make_unique<AddStopRequest>();
        case UpdateRequest::Type::ADD_ROUTE:
            return make_unique<AddRouteRequest>();
        default:
            return nullptr;
    }
}

ReadRequestHolder ReadRequest::Create(ReadRequest::Type type) {
    switch (type) {
        case ReadRequest::Type::READ_ROUTE:
            return make_unique<ReadRouteRequest>();
        case ReadRequest::Type::READ_STOP:
            return make_unique<ReadStopRequest>();
        default:
            return nullptr;
    }
}

// --------------------------------------------------------------------------------------------------

optional<UpdateRequest::Type> UpdateRequestTypeFromStr(string_view typeStr) {
    static const map<string_view, UpdateRequest::Type> str_type = {
        {"Stop", UpdateRequest::Type::ADD_STOP},
        {"Bus", UpdateRequest::Type::ADD_ROUTE},
    };
    const auto it = str_type.find(typeStr);
    if (it != str_type.end())
        return it->second;
    else
        return nullopt;
}

optional<ReadRequest::Type> ReadRequestTypeFromStr(string_view typeStr) {
    static const map<string_view, ReadRequest::Type> str_type = {
        {"Bus", ReadRequest::Type::READ_ROUTE},
        {"Stop", ReadRequest::Type::READ_STOP},
    };
    const auto it = str_type.find(typeStr);
    if (it != str_type.end())
        return it->second;
    else
        return nullopt;
}

// --------------------------------------------------------------------------------------------------

UpdateRequestHolder ParseUpdateRequest(string_view requestStr) {
    const optional<UpdateRequest::Type> requestType = UpdateRequestTypeFromStr(ReadToken(requestStr));
    if (!requestType)
        return nullptr;
    UpdateRequestHolder request = UpdateRequest::Create(*requestType);
    if (request)
        request->ParseFrom(requestStr);
    return request;
}

ReadRequestHolder ParseReadRequest(string_view requestStr) {
    const optional<ReadRequest::Type> requestType = ReadRequestTypeFromStr(ReadToken(requestStr));
    if (!requestType)
        return nullptr;
    ReadRequestHolder request(ReadRequest::Create(*requestType));
    if (request)
        request->ParseFrom(requestStr);
    return request;
}

// manager ------------------------------------------------------------------------------------------

Manager::Manager()
    : db_(make_unique<DataBase>())
    , update_requests_(queue<UpdateRequestHolder>())
    ,   read_requests_(queue<  ReadRequestHolder>()){}

Manager::~Manager(){}


void Manager::UpdateRequests(istream& is) {
    const size_t requestCount = ReadNumberOnLine(is);
    for (size_t i = 0; i < requestCount; i++) {
        string requestStr;
        getline(is, requestStr);
        if (auto request = ParseUpdateRequest(requestStr))
            update_requests_.push(move(request));
    }
}

void Manager::ProcessUpdateRequests(){
    while(!update_requests_.empty()){
        update_requests_.front()->Process(*db_);
        update_requests_.pop();
    }
}

void Manager::ReadRequests(istream& is) {
    const size_t requestCount = ReadNumberOnLine(is);
    for (size_t i = 0; i < requestCount; i++) {
        string requestStr;
        getline(is, requestStr);
        if (auto request = ParseReadRequest(requestStr))
            read_requests_.push(move(request));
    }
}

void Manager::ProcessReadRequests(ostream& os){
    while(!read_requests_.empty()){
        read_requests_.front()->Process(*db_, os);
        read_requests_.pop();
    }
}



