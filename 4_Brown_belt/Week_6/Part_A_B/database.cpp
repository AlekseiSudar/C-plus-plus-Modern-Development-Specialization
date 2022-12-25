#include "stop.h"
#include "route.h"
#include "database.h"
#include <cmath> // Stop::Coord::Distance

using namespace std;

// stop  --------------------------------------------------------------------------------------------

double Stop::Coord::Distance(const Coord& lhs, const Coord& rhs) {   
    return acos(
        sin(lhs.lat_) * sin(rhs.lat_) +
        cos(lhs.lat_) * cos(rhs.lat_) *
            cos(abs(lhs.lon_ - rhs.lon_))
        ) * RADIUS;
}

// определение компоратора для set<shared_ptr<Route>, comp> из stop.h
bool comp::operator()(shared_ptr<Route> lhs, shared_ptr<Route> rhs) const{
    return lhs->GetName() < rhs->GetName();
}

// route --------------------------------------------------------------------------------------------

//template <typename It>
//double GetRoadLength(It begin, It end){
//    double lenght = 0.0;
//    end--;
//    while(begin != end){
//        lenght += Stop::Distance(*(*begin),*(*next(begin)));
//        begin++;
//    }
//    return lenght;
//}
//
//double Route::GetRouteLength() const{
//    double lenght = GetRoadLength(stops_.begin(), stops_.end());
//    if (!is_cycle_)
//        lenght += GetRoadLength(stops_.rbegin(), stops_.rend());
//    return lenght;
//}

double Route::GetRouteLength() const{
    double lenght = 0.0;
    auto begin = stops_.begin(), end= stops_.end() - 1;
    while(begin != end){
        lenght += Stop::Distance(*(*begin),*(*next(begin)));
        begin++;
    }
    return is_cycle_ ? lenght : 2 * lenght;
}

// database -----------------------------------------------------------------------------------------

void DataBase::AddStop(const string& name, double lat, double lon){
    if (!stops_.count(name))
        stops_[name] = make_shared<Stop>(name);
    stops_[name]->ApdateCoord(lat, lon);
}

void DataBase::AddRoute(bool is_cycle, const string& name, const vector<string>& stops){
    routes_[name] = make_shared<Route>(is_cycle, name);

    vector<Stop_ptr> stops_ptr;
    for(const auto& stop : stops){
        if (!stops_.count(stop))
            stops_[stop] = make_shared<Stop>(stop);
        stops_ptr.push_back(stops_[stop]);
    }

    for (auto stop : stops_ptr)
        stop->AddRoute(routes_[name]);

    routes_[name]->AddStops(stops_ptr);
}