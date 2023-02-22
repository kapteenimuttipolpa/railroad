// Datastructures.hh
//
// Student name: Ville Inkinen
// Student email: Ville.inkinen@tuni.fi
// Student number: 151361672

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_set>
#include <list>
#include <set>
#include <map>
#include <cmath>
#include <stack>
#include <queue>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;


// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};



// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement
struct Region;

struct Station{
    StationID id;
    Name name;
    Coord coord;
    std::vector<std::pair<TrainID, Time>> departures;
    Region* ptr;
    std::vector<StationID> neighbours;

};

struct Region{
    Name name;
    std::vector<Coord> coord;
    RegionID id;
    Region* parent;
};

struct CoordComparator{
    bool operator()(const Coord& c1, const Coord& c2) const{
        if (c1.x == c2.x && c1.y == c2.y){
            return false;
        }
        else if (c1.x == c2.x){
            return c1.y < c2.y;
        }
        else if (c1.y == c2.y){
            return c1.x < c2.x;
        }
        else{
            return (c1.x*c1.x + c1.y*c1.y) < (c2.x*c2.x + c2.y*c2.y);
        }
    }
};

struct Train{

    std::vector<std::pair<StationID, Time> > stationtimes;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Looking up the size of unordered_map is constant
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Clearing in for loop is linear
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Copies the data from unordered_map to vector
    std::vector<StationID> all_stations();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Adding values to std::map logn time
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    Name get_station_name(StationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: iterating a map is linear
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: iterating a map is linear
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find on unordered map runs on logaritmic time
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding and ereasing from map takes logaritmic time
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating the vector takes linear time
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort has time complexity of nlogn, where n is
    //the size of departures
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1)
    // Short rationale for estimate: Inserting in unordered_map is constant
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating unordered_map is linear
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    Name get_region_name(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching in unordered_map is on average constant time complexity
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case is O(n) if the tree is unbalanced
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations


    // Estimate of performance: O(n)
    // Short rationale for estimate: where n is the number of regions in regions_map
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n log k)
    // Short rationale for estimate: partial_sort has complexity of n log k but
    // but the function is basically o(n) because K = 3
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(log n)
    // Short rationale for estimate: ereasing from map is logaritmic
    bool remove_station(StationID id);

    // Estimate of performance: O(h1 * h2)
    // Short rationale for estimate: worst case is when two regions are maxium distance away from eachother
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);
    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(n)
    // Short rationale for estimate: Looping through vector is linear
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance: O(1)
    // Short rationale for estimate: searching from unordered_map is constant
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Only loops the vector once
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-looping through vector
    void clear_trains();

    // Estimate of performance: O(V2)
    // Short rationale for estimate: using modified bfs algorithm that calculates the shortest route
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance: O(V2)
    // Short rationale for estimate: Using modified bfs algorithm that calculates the shortest route
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance: O(V + E)
    // Short rationale for estimate: Using BFS to find a route with cycle
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance: O(V2)
    // Short rationale for estimate: Using djikstra like algorithm to find the shortest distance from a to b
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);


private:
    // Add stuff needed for your class implementation here
    
    std::vector<std::pair<TrainID, Time>> departures_vec;
    std::unordered_map<RegionID, Region> regions_map;
    std::vector<StationID> neighbours;
    std::unordered_map<StationID, Station> stations_map;
    std::map<Coord, StationID, CoordComparator> coord_map;
    std::map<Name, StationID> stations_sorted;
    std::unordered_map<TrainID, Train> train_map;

    int distance_between(StationID fromid, StationID toid){

        int x1 = stations_map[fromid].coord.x;
        int y1 = stations_map[fromid].coord.y;
        int x2 = stations_map[toid].coord.x;
        int y2 = stations_map[toid].coord.y;

        return std::sqrt(std::pow(x1-x2, 2) + std::pow(y1-y2, 2));
 }


};

#endif // DATASTRUCTURES_HH
