// Datastructures.cc
//
// Student name: Ville Inkinen
// Student email: ville.inkinen@tuni.fi
// Student number: 151361672

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <iostream>
#include <climits>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)
/**
 * @brief Datastructures Constructor for Datastuctures class
 */
Datastructures::Datastructures()
{

  departures_vec = std::vector<std::pair<TrainID, Time>>();
  regions_map = std::unordered_map<RegionID, Region>();
  stations_map = std::unordered_map<StationID, Station>();
  coord_map = std::map<Coord, StationID, CoordComparator>();
  train_map = std::unordered_map<TrainID, Train>();


}

Datastructures::~Datastructures()
{

}
/**
 * @brief station_count Function that calculates count of stations in the datastructures
 * @return count of station
 */
unsigned int Datastructures::station_count()
{
    return stations_map.size();
}

/**
 * @brief clear_all clears all datastructures
 */
void Datastructures::clear_all()
{

stations_map.clear();
regions_map.clear();
departures_vec.clear();
coord_map.clear();
stations_sorted.clear();

}
/**
 * @brief all_stations function that gets all the stations in the datastructure
 * @return  station_ids vector
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_stations_vec;
    for(auto& station : stations_map){
        all_stations_vec.push_back(station.first);
    }

    return all_stations_vec;
}
/**
 * @brief add_station function that adds Station to the datastructure
 * @param id StationID
 * @param name Station name
 * @param coord Stations coords
 * @return boolean value
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord coord)
{

if(stations_map.find(id)!=stations_map.end()){return false;}

stations_map[id] = Station{id, name, coord, departures_vec, nullptr, neighbours};
stations_sorted[name] = {id};
coord_map[coord] = {id};
    return true;

}
/**
 * @brief get_station_name function that gets the name of Station by StationID
 * @param id StationID
 * @return Name name of the station
 */
Name Datastructures::get_station_name(StationID id)
{

    if(stations_map.find(id)==stations_map.end()){return NO_NAME;}
    return stations_map[id].name;
}

/**
 * @brief get_station_coordinates function that gets the coordinates of station by StationID
 * @param id param 1 StationID
 * @return Coord coordinates
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    if(stations_map.find(id)==stations_map.end()){return NO_COORD;}
    return stations_map[id].coord;
}
/**
 * @brief stations_alphabetically function that lists all stations alphabetically using stations_sorted map
 * @return vector of stationids
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
        std::vector<StationID> temp;
        for( auto it = stations_sorted.begin(); it != stations_sorted.end(); ++it ) {
               temp.push_back( it->second );
           }
        return temp;
}
/**
 * @brief stations_distance_increasing function that lists all stations sort by distance using coord_map
 * @return vector of stationids
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> temp;

    for (auto it = coord_map.begin(); it != coord_map.end(); it++){
        temp.push_back(it -> second);
    }
    return temp;

}
/**
 * @brief find_station_with_coord function that finds a station with given coordinates
 * @param coord param 1 coordinates
 * @return StationID id
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
   if(coord_map.find(xy) == coord_map.end()){return NO_STATION;}
   return coord_map.at(xy);
}
/**
 * @brief change_station_coord function that change the coordinates of given Station by stationid
 * @param id param 1
 * @param coord param 2
 * @return bool success
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    if(stations_map.find(id) == stations_map.end()){return false;}

   if (coord_map.find(stations_map.at(id).coord) != coord_map.end()) {
            coord_map.erase(stations_map.at(id).coord);
            stations_map.at(id).coord = newcoord;
            coord_map.insert({newcoord, id});
            return true;
        }
        return false;
}
/**
 * @brief add_departure function that adds a departure to the given station by station id
 * @param station_id param 1 StationID
 * @param train_id param 2 TrainID
 * @param time param 3 Departure time
 * @return boolean value
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    if(stations_map.find(stationid) == stations_map.end()){
            return false;
        }
        else{
            stations_map[stationid].departures.push_back(std::make_pair(trainid, time));
            return true;
        }
    return true;
}
/**
 * @brief remove_departure function that removes a departure from the given station by StationID
 * @param station_id param 1
 * @param train_id param 2
 * @param time param 3
 * @return bool success
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    if(stations_map.find(stationid) == stations_map.end()){
          return false;
      }
      else{
          for(auto it = stations_map[stationid].departures.begin(); it != stations_map[stationid].departures.end(); ++it){
              if(it->first == trainid && it->second == time){
                  stations_map[stationid].departures.erase(it);
                  return true;
              }
          }
          return false;
      }
}
/**
 * @brief station_departures_after function that lists the departures of a station after given time
 * @param id param 1
 * @param time param 2
 * @return vector pair of time and trainid
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> result;
    if(stations_map.find(stationid) == stations_map.end()){
        result.push_back(std::make_pair(NO_TIME, NO_TRAIN));
        return result;
    }
    else{
        for(auto it = stations_map[stationid].departures.begin(); it != stations_map[stationid].departures.end(); ++it){
            if(it->second > time){
                result.push_back(std::make_pair(it->second, it->first));
            }
        }
        std::sort(result.begin(), result.end(), [&](std::pair<Time, TrainID> a, std::pair<Time, TrainID> b) {return a.first < b.first;});
        return result;
    }
}
/**
 * @brief add_region function that adds a region to the datastructure
 * @param id param 1
 * @param name param 2
 * @param coords param 3
 * @return bool
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{

   if(regions_map.find(id)!=regions_map.end()){return false;}

    regions_map[id] = Region{name, coords, id, nullptr};
    return true;

}
/**
 * @brief all_regions function that list all regions in the datastructure
 * @return vector of regionids
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> regions_vec;
    for(auto& region : regions_map){
        regions_vec.push_back(region.first);
    }
    return regions_vec;
}

/**
 * @brief get_region_name function that return the name of the region by regionid
 * @param id param 1
 * @return Name regionname
 */
