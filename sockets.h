/**************************************************************************************
 *
 * @Propòsit:  Mòdul referent a totes les funcions necessàries per realitzar
 *             la comunicació entre processos mitjançant sockets.
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#ifndef _SOCKETS_H_
#define _SOCKETS_H_

// Libreries generals
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <time.h>
#include "all.h"
#include "Arda/linkedListClient.h"

// Capçaleres de les trames
#define HEADER_NEW_SON              "[NEW_SON]"
#define HEADER_CONOK                "[CONOK]"
#define HEADER_CONKO                "[CONKO]"
#define HEADER_LIST_REQUEST         "[LIST_REQUEST]"
#define HEADER_LIST_RESPONSE        "[LIST_RESPONSE]"
#define HEADER_MSG                  "[MSG]"
#define HEADER_MSG_OK               "[MSGOK]"
#define HEADER_NEW_FILE             "[NEW_FILE]"
#define HEADER_DATA_FILE            "[FILE_DATA]"
#define HEADER_EXIT                 "[EXIT]"
#define HEADER_CHECK_OK             "[CHECK_OK]"
#define HEADER_CHECK_KO             "[CHECK_KO]"
#define HEADER_COUNT_MSG_ARDA       "[NEW_MSG]"


#define HEADER_CONOK_GET            "CONOK"
#define HEADER_LIST_REQUEST_GET     "LIST_REQUEST"
#define HEADER_LIST_RESPONSE_GET    "LIST_RESPONSE"

// Estructures pròpies
typedef struct {
    char *type;
    char *header;
    int length;
    char *data;
} TramaSocket;

typedef struct {
    char *user_name;
    char *ip;
    char *port;
    char *pid;
} User;


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per llegir i guardar la informació de la trama enviada pel client.
*
*   @Parameters:
*               (in/out) *tramaSocket    =   Punter a estructura de tipus TramaSocket en la que es guardaran les 
*                                            dades obtingudes de la trama llegida.
*
*               (in) fd_client           =   File descriptor del client del que es vol llegir la trama
*
*   @Return:    ---
*
******************************************************************************************************************/
void SOCKETS_getTrama(TramaSocket *tramaSocket, int fd_client);


void SOCKETS_getTramaFitxer(TramaSocket *tramaSocket, int fd_client);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per alliberar la memòria dinàmica de la trama indicada.
*
*   @Parameters:
*               (in/out) *tramaSocket    =   Trama de la que volem alliberar la memòria.
*
*   @Return:    ---
*
******************************************************************************************************************/
void SOCKETS_freeTrama(TramaSocket *tramaSocket);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per enviar una trama al fd indicat.
*
*   @Parameters:
*               (in) fd            =   File descriptor del client al que es vol enviar la trama
*               (in) type          =   Tipus de la trama
*               (in) *header       =   Capçalera de la trama
*               (in) lenght        =   Llargaria del camp data la trama
*               (in) *data         =   Cadena amb tota la informació a enviar
*
*   @Return:    ---
*
******************************************************************************************************************/
void SOCKETS_sendTrama(int fd, char type, char *header, unsigned int length, char *data);


void SOCKETS_sendTramaFitxer(int fd, char type, char *header, unsigned int length, char *data);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per parsejar i guardar la informació sobre un nou usuari de la trama rebuda.
*
*   @Parameters:
*               (in) new_trama         =   Trama que conté la informació a llegir
*               (in/out) *new_user     =   ClientSocket en el que guardarem la informació del nou usuari
*
*   @Return:    ---
*
******************************************************************************************************************/
void SOCKETS_parseDataSocket(TramaSocket new_trama, ClientSocket *newUser);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode alliberar tota la memòria de la llista d'usuaris.
*
*   @Parameters:
*               (in/out) *users      =   Llista d'usuaris a alliberar
*               (in) num_users       =   Número d'usuaris de la llista
*
*   @Return:    ---
*
******************************************************************************************************************/
void SOCKETS_freeUsersConnected(User *users, int num_users);


#endif      //_SOCKETS_H_