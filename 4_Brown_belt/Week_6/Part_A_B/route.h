#pragma once
#include <memory>
#include <string>
#include <vector>
#include <set>

class Stop;

class Route{
private:
    using Stop_shared = std::shared_ptr<Stop>;
public:
    Route(bool is_cycle, const std::string& name)
        : is_cycle_(is_cycle), name_(name) {}
    virtual ~Route() = default;

    void AddStops(std::vector<Stop_shared>& stops){
        stops_.reserve(stops.size());
        for (auto stop : stops){
            stops_.push_back(stop);
            unique_stops_.insert(stop);
        }
    }
    const std::string&  GetName() const{ return name_; }
    size_t       GetStopsNumber() const{ return is_cycle_ ? stops_.size() : 2 * stops_.size() - 1; }
    size_t GetUniqueStopsNumber() const{ return unique_stops_.size(); }
    double GetRouteLength() const;
private:
    const bool is_cycle_;
    const std::string name_;
    std::vector<Stop_shared> stops_;
    std::set<Stop_shared> unique_stops_;
};