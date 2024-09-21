/**************************************************************************************
 *
 * @Propòsit:  Mòdul que fa referència als mètodes necessaris per la correcta
 *             comunicació amb sockets d'IluvatarSon.
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#ifndef _SOCKETSILUVATR_H_
#define _SOCKETSILUVATR_H_

// Llibreries
#include "../all.h"
#include "../sockets.h"
#include "manageFile.h"

// Missatges d'error
#define ERR_CONN_SERVER "ERROR trying to connect with the server\n"
#define ERR_SOCKET_CREATION "ERROR creating the socket\n"


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per realitzar la connexió amb el servidor.
*
*   @Parameters:
*               (in) *ip            =   IP del servidor al que es vol connectar
*               (in) *port          =   Port del servidor al que es vol connectar                                                  
*
*   @Return:    (out) socket_conn   =   -1  : Error al intentar connectar-se al servidor
*                                       !-1 : Socket
*
******************************************************************************************************************/
int SOCKETSILUVATAR_connect_to_server(char *ip, char* port);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per enviar la trama de "nova conexió" al servidor indicat.
*
*   @Parameters:
*               (in) fd_server          =   File descriptor del servidor al que voem enviar la trama
*               (in) configIluvatar     =   Dades del IluvatarSon que es vol connectar                            
*
*   @Return:    ---
*
******************************************************************************************************************/
void SOCKETSILUVATAR_send_request_new_connection(int fd_server, ConfigIluvatar configIluvatar);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per parsejar les dades de la trama amb la informació dels usuaris i enmagatzemar-la
*               correctament a l'array de users.
*
*   @Parameters:
*               (in) *data                           =   Cadena amb tota la informació dels usuaris
*               (in/out) *num_users_connected_arda   =   Número d'usuaris connectats a Arda                                                 
*
*   @Return:    (out) *users                         =   Array amb tota la informació dels usuaris 
*
******************************************************************************************************************/
User* SOCKETSILUVATAR_parseToUsersArray(char *data, int *num_users_connected_arda);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per crear el servidor d'IluvatarSon per poder rebre missatges i fitxers.
*
*   @Parameters:
*               (in) *port                               =   Port de l'IluvatarSon
*               (in) *ip                                 =   IP de l'IluvatarSon
*               (in/out) *ILUVATAR_fd_server_iluvatar    =   Socket del servidor d'IluvatarSon                                                
*
*   @Return:    (out) result                         =   0 : Servidor d'IluvatarSon creat correctament
*                                                       -1 : Error al crear servidor d'IluvatarSon
*
******************************************************************************************************************/
int SOCKETSILUVATAR_startMsgServer(char *port, char *ip, int *ILUVATAR_fd_server_iluvatar);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode rebre l'ack del client per saber si el misatge s'ha rebut correctament
*
*   @Parameters:
*               (in) fd_client                           =   fd per on enviem la informació al client                                           
*
*   @Return:    (out) correct                         =   0 : No s'ha rebut una ack correcte
*                                                        1 : Correcte ack
*
******************************************************************************************************************/
int SOCKETSILUVATAR_checkACK(int fd_client);


#endif      //  _SOCKETSILUVATR_H_
