#include "stop.h"
#include "route.h"
#include "database.h"
#include "manager.h"
#include <optional>

using namespace std;
using namespace Json;

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
    virtual void ParseFrom(const Node& node) = 0;
    virtual void Process(DataBase& db, vector<Node>& answers) const = 0;
};

// --------------------------------------------------------------------------------------------------

class AddStopRequest : public Request {
public:
    AddStopRequest() : Request(Type::ADD_STOP) {}
    void ParseFrom(const Node& node) override {
        name = node.AsMap().at("name").AsString();
        lat = node.AsMap().at("latitude").AsDouble();
        lon = node.AsMap().at("longitude").AsDouble();
        for (const auto& [key, value] : node.AsMap().at("road_distances").AsMap())
            distances.push_back({key, (int)value.AsDouble()});
    }
    void Process(DataBase& db, vector<Node>& answers) const override {
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
    void ParseFrom(const Node& node) override {
        name = node.AsMap().at("name").AsString();
        is_cycle = node.AsMap().at("is_roundtrip").AsBool();
        for (const Node& stop : node.AsMap().at("stops").AsArray())
            stops.push_back(stop.AsString());
    }
    void Process(DataBase& db, vector<Node>& answers) const override {
        db.AddRoute(is_cycle, name, stops);
    }
    bool is_cycle;
private:
    string name;
    vector<string> stops;
};

class ReadStopRequest : public Request {
public:
    ReadStopRequest() : Request(Type::READ_STOP) {}
    void ParseFrom(const Node& node) override {
        name = node.AsMap().at("name").AsString();
        id = (int)node.AsMap().at("id").AsDouble();
    }
    void Process(DataBase& db, vector<Node>& answers) const override {
        map<string, Node> answer;
        answer["request_id"] = Node(id);

        shared_ptr<Stop> stop = db.GetStop(name);
        if (stop == nullptr){
            answer["error_message"] = Node("not found"s);
        }
        else{
            const auto& routes = stop->GetRoutes();

            vector<Node> buses;
            for (auto route : routes)
                buses.push_back(Node(route->GetName()));
            answer["buses"] = Node(buses);
        }

        answers.push_back(Node(answer));
    }
private:
    string name;
    int id;
};

class ReadRouteRequest : public Request {
public:
    ReadRouteRequest() : Request(Type::READ_ROUTE) {}
    void ParseFrom(const Node& node) override {
        name = node.AsMap().at("name").AsString();
        id = (int)node.AsMap().at("id").AsDouble();
    }
    void Process(DataBase& db, vector<Node>& answers) const override {
        map<string, Node> answer;
        answer["request_id"] = Node(id);

        shared_ptr<Route> route = db.GetRoute(name);
        if (route == nullptr){
           answer["error_message"] = Node("not found"s);
        }
        else{
            double geo_lenght = route->GetGeoLength();
            size_t road_lenght = route->GetRoadLength();
            answer[       "stop_count"] = Node(int(route->GetStopsNumber()));
            answer["unique_stop_count"] = Node(int(route->GetUniqueStopsNumber()));
            answer[     "route_length"] = Node(int(road_lenght));
            answer[        "curvature"] = Node(road_lenght / geo_lenght);
        }
        answers.push_back(Node(answer));
    }
private:
    string name;
    int id;
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

RequestHolder ParseUpdateRequest(const Node& node) {
    const optional<Request::Type> requestType = UpdateRequestTypeFromStr(node.AsMap().at("type").AsString());
    if (!requestType)
        return nullptr;
    RequestHolder request = Request::Create(*requestType);
    if (request)
        request->ParseFrom(node);
    return request;
}

RequestHolder ParseReadRequest(const Node& node) {
    const optional<Request::Type> requestType = ReadRequestTypeFromStr(node.AsMap().at("type").AsString());
    if (!requestType)
        return nullptr;
    RequestHolder request(Request::Create(*requestType));
    if (request)
        request->ParseFrom(node);
    return request;
}

// manager ------------------------------------------------------------------------------------------

Manager::Manager()
    : db_(make_unique<DataBase>())
    , requests_(queue<RequestHolder>()){}

Manager::~Manager(){}

void Manager::UpdateRequests(const Node& node) {
    for (const Node& request_n : node.AsArray())
        if (auto request = ParseUpdateRequest(request_n))
            requests_.push(move(request));
}

void Manager::ReadRequests(const Node& node) {
    for (const Node& request_n : node.AsArray())
        if (auto request = ParseReadRequest(request_n))
            requests_.push(move(request));
}

void Manager::GetRequests(istream& is) {
    Document doc = Load(is);

    UpdateRequests(doc.GetRoot().AsMap().at("base_requests"));
    ReadRequests(doc.GetRoot().AsMap().at("stat_requests"));
}

void Manager::ProcessRequests(ostream& os){
    vector<Node> answers;
    while(!requests_.empty()){
        requests_.front()->Process(*db_, answers);
        requests_.pop();
    }
    Node answers_node(move(answers));

    PrintNode(answers_node, os);
}
