//
// Created by theo on 11/26/24.
//

#include "Train.h"


Train::Train(const int id, const std::string &departure, const std::string &arrival, const std::string &category1,
             const std::string &start_station1,
             const std::string &end_station1): id(id), departure(departure), arrival(arrival), category(category1),
                                               start_station(start_station1), end_station(end_station1) {
}

int Train::getId() const {
    return id;
}


std::string Train::get_train_data() const {
    /*std::cout << "Train ID: " << train_id << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Departure: " << departure << std::endl;
    std::cout << "Arrival: " << arrival << std::endl;
    std::cout << "Start station: " << start_station << std::endl;
    std::cout << "End station: " << end_station << std::endl;*/
    std::string train_data;
    train_data.append("Train ID: " + std::to_string(id) + '\n');
    if (delay_departure.size() > 0) {
        if (delay_departure[0] == '-')
            train_data.append("Departure: " + departure + " - " + delay_departure.substr(1) + " faster" + '\n');
        else
            train_data.append("Departure: " + departure + " + " + delay_departure + " delay" + '\n');
    } else
        train_data.append("Departure: " + departure + '\n');

    if (delay_arrival.size() > 0) {
        if (delay_arrival[0] == '-')
            train_data.append("Arrival: " + arrival + " - " + delay_arrival.substr(1) + " faster" + '\n');
        else
            train_data.append("Arrival: " + arrival + " + " + delay_arrival + " delay" + '\n');
    } else
        train_data.append("Arrival: " + arrival + '\n');
    train_data.append("Category: " + category + '\n');
    train_data.append("Start Station: " + start_station + '\n');
    train_data.append("End Station: " + end_station + '\n');
    return train_data;
}

void Train::update_delay(const std::string &delay_received) {
    if (delay_received == "null") {
        this->delay_arrival.erase(this->delay_arrival.begin(), this->delay_arrival.end());
        return;
    }
    this->delay_arrival.append(delay_received);
}

void Train::update_delay_departure(const std::string &delay_received) {
    if (delay_received == "null") {
        this->delay_departure.erase(this->delay_departure.begin(), this->delay_departure.end());
        return;
    }
    this->delay_departure = delay_received;
}

std::string Train::arrival_time() const {
    if (!delay_arrival.empty()) {
        return std::string(this->arrival + (delay_arrival[0] == '-'
                                                ? " - " + delay_arrival.substr(1) + " faster"
                                                : " + " + delay_arrival + " delay"));
    }
    return this->arrival;
}

std::string Train::departure_time() const {
    if (!delay_departure.empty()) {
        return std::string(this->departure + (delay_departure[0] == '-'
                                                ? " - " + delay_departure.substr(1) + " faster"
                                                : " + " + delay_departure + " delay"));
    }
    return this->departure;
}

std::string Train::get_end_station() const {
    return this->end_station;
}

std::string Train::get_start_station() const {
    return this->start_station;
}
