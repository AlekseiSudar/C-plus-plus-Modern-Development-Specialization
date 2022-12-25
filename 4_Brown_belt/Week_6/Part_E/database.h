#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <tuple>
#include "router.h"

class Stop;
class Route;

class DataBase{
private:
    using Stop_ptr  = std::shared_ptr<Stop>;
    using Route_ptr = std::shared_ptr<Route>;
    
public:
    DataBase() {}

    void AddStop (const std::string& name, double lat, double lon, const std::vector<std::pair<std::string, size_t>>& distances);
    void AddRoute(bool is_cycle, const std::string& name, const std::vector<std::string>& stops);

    Stop_ptr  GetStop(const std::string name){
        return  stops_.count(name) ?  stops_.at(name) : nullptr;
    }
    Route_ptr GetRoute(const std::string name){
        return routes_.count(name) ? routes_.at(name) : nullptr;
    }

    double GetWaitTime(){
        return bus_wait_time_;
    }
    void SetWaitTime(int time){
        bus_wait_time_ = time;
    }
    void SetBusVelocity(int velocity){
        bus_velocity_ = velocity * 1000.0 / 60.0;
    }

    struct GraphData{
        GraphData(size_t vertexs_num) : graph(vertexs_num){}
        std::map<Stop_ptr, Graph::VertexId> Vid_by_stop;
        std::vector<Stop_ptr > stop_by_Vid;
        std::vector<Route_ptr>  bus_by_Eid;
        std::vector<size_t   > span_by_Eid;
        Graph::DirectedWeightedGraph<double> graph;
        std::unique_ptr<Graph::Router<double>> router = nullptr;
    };

    std::optional<Graph::Router<double>::RouteInfo> BuildRoute(Stop_ptr from, Stop_ptr to);
    const GraphData& GetGrapfData(){ return *graph_data_; }
    
private:
    std::map<std::string, Stop_ptr > stops_;
    std::map<std::string, Route_ptr> routes_;
    double bus_wait_time_; //min
    double bus_velocity_;  //m/min

    void UpdateGraph();

    
    std::unique_ptr<GraphData> graph_data_ = nullptr;
    
};
