/**************************************************************************************
 *
 * @Propòsit:  Mòdul que s'encarrega de gestionar i realitzar totes les comandes i 
 *             funcions que IluvatarSon necessita per funcionar correctament.
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#ifndef _COMMANDPARSER_H_
#define _COMMANDPARSER_H_

// Llibreries
#include "manageFile.h"
#include "socketsIluvatar.h"
#include "queueMsg.h"
#include "../sockets.h"
#include "../all.h"

// Missatges
#define MSG_UNKNOWN_COMMAND "Unknown command\n"
#define MSG_EMPTY_USR "Message recipient not defined\n"
#define MSG_EMPTY_MSG "Empty message content\n"
#define MSG_LIST_USERS "There are %d children of Iluvatar connected:\n"

// Constants de les comandes
#define COMMAND_EXIT "EXIT"
#define NUM_COMMAND_EXIT -1
#define COMMAND_UPDATE_USERS "UPDATE USERS"
#define NUM_COMMAND_UPDATE_USERS 1
#define COMMAND_LIST_USERS "LIST USERS"
#define NUM_COMMAND_LIST_USERS 2
#define COMMAND_SEND_MSG "SEND MSG"
#define NUM_COMMAND_SEND_MSG 3
#define COMMAND_SEND_FILE "SEND FILE"
#define NUM_COMMAND_SEND_FILE 4
#define NUM_COMMAND_LINUX 5

// Constants de les ips dels servidors coneguts
#define IP_MATAGALLS "172.16.205.3"
#define IP_PUIGPEDROS "172.16.205.17"
#define IP_MONTSERRAT "172.16.205.4"

// Estructura pròpia pel fitxer
typedef struct fitxer {
    char *user_name;
    char *fileName;
    char *fileSize;
    char *MD5sum;
} Fitxer;


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per alliberar la memòria del Fitxer indicat.
*
*   @Parameters:
*               (in/out) *fitxer       =   Estructura de tipus Fitxer de la que alliberar la memòria.
*
*   @Return:    ---
*
******************************************************************************************************************/
void COMMANDPARSER_freeFitxer(Fitxer* fitxer);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per obtenir el nom del servidor que correspon a la ip indicada.
*
*   @Parameters:
*               (in) *ip        =   IP del servidor.
*
*   @Return:    (out) *nom      =   Nom del servidor o "Server unknown" en detriment.
*
******************************************************************************************************************/
char* COMMANDPARSER_getServerName(char *ip);

/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
char** COMMANDPRASER_readArguments(char* comandes, char* separator);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per comprovar que el text per enviar un missatge es correcte.
*
*   @Parameters:
*               (in) *argument4        =   Missatge a enviar.
*
*   @Return:    (out) return           =   0 : Format del text del missatge correcte
*                                          1 : No s'ha introduït el text entre dobles comilles
*                                          2 : Missatge buit
*
******************************************************************************************************************/
int COMMANDPARSER_checkSendMsg(char* argument4);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per comprovar que el format del file a enviar es correcte.
*
*   @Parameters:
*               (in) *argument4        =   File a enviar.
*
*   @Return:    (out) return           =   0 : Format del file correcte
*                                          1 : El file no té extensió
*
******************************************************************************************************************/
int COMMANDPARSER_checkSendFile(char* argument4);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per comprovar que l'usuari indicat existeix a la llista d'usuaris.
*
*   @Parameters:
*               (in) *users            =   Llista d'usuaris d'IluvatarSon.
*               (in) *user_name        =   Nom de l'usuari a comprovar.
*               (in) num_users         =   Quantitat d'usuaris existents a la llista de users.
*
*   @Return:    (out) return           =   i : Posició de l'usuari trobat en la llista.
*                                         -1 : L'usuari no s'ha trobat en la llista.
*
******************************************************************************************************************/
int COMMANDPARSER_checkUserExist(User *users, char *user_name, int num_users);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per parsejar i mostrar la informació de la trama d'un nou missatge.
*
*   @Parameters:
*               (in) *data             =   Cadena amb la informació del nou missatge
*               (in) length            =   Llargaria de la cadena data
*               (in) *users            =   Llista d'usuaris d'IluvatarSon.
*               (in) num_users         =   Quantitat d'usuaris existents a la llista de users.
*
*   @Return:    ---
*
******************************************************************************************************************/
void COMMANDPARSER_parseDataSendMsg(char *data, int length, User *users, int num_users);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per parsejar i mostrar la informació de la trama d'informació d'un nou file a rebre.
*
*   @Parameters:
*               (in/out) *fitxer       =   Estructura tipus Fitxer en la que s'enmagatzemaran les dades del file
*               (in) *data             =   Cadena amb la informació del nou file a rebre
*               (in) length            =   Llargaria de la cadena data
*
*   @Return:    ---
*
******************************************************************************************************************/
void COMMANDPRASER_parseDataSendFile(Fitxer* fitxer, char* data, int length);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per reconeixer i executar les comandes de Linux.
*
*   @Parameters:
*               (in) *command            =   Comanda a interpretar
*               (in) num_args            =   Número d'arguments de la comanda
*               (in) *configIluvatar     =   Estructura de tipus ConfigIluvatar del IluvatarSon que ho executa
*               (in) *users              =   Llista d'usuaris d'IluvatarSon.
*               (in) num_users           =   Quantitat d'usuaris existents a la llista de users.
*
*   @Return:    ---
*
******************************************************************************************************************/
void COMMANDPARSER_commandLinuxHandler(char* command, int num_args, ConfigIluvatar *configIluvatar, User *users, int num_users);

/****************************************************************************************************************
*
*   @Objectiu:  Mètode per mostrar per pantalla tots els usuaris de forma correcta.
*
*   @Parameters:
*               (in) *users              =   Llista d'usuaris d'IluvatarSon.
*               (in) num_users           =   Quantitat d'usuaris existents a la llista de users.
*
*   @Return:    ---
*
******************************************************************************************************************/
void COMMANDPARSER_listUsers(User *users, int num_users);


/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
void COMMANDPARSER_sendFile(int fd_client, char* fileToOpen);


/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
void COMMANDPARSER_readFile(int fd_client, TramaSocket* tramaSocketLocal, Fitxer* fitxer, ConfigIluvatar config, User *users, int num_users);


void COMMANDPARSER_sendFileQueue(int pid, char* fileToOpen, int total_file_size);


void COMMANDPARSER_readFileQueue(int id, int pid, Fitxer* fitxer, ConfigIluvatar configIluvatar, User *users, int num_users);


/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
void COMMANDPRASER_checkMD5 (int fd_client);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode que parseja la comanda i realiza les funcions específiques de les comandes acceptades
*
*   @Parameters:
*               (in) 
*                   command             =   Comanda introduida per linia de comandes per l'usuari
*                   *configIluvatar     =   Fitxer de configuració del proces IluvatarSon
*                   *users              =   Array d'usuaris guardats al procés
*                   num_users           =   Numero d'usuaris guardats al array anteriorment explicat
*
*   @Return:    ---
*
******************************************************************************************************************/
int COMMANDPARSER_parseCommand(char* command, ConfigIluvatar *configIluvatar, User *users, int num_users);

#endif //_COMMANDPARSER_H_
