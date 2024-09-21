#define _GNU_SOURCE
#include "socketsIluvatar.h"

int SOCKETSILUVATAR_connect_to_server(char *ip, char* port) {
    struct sockaddr_in s_addr;
    int socket_conn = -1;
    
    socket_conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_conn < 0) {
        printF(ERR_SOCKET_CREATION);
    } else {

        memset(&s_addr, 0, sizeof(s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(atoi(port));
        s_addr.sin_addr.s_addr = inet_addr(ip);

        if (connect(socket_conn, (void *) &s_addr, sizeof(s_addr)) < 0) {
            return -1;
        }
    }
    return socket_conn;
}

void SOCKETSILUVATAR_send_request_new_connection(int fd_server, ConfigIluvatar configIluvatar){
    char header = '1';
    char *buff;
    char *aux;
    unsigned char bytes[2];

    //  Size of data
    asprintf(&aux, "%s&%s&%s&%d", configIluvatar.user_name, configIluvatar.ip_iluvatar, configIluvatar.port_iluvatar, getpid());
    
    //  Convert int to 2 byte binary
    unsigned int size_data = strlen(aux);
    bytes[0] = ((size_data & 0XFF));
    bytes[1] = ((size_data >> 8) & 0XFF);

    asprintf(&buff, "%c%s%02x%02x%s", header, HEADER_NEW_SON, bytes[1], bytes[0], aux);
    write(fd_server, buff, strlen(buff));

    free(aux);
    free(buff);
}

User* SOCKETSILUVATAR_parseToUsersArray(char *data, int *num_users_connected_arda){
    int z = 0, x = 0, j = 0;
    User *users;

    users = (User *) malloc (sizeof(User));
    users[z].user_name = (char *) malloc(1);

    for(size_t i = 0; i < strlen(data); i++){
        if (data[i] != '&' && data[i] != '#'){
            switch (j){
                case 0:
                    users[z].user_name[x] = data[i];
                    users[z].user_name = (char *) realloc(users[z].user_name, ++x + 1);
                    break;

                case 1:
                    users[z].ip[x] = data[i];
                    users[z].ip = (char *) realloc(users[z].ip, ++x + 1);
                    break;

                case 2:
                    users[z].port[x] = data[i];
                    users[z].port = (char *) realloc(users[z].port, ++x + 1);
                    break;

                default:
                    users[z].pid[x] = data[i];
                    users[z].pid = (char *) realloc(users[z].pid, ++x + 1);
                    break;
            }

        } else if (data[i] == '&'){
            switch (j){
                case 0:
                    users[z].user_name[x] = '\0';
                    users[z].ip = (char *) malloc(1);
                    break;

                case 1:
                    users[z].ip[x] = '\0';
                    users[z].port = (char *) malloc(1);
                    break;

                case 2:
                    users[z].port[x] = '\0';
                    users[z].pid = (char *) malloc(1);
                    break;
            }
            j++;
            x = 0;

        } else if (data[i] == '#'){
            users[z].pid[x] = '\0';
            users = (User*)realloc(users, sizeof(User) * (++z + 1) );
            users[z].user_name = (char *) malloc(1);
            j = 0;
            x = 0;
        }
    }
    users[z].pid[x] = '\0';
    *num_users_connected_arda = z + 1;

    return users;
}


int SOCKETSILUVATAR_startMsgServer(char *port, char *ip, int *ILUVATAR_fd_server_iluvatar){
    struct sockaddr_in s_addr;

    // Creamos socket.
    *ILUVATAR_fd_server_iluvatar = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*ILUVATAR_fd_server_iluvatar < 0) {
        printF(ERR_SOCKET_CREATION);
        return -1;
    }

    bzero(&s_addr, sizeof(s_addr));            // Llenamos información de 0's.
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (atoi(port));
    s_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind (*ILUVATAR_fd_server_iluvatar, (void *) &s_addr, sizeof (s_addr)) < 0) {
        printF("ERROR binding Iluvatar server\n");
        close(*ILUVATAR_fd_server_iluvatar);
        return -1;
    }

    listen(*ILUVATAR_fd_server_iluvatar, 20);
    return 0;
}

int SOCKETSILUVATAR_checkACK(int fd_client){
    TramaSocket tramaLocal;
    int correct = 0;  

    SOCKETS_getTrama(&tramaLocal, fd_client);

    if (strcmp(tramaLocal.header, "MSGOK") == 0 && atoi(tramaLocal.type) == 3){
        correct = 1;
    }

    SOCKETS_freeTrama(&tramaLocal);

    return correct;
}