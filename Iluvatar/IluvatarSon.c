/**************************************************************************************
 *
 * @Propòsit:  Programa principal que s'encarrega del procés IluvatarSon
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#define  _GNU_SOURCE 1

#include "../all.h"
#include "../sockets.h"
#include "manageFile.h"
#include "commandParser.h"
#include "socketsIluvatar.h"
#include "queueMsg.h"

#define ERR_ARGS "Error! You must introduce the name of the configuration file.\n"
#define ERR_OPEN_FILE "Error opening <%s> file. Check it and try again\n"
#define MSG_WELCOME "Welcome %s, son of Iluvatar\n"
#define MSG_EXIT "\nDisconnecting from Arda. See you soon, son of Iluvatar\n"
#define NUM_ARGS 2

ConfigIluvatar configIluvatar;
int ILUVATAR_fd_server;             //  Server arda
int ILUVATAR_fd_server_iluvatar;    //  Server iluvatar
TramaSocket tramaSocket;
User *users;
int num_users_connected_arda;
pthread_t global_thrd; 
pthread_t global_thrd_queue;


//  MSG cueue information NEWW
int id_msg_sender;
int id_msg_rcv;


int ILUVATAR_checkConn(){
    struct pollfd pfd[1];
    pfd[0].fd = ILUVATAR_fd_server;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;
    int ret = poll(pfd, 1, 500);

    switch(ret) {
        case POLL_ERR:
            printF("ERROR AL HACER EL POLLING\n");
            break;
        default:   
            if(pfd[0].revents & POLLIN) {
                //printF("SERVIDOR NO ACTIVO\n");
                return 1;
            } else {
                return 0;
            }
            break;
    }
    return 0;
}

void ILUVATAR_exit() {

    if(ILUVATAR_checkConn()){
        raise(SIGPIPE);
    } else {
        SOCKETS_sendTrama(ILUVATAR_fd_server, '6', HEADER_EXIT, strlen(configIluvatar.user_name), configIluvatar.user_name);
        SOCKETS_getTrama(&tramaSocket, ILUVATAR_fd_server);

        if(strcmp(tramaSocket.header, HEADER_CONOK_GET) == 0){

            SOCKETS_freeTrama(&tramaSocket);
            MANAGEFILE_freeConfigIluvatar(&configIluvatar);
            SOCKETS_freeUsersConnected(users, num_users_connected_arda);

            close(ILUVATAR_fd_server);
            close(ILUVATAR_fd_server_iluvatar);

            pthread_cancel(global_thrd);
            pthread_join(global_thrd, NULL);
            pthread_detach(global_thrd);

            pthread_cancel(global_thrd_queue);
            pthread_join(global_thrd_queue, NULL);
            pthread_detach(global_thrd_queue);

            msgctl(id_msg_rcv, IPC_RMID, (struct msqid_ds *)NULL);

            printF(MSG_EXIT);

            signal(SIGINT,SIG_DFL);
            raise(SIGINT);
        } else {
            SOCKETS_freeTrama(&tramaSocket);
        }
    }
}

void ILUVATAR_ardaKoConn(){

    MANAGEFILE_freeConfigIluvatar(&configIluvatar);
    SOCKETS_freeUsersConnected(users, num_users_connected_arda);

    close(ILUVATAR_fd_server);
    close(ILUVATAR_fd_server_iluvatar);

    pthread_cancel(global_thrd);
    pthread_join(global_thrd, NULL);
    pthread_detach(global_thrd);

    pthread_cancel(global_thrd_queue);
    pthread_join(global_thrd_queue, NULL);
    pthread_detach(global_thrd_queue);

    msgctl(id_msg_rcv, IPC_RMID, (struct msqid_ds *)NULL);

    printF("\nArda is disconnected, try later Son of Iluvatar\n");

    signal(SIGINT,SIG_DFL);
    raise(SIGINT);
}

void *ILUVATAR_thdMsg(void* arg){
    InfoMsg infoMsg;
    DataMsg dataMsg;
    char *aux;
    int id = *((int*) arg);
    int pos_user = 0;
    Fitxer fitxer;
    infoMsg.tipus = -1;

    while(1){
        msgrcv(id, (struct msgbuf *) &infoMsg, sizeof(int) * 3, 0, 0);

       if(infoMsg.tipus == MSG_TYPE) {
            msgrcv(id, (struct msgbuf *)&dataMsg, sizeof(char)*infoMsg.data_length, 0, 0);

            pos_user = QUEUEMSG_searchUserByPid(users, infoMsg.id_mailbox, num_users_connected_arda);

            if (pos_user == -1){
                asprintf(&aux, "\nNew message received!\nYour neighbor 'X' says:\n%s\n\n$ ", dataMsg.data);
                printF(aux);
            } else {
                asprintf(&aux, "\nNew message received!\nYour neighbor %s says:\n%s\n\n$ ", users[pos_user].user_name, dataMsg.data);
                printF(aux);
            }
            QUEUEMSG_sendInfoMsg(infoMsg.id_mailbox, 0, ACK_MSG);

            free(aux);

        } else if (infoMsg.tipus == FILE_TYPE) {
            // Llegim missatge inicial amb la informacio del file que rebrem i la parsegem
            msgrcv(id, (struct msgbuf *)&dataMsg, sizeof(char)*infoMsg.data_length, 0, 0);
            COMMANDPRASER_parseDataSendFile(&fitxer, dataMsg.data, infoMsg.data_length);

            // Llegim tots els missatges fins aconseguir el file sencer
            COMMANDPARSER_readFileQueue(id, infoMsg.id_mailbox, &fitxer, configIluvatar, users, num_users_connected_arda);
            COMMANDPARSER_freeFitxer(&fitxer);

        } else if(infoMsg.tipus == ACK_MSG) {
            printF("\nMessage correctly sent\n\n");

        } else if(infoMsg.tipus == ACK_FILE_OK) {
            printF("\nFile correctly sent\n\n$");

        } else if(infoMsg.tipus == ACK_FILE_KO) {
            printF("\nFile NOT correctly sent\n\n$");
        }

        bzero(&dataMsg.data, 512);
    } 
}

void *ILUVATAR_launchIluvatarServer(){
    TramaSocket tramaSocketLocal;
    int fd_client;
    Fitxer fitxer;

    struct sockaddr_in c_addr;              
    socklen_t c_len = sizeof(c_addr);

    while (1){
        fd_client = accept(ILUVATAR_fd_server_iluvatar, (void *) &c_addr, &c_len);
        SOCKETS_getTrama(&tramaSocketLocal, fd_client);
        
        if(atoi(tramaSocketLocal.type) == 3){   // Get MSG
            COMMANDPARSER_parseDataSendMsg(tramaSocketLocal.data, tramaSocketLocal.length, users, num_users_connected_arda);
            SOCKETS_freeTrama(&tramaSocketLocal);
            //  Send trama ACK
            SOCKETS_sendTrama(fd_client, '3', HEADER_MSG_OK, 0, "");
        
        } else if(atoi(tramaSocketLocal.type) == 4) {   // Get FILE
            COMMANDPRASER_parseDataSendFile(&fitxer, tramaSocketLocal.data, tramaSocketLocal.length);
            COMMANDPARSER_readFile(fd_client, &tramaSocketLocal, &fitxer, configIluvatar, users, num_users_connected_arda);
        }
        
        close(fd_client);
    }
}

int main(int arg, const char* argv[]) {
    char* buff;
    int num_command = 1;

    if(arg != NUM_ARGS) {
        printF(ERR_ARGS); 
        exit(EXIT_FAILURE);
    } else {
        if(!MANAGEFILE_readConfigIluvatar(argv[1], &configIluvatar)) {     
            raise(SIGINT);                                           
        }
    }

    //Connection to arda
    ILUVATAR_fd_server = SOCKETSILUVATAR_connect_to_server(configIluvatar.ip_arda, configIluvatar.port_arda);
    if (ILUVATAR_fd_server < 0){
        //  TODO: si no esta arda connectat queda 1 file descriptor obert
        MANAGEFILE_freeConfigIluvatar(&configIluvatar);
        printF(ERR_CONN_SERVER);
        raise(SIGINT);
    }

    //  Reprogramamos SIGINT
    signal(SIGINT, ILUVATAR_exit);
    signal(SIGPIPE, ILUVATAR_ardaKoConn);

    SOCKETSILUVATAR_send_request_new_connection(ILUVATAR_fd_server, configIluvatar);

    SOCKETS_getTrama(&tramaSocket, ILUVATAR_fd_server);
    users = SOCKETSILUVATAR_parseToUsersArray(tramaSocket.data, &num_users_connected_arda);
    SOCKETS_freeTrama(&tramaSocket);

    //  Creating server of iluvatar
    if(SOCKETSILUVATAR_startMsgServer(configIluvatar.port_iluvatar, configIluvatar.ip_iluvatar, &ILUVATAR_fd_server_iluvatar) < 0){
        printF("ERROR starting Iluvatar server");
        raise(SIGINT);
    }

    pthread_create(&global_thrd, NULL, ILUVATAR_launchIluvatarServer, NULL);

    //  Creating Iluvatars queue
    if (!QUEUEMSG_initQueue(&id_msg_rcv)){
        raise(SIGINT);
    }

    pthread_create(&global_thrd_queue, NULL, ILUVATAR_thdMsg, (void *)&id_msg_rcv);

    //  Welcome msg
    asprintf(&buff, MSG_WELCOME, configIluvatar.user_name);
    printF(buff);
    free(buff);

    //  Read command from the user
    while (num_command != NUM_COMMAND_EXIT){
        if (num_command != 0){
            printF("$ ");
        }
        buff = ALL_readUntil(0, '\n');
        if(ILUVATAR_checkConn()){
            free(buff);
            raise(SIGPIPE);
        }
        num_command = COMMANDPARSER_parseCommand(buff, &configIluvatar, users, num_users_connected_arda);

        if(num_command == NUM_COMMAND_UPDATE_USERS){
            SOCKETS_freeUsersConnected(users, num_users_connected_arda);
            SOCKETS_sendTrama(ILUVATAR_fd_server, '2', HEADER_LIST_REQUEST, strlen(configIluvatar.user_name), configIluvatar.user_name);
            SOCKETS_getTrama(&tramaSocket, ILUVATAR_fd_server);
            users = SOCKETSILUVATAR_parseToUsersArray(tramaSocket.data, &num_users_connected_arda);
            SOCKETS_freeTrama(&tramaSocket);
            printF("Users list updated\n");
        }

        if(num_command == NUM_COMMAND_SEND_MSG){
            SOCKETS_sendTrama(ILUVATAR_fd_server, '8', HEADER_COUNT_MSG_ARDA, strlen(configIluvatar.user_name), configIluvatar.user_name);
        }

        free(buff);
    }
    raise(SIGINT);
}