Name Datastructures::get_region_name(RegionID id)
{

    if(regions_map.find(id)==regions_map.end()){return NO_NAME;}
    return regions_map[id].name;
}

/**
 * @brief get_region_coords function that returns the coordinates of a region by regionid
 * @param id param 1
 * @return vector of coordinates
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{

    if(regions_map.find(id)==regions_map.end()){return std::vector<Coord>{NO_COORD};}
        return regions_map[id].coord;

}

/**
 * @brief add_subregion_to_region function that adds a subregion to the datastructure
 * @param id param 1
 * @param parentid param 2
 * @return bool
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{

    if(regions_map.find(id)==regions_map.end() || regions_map.find(parentid)==regions_map.end()){return false;}

    if(regions_map[id].parent != nullptr){return false;}
    regions_map[id].parent = &regions_map[parentid];
        return true;

}
/**
 * @brief add_station_to_region function that adds a station to region
 * @param id param 1
 * @param parentid param 2
 * @return bool
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    if(stations_map.find(id)==stations_map.end() || regions_map.find(parentid)==regions_map.end()){return false;}
        stations_map[id].ptr = &regions_map[parentid];
        return true;
}

/**
 * @brief station_in_regions Function that return all region that a station is in
 * @param id param 1
 * @return vector of regionids
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> result;
    if(stations_map.find(id)==stations_map.end()){result.push_back(NO_REGION); return result;}

    Region* regionPtr = stations_map[id].ptr;
    while (regionPtr != nullptr) {
        result.push_back(regionPtr -> id);
        regionPtr = regionPtr -> parent;
    }
    return result;
}

/**
 * @brief all_subregions_of_region function that returns all the subregions of a region.
 * @param id param 1
 * @return vector of regionids
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID> result;
    if(regions_map.find(id)==regions_map.end()){result.push_back(NO_REGION) ; return result;}

    for(auto it = regions_map.begin(); it != regions_map.end(); ++it){
        if(it->second.parent != nullptr && it->second.parent->id == id){
             result.push_back(it->first);
            }
          }
    return result;
}
/**
 * @brief stations_closest_to function that return the three closest stations to a coordinate
 * @param xy param 1
 * @return vector of stationids
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<Station> stations;
    for (auto& station : stations_map)
    {
        stations.push_back(station.second);
    }

    std::partial_sort(stations.begin(), stations.begin() + 3, stations.end(), [xy](const auto &a, const auto &b){
        return sqrt( pow(a.coord.x - xy.x, 2) + pow(a.coord.y - xy.y, 2)) <
                sqrt( pow(b.coord.x - xy.x, 2) + pow(b.coord.y - xy.y, 2)); });


    std::vector<StationID> closest_three = {};
    for (unsigned i = 0; i < stations.size(); i++) {
        if (i == 3) {
            break;
        }
        closest_three.push_back(stations[i].id);
    }

    return closest_three;

}
/**
 * @brief remove_station function that removes a station from the datastructure
 * @param id param 1
 * @return bool
 */
