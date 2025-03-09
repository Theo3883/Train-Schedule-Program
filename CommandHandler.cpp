//
// Created by theo on 11/28/24.
//

#include "CommandHandler.h"

bool cauta_username_xml(const char* username) {
    const char* filename = "usernames.xml";
    xmlDoc* doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        perror("Fisierul de usernames nu exista\n");
        return false;
    }

    xmlNode* root = xmlDocGetRootElement(doc);
    if (!root) {
        std::cout << "Documentul este gol: " << filename <<'\n';
        xmlFreeDoc(doc);
        return false;
    }

    xmlNode* cur_node = NULL;
    for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE && strcmp((char*)cur_node->name, "User") == 0) {
            xmlNode* username_node = cur_node->children;
            for (; username_node; username_node = username_node->next) {
                if (username_node->type == XML_ELEMENT_NODE && strcmp((char*)username_node->name, "Username") == 0) {
                    xmlChar* xml_username = xmlNodeGetContent(username_node);
                    if (xml_username && strcmp((char*)xml_username, username) == 0) {
                        xmlFree(xml_username);
                        xmlFreeDoc(doc);
                        return true;
                    }
                    if (xml_username) {
                        xmlFree(xml_username);
                    }
                }
            }
        }
    }

    xmlFreeDoc(doc);
    return false;
}

std::string CommandHandler::receiveCommand(char command[]) {
    command[strlen(command) - 1] = '\0';
    char *command1 = strtok(command, ":");
    std::cout<<"Commanda este:"<<command1<<std::endl;

    if (strcmp(command1, "Mersul trenurilor azi") == 0) {
        return trains.get_trains_data();
    }
    if (strcmp(command1, "Detalii tren") == 0) {
       // std::cout<<"Trimit inapoi:" << trains.get_train_data(10242);
        const char *train = strtok(nullptr, ":");
        int train_id = atoi(train);
        return trains.get_train_data(train_id);
    }
    if (strcmp(command1, "login") == 0) {
        char *username = strtok(nullptr, ":");
        if (cauta_username_xml(username) == true) {
            this->logged = true;
            return "Logged in";
        }
        else {
            this->logged = false;
            return "Username incorect";
        }
    }
    if (strcmp(command1, "logout") == 0) {
        this->logged = false;
        return "Logged out";
    }
    if (strcmp(command1, "quitserver") == 0) {
        if (this->logged == false)
            return "Not logged in";
        else {
            this->logged = false;
            trains.delete_intarziere_arrival(std::nullopt, true);
            trains.delete_intarziere_departure(std::nullopt, true);
            return "quit";
        }
    }
    if (strcmp(command1, "Traseu") == 0) {
        char* start_station = strtok(nullptr, " ");
        char* end_station = strtok(nullptr, "\n");
        if (start_station == nullptr || end_station == nullptr)
            return "Orase lipsa";
        //trains.print_graphs("Bucureşti", "Cluj");
        return trains.print_graphs(start_station, end_station);
        return "printed";
    }
     if (strcmp(command1, "quit") == 0) {
        this->logged = false;
        return "quit";
    }
    if (strcmp(command1, "Next hour departures") == 0) {
        return trains.next_hour_trains(true);
    }
    if (strcmp(command1, "Next hour departures from") == 0) {
        char* station = strtok(nullptr, " ");
        if (station == nullptr)
            return "Oras lipsa";
        return trains.next_hour_trains_city(station, true);
    }
    if (strcmp(command1, "Next hour arrivals") == 0) {
        return trains.next_hour_trains(false); ///false pentru arrivals
    }
    if (strcmp(command1, "Next hour arrivals from") == 0) {
        char* station = strtok(nullptr, " ");
        if (station == nullptr)
            return "Oras lipsa";
        return trains.next_hour_trains_city(station, false);
    }
    if (strcmp(command1, "Update arrival") == 0) {
        if (this->logged == true) {
            char *train_id = strtok(nullptr, " ");
            int train_idx = atoi(train_id);
            char *delay_time = strtok(nullptr, "\0");
            if (delay_time == nullptr || train_id ==nullptr)
                return "Tren sau timp lipsa";
            if (strcmp(delay_time,"null") ==0) {
                trains.delete_intarziere_arrival(train_idx, false);
                return "Updated train data";
            }
            if (trains.update_train_data_arrival(train_idx, delay_time))
                return "Updated train data";
            else
                return "Wrong train data";
        }
        return "Not logged in";
    }
    if (strcmp(command1, "Update departure") == 0) {
        if (this->logged == true) {
            char *train_id = strtok(nullptr, " ");
            int train_idx = atoi(train_id);
            char *delay_time = strtok(nullptr, "\0");
            if (delay_time == nullptr || train_id ==nullptr)
                return "Tren sau timp lipsa";
            if (strcmp(delay_time,"null") ==0) {
                trains.delete_intarziere_departure(train_idx, false);
                return "Updated train data";
            }
            if (trains.update_train_data_departure(train_idx, delay_time))
                return "Updated train data";
            else
                return "Wrong train data";
        }
        return "Not logged in";
    }
    return "Comanda invalida";
}
