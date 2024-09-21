/**************************************************************************************
 *
 * @Propòsit:  Programa principal que s'encarrega del procés del servido Arda
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include "../all.h"
#include "../sockets.h"
#include "filesArda.h"
#include "socketsArda.h"
#include "linkedListClient.h"

#define NUM_ARGS 2

ConfigArda configArda;
LinkedListClient ardaLinkedList;
int num_usuarios_connectados;
int num_msg_enviados = 0;
int SOCKETSARDA_fd_socket;

//  Semaforos
pthread_mutex_t semaforo_ardaLinkedList;

void ARDA_freeArdaLinkedList() {
    LINKEDLISTClient_setPOV(&ardaLinkedList,0);
    for(int i = 0; i < num_usuarios_connectados; i++) {
        ClientSocket userConnected = LINKEDLISTClient_get(&ardaLinkedList);
        close(userConnected.fd_client);
        free(userConnected.user_name);
        free(userConnected.ip);
        free(userConnected.port);
        free(userConnected.pid);
        pthread_cancel(userConnected.thrd);
        pthread_join(userConnected.thrd, NULL);
        pthread_detach(userConnected.thrd);
        LINKEDLISTClient_remove(&ardaLinkedList);
    }
    LINKEDLISTClient_setPOV(&ardaLinkedList,0);
    LINKEDLISTClient_destroy(&ardaLinkedList);
}


void ARDA_exit(){
    char *aux;

    FILESARDA_freeConfigArda(&configArda);

    ARDA_freeArdaLinkedList();

    asprintf(&aux, MSG_KO_ARDA, num_msg_enviados);
    printF(aux);
    free(aux);

    close(SOCKETSARDA_fd_socket);

    pthread_mutex_destroy(&semaforo_ardaLinkedList);
    
    signal(SIGINT,SIG_DFL);
    raise(SIGINT);
}


void* ARDA_threadClient(void* fd) {

    int fd_client = *((int*) fd);
    int abortConnClient = 0;
    TramaSocket tramaSocket;
    ClientSocket miUsuario; //  El que tenemos que añadir a la linkedlist, lo guardamos en el thread para saber cual es el nustro

    SOCKETS_getTrama(&tramaSocket, fd_client);
    pthread_mutex_lock(&semaforo_ardaLinkedList);
    SOCKETSARDA_manage_new_connection(tramaSocket, &miUsuario, &ardaLinkedList, &num_usuarios_connectados, fd_client, pthread_self());
    pthread_mutex_unlock(&semaforo_ardaLinkedList);
    SOCKETS_freeTrama(&tramaSocket);
    // LINKEDLISTClient_freeClientSocket(&miUsuario);

    do {
        SOCKETS_getTrama(&tramaSocket, fd_client);
        pthread_mutex_lock(&semaforo_ardaLinkedList);
        abortConnClient = SOCKETSARDA_manage_other_sockets(tramaSocket, &ardaLinkedList, &num_usuarios_connectados, fd_client, &num_msg_enviados);
        pthread_mutex_unlock(&semaforo_ardaLinkedList);
        SOCKETS_freeTrama(&tramaSocket);
    } while(!abortConnClient);

    if(abortConnClient){
        close(fd_client);
        pthread_cancel(pthread_self());
        pthread_detach(pthread_self());
    }
    return NULL;
}


void launchServer() {

    int fd_client;
    struct sockaddr_in c_addr;              
    socklen_t c_len = sizeof(c_addr);

    printF(MSG_WELCOME);
    printF(MSG_WAIT_CONN);

    while(1) {

        // Aceptamos conexión de cliente.
        fd_client = accept(SOCKETSARDA_fd_socket, (void *) &c_addr, &c_len);

        // Declaramos thread.
        pthread_t thrd; 

        // Creamos thread del usuario.  
        pthread_create(&thrd, NULL, ARDA_threadClient, &fd_client);
    }    
}


int main(int arg, char *argv[]) {

    // Comprobar número de argumentos.
    if(arg != NUM_ARGS) {
        return EXIT_FAILURE;
    } else {
        if(!FILESARDA_readConfigArda(argv[1], &configArda)) {
            return EXIT_FAILURE;
        }
    }
    
    ardaLinkedList = LINKEDLISTClient_create();
    signal(SIGINT, ARDA_exit);
    // Iniciamos el servidor de Arda.
    if (SOCKETSARDA_start_server(configArda.port_arda, configArda.ip_arda, &SOCKETSARDA_fd_socket) < 0) {
        
        FILESARDA_freeConfigArda(&configArda);
        LINKEDLISTClient_destroy(&ardaLinkedList);
        return EXIT_FAILURE;
    }

    pthread_mutex_init(&semaforo_ardaLinkedList,NULL);
    
    launchServer();
    return 0;
}
