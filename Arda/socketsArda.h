/**************************************************************************************
 *
 * @Propòsit:  Mòdul que fa referència als mètodes necessaris per la correcta
 *             comunicació amb sockets d'Arda.
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#ifndef _SOCKETSARDA_H_
#define _SOCKETSARDA_H_

// Llibreries
#include "../all.h"
#include "../sockets.h"
#include "linkedListClient.h"

// Missatges d'error
#define ERR_START_SERVER "ERROR during the creation of the server.\n"
#define ERR_SOCKET_CREATION "ERROR during the creation of the socket\n"
#define ERR_BIND "Error during the bind of the port\n"

// Missatges Arda
#define MSG_WELCOME "\nARDA SERVER\n"
#define MSG_WAIT_CONN "Waiting for connections...\n"
#define MSG_NEW_CONN "\nNew login: %s, IP: %s, port: %s, PID: %s\n"
#define MSG_DEL_CONN "\nNew exit petition: %s has left Arda\n"
#define MSG_KO_CONN_AUX "No s'ha pogut connectar al socket\n"
#define MSG_USR_LIST_PETITION "\nNew petition: %s demands the users list\nSending users list to %s\n"
#define MSG_UPD_USR_LIST "Updating users list\n"
#define MSG_SND_USR_LIST "Sending users list\n"
#define MSG_RESPONSE_SND "Response sent\n"
#define MSG_KO_ARDA "\nDisconnecting Arda from all Iluvatar’s children\n%d Messages have been sent through the network\nClosing server\n"

// Constants
#define CLOSE_CLIENT 1
#define CONTINUE_CLIENT 0

#define LISTEN_BACKLOG 64

/****************************************************************************************************************
*
*   @Objectiu:  Mètode per crear el servidor d'Arda.
*
*   @Parameters:
*               (in) *port                               =   Port d'Arda
*               (in) *ip                                 =   IP d'Arda
*               (in/out) *ILUVATAR_fd_server_iluvatar    =   Socket del servidor d'Arda                                                
*
*   @Return:    (out) result                         =   0 : Servidor d'Arda creat correctament
*                                                       -1 : Error al crear servidor d'Arda
*
******************************************************************************************************************/
int SOCKETSARDA_start_server(char *port, char *ip, int *SOCKETSARDA_fd_socket);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per gestionar una nova conexió a Arda
*
*   @Parameters:
*               (in) tramaSocket                                =   Trama rebuda a gestionar
*                    *newUser                                   =   Usuari a afegir a la LinkedList
*                    *ardaLinkedList                            =   LinkedList d'usuaris
*                    *num_usuarios_connectados                  =   Numero d'usuaris connectats
*                    fd_client                                  =   fd al que enviem la resposta
*                    this_thread                                =   Thread del proceso
*                                                             
*
*   @Return:    
*
******************************************************************************************************************/
void SOCKETSARDA_manage_new_connection(TramaSocket tramaSocket, ClientSocket *newUser, LinkedListClient *ardaLinkedList, int *num_usuarios_connectados, int fd_client, pthread_t this_thread);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per gestionar una nova conexió a Arda
*
*   @Parameters:
*               (in) tramaSocket                                =   Trama rebuda a gestionar
*                    *ardaLinkedList                            =   LinkedList d'usuaris
*                    *num_usuarios_connectados                  =   Numero d'usuaris connectats
*                    fd_client                                  =   fd al que enviem la resposta
*                    *num_msg_enviados                          =   Numero de mensajes que se han envido entre procesos IluvatarSon
*                                                             
*
*   @Return:    (out)
*                    Exit                                       =   0   Abort client
*                    Continue                                   =   1   Continue client
*
******************************************************************************************************************/
int SOCKETSARDA_manage_other_sockets(TramaSocket tramaSocket, LinkedListClient *ardaLinkedList, int *num_usuarios_connectados, int fd_client, int *num_msg_enviados); 

#endif      //_SOCKETSARDA_H_