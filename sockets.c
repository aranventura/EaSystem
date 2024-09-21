#define _GNU_SOURCE
#include "sockets.h"

void SOCKETS_getTrama(TramaSocket *tramaSocket, int fd_client){

    char *buff = (char*)malloc(sizeof(char)*4);

    tramaSocket->type = ALL_readUntil(fd_client, '[');
    //printF(tramaSocket->type);

    tramaSocket->header = ALL_readUntil(fd_client, ']');
    //printF(tramaSocket->header);

    read(fd_client, buff, sizeof(char)*4);
    tramaSocket->length = (int)strtol(buff, NULL, 16);
    free(buff);

    tramaSocket->data = (char*)malloc(sizeof(char)*tramaSocket->length+1);
    read(fd_client, tramaSocket->data, sizeof(char)*tramaSocket->length);
    tramaSocket->data[tramaSocket->length] = '\0';
    // printF(tramaSocket->data);
}

void SOCKETS_getTramaFitxer(TramaSocket *tramaSocket, int fd_client) {
    char *buff = (char*)malloc(sizeof(char)*4);

    tramaSocket->type = ALL_readUntil(fd_client, '[');
    //printF(tramaSocket->type);

    tramaSocket->header = ALL_readUntil(fd_client, ']');
    //printF(tramaSocket->header);

    read(fd_client, buff, sizeof(char)*4);
    
    tramaSocket->length = (int)strtol(buff, NULL, 16);
    free(buff);

    //read(fd_client, tramaSocket->data, tramaSocket->length);
    for(int i = 0; i < tramaSocket->length; i++) {
        read(fd_client, &tramaSocket->data[i], 1);
    }

}

void SOCKETS_freeTrama(TramaSocket *tramaSocket){
    free(tramaSocket->type);
    free(tramaSocket->header);
    free(tramaSocket->data);
}

void SOCKETS_sendTrama(int fd, char type, char *header, unsigned int length, char *data){
    char *buff;
    unsigned char bytes[2];
    
    bytes[0] = ((length & 0XFF));
    bytes[1] = ((length >> 8) & 0XFF);

    if (length == 0){
        asprintf(&buff, "%c%s%02x%02x", type, header, bytes[1], bytes[0]);
    } else {
        asprintf(&buff, "%c%s%02x%02x%s", type, header, bytes[1], bytes[0], data);
    }

    write(fd, buff, strlen(buff));
    free(buff);
    
}

void SOCKETS_sendTramaFitxer(int fd, char type, char *header, unsigned int length, char *data) {
    int sizeTrma = 1 + strlen(header) + 4 + length;
    char* trama = (char*) malloc (sizeTrma);
    char* aux;
    int i = 0, j = 0, k = 0;
    unsigned char bytes[2];
    
    bytes[0] = ((length & 0XFF));
    bytes[1] = ((length >> 8) & 0XFF);
    asprintf(&aux, "%02x%02x", bytes[1], bytes[0]);
    
    for(i = 0 ; i < sizeTrma ; i++) {
        if (i == 0) {
            trama[0] = type;

        } else if(i < (int)(strlen(header)+1)) {
            trama[i] = header[j];
            j++;

        } else if (i == (int)(strlen(header)+1)) {
            trama[i] = aux[0];
            i++;
            trama[i] = aux[1];
            i++;
            trama[i] = aux[2];
            i++;
            trama[i] = aux[3];
            
        } else {
            trama[i] = data[k];
            k++;
        }
    }

    write(fd, trama, sizeTrma);
    free(trama);
    free(aux);
}

void SOCKETS_parseDataSocket(TramaSocket new_trama, ClientSocket *newUser){
    long unsigned int i = 0;
    int j = 0, x = 0;

    newUser->user_name = (char *) malloc(1);
    newUser->ip = (char *) malloc(1);
    newUser->port = (char *) malloc(1);
    newUser->pid = (char *) malloc(1);

    for(i = 0; i < strlen(new_trama.data); i++){
        if (new_trama.data[i] != '&'){
            switch (j){
                case 0:
                    newUser->user_name[x] = new_trama.data[i];
                    newUser->user_name = (char *) realloc(newUser->user_name, ++x + 1);
                    break;

                case 1:
                    newUser->ip[x] = new_trama.data[i];
                    newUser->ip = (char *) realloc(newUser->ip, ++x + 1);
                    break;

                case 2:
                    newUser->port[x] = new_trama.data[i];
                    newUser->port = (char *) realloc(newUser->port, ++x + 1);
                    break;

                default:
                    newUser->pid[x] = new_trama.data[i];
                    newUser->pid = (char *) realloc(newUser->pid, ++x + 1);
                    break;
            }

        } else {
            switch (j){
                case 0:
                    newUser->user_name[x] = '\0';
                    break;

                case 1:
                    newUser->ip[x] = '\0';
                    break;

                case 2:
                    newUser->port[x] = '\0';
                    break;
            }
            j++;
            x = 0;
        }
    }
    newUser->pid[x] = '\0';
}

void SOCKETS_freeUsersConnected(User *users, int num_users){
    for(int i = 0; i < num_users; i++){
        free(users[i].user_name);
        free(users[i].ip);
        free(users[i].port);
        free(users[i].pid);
    }
    free(users);
}