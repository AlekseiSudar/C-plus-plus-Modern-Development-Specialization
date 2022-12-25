#pragma once
#include <memory>
#include <string>
#include <set>

class Route;

class comp{
public:
    bool operator()(std::shared_ptr<Route> lhs, std::shared_ptr<Route> rhs) const;
};

class Stop{
private:
    using Route_ptr = std::shared_ptr<Route>;
public:
    Stop(const std::string& name)
        : name_(name), coords_(nullptr) {}
    void ApdateCoord(double lat, double lon){
        coords_ = std::make_unique<Coord>(lat, lon);
    }
    void AddRoute(Route_ptr route){
        routes_.insert(route);
    }
    static double Distance(const Stop& lhs, const Stop& rhs) {
        return Coord::Distance(*lhs.coords_, *rhs.coords_);
    }
    const auto& GetRoutes() const{
        return routes_;
    }
private:
    class Coord {
    public:
        Coord(double lat, double lon) 
            : lat_(lat * PIdiv180), lon_(lon * PIdiv180) {}
        static double Distance(const Coord& lhs, const Coord& rhs);
    private:
        constexpr static double PIdiv180 = 3.1415926535 / 180.0;
        constexpr static double RADIUS = 6371000.0;
        double lat_;
        double lon_;
    };
    const std::string name_;
    std::unique_ptr<Coord> coords_;
    std::set<Route_ptr, comp> routes_;
};
