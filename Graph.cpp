//
// Created by theo on 12/10/24.
//

#ifndef GRAPH_H
#define GRAPH_H

#include "Graph.h"

TrainGraph::TrainGraph(const std::vector<Train*>& trains) {
    int index = 0;

    for (const auto& train : trains) {
        std::string start_station = train->get_start_station();
        std::string end_station = train->get_end_station();

        // putem statia de plecare in map daca nu exista
        if (station_index.find(start_station) == station_index.end()) {
            station_index[start_station] = index++;
            lista_adiacenta.push_back({});
        }

        // putem statia finala in map daca nu exista
        if (station_index.find(end_station) == station_index.end()) {
            station_index[end_station] = index++;
            lista_adiacenta.push_back({});
        }

        // adaugam muchia in list de adiacenta
        add_muchie(start_station, end_station);
    }
}

TrainGraph::~TrainGraph() {
    for (auto& list : lista_adiacenta) {
        list.clear();
    }
    lista_adiacenta.clear();

    station_index.clear();
}

void TrainGraph::add_muchie(const std::string& start_station, const std::string& end_station) {
    int start_index = station_index[start_station];
    std::set<std::string> muchii(lista_adiacenta[start_index].begin(), lista_adiacenta[start_index].end()); // muchiile sunt unice
    muchii.insert(end_station);
    lista_adiacenta[start_index] = std::vector<std::string>(muchii.begin(), muchii.end());
}

void TrainGraph::print_graph() const {
    for (const auto& pair : station_index) {
        std::cout << pair.first << " -> ";
        for (const auto& dest : lista_adiacenta[pair.second]) {
            std::cout << dest << " ";
        }
        std::cout << std::endl;
    }
}
std::string TrainGraph::find_shortest_path(const std::string& start_station, const std::string& end_station) const {
    if (station_index.find(start_station) == station_index.end() || station_index.find(end_station) == station_index.end()) {
        return "Start or end station wrong";
    }

    int n = station_index.size();
    std::vector<int> distances(n, 2e9);
    std::vector<int> previous(n, -1);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    int start_index = station_index.at(start_station);
    distances[start_index] = 0;
    pq.push({0, start_index});

    while (!pq.empty()) {
        int current_distance = pq.top().first;
        int current_index = pq.top().second;
        pq.pop();

        if (current_distance > distances[current_index]) {
            continue;
        }

        for (const auto& nod : lista_adiacenta[current_index]) {
            int nod_index = station_index.at(nod);
            int new_distance = current_distance + 1; // inca n-am implementat cu timpii trenurilor

            if (new_distance < distances[nod_index]) {
                distances[nod_index] = new_distance;
                previous[nod_index] = current_index;
                pq.push({new_distance, nod_index});
            }
        }
    }

    int end_index = station_index.at(end_station);
    if (distances[end_index] == 2e9) {
        return "No path found :(";
    }

    std::vector<std::string> path;
    for (int at = end_index; at != -1; at = previous[at]) {
        for (const auto& pair : station_index) {
            if (pair.second == at) {
                path.push_back(pair.first);
                break;
            }
        }
    }
    std::reverse(path.begin(), path.end());
    std::string path_string;
    for (auto& station : path) {
        path_string += station + " -> ";
    }
    return path_string;

}
#endif // GRAPH_H