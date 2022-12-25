#include "stop.h"
#include "route.h"
#include "database.h"
#include "manager.h"
#include "parser.h"
#include <optional>

using namespace std;

class Request;
using RequestHolder = std::unique_ptr<Request>;
class Request {
public:
    enum class Type {
        ADD_STOP,
        ADD_ROUTE,
        READ_STOP,
        READ_ROUTE,
    };
    Request(Request::Type type) {}
    virtual ~Request() = default;
    static RequestHolder Create(Request::Type type);
    virtual void ParseFrom(string_view input) = 0;
    virtual void Process(DataBase& db, ostream& os) const = 0;
};

// --------------------------------------------------------------------------------------------------

class AddStopRequest : public Request {
public:
    AddStopRequest() : Request(Type::ADD_STOP) {}
    void ParseFrom(string_view input) override {
        name =  ReadToken(input, ": ");
        lat = stod(string(ReadToken(input, ", ")));
        lon = stod(string(ReadToken(input, ", ")));

        size_t distance;
        string other_stop;
        while (input.length() > 0){
            distance = stod(string(ReadToken(input, "m ")));
            other_stop = ReadToken(input, " ");
            other_stop = ReadToken(input, ",");
            distances.push_back({other_stop, distance});
        }
    }
    void Process(DataBase& db, ostream& out_stream) const override {
        db.AddStop(name, lat, lon, distances);
    }
    vector<pair<string, size_t>> distances;
private:
    string name;
    double lat, lon;
};

class AddRouteRequest : public Request {
public:
    AddRouteRequest() : Request(Type::ADD_ROUTE) {}
    void ParseFrom(string_view input) override {
        name = ReadToken(input, ": ");
        string_view delimiter = input.find(" - ") != input.npos ? " - " : " > ";
        while (input.length() > 0)
            stops.push_back(string(ReadToken(input, delimiter)));
        is_cycle = delimiter == " > " ? true : false;
    }
    void Process(DataBase& db, ostream& out_stream) const override {
        db.AddRoute(is_cycle, name, stops);
    }
    bool is_cycle = false;
private:
    string name;
    vector<string> stops;
};

class ReadStopRequest : public Request {
public:
    ReadStopRequest() : Request(Type::READ_STOP) {}
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
private:
    string name;
};

class ReadRouteRequest : public Request {
public:
    ReadRouteRequest() : Request(Type::READ_ROUTE) {}
    void ParseFrom(string_view input) override {
        name = input;
    }
    void Process(DataBase& db, ostream& os) const override {
        shared_ptr<Route> route = db.GetRoute(name);
        if (route == nullptr){
            os << "Bus " << name << ": not found\n";
        }
        else{
            double geo_lenght = route->GetGeoLength();
            size_t road_lenght = route->GetRoadLength();
            os << "Bus " << name << ": "
                << route->GetStopsNumber()       << " stops on route, "
                << route->GetUniqueStopsNumber() << " unique stops, "
                << road_lenght << " route length, "
                << road_lenght / geo_lenght << " curvature\n";
        }
    }
private:
    string name;
};

// --------------------------------------------------------------------------------------------------

RequestHolder Request::Create(Request::Type type) {
    switch (type) {
        case Request::Type::ADD_STOP:
            return make_unique<AddStopRequest>();
        case Request::Type::ADD_ROUTE:
            return make_unique<AddRouteRequest>();
        case Request::Type::READ_ROUTE:
            return make_unique<ReadRouteRequest>();
        case Request::Type::READ_STOP:
            return make_unique<ReadStopRequest>();
        default:
            return nullptr;
    }
}

// --------------------------------------------------------------------------------------------------

optional<Request::Type> UpdateRequestTypeFromStr(string_view typeStr) {
    static const map<string_view, Request::Type> str_type = {
        {"Stop", Request::Type::ADD_STOP},
        {"Bus", Request::Type::ADD_ROUTE},
    };
    if (const auto it = str_type.find(typeStr); it != str_type.end())
        return it->second;
    else
        return nullopt;
}

optional<Request::Type> ReadRequestTypeFromStr(string_view typeStr) {
    static const map<string_view, Request::Type> str_type = {
        {"Bus", Request::Type::READ_ROUTE},
        {"Stop", Request::Type::READ_STOP},
    };
    if (const auto it = str_type.find(typeStr); it != str_type.end())
        return it->second;
    else
        return nullopt;
}

// --------------------------------------------------------------------------------------------------

RequestHolder ParseUpdateRequest(string_view requestStr) {
    const optional<Request::Type> requestType = UpdateRequestTypeFromStr(ReadToken(requestStr));
    if (!requestType)
        return nullptr;
    RequestHolder request = Request::Create(*requestType);
    if (request)
        request->ParseFrom(requestStr);
    return request;
}

RequestHolder ParseReadRequest(string_view requestStr) {
    const optional<Request::Type> requestType = ReadRequestTypeFromStr(ReadToken(requestStr));
    if (!requestType)
        return nullptr;
    RequestHolder request(Request::Create(*requestType));
    if (request)
        request->ParseFrom(requestStr);
    return request;
}

// manager ------------------------------------------------------------------------------------------

Manager::Manager()
    : db_(make_unique<DataBase>())
    , requests_(queue<RequestHolder>()){}

Manager::~Manager(){}

void Manager::UpdateRequests(istream& is) {
    const size_t requestCount = ReadNumberOnLine(is);
    for (size_t i = 0; i < requestCount; i++) {
        string requestStr;
        getline(is, requestStr);
        if (auto request = ParseUpdateRequest(requestStr))
            requests_.push(move(request));
    }
}

void Manager::ReadRequests(istream& is) {
    const size_t requestCount = ReadNumberOnLine(is);
    for (size_t i = 0; i < requestCount; i++) {
        string requestStr;
        getline(is, requestStr);
        if (auto request = ParseReadRequest(requestStr))
            requests_.push(move(request));
    }
}

void Manager::ProcessRequests(ostream& os){
    while(!requests_.empty()){
        requests_.front()->Process(*db_, os);
        requests_.pop();
    }
}
