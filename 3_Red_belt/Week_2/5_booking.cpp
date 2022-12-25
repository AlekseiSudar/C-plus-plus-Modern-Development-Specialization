#include <iostream>
#include <string>
#include <queue>
#include <map>
using namespace std;

class Event {
public:
	Event(long time, unsigned client_id, unsigned room_count) :
		time_(time), client_(client_id), room_count_(room_count) {}

	long Time() { return time_; }
	unsigned Client() { return client_; }
	unsigned Rooms() { return room_count_; }

private:
	long time_;
	unsigned client_;
	unsigned room_count_;
};

class EventManager {
public:
	void Add(long time, unsigned client_id, unsigned room_count) {
		Adjust(time);
		Event a(time, client_id, room_count);
		events_.push(a);
		rooms_count_ += room_count;

		if (clients_.count(client_id))
			clients_[client_id] += room_count;
		else
			clients_[client_id] = room_count;
	}

	unsigned CountRooms(long time) {
		Adjust(time);
		return rooms_count_;
	}

	unsigned CountClients(long time) {
		Adjust(time);
		return clients_.size();
	}

private:
	queue<Event> events_;
	unsigned rooms_count_ = 0;
	map<unsigned, unsigned> clients_;

	static const long day = 86400;
	void Adjust(long time) {
		
		while (!events_.empty() && events_.front().Time() <= (time - day)) {
			unsigned r = events_.front().Rooms();
			unsigned c = events_.front().Client();
			
			rooms_count_ -= r;
			clients_[c] -= r;
			if (clients_[c] == 0)
				clients_.erase(c);

			events_.pop();
		}
	}
};

class ReservationSystem {
private:
	long time_;
	map<string, EventManager> hotels;

public:
	void Book(long time, string hotel_name, unsigned client_id, unsigned room_count) {
		time_ = time;
		hotels[hotel_name].Add(time, client_id, room_count);
	}

	unsigned ClientsLastDay(string hotel_name) {
		return hotels[hotel_name].CountClients(time_);
	}

	unsigned RoomsLastDay(string hotel_name) {
		return hotels[hotel_name].CountRooms(time_);
	}

};

int main() {
	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReservationSystem system;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;

		if (query_type == "BOOK") {
			long time;
			unsigned client_id;
			unsigned room_count;
			string hotel_name;
			cin >> time >> hotel_name >> client_id >> room_count;

			system.Book(time, hotel_name, client_id, room_count);
		}
		else {
			string hotel_name;
			cin >> hotel_name;
			if (query_type == "CLIENTS") {
				cout << system.ClientsLastDay(hotel_name) << "\n";
			}
			else if (query_type == "ROOMS") {
				cout << system.RoomsLastDay(hotel_name) << "\n";;

			}
		}
	}


	return 0;
}