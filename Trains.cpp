//
// Created by theo on 11/26/24.
//

#include "Trains.h"

std::string Trains::convert(char hour[10]) {
    int hour_num = atoi(hour);
    const int hours = hour_num / 3600;
    const int minute = hour_num % 3600 / 60;
    const int second = hour_num % 3600 % 60;
    std::string hoursString;
    std::string minuteString = std::to_string(minute);
    std::string secondString;
    if (hours < 10)
        hoursString = '0' + std::to_string(hours);
    else
        hoursString = std::to_string(hours);
    if (second)
        secondString = std::to_string(second);
    else
        secondString = "00";
    if (minuteString.size() == 1)
        minuteString.insert(0, 1, '0');
    hoursString.append(":");
    hoursString.append(minuteString);
    hoursString.append(":");
    hoursString.append(secondString);
    return hoursString;
}

std::string Trains::get_city(const char station[]) const {

    std::string station_string_final;
    station_string_final.erase(station_string_final.begin(), station_string_final.end());

    for (auto &train: trains) {
        char start_station_train[50];
        char end_station_train[50];
        std::string start_station_string = train->get_start_station();
        std::string end_station_string = train->get_end_station();
        strcpy(start_station_train, start_station_string.c_str());
        strcpy(end_station_train, end_station_string.c_str());
        char *start = strtok(start_station_train, " ");
        char *end = strtok(end_station_train, " ");
        if (strcmp(start, station) == 0 && station_string_final.empty()) {
            station_string_final.append(start_station_string);
            return station_string_final;
        } else if (strcmp(end, station) == 0 && station_string_final.empty()) {
            station_string_final.append(end_station_string);
            return station_string_final;
        }
       /* if (strcmp(end, end_station) == 0 && end_station_string_final.empty()) {
            end_station_string_final.append(end_station_string);
            ok++;
        } else if (strcmp(start, end_station) == 0 && end_station_string_final.empty()) {
            end_station_string_final.append(start_station_string);
            ok++;
        }*/
    }
    return "nullptr";
}

