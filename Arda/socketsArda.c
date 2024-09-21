#define _GNU_SOURCE
#include "socketsArda.h"

int SOCKETSARDA_start_server(char *port, char *ip, int *SOCKETSARDA_fd_socket) {

    struct sockaddr_in s_addr;

    // Creamos socket.
    *SOCKETSARDA_fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*SOCKETSARDA_fd_socket < 0) {
        printF(ERR_SOCKET_CREATION);
        return -1;
    }

    bzero(&s_addr, sizeof(s_addr));            // Llenamos información de 0's.
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (atoi(port));
    s_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind (*SOCKETSARDA_fd_socket, (void *) &s_addr, sizeof (s_addr)) < 0) {
        printF(ERR_BIND);
        close(*SOCKETSARDA_fd_socket);
        return -1;
    }

    listen(*SOCKETSARDA_fd_socket, 20);
    return 0;
}

void SOCKETSARDA_manage_new_connection(TramaSocket tramaSocket, ClientSocket *newUser, LinkedListClient *ardaLinkedList, int *num_usuarios_connectados, int fd_client, pthread_t this_thread){
    //char *token;
    char *buff;
    char *aux;

    SOCKETS_parseDataSocket(tramaSocket, newUser);

    /*
    token = strtok(tramaSocket.data, "&");
    printF(token);
    buff = ALL_mystrdup(token);
    asprintf(&newUser->user_name, "%s", buff);
    //printF(buff);
    free(buff);

    token = strtok(NULL, "&");
    buff = ALL_mystrdup(token);
    asprintf(&newUser->ip, "%s", buff);
    //printF(buff);
    free(buff);

    token = strtok(NULL, "&");
    buff = ALL_mystrdup(token);
    asprintf(&newUser->port, "%s", buff);
    //printF(buff);
    free(buff);

    token = strtok(NULL, "\0");
    buff = ALL_mystrdup(token);
    asprintf(&newUser->pid, "%s", buff);
    //printF(buff);
    free(buff);
    free(token);
    */
    

    newUser->fd_client = fd_client;
    newUser->thrd = this_thread;

    //  Print infromation from the new user connected
    asprintf(&buff, MSG_NEW_CONN, newUser->user_name, newUser->ip, newUser->port, newUser->pid);
    printF(buff);
    free(buff);
    
    //  Add newUser to the linkedList
    printf(MSG_UPD_USR_LIST);
    LINKEDLISTClient_add(ardaLinkedList, *newUser);
    
    if (ardaLinkedList->error == LIST_ERROR_MALLOC){
        //  Enviar trama de error para Iluvatar
        printF("ERROR trying to update the list");
        raise(SIGINT);
    } else {
        *num_usuarios_connectados = *num_usuarios_connectados + 1;

        //  Get the information of all the users(linkedList) and send it back to the newUser
        LINKEDLISTClient_setPOV(ardaLinkedList,0);
        for(int i = 0; i < LINKEDLISTClient_getLength(*ardaLinkedList); i++){
            ClientSocket auxUsers = LINKEDLISTClient_get(ardaLinkedList);
            if(i == 0){
                asprintf(&aux, "%s&%s&%s&%s", auxUsers.user_name, auxUsers.ip, auxUsers.port, auxUsers.pid);
                buff = (char*)malloc(sizeof(char)*(strlen(aux)+1));
                strcpy(buff, aux);
            } else {
                asprintf(&aux, "#%s&%s&%s&%s", auxUsers.user_name, auxUsers.ip, auxUsers.port, auxUsers.pid);
                buff = (char*)realloc(buff, sizeof(char) * ((strlen(aux)+1) + strlen(buff)));
                strcat(buff, aux);
            }
            free(aux);
            LINKEDLISTClient_next(ardaLinkedList);
        }
        printF(MSG_SND_USR_LIST);
        SOCKETS_sendTrama(fd_client, '1', HEADER_CONOK, strlen(buff), buff);
        printF(MSG_RESPONSE_SND);
        free(buff);
    }
}

int SOCKETSARDA_manage_other_sockets(TramaSocket tramaSocket, LinkedListClient *ardaLinkedList, int *num_usuarios_connectados, int fd_client, int *num_msg_enviados){
    char *buff;
    char *aux;

    switch(atoi(tramaSocket.type))
    {
        case 2:
            asprintf(&buff, MSG_USR_LIST_PETITION, tramaSocket.data, tramaSocket.data);
            printF(buff);
            free(buff);
            //  Get the information of all the users(linkedList) and send it back to the newUser
            LINKEDLISTClient_setPOV(ardaLinkedList,0);
            for(int i = 0; i < LINKEDLISTClient_getLength(*ardaLinkedList); i++){
                ClientSocket auxUsers = LINKEDLISTClient_get(ardaLinkedList);
                if(i == 0){
                    asprintf(&aux, "%s&%s&%s&%s", auxUsers.user_name, auxUsers.ip, auxUsers.port, auxUsers.pid);
                    buff = (char*)malloc(sizeof(char)*(strlen(aux)+1));
                    strcpy(buff, aux);
                } else {
                    asprintf(&aux, "#%s&%s&%s&%s", auxUsers.user_name, auxUsers.ip, auxUsers.port, auxUsers.pid);
                    buff = (char*)realloc(buff, sizeof(char) * ((strlen(aux)+1) + strlen(buff)));
                    strcat(buff, aux);
                }
                free(aux);
                LINKEDLISTClient_next(ardaLinkedList);
            }
            SOCKETS_sendTrama(fd_client, '2', HEADER_LIST_RESPONSE, strlen(buff), buff);
            free(buff);
        break;

        case 6:
            asprintf(&buff, MSG_DEL_CONN, tramaSocket.data);
            printF(buff);
            free(buff);
            printF(MSG_UPD_USR_LIST);
            LINKEDLISTClient_setPOV(ardaLinkedList,0);
            for(int i = 0; i < LINKEDLISTClient_getLength(*ardaLinkedList); i++){
                ClientSocket auxUsers = LINKEDLISTClient_get(ardaLinkedList);
                if (strcmp(auxUsers.user_name, tramaSocket.data) == 0){
                    free(auxUsers.user_name);
                    free(auxUsers.ip);
                    free(auxUsers.port);
                    free(auxUsers.pid);
                    LINKEDLISTClient_remove(ardaLinkedList);
                    *num_usuarios_connectados = *num_usuarios_connectados - 1;
                    LINKEDLISTClient_setPOV(ardaLinkedList, LINKEDLISTClient_getLength(*ardaLinkedList)-1);
                    printF(MSG_SND_USR_LIST);
                    SOCKETS_sendTrama(fd_client, '6', HEADER_CONOK, 0, "");
                    printF(MSG_RESPONSE_SND);
                    return CLOSE_CLIENT;
                } else {
                    LINKEDLISTClient_next(ardaLinkedList);
                }
            }
            printF("ERROR, user not found\n");
        break;

        case 8:
            *num_msg_enviados = *num_msg_enviados + 1;
        break;

        default:
            printF(tramaSocket.type);
        break;
    }
    return CONTINUE_CLIENT;
}
