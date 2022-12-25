#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <tuple>

class Stop;
class Route;

class DataBase{
private:
    using Stop_ptr  = std::shared_ptr<Stop>;
    using Route_ptr = std::shared_ptr<Route>;
public:
    DataBase(){}

    void AddStop (const std::string& name, double lat, double lon, const std::vector<std::pair<std::string, size_t>>& distances);
    void AddRoute(bool is_cycle, const std::string& name, const std::vector<std::string>& stops);

    Stop_ptr  GetStop(const std::string name){
        return  stops_.count(name) ?  stops_.at(name) : nullptr;
    }
    Route_ptr GetRoute(const std::string name){
        return routes_.count(name) ? routes_.at(name) : nullptr;
    }
    
private:
    std::map<std::string, Stop_ptr > stops_;
    std::map<std::string, Route_ptr> routes_;
};
