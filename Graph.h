//
// Created by theo on 12/10/24.
//
#pragma once
#define GRAPH_H

#include <vector>
#include <string>
#include <map>
#include "Trains.h"
#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>

class TrainGraph {

private:
    std::vector<std::vector<std::string>> lista_adiacenta;
    std::map<std::string, int> station_index;
    void add_muchie(const std::string& start_station, const std::string& end_station);

public:
    TrainGraph(const std::vector<Train *>& trains);
    ~TrainGraph();
    void print_graph() const;
    std::string find_shortest_path(const std::string& start_station, const std::string& end_station) const;
};