bool Datastructures::remove_station(StationID id)
{

    auto station_iter = stations_map.find(id);
    if (station_iter == stations_map.end())
    {
        return false;
    }

    auto station = station_iter->second;

    stations_map.erase(station_iter);
    stations_sorted.erase(station.name);
    coord_map.erase(station.coord);


    return true;
}
/**
 * @brief common_parent_of_regions function that return the first common parentregion of two regions
 * @param id1 param 1
 * @param id2 param 2
 * @return RegionID id
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    if(regions_map.find(id1)==regions_map.end() || regions_map.find(id2)==regions_map.end()){return NO_REGION;}

      std::vector<RegionID> id1Parents;
      std::vector<RegionID> id2Parents;

      Region* regionPtr = &regions_map[id1];
      while (regionPtr != nullptr) {
          id1Parents.push_back(regionPtr -> id);
          regionPtr = regionPtr -> parent;
      }

      regionPtr = &regions_map[id2];
      while (regionPtr != nullptr) {
          id2Parents.push_back(regionPtr -> id);
          regionPtr = regionPtr -> parent;
      }

      for(auto it = id1Parents.begin(); it != id1Parents.end(); ++it){
          for(auto it2 = id2Parents.begin(); it2 != id2Parents.end(); ++it2){
              if(*it == *it2){
                  return *it;
              }
          }
      }
      return NO_REGION;
}
/**
 * @brief add_train adds trainid and vector of pairs to datastructures
 * @param trainid param 1 Unique id for train in string format
 * @param stationtimes param 2 Vector of pairs containing stationID and train time
 * @return bool
 */

bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes)

{
    if(train_map.find(trainid) != train_map.end()){return false;}
    for (const auto& StationID : stationtimes){
        if(stations_map.find(StationID.first) == stations_map.end()){return false;}
    }


    train_map[trainid] = Train{stationtimes};

    for (auto it = stationtimes.begin(); it != stationtimes.end()-1; it++){
        stations_map [it -> first].neighbours.push_back((it + 1) -> first);
        stations_map[it ->first].departures.push_back(std::make_pair(trainid, it->second));



    }

    return true;
}
/**
 * @brief next_stations_from finds the next stations from given parameter
 * @param id param 1 StationID
 * @return Vector of next_stations / neighbours
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
     if(stations_map.find(id) == stations_map.end()){return std::vector<StationID>{NO_STATION};}

     return stations_map[id].neighbours;

}
/**
 * @brief Datastructures::train_stations_from Returns a vector of stations that the given train passed
 after leaving the given station.
 * @param stationid param 1 The station the train leaves from
 * @param trainid param 2 The train that goes through the stations
 * @return returns a vector with station IDs
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{

    std::vector<StationID> nextstations;
       if(stations_map.find(stationid) == stations_map.end()){
           return std::vector<StationID>{NO_STATION};
       }
       if(train_map.find(trainid) == train_map.end()){
           return std::vector<StationID>{NO_STATION};
       }

       for (auto it = train_map[trainid].stationtimes.begin(); it != train_map[trainid].stationtimes.end(); it++){
           if(it->first == stationid){
               for (auto it2 = it; it2 != train_map[trainid].stationtimes.end(); it2++){
                   if(it2->first != stationid){
                   nextstations.push_back(it2->first);
                   }

               }
           }
       }
       if(nextstations.empty()){
           return std::vector<StationID> {NO_STATION};
       }
       return nextstations;

}
/**
 * @brief Datastructures::clear_trains clears the train datastructure
 */
void Datastructures::clear_trains()
{
    train_map.clear();
    for(auto it = stations_map.begin(); it != stations_map.end(); ++it){
            it->second.neighbours.clear();
        }
}
/**
 * @brief Datastructures::route_any find any route between fromid and toid
 * @param fromid param 1 Starting station
 * @param toid param 2 Destination station
 * @return vector with stationID's and distances on the route
 */

