//
// Created by theo on 11/26/24.
//

#ifndef TRAIN_H
#define TRAIN_H
#include <string>
#include <cstring>
#include <iostream>
#include <iostream>
#include <utility>


class Train{
private:
    int id;                 // Numarul trenului
    std::string departure;     // Ora plecare
    std::string arrival;       // Ora sosire
    std::string category;      // Categorie tren
    std::string start_station; // Statia de plecare
    std::string end_station;   // Stația finală
    std::string delay_arrival; // Intarziere sosire
    std::string delay_departure; // Intarziere plecare


public:
    Train(int, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);
    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string get_train_data() const;
    void update_delay(const std::string& delay_received);
    void update_delay_departure(const std::string& delay_received);
    [[nodiscard]] std::string arrival_time() const;
    [[nodiscard]] std::string departure_time() const;
    [[nodiscard]] std::string get_end_station() const;
    [[nodiscard]] std::string get_start_station() const;
};

#endif //TRAIN_H
