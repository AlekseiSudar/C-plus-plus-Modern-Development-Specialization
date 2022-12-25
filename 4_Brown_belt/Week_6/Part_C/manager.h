#include <iostream>
#include <memory>
#include <queue>

class DataBase;

class Request;
using RequestHolder = std::unique_ptr<Request>;

class Manager{
public:
    Manager();
    ~Manager();
    
    void UpdateRequests(std::istream& in_stream = std::cin);
    void   ReadRequests(std::istream& in_stream = std::cin);

    void ProcessRequests(std::ostream& out_stream = std::cout);
private:
    std::queue<RequestHolder> requests_;
    std::unique_ptr<DataBase> db_;
};



