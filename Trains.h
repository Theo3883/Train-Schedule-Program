//
// Created by theo on 11/26/24.
//

#ifndef TRAINS_H
#define TRAINS_H

#include "Train.h"
#include <vector>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <optional>
#include "Graph.h"

class Trains {

private:
    std::vector<Train *> trains;
    std::string convert(char hour[10]);
    std::string get_city(const char station[]) const;

public:
    Trains();

    std::string get_train_data(int id) const;
    bool update_train_data_arrival(int,char*) ;

    bool update_train_data_departure(int train_id, char *delayChar);

    std::string get_trains_data() const;
    //std::string next_hour_departure_trains() const;
    std::string next_hour_trains(const bool&) const;
    std::string next_hour_trains_city(const char station[],const bool &caz);
    void delete_intarziere_arrival(std::optional<int> train_id, bool caz) const;
    std::string  print_graphs(const char start_station[], const char end_station[]) const;
    void delete_intarziere_departure(std::optional<int> train_id, bool caz) const;
};


#endif //TRAINS_H
