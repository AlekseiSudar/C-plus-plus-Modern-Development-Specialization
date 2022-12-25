#include <memory>
#include <queue>
#include "json.h"

class DataBase;

class Request;
using RequestHolder = std::unique_ptr<Request>;

class Manager{
public:
    Manager();
    ~Manager();
    void GetRequests(std::istream& in_stream = std::cin);
    void ProcessRequests(std::ostream& out_stream = std::cout);
private:
    void SetRoutingSettings(const Json::Node& node);
    void UpdateRequests(const Json::Node& node);
    void   ReadRequests(const Json::Node& node);
    std::queue<RequestHolder> requests_;
    std::unique_ptr<DataBase> db_;
};



