#include <iostream>
#include <queue>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <set>

#include "test_runner.h"
#include "profile.h"

using namespace std;

class BookingManager {

    struct HotelInfo {
        HotelInfo() : clients_count(0), room_count(0)  {}
        int clients_count;
        int room_count;
        unordered_map<int, int> client_to_count;
    };

    struct Booking {
        int64_t time;
        string hotel_name;
        int client_id;
        int room_count;
    };

    queue<Booking> Data;
    unordered_map<string, HotelInfo> hotels_info;

    static const int MAX_TIME = 86400;

public:

    void Book(const Booking& booking) {
        HotelInfo& current_hotel = hotels_info[booking.hotel_name];
        current_hotel.room_count += booking.room_count;
        if (current_hotel.client_to_count.count(booking.client_id) == 0)
        {
            ++current_hotel.clients_count;
            current_hotel.client_to_count[booking.client_id] = 0;
        }
        ++current_hotel.client_to_count[booking.client_id];
        Data.push(booking);

        while ( !Data.empty() && (Data.front().time <= booking.time - MAX_TIME) ) {
            HotelInfo& popping_hotel = hotels_info[Data.front().hotel_name];
            popping_hotel.room_count -= Data.front().room_count;
            --popping_hotel.client_to_count[Data.front().client_id];
            if (popping_hotel.client_to_count[Data.front().client_id] == 0)
            {
                --popping_hotel.clients_count;
                popping_hotel.client_to_count.erase(Data.front().client_id);
            }
            Data.pop();
        }
    }

    int Clients(const string& hotel_name) const {
        if (hotels_info.count(hotel_name) < 1) return 0;
        return hotels_info.at(hotel_name).clients_count;
    }

    int Rooms(const string& hotel_name) const {
        if (hotels_info.count(hotel_name) < 1) return 0;
        return hotels_info.at(hotel_name).room_count;
    }
};

void TestAll();

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    TestAll();

    BookingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
      string query_type;
      cin >> query_type;

      if (query_type == "BOOK") {
        int64_t time;
        string hotel_name;
        int client_id;
        int room_count;
        cin >> time >> hotel_name >> client_id >> room_count;
        manager.Book({time, hotel_name, client_id, room_count});
      } else if (query_type == "CLIENTS") {
        string hotel_name;
        cin >> hotel_name;
        cout << manager.Clients(hotel_name) << "\n";
      } else if (query_type == "ROOMS") {
        string hotel_name;
        cin >> hotel_name;
        cout << manager.Rooms(hotel_name) << "\n";
      }
    }

    return 0;
}


void FirstTest() {
    BookingManager manager;

    // Test empty bookings
    ASSERT_EQUAL(manager.Clients("mariott"), 0);
    ASSERT_EQUAL(manager.Rooms("mariott"), 0);

    manager.Book({0, "mariott", 1, 10});
    manager.Book({0, "mariott", 2, 1});
    manager.Book({0, "mariott", 3, 1});
    manager.Book({0, "mariott", 4, 1});
    manager.Book({0, "hilton", 1, 1});
    manager.Book({1, "hilton", 2, 1});

    // Test correctness
    ASSERT_EQUAL(manager.Clients("mariott"), 4);
    ASSERT_EQUAL(manager.Rooms("mariott"), 13);
    ASSERT_EQUAL(manager.Clients("hilton"), 2);
    ASSERT_EQUAL(manager.Rooms("hilton"), 2);

    // Test event past 1 day resets statics
    manager.Book({86400, "mariott", 1, 1});
    ASSERT_EQUAL(manager.Clients("mariott"), 1);
    ASSERT_EQUAL(manager.Rooms("mariott"), 1);
    ASSERT_EQUAL(manager.Clients("hilton"), 1);
    ASSERT_EQUAL(manager.Rooms("hilton"), 1);

    // Test no clients and room for the last day
    manager.Book({86401, "mariott", 5, 1});
    ASSERT_EQUAL(manager.Clients("mariott"), 2);
    ASSERT_EQUAL(manager.Rooms("mariott"), 2);
    ASSERT_EQUAL(manager.Clients("hilton"), 0);
    ASSERT_EQUAL(manager.Rooms("hilton"), 0);
}


void SecondTest() {
    BookingManager manager;

    manager.Book({-1'000'000'000'000'000'000, "ibis", 1'000'000'000, 1'000});
    ASSERT_EQUAL(manager.Clients("ibis"), 1);
    ASSERT_EQUAL(manager.Rooms("ibis"), 1000);

    manager.Book({-1'000'000'000'000'000'000, "ibis", 999'000'000, 500});
    ASSERT_EQUAL(manager.Clients("ibis"), 2);
    ASSERT_EQUAL(manager.Rooms("ibis"), 1500);

    manager.Book({-999'999'999'999'999'999, "antarctica", 1'000'000, 300});
    manager.Book({-999'999'999'999'999'999, "antarctica", 99'000'000, 100});
    manager.Book({-999'999'999'999'999'999, "antarctica", 99'000'000, 50});
    ASSERT_EQUAL(manager.Clients("ibis"), 2);
    ASSERT_EQUAL(manager.Rooms("ibis"), 1500);
    ASSERT_EQUAL(manager.Clients("antarctica"), 2);
    ASSERT_EQUAL(manager.Rooms("antarctica"), 450);

    manager.Book({-999'999'999'999'999'999, "ibis", 999'000'000, 300});
    manager.Book({-999'999'999'999'999'998, "ibis", 999'000'000, 100});
    manager.Book({-999'999'999'999'999'997, "ibis", 999'000'000, 50});
    ASSERT_EQUAL(manager.Clients("ibis"), 2);
    ASSERT_EQUAL(manager.Rooms("ibis"), 1950);
    ASSERT_EQUAL(manager.Clients("antarctica"), 2);
    ASSERT_EQUAL(manager.Rooms("antarctica"), 450);

    manager.Book({-999'999'999'000'000'000, "ibis", 999'000'000, 30});
    manager.Book({-999'999'998'000'000'000, "ibis", 999'000'000, 15});
    manager.Book({-999'999'997'999'999'999, "ibis", 999'000'000, 2});
    ASSERT_EQUAL(manager.Clients("ibis"), 1);
    ASSERT_EQUAL(manager.Rooms("ibis"), 17);
    ASSERT_EQUAL(manager.Clients("antarctica"), 0);
    ASSERT_EQUAL(manager.Rooms("antarctica"), 0);

    manager.Book({-999'999'997'999'999'998, "antarctica", 1'000'000, 30});
    manager.Book({-999'999'997'999'999'997, "antarctica", 99'000'000, 15});
    manager.Book({-999'999'997'999'999'996, "antarctica", 9'000'000, 5});
    ASSERT_EQUAL(manager.Clients("ibis"), 1);
    ASSERT_EQUAL(manager.Rooms("ibis"), 17);
    ASSERT_EQUAL(manager.Clients("antarctica"), 3);
    ASSERT_EQUAL(manager.Rooms("antarctica"), 50);
}


void TestAll()
{
    TestRunner tr;
    RUN_TEST(tr,FirstTest);
    RUN_TEST(tr,SecondTest);
}