Trains::Trains() {
    //char *filename = "/home/parallels/Desktop/Retele-project-master/map.xml";
    //char *filename = "/home/theo/Desktop/Retele-project-master/map.xml";
    char *filename = "map.xml";
    xmlDoc *doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        fprintf(stderr, "Failed to load XML file: %s\n", filename);
        return;
    }

    xmlNode *root = xmlDocGetRootElement(doc);
    if (!root) {
        fprintf(stderr, "Empty XML document: %s\n", filename);
        xmlFreeDoc(doc);
        return;
    }

    xmlNode *cur_node = NULL;
    int train_count = 0;

    int id; // Numarul trenului
    char departure[10]; // Ora plecare
    char arrival[10]; // Ora sosire
    char category[20]; // Categorie tren
    char start_station[50]; // Stația de plecare
    char end_station[50]; // Stația finală
    std::string categoryString;
    std::string start_stationString;
    std::string end_stationString;

    for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE && strcmp((char *) cur_node->name, "XmlMts") == 0) {
            xmlNode *mts_node = cur_node->children;
            for (; mts_node; mts_node = mts_node->next) {
                if (mts_node->type == XML_ELEMENT_NODE && strcmp((char *) mts_node->name, "Mt") == 0) {
                    xmlNode *trenuri_node = mts_node->children;
                    for (; trenuri_node; trenuri_node = trenuri_node->next) {
                        if (trenuri_node->type == XML_ELEMENT_NODE && strcmp((char *) trenuri_node->name, "Trenuri") ==
                            0) {
                            xmlNode *tren_node = trenuri_node->children;
                            for (; tren_node; tren_node = tren_node->next) {
                                if (tren_node->type == XML_ELEMENT_NODE && strcmp((char *) tren_node->name, "Tren") ==
                                    0) {
                                    // Train train;
                                    // memset(&train, 0, sizeof(Train)); // Inițializare

                                    // Citește atributele trenului
                                    memset(departure, 0, sizeof(departure));
                                    xmlChar *numar = xmlGetProp(tren_node, (const xmlChar *) "Numar");
                                    if (numar) {
                                        id = atoi((char *) numar);
                                        xmlFree(numar);
                                    }

                                    xmlChar *categorie = xmlGetProp(tren_node, (const xmlChar *) "CategorieTren");
                                    if (categorie) {
                                        strncpy(category, (char *) categorie, sizeof(category) - 1);
                                        xmlFree(categorie);
                                    }

                                    // Parcurge elementele traseului
                                    xmlNode *trase_node = tren_node->children;
                                    for (; trase_node; trase_node = trase_node->next) {
                                        if (trase_node->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) trase_node->name, "Trase") == 0) {
                                            xmlNode *trasa_node = trase_node->children;
                                            for (; trasa_node; trasa_node = trasa_node->next) {
                                                if (trasa_node->type == XML_ELEMENT_NODE && strcmp(
                                                        (char *) trasa_node->name, "Trasa") == 0) {
                                                    xmlNode *element_trasa_node = trasa_node->children;
                                                    for (; element_trasa_node;
                                                           element_trasa_node = element_trasa_node->next) {
                                                        if (element_trasa_node->type == XML_ELEMENT_NODE && strcmp(
                                                                (char *) element_trasa_node->name,
                                                                "ElementTrasa") == 0) {
                                                            // Caută stația de plecare/sosire și ora
                                                            xmlChar *ora_plecare = xmlGetProp(
                                                                element_trasa_node, (const xmlChar *) "OraS");
                                                            xmlChar *ora_sosire = xmlGetProp(
                                                                element_trasa_node, (const xmlChar *) "OraP");
                                                            xmlChar *statie_plecare = xmlGetProp(
                                                                element_trasa_node, (const xmlChar *) "DenStaOrigine");
                                                            xmlChar *statie_sosire = xmlGetProp(
                                                                element_trasa_node,
                                                                (const xmlChar *) "DenStaDestinatie");

                                                            if (ora_plecare && ora_sosire && statie_plecare &&
                                                                statie_sosire) {
                                                                // Reține doar prima stație de plecare și ultima stație de sosire
                                                                if (strlen(departure) == 0) {
                                                                    strncpy(departure, (char *) ora_plecare,
                                                                            sizeof(departure) - 1);
                                                                    // printf("DEparture: %s\n", ora_plecare);
                                                                    strncpy(start_station,
                                                                            (char *) statie_plecare,
                                                                            sizeof(start_station) - 1);
                                                                    // printf("Start station: %s\n", statie_plecare);
                                                                }
                                                                strncpy(arrival, (char *) ora_sosire,
                                                                        sizeof(arrival) - 1);
                                                                // printf("Arrival: %s\n", ora_sosire);
                                                                strncpy(end_station, (char *) statie_sosire,
                                                                        sizeof(end_station) - 1);
                                                                // printf("Endstation: %s\n", statie_sosire);

                                                                xmlFree(ora_plecare);
                                                                xmlFree(ora_sosire);
                                                                xmlFree(statie_plecare);
                                                                xmlFree(statie_sosire);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    bool caz = true;
                                    xmlNode *delay = tren_node->children;
                                    xmlChar *delay_arrival_value = nullptr ;
                                    xmlChar *delay_departure_value = nullptr;
                                    for (; delay; delay = delay->next) {
                                        if (delay->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) delay->name, "Intarziere") == 0) {
                                                    delay_arrival_value = xmlGetProp(delay, (const xmlChar *) "delay");
                                        }
                                    }
                                    for (xmlNode *delay_departure = tren_node->children; delay_departure; delay_departure = delay_departure->next) {
                                        if (delay_departure->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) delay_departure->name, "Intarziere_departure") == 0) {
                                                    delay_departure_value = xmlGetProp(delay_departure, (const xmlChar *) "delay");
                                                }
                                    }
                                    if (delay_arrival_value || delay_departure_value) {
                                        caz = false;
                                        std::string delayArrivalString;
                                        std::string delayDepartureString;
                                        if (delay_arrival_value && strlen((char *) delay_arrival_value) > 0) {
                                                delayArrivalString= std::string((char *) delay_arrival_value);
                                        }
                                        if (delay_departure_value && strlen((char *) delay_departure_value) > 0) {
                                             delayDepartureString=std::string((char *) delay_departure_value);
                                        }
                                        std::string departure_converted(this->convert(departure));
                                        std::string arrival_converted(this->convert(arrival));
                                        categoryString.erase(categoryString.begin(), categoryString.end());
                                        start_stationString.erase(start_stationString.begin(),
                                                                  start_stationString.end());
                                        end_stationString.erase(end_stationString.begin(),
                                                                end_stationString.end());
                                        categoryString.append(category);
                                        start_stationString.append(start_station);
                                        end_stationString.append(end_station);

                                        // std::cout<< categoryString<<" "<<start_stationString<<" "<<end_stationString<< '\n';
                                        trains.push_back(new Train(id, departure_converted, arrival_converted,
                                                                   categoryString, start_stationString,
                                                                   end_stationString));
                                        if (!delayArrivalString.empty())
                                            trains[trains.size() - 1]->update_delay(delayArrivalString);
                                        if (!delayDepartureString.empty())
                                            trains[trains.size() - 1]->update_delay_departure(delayDepartureString);
                                        xmlFree(delay_arrival_value);
                                        xmlFree(delay_departure_value);
                                    }
                                    if (id != 0 && caz) {
                                        train_count++;
                                        std::string departure_converted(this->convert(departure));
                                        std::string arrival_converted(this->convert(arrival));
                                        categoryString.erase(categoryString.begin(), categoryString.end());
                                        start_stationString.erase(start_stationString.begin(),
                                                                  start_stationString.end());
                                        end_stationString.erase(end_stationString.begin(), end_stationString.end());
                                        categoryString.append(category);
                                        start_stationString.append(start_station);
                                        end_stationString.append(end_station);

                                        // std::cout<< categoryString<<" "<<start_stationString<<" "<<end_stationString<< '\n';
                                        trains.push_back(new Train(id, departure_converted, arrival_converted,
                                                                   categoryString, start_stationString,
                                                                   end_stationString));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    xmlFreeDoc(doc);
    printf("Loaded %d trains from %s\n", train_count, filename);
}

std::string Trains::get_trains_data() const {
    std::string trains_data;
    for (const auto &train: trains) {
        trains_data.append(train->get_train_data() + '\n');
    }
    return trains_data;
}

std::string Trains::next_hour_trains(const bool &caz) const {
    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    const std::tm localTime = *std::localtime(&currentTime);
    const int secondsSinceMidnight = localTime.tm_hour * 3600 + localTime.tm_min * 60 + localTime.tm_sec;

    std::string response;
    for (const auto &train: trains) {
        std::string arrival_time;
        std::string text;
        if (caz == false) {
            arrival_time = train->arrival_time();
            text = " ajunge la ";
        } else {
            arrival_time = train->departure_time();
            text = " pleaca de la ";
        }
        std::string hours = arrival_time.substr(0, 2);
        std::string minutes = arrival_time.substr(3, 2);
        std::string seconds = arrival_time.substr(6, 2);
        int arrival_seconds = stoi(hours) * 3600 + stoi(minutes) * 60 + stoi(seconds);
        if (secondsSinceMidnight <= arrival_seconds && arrival_seconds <= secondsSinceMidnight + 3600)
            response.append(
                "Trenul: " + std::to_string(train->getId()) + text + train->get_end_station() + " la ora " +
                arrival_time + '\n');
    }
    std::string seconds = std::to_string(secondsSinceMidnight);

    return response;
}

std::string Trains::next_hour_trains_city(const char station[],const bool &caz) {
    std::string station_string = this->get_city(station);
    if (station_string == "nullptr")
        return "Orasul nu exista";

    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    const std::tm localTime = *std::localtime(&currentTime);
    const int secondsSinceMidnight = localTime.tm_hour * 3600 + localTime.tm_min * 60 + localTime.tm_sec;

    std::string response;
    for (const auto &train: trains) {
        std::string arrival_time;
        std::string text;
        if (caz == false ) {  ///false pentru arrivals
            arrival_time = train->arrival_time();
            text = " ajunge de la ";
        } else {
            arrival_time = train->departure_time();
            text = " pleaca la ";
        }
        std::string hours = arrival_time.substr(0, 2);
        std::string minutes = arrival_time.substr(3, 2);
        std::string seconds = arrival_time.substr(6, 2);
        int arrival_seconds = stoi(hours) * 3600 + stoi(minutes) * 60 + stoi(seconds);
        if (secondsSinceMidnight <= arrival_seconds && arrival_seconds <= secondsSinceMidnight + 3600 && (
                caz ? train->get_start_station() : train->get_end_station()) == station_string)
            response.append(
                "Trenul: " + std::to_string(train->getId()) + text + (caz ? train->get_end_station() : train->get_start_station()) + " la ora " +
                arrival_time + '\n');
    }
    std::string seconds = std::to_string(secondsSinceMidnight);

    return response.empty() ? "Nu exista trenuri" : response;
}


std::string Trains::get_train_data(const int id) const {
    for (const auto &train: trains) {
        if (train->getId() == id)
            return train->get_train_data();
    }
    return "Trenul nu exista";
}

/*bool Trains::update_train_data(const int train_id, char *delayChar)
{
    std::string delay;
    if (strcmp(delayChar, "null") == 0)
        delay = "null";
    else
        delay = this->convert(delayChar);
    for (const auto &train : trains)
    {
        if (train->getId() == train_id)
        {
            train->update_delay(delay);
            return true;
        }
    }
    return false;
}*/

bool Trains::update_train_data_arrival(const int train_id, char *delayChar) {
    std::string delay;
    if (delayChar[0] == '-') {
        delay.append("-");
        strcpy(delayChar, delayChar + 1);
        (void)(std::cout << "Timpul este:" << delayChar << std::endl);
    }

    if (strcmp(delayChar, "null") == 0)
        delay = "null";
    else
        delay.append(this->convert(delayChar));

    for (const auto &train: trains) {
        if (train->getId() == train_id) {
            train->update_delay(delay);

            //char *filename = "/home/theo/Desktop/Retele-project-master/map.xml";
            char *filename = "map.xml";
            xmlDoc *doc = xmlReadFile(filename, NULL, 0);
            if (!doc) {
                fprintf(stderr, "Failed to load XML file: %s\n", filename);
                return false;
            }

            xmlNode *root = xmlDocGetRootElement(doc);
            if (!root) {
                fprintf(stderr, "Empty XML document: %s\n", filename);
                xmlFreeDoc(doc);
                return false;
            }

            xmlNode *cur_node = NULL;
            for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
                if (cur_node->type == XML_ELEMENT_NODE && strcmp((char *) cur_node->name, "XmlMts") == 0) {
                    xmlNode *mts_node = cur_node->children;
                    for (; mts_node; mts_node = mts_node->next) {
                        if (mts_node->type == XML_ELEMENT_NODE && strcmp((char *) mts_node->name, "Mt") == 0) {
                            xmlNode *trenuri_node = mts_node->children;
                            for (; trenuri_node; trenuri_node = trenuri_node->next) {
                                if (trenuri_node->type == XML_ELEMENT_NODE && strcmp(
                                        (char *) trenuri_node->name, "Trenuri") == 0) {
                                    xmlNode *tren_node = trenuri_node->children;
                                    for (; tren_node; tren_node = tren_node->next) {
                                        if (tren_node->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) tren_node->name, "Tren") == 0) {
                                            xmlChar *numar = xmlGetProp(tren_node, (const xmlChar *) "Numar");
                                            if (numar && atoi((char *) numar) == train_id) {
                                                xmlNode *restrictii_node = NULL;
                                                for (xmlNode *child = tren_node->children; child; child = child->next) {
                                                    if (child->type == XML_ELEMENT_NODE && strcmp(
                                                            (char *) child->name, "Intarziere") == 0) {
                                                        restrictii_node = child;
                                                        break;
                                                    }
                                                }

                                                if (restrictii_node) {
                                                    xmlSetProp(restrictii_node, BAD_CAST "delay",
                                                               BAD_CAST delay.c_str());
                                                } else {
                                                    restrictii_node = xmlNewChild(
                                                        tren_node, NULL, BAD_CAST "Intarziere", NULL);
                                                    xmlNewProp(restrictii_node, BAD_CAST "delay",
                                                               BAD_CAST delay.c_str());
                                                }

                                                xmlFree(numar);
                                                break;
                                            }
                                            if (numar)
                                                xmlFree(numar);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
            xmlFreeDoc(doc);

            return true;
        }
    }
    return false;
}
bool Trains::update_train_data_departure(const int train_id, char *delayChar) {
    std::string delay;
    if (delayChar[0] == '-') {
        delay.append("-");
        strcpy(delayChar, delayChar + 1);
        (void)(std::cout << "Timpul este:" << delayChar << std::endl);
    }

    if (strcmp(delayChar, "null") == 0)
        delay = "null";
    else
        delay.append(this->convert(delayChar));

    for (const auto &train: trains) {
        if (train->getId() == train_id) {
            train->update_delay(delay);

            //char *filename = "/home/theo/Desktop/Retele-project-master/map.xml";
            char *filename = "map.xml";
            xmlDoc *doc = xmlReadFile(filename, NULL, 0);
            if (!doc) {
                fprintf(stderr, "Failed to load XML file: %s\n", filename);
                return false;
            }

            xmlNode *root = xmlDocGetRootElement(doc);
            if (!root) {
                fprintf(stderr, "Empty XML document: %s\n", filename);
                xmlFreeDoc(doc);
                return false;
            }

            xmlNode *cur_node = NULL;
            for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
                if (cur_node->type == XML_ELEMENT_NODE && strcmp((char *) cur_node->name, "XmlMts") == 0) {
                    xmlNode *mts_node = cur_node->children;
                    for (; mts_node; mts_node = mts_node->next) {
                        if (mts_node->type == XML_ELEMENT_NODE && strcmp((char *) mts_node->name, "Mt") == 0) {
                            xmlNode *trenuri_node = mts_node->children;
                            for (; trenuri_node; trenuri_node = trenuri_node->next) {
                                if (trenuri_node->type == XML_ELEMENT_NODE && strcmp(
                                        (char *) trenuri_node->name, "Trenuri") == 0) {
                                    xmlNode *tren_node = trenuri_node->children;
                                    for (; tren_node; tren_node = tren_node->next) {
                                        if (tren_node->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) tren_node->name, "Tren") == 0) {
                                            xmlChar *numar = xmlGetProp(tren_node, (const xmlChar *) "Numar");
                                            if (numar && atoi((char *) numar) == train_id) {
                                                xmlNode *restrictii_node = NULL;
                                                for (xmlNode *child = tren_node->children; child; child = child->next) {
                                                    if (child->type == XML_ELEMENT_NODE && strcmp(
                                                            (char *) child->name, "Intarziere_departure") == 0) {
                                                        restrictii_node = child;
                                                        break;
                                                    }
                                                }

                                                if (restrictii_node) {
                                                    xmlSetProp(restrictii_node, BAD_CAST "delay",
                                                               BAD_CAST delay.c_str());
                                                } else {
                                                    restrictii_node = xmlNewChild(
                                                        tren_node, NULL, BAD_CAST "Intarziere_departure", NULL);
                                                    xmlNewProp(restrictii_node, BAD_CAST "delay",
                                                               BAD_CAST delay.c_str());
                                                }

                                                xmlFree(numar);
                                                break;
                                            }
                                            if (numar)
                                                xmlFree(numar);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
            xmlFreeDoc(doc);

            return true;
        }
    }
    return false;
}

void Trains::delete_intarziere_arrival(const std::optional<int> train_id, const bool caz) const {
    for (const auto &train: trains) {
        if (train->getId() == train_id || caz) {
            //train->update_delay(delay);

            // Update the XML file
            //char *filename = "/home/theo/Desktop/Retele-project-master/map.xml";
            char *filename = "map.xml";
            xmlDoc *doc = xmlReadFile(filename, NULL, 0);
            if (!doc) {
                fprintf(stderr, "Failed to load XML file: %s\n", filename);
                return;
            }

            xmlNode *root = xmlDocGetRootElement(doc);
            if (!root) {
                fprintf(stderr, "Empty XML document: %s\n", filename);
                xmlFreeDoc(doc);
                return;
            }

            xmlNode *cur_node = NULL;
            for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
                if (cur_node->type == XML_ELEMENT_NODE && strcmp((char *) cur_node->name, "XmlMts") == 0) {
                    xmlNode *mts_node = cur_node->children;
                    for (; mts_node; mts_node = mts_node->next) {
                        if (mts_node->type == XML_ELEMENT_NODE && strcmp((char *) mts_node->name, "Mt") == 0) {
                            xmlNode *trenuri_node = mts_node->children;
                            for (; trenuri_node; trenuri_node = trenuri_node->next) {
                                if (trenuri_node->type == XML_ELEMENT_NODE && strcmp(
                                        (char *) trenuri_node->name, "Trenuri") == 0) {
                                    xmlNode *tren_node = trenuri_node->children;
                                    for (; tren_node; tren_node = tren_node->next) {
                                        if (tren_node->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) tren_node->name, "Tren") == 0) {
                                            xmlNode *intarziere_node = NULL;
                                            for (xmlNode *child = tren_node->children; child; child = child->next) {
                                                if (child->type == XML_ELEMENT_NODE && strcmp(
                                                        (char *) child->name, "Intarziere") == 0) {
                                                    intarziere_node = child;
                                                    break;
                                                }
                                            }

                                            if (intarziere_node && (
                                                    !train_id.has_value() || train_id.value() == train->getId())) {
                                                xmlUnlinkNode(intarziere_node);
                                                xmlFreeNode(intarziere_node);
                                            }
                                            if (train_id.has_value()) {
                                                xmlChar *numar = xmlGetProp(tren_node, (const xmlChar *) "Numar");
                                                if (numar && atoi((char *) numar) == train_id.value()) {
                                                    xmlFree(numar);
                                                    break;
                                                }
                                                if (numar) {
                                                    xmlFree(numar);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
            xmlFreeDoc(doc);
        }
    }
}
void Trains::delete_intarziere_departure(const std::optional<int> train_id, const bool caz) const {
    for (const auto &train: trains) {
        if (train->getId() == train_id || caz) {
            //train->update_delay(delay);

            // Update the XML file
            //char *filename = "/home/theo/Desktop/Retele-project-master/map.xml";
            char *filename = "map.xml";
            xmlDoc *doc = xmlReadFile(filename, NULL, 0);
            if (!doc) {
                fprintf(stderr, "Failed to load XML file: %s\n", filename);
                return;
            }

            xmlNode *root = xmlDocGetRootElement(doc);
            if (!root) {
                fprintf(stderr, "Empty XML document: %s\n", filename);
                xmlFreeDoc(doc);
                return;
            }

            xmlNode *cur_node = NULL;
            for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
                if (cur_node->type == XML_ELEMENT_NODE && strcmp((char *) cur_node->name, "XmlMts") == 0) {
                    xmlNode *mts_node = cur_node->children;
                    for (; mts_node; mts_node = mts_node->next) {
                        if (mts_node->type == XML_ELEMENT_NODE && strcmp((char *) mts_node->name, "Mt") == 0) {
                            xmlNode *trenuri_node = mts_node->children;
                            for (; trenuri_node; trenuri_node = trenuri_node->next) {
                                if (trenuri_node->type == XML_ELEMENT_NODE && strcmp(
                                        (char *) trenuri_node->name, "Trenuri") == 0) {
                                    xmlNode *tren_node = trenuri_node->children;
                                    for (; tren_node; tren_node = tren_node->next) {
                                        if (tren_node->type == XML_ELEMENT_NODE && strcmp(
                                                (char *) tren_node->name, "Tren") == 0) {
                                            xmlNode *intarziere_node = NULL;
                                            for (xmlNode *child = tren_node->children; child; child = child->next) {
                                                if (child->type == XML_ELEMENT_NODE && strcmp(
                                                        (char *) child->name, "Intarziere_departure") == 0) {
                                                    intarziere_node = child;
                                                    break;
                                                }
                                            }

                                            if (intarziere_node && (
                                                    !train_id.has_value() || train_id.value() == train->getId())) {
                                                xmlUnlinkNode(intarziere_node);
                                                xmlFreeNode(intarziere_node);
                                            }
                                            if (train_id.has_value()) {
                                                xmlChar *numar = xmlGetProp(tren_node, (const xmlChar *) "Numar");
                                                if (numar && atoi((char *) numar) == train_id.value()) {
                                                    xmlFree(numar);
                                                    break;
                                                }
                                                if (numar) {
                                                    xmlFree(numar);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
            xmlFreeDoc(doc);
        }
    }
}

std::string Trains::print_graphs(const char start_station[], const char end_station[]) const {
    std::cout<<"Orasele sunt: "<<start_station<<" "<<end_station<<'\n';
    TrainGraph train_graph(this->trains);
    //train_graph.print_graph();
    const std::string start_station_string_final = this->get_city(start_station);
    const std::string end_station_string_final = this->get_city(end_station);

    if (start_station_string_final != "nullptr" && end_station_string_final != "nullptr") {
        std::cout<<"Caut ruta de la "<<start_station_string_final<<" la "<<end_station_string_final<<'\n';
        std::string response = "Traseul este:" + train_graph.find_shortest_path(start_station_string_final, end_station_string_final) + '\n';
        return response;
    }
    else
        return "Wrong stations";
}