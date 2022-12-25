#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    Email() = default;
    Email(string from_, string to_, string body_) :
        from(move(from_)),
        to(move(to_)),
        body(move(body_)) {}
    string from;
    string to;
    string body;
};

class Worker {
public:
    virtual ~Worker() = default;
    
    virtual void Process(unique_ptr<Email> email) = 0;
    
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }
    
    void SetNext(unique_ptr<Worker> next){
        next_ = move(next);
    }
protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const{
        if(next_) next_->Process(move(email));
    }
private:
    unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
    Reader(istream& is) : is_(is) {}

    void Process(unique_ptr<Email> email) override {
        throw logic_error("Unimplemented");
    }
    
    void Run() override{
        while(is_){
            string from, to, body;
            getline(is_, from);
            getline(is_, to);
            getline(is_, body);
            if (is_) PassOn(make_unique<Email>(from, to, body));
        }
    }
private:
    istream& is_;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
    
    Filter(Function func) : func_(move(func)) {}

    void Process(unique_ptr<Email> email) override {
        if (func_(*email)) PassOn(move(email));
    }
private:
    Function func_;
};


class Copier : public Worker {
public:
    Copier(const string& address) : to_(address) {}

    void Process(unique_ptr<Email> email) override {
        if (email->to != to_){
            Email other_email(email->from, to_, email->body); 
            PassOn(move(email));
            PassOn(make_unique<Email>(other_email));
        }
        else{
            PassOn(move(email));
        }
    }
private:
    string to_;
};


class Sender : public Worker {
public:
    explicit Sender(ostream&os) : os_(os) {}

    void Process(unique_ptr<Email> email) override {
        os_ << email->from << endl
            << email->to   << endl
            << email->body << endl;
        PassOn(move(email));
    }
private:
    ostream& os_;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in){
        workers_.push_back(make_unique<Reader>(in));
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter){
        workers_.push_back(make_unique<Filter>(move(filter)));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient){
        workers_.push_back(make_unique<Copier>(move(recipient)));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out){
        workers_.push_back(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build(){
        for (size_t i = workers_.size() - 1; i > 0; --i) {
            workers_[i - 1]->SetNext(move(workers_[i]));
        }
        return move(workers_[0]);
    }
private:
    vector<unique_ptr<Worker>> workers_;
};


void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
    );

    string res = outStream.str();

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