// HUOM. Jos kirjoittaisin alemmat funktion uudelleen, tekisin joitain asioita eri tavalla
// mm. visited unordered_set sijaan käyttäisin enum tyyppiä asemissa, mikä kuvastaa onko asema
// käsitelty tai vaihtoehtoisesti boolean arvoa..
// Tekisin myös erillisen funktion, ettei samaa koodia tarvitsisi toistaa alemmissa funktioissa.´
std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid)
{
    std::vector<std::pair<StationID, Distance>> result;

    if(stations_map.find(fromid) == stations_map.end() || stations_map.find(toid) == stations_map.end()){return std::vector<std::pair<StationID, Distance>>{{NO_STATION, NO_DISTANCE}};}
    std::unordered_set<std::string> visited;
    std::vector<StationID> queue;
    std::unordered_map<StationID, StationID> parent;
    queue.push_back(fromid);
    visited.insert(fromid);
    parent[fromid] = fromid;

    while(!queue.empty()){
        StationID current = queue.front();
        queue.erase(queue.begin());
        for(auto it = stations_map[current].neighbours.begin(); it != stations_map[current].neighbours.end(); ++it){
            if(visited.find(*it) == visited.end()){
                queue.push_back(*it);
                visited.insert(*it);
                parent[*it] = current;

            }
        }
    }

    if(visited.find(toid) == visited.end()){return std::vector<std::pair<StationID, Distance>>{};}
    StationID current = toid;

    while(current != fromid){
        result.push_back(std::make_pair(current, distance_between(parent[current], current)));
        current = parent[current];
    }
    result.push_back(std::make_pair(fromid, 0));
   std::reverse(result.begin(), result.end());
   unsigned x = 1;

   for (x = 1; x < result.size(); x++){
       result[x].second += result[x-1].second;
   }

    return result;

}
/**
 * @brief Datastructures::route_least_stations Finds the route with least stations using route_any() function
 * @param fromid param 1 Starting stations
 * @param toid param 2 destination station
 * @return vector with stationID's and distances on the route
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid)
{
    std::vector<std::pair<StationID, Distance>> result = route_any(fromid, toid);
    return result;
}
/**
 * @brief Datastructures::route_with_cycle finds a route with cycle
 * @param fromid param 1 starting station
 * @return vector with stationid's where first is fromid and last is the first repeated stationID
 */
std::vector<StationID> Datastructures::route_with_cycle(StationID fromid)
{
    std::vector<StationID> result;
    if(stations_map.find(fromid) == stations_map.end()){return std::vector<StationID>{NO_STATION};}

    std::unordered_set<std::string> visited;
    std::vector<StationID> queue;
    std::unordered_map<StationID, StationID> parent;
    queue.push_back(fromid);
    visited.insert(fromid);
    parent[fromid] = fromid;
    while(!queue.empty()){
        StationID current = queue.front();
        queue.erase(queue.begin());
        for(auto it = stations_map[current].neighbours.begin(); it != stations_map[current].neighbours.end(); ++it){
            if(visited.find(*it) == visited.end()){
                queue.push_back(*it);
                visited.insert(*it);
                parent[*it] = current;

            }
            else{
                StationID current2 = current;
                while(current2 != fromid){
                    result.push_back(current2);
                    current2 = parent[current2];
                }

                result.push_back(fromid);

                std::reverse(result.begin(), result.end());
                result.push_back(*it);
                return result;
            }
        }
    }

return result;
}
/**
 * @brief Datastructures::route_shortest_distance finds the route with shortest distance
 * @param fromid param 1 starting station
 * @param toid param 2 destination station
 * @return returns vector of pairs (StationID, Distance)
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID fromid, StationID toid)
{
    std::vector<std::pair<StationID, Distance>> result;
    if(stations_map.find(fromid) == stations_map.end() || stations_map.find(toid) == stations_map.end()){return std::vector<std::pair<StationID, Distance>>{{NO_STATION, NO_DISTANCE}};}

    std::unordered_set<std::string> visited;
    std::vector<StationID> queue;
    std::unordered_map<StationID, StationID> parent;
    std::unordered_map<StationID, Distance> distance;
    queue.push_back(fromid);
    visited.insert(fromid);
    parent[fromid] = fromid;
    distance[fromid] = 0;
    while(!queue.empty()){
        StationID current = queue.front();
        queue.erase(queue.begin());
        for(auto it = stations_map[current].neighbours.begin(); it != stations_map[current].neighbours.end(); ++it){
            if(visited.find(*it) == visited.end()){
                queue.push_back(*it);
                visited.insert(*it);
                parent[*it] = current;
                distance[*it] = distance[current] + distance_between(current, *it);
            }
            else if(distance[current] + distance_between(current, *it) < distance[*it]){
                parent[*it] = current;
                distance[*it] = distance[current] + distance_between(current, *it);
            }
        }
    }
    if(visited.find(toid) == visited.end()){return std::vector<std::pair<StationID, Distance>>{};}
    StationID current = toid;
    while(current != fromid){
        result.push_back(std::make_pair(current, distance_between(parent[current], current)));

        current = parent[current];
    }
    result.push_back(std::make_pair(fromid, 0));
    std::reverse(result.begin(), result.end());
    unsigned x = 1;
    for (x = 1; x < result.size(); x++){
        result[x].second += result[x-1].second;
    }
    return result;

}



