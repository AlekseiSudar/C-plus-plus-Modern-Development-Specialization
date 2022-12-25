#include <iostream>
#include <memory>
#include <queue>

class DataBase;
class UpdateRequest;
using UpdateRequestHolder = std::unique_ptr<UpdateRequest>;
class ReadRequest;
using ReadRequestHolder = std::unique_ptr<ReadRequest>;


class Manager{
public:
    Manager();
    ~Manager();
    
    void UpdateRequests(std::istream& in_stream = std::cin);
    void   ReadRequests(std::istream& in_stream = std::cin);

    void ProcessUpdateRequests();
    void ProcessReadRequests(std::ostream& out_stream = std::cout);
private:
    std::queue<UpdateRequestHolder> update_requests_;
    std::queue<ReadRequestHolder> read_requests_;
    std::unique_ptr<DataBase> db_;
};



