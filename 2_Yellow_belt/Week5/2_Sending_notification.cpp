#include <iostream>
#include <string>
using namespace std;

void SendSms(const string& number, const string& message);
void SendEmail(const string& email, const string& message);

class INotifier {
public:
	INotifier(const string& address) : address_(address) {};

	virtual void Notify(const string& message) = 0;

	const string address_;
};

class SmsNotifier : public INotifier {
public:
	SmsNotifier(const string& address) : INotifier(address) {};
	void Notify(const string& message) override {
		SendSms(address_, message);
	}
};

class EmailNotifier : public INotifier {
public:
	EmailNotifier(const string& address) : INotifier(address) {};
	void Notify(const string& message) override {
		SendEmail(address_, message);
	}
};
