#include "Buses.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "ReadFromConsole.h"

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

struct BusesForStopResponse {
    string stop;
    vector<string> buses;
};

struct StopsForBusResponse {
    string bus;
    vector<string> stops;
};

struct AllBusesResponse {
    vector<string> buses;
};

istream& operator>>(istream& is, Query& q) {
    string str;
    getline(is, str);
    cin >> str;
    if (str == "NEW_BUS")
    {
        q.type = QueryType::NewBus;
        cin >> q.bus;
        int stopsCount;
        cin >> stopsCount;
        for (int i = 0; i < stopsCount; i++)
        {
            cin >> str;
            q.stops.push_back(str);
        }
    }
    else if (str == "BUSES_FOR_STOP")
    {
        q.type = QueryType::BusesForStop;
        cin >> q.stop;
    }
    else if (str == "STOPS_FOR_BUS")
    {
        q.type = QueryType::StopsForBus;
        cin >> q.bus;
    }
    else if (str == "ALL_BUSES")
    {
        q.type = QueryType::AllBuses;
    }
    

    return is;
}

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {

    for_each(r.buses.begin(), r.buses.end(), [&os](auto x) {os << x << "\t"; });
    return os;
}

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {

    for_each(r.stops.begin(), r.stops.end(), [&os](auto x) {os << x << "\t"; });
    return os;
}

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    for_each(r.buses.begin(), r.buses.end(), [&os](auto x) {os << x << "\t"; });
    return os;
}

class BusManager {

protected:
    vector<pair<string, vector<string>>> busesAndStops;

public:
    void AddBus(const string& bus, const vector<string>& stops) {
        remove_if(busesAndStops.begin(), busesAndStops.end(), [bus](auto x) {return x.first == bus; });
        busesAndStops.push_back({ bus,stops });
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        //"No stop" "No buses"
        vector<string> stops;
        for_each(busesAndStops.begin(), busesAndStops.end(), [&stops, stop](pair<string, vector<string>> x) {
            if(find(x.second.begin(), x.second.end(),stop) != x.second.end())
                stops.push_back(x.first);
            });
        return  stops.size() > 0 ? BusesForStopResponse({ stop, stops }) : BusesForStopResponse({stop, vector < string>({"No buses"})});
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        //"No bus" "No stops"
        auto a = find_if(busesAndStops.begin(), busesAndStops.end(), [bus](auto x) {return x.first == bus; });
        if (a == busesAndStops.end())
            return { "No bus" ,{"No stops"} };
        return { bus, a[0].second.size() > 0 ? a[0].second : vector < string>({"No stops"}) };
    }

    AllBusesResponse GetAllBuses() const {
        //"No buses"
        vector<string> buses;
        for_each(busesAndStops.begin(), busesAndStops.end(), [&buses](auto x) {buses.push_back(x.first); });
        return { buses.size() == 0 ? vector<string>({ "No buses" }) : buses };
    }
};

// Не меняя тела функции main, реализуйте функции и классы выше

int BusesTask() {
    int query_count;
    Query q;

    string str("10\n\
        ALL_BUSES\n\
        BUSES_FOR_STOP Marushkino\n\
        STOPS_FOR_BUS 32K\n\
        NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo\n\
        NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo\n\
        BUSES_FOR_STOP Vnukovo\n\
        NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo\n\
        NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo\n\
        STOPS_FOR_BUS 272\n\
        ALL_BUSES\n");
    streambuf* orig = ReadFromConsole::AddToCin(str, cin);


    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }
    std::cin.rdbuf(orig);
    return 1;
}