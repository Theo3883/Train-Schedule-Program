//
// Created by theo on 11/28/24.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <string>
#include "Trains.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>
#include <iostream>

class CommandHandler {
private:
    bool logged = false;
    Trains trains;
public:
    CommandHandler() = default;
    CommandHandler(const bool logged): logged(logged) {};
    std::string receiveCommand(char []);
    bool isLogged() const { return logged; }

};



#endif //COMMANDHANDLER_H
