#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "CommandHandler.h"
#include <map>
#include <iostream>

#define PORT 2908

extern int errno;

typedef struct thData {
    int idThread;
    int cl;
} thData;

std::map<int, CommandHandler> commandHandlers;

bool raspunde(void *arg) {
    char message[100];
    thData tdL = *((thData*)arg);

    while (true) {
        if (read(tdL.cl, &message, sizeof(message)) <= 0) {
            printf("[Thread %d]\n", tdL.idThread);
            (void)(perror("Eroare la read() de la client.\n"));
            return false;
        }

        printf("[Thread %d]Mesajul a fost receptionat...%s\n", tdL.idThread, message);

        const std::string response = commandHandlers[tdL.idThread].receiveCommand(message);

        printf("[Thread %d]Trimitem mesajul inapoi...%s\n", tdL.idThread, response.c_str());

        if (write(tdL.cl, response.c_str(), response.length() + 1) <= 0) {
            (void)(printf("[Thread %d] ", tdL.idThread));
            (void)(perror("[Thread]Eroare la write() catre client.\n"));
        } else {
            (void)(printf("[Thread %d]Mesajul a fost trasmis cu succes. cu lungimea:%d\n", tdL.idThread, response.length()));
            if (response == "Updated train data") {
                for (auto &handler: commandHandlers) {
                    handler.second = CommandHandler(commandHandlers[tdL.idThread].isLogged());
                }
            }
            if (response == "quit") {
                return true;
            }
        }
    }
    return false;
}

static void *treat(void *arg) {
    thData tdL = *((thData*)arg);
    (void)(printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread));
    fflush(stdout);
    pthread_detach(pthread_self());

    commandHandlers[tdL.idThread] = CommandHandler();

    if (raspunde((thData*)arg)) {
        commandHandlers.erase(tdL.idThread);
        (void)(std::cout << "Clientul cu id-ul " << tdL.idThread << " a fost sters din map si s-a deconectat\n");
        close(tdL.cl);
        free(arg);
        return nullptr;
    }
    free(arg);
    return nullptr;
}

int main() {
    struct sockaddr_in server;
    struct sockaddr_in from;
    int sd;
    pthread_t th[100];
    int i = 0;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        (void)(perror("[server]Eroare la socket().\n"));
        return errno;
    }

    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1) {
        (void)(perror("[server]Eroare la bind().\n"));
        return errno;
    }

    if (listen(sd, 2) == -1) {
        (void)(perror("[server]Eroare la listen().\n"));
        return errno;
    }

    while (true) {
        int client;
        thData *td;
        socklen_t length = sizeof(from);

        (void)(printf("[server]Asteptam la portul %d...\n", PORT));
        fflush(stdout);

        if ((client = accept(sd, (struct sockaddr*)&from, &length)) < 0) {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        td = (thData*)malloc(sizeof(thData));
        if (td == NULL) {
            perror("[server]Eroare la malloc().\n");
            close(client);
            continue;
        }

        td->idThread = i++;
        td->cl = client;

        if (pthread_create(&th[i], NULL, &treat, td) != 0) {
            perror("[server]Eroare la pthread_create().\n");
            free(td);
            close(client);
        }
    }
}