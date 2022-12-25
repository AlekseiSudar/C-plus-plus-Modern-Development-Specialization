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
        SET_SETTINGS,
        ADD_STOP,
        ADD_ROUTE,
        READ_STOP,
        READ_ROUTE,
        BUILD_ROUTE,
    };
    Request(Request::Type type) {}
    virtual ~Request() = default;
    static RequestHolder Create(Request::Type type);
    virtual void ParseFrom(const Node& node) = 0;
    virtual void Process(DataBase& db, vector<Node>& answers) const = 0;
};

// --------------------------------------------------------------------------------------------------

class SetSettingsRequest : public Request {
public:
    SetSettingsRequest() : Request(Type::ADD_STOP) {}
    void ParseFrom(const Node& node) override {
        bus_wait_time = node.AsMap().at("bus_wait_time").AsDouble();
        bus_velocity  = node.AsMap().at("bus_velocity" ).AsDouble();
    }
    void Process(DataBase& db, vector<Node>& answers) const override {
        db.SetWaitTime(bus_wait_time);
        db.SetBusVelocity(bus_velocity);
    }
    vector<pair<string, size_t>> distances;
private:
    double bus_wait_time;
    double bus_velocity;
};

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
private:
    string name;
    bool is_cycle;
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

class BuildRouteRequest : public Request {
public:
    BuildRouteRequest() : Request(Type::BUILD_ROUTE) {}
    void ParseFrom(const Node& node) override {
        from = node.AsMap().at("from").AsString();
        to   = node.AsMap().at("to"  ).AsString();
        id = (int)node.AsMap().at("id").AsDouble();
    }
    void Process(DataBase& db, vector<Node>& answers) const override {
        map<string, Node> answer;
        answer["request_id"] = Node(id);

        std::optional<Graph::Router<double>::RouteInfo> route_info = db.BuildRoute(db.GetStop(from), db.GetStop(to));
        if (!route_info)
            answer["error_message"] = Node("not found");
        else{
            answer["total_time"] = Node(route_info->weight);

            vector<Node> items;
            const DataBase::GraphData& graph_data = db.GetGrapfData();
            for (size_t i = 0; i < route_info->edge_count; i++){
                Graph::EdgeId edgeID = graph_data.router->GetRouteEdge(route_info->id, i);
                Graph::Edge<double> edge = graph_data.graph.GetEdge(edgeID);
                
                map<string, Node> stop_item;
                stop_item["type"] = Node("Wait"s);
                stop_item["time"] = Node((int)db.GetWaitTime());
                stop_item["stop_name"] = Node(graph_data.stop_by_Vid[edge.from]->GetName());
                items.push_back(Node(move(stop_item)));

                map<string, Node> bus_item;
                bus_item["type"] = Node("Bus"s);
                bus_item["bus"] = Node(graph_data.bus_by_Eid[edgeID]->GetName());
                bus_item["span_count"] = Node((int)graph_data.span_by_Eid[edgeID]);
                bus_item["time"] = Node(edge.weight - db.GetWaitTime());
                items.push_back(Node(move(bus_item))); 
            }
            answer["items"] = Node(move(items));
        }
        answers.push_back(Node(answer));
    }
private:
    string from;
    string to;
    int id;
};

// --------------------------------------------------------------------------------------------------

RequestHolder Request::Create(Request::Type type) {
    switch (type) {
        case Request::Type::SET_SETTINGS:
            return make_unique<SetSettingsRequest>();
        case Request::Type::ADD_STOP:
            return make_unique<AddStopRequest>();
        case Request::Type::ADD_ROUTE:
            return make_unique<AddRouteRequest>();
        case Request::Type::READ_ROUTE:
            return make_unique<ReadRouteRequest>();
        case Request::Type::READ_STOP:
            return make_unique<ReadStopRequest>();
        case Request::Type::BUILD_ROUTE:
            return make_unique<BuildRouteRequest>();
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
        {"Route", Request::Type::BUILD_ROUTE},
    };
    if (const auto it = str_type.find(typeStr); it != str_type.end())
        return it->second;
    else
        return nullopt;
}

// --------------------------------------------------------------------------------------------------

RequestHolder ParseSetSettingsRequest(const Node& node) {
    RequestHolder request = Request::Create(Request::Type::SET_SETTINGS);
    if (request)
        request->ParseFrom(node);
    return request;
}

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

void Manager::SetRoutingSettings(const Json::Node& node){
    auto request = ParseSetSettingsRequest(node);
    requests_.push(move(request));
}

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
    SetRoutingSettings(doc.GetRoot().AsMap().at("routing_settings"));
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
