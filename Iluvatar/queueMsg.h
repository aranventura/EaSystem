/**************************************************************************************
 *
 * @Propòsit:  Mòdul que fa referència als mètodes necessaris per la correcta
 *             comunicació amb cues de missatges d'IluvatarSon.
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/


#ifndef _QUEUEMSG_H_
#define _QUEUEMSG_H_

#include "../all.h"
#include "../sockets.h"

#define MSG_TYPE 0
#define FILE_TYPE 1
#define ACK_MSG 2
#define ACK_FILE_OK 3
#define ACK_FILE_KO 4

typedef struct{
	long id_mailbox;
	int pid;
	int tipus;
	int data_length;
}InfoMsg;

typedef struct{
	long id_mailbox;
	char data[512];
}DataMsg;


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per a crear una nova cua de missatges a partir del pid
*
*   @Parameters:
*               (in) *id                           =   id de la cua                                         
*
*   @Return:    (out)                         =          0 : Error al crear la cua de missatges
*                                                        1 : Correcte inicialització de la cua
*
******************************************************************************************************************/
int QUEUEMSG_initQueue(int *id);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode rebre l'ack del client per saber si el misatge s'ha rebut correctament
*
*   @Parameters:
*               (in) *msg                           =	fd per on enviem la informació al client 
*					 pid						   =	pid per obtenir id de la cua de missatges
*
*   @Return:    
*
******************************************************************************************************************/
void QUEUEMSG_sendDataMsg(char *msg, int pid);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per enviar informació d'un fitxer a la cua de missatges
*
*   @Parameters:
*               (in) *msg                           =	fd per on enviem la informació al client 
*					 pid						   	=	pid per obtenir id de la cua de missatges
*					 bytes							=	numero de bytes que s'envien a la cua (512 max)
*
*   @Return:    
*
******************************************************************************************************************/
void QUEUEMSG_sendDataFile(char *msg, int bytes, int pid);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per enviar informació d'un fitxer a la cua de missatges
*
*   @Parameters:
*               (in) pid						   	=	pid per obtenir id de la cua de missatges
*					 msg_length					    =	length del misatge que hem d'enviar per poder llegir exactament la llargada corresponent
*					 type							= 	tipus de missatge

*   @Return:    (out) 								=	 0 : La cua no existeix
*                                                        1 : Hem enviat el missatge a la cua			
*
******************************************************************************************************************/
int QUEUEMSG_sendInfoMsg(int pid, int msg_length, int type);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode saber l'usuari que ens ha enviat el missatge
*
*   @Parameters:
*               (in) *users						   	=	Array de usuarios guardados en el proceso Iluvatar
*					 pid					    	=	pid per obtenir id de la cua de missatges
*					 num_users						= 	numero de usuarios guardados en el array
*
*   @Return:    (out) pos							=	La posicio del array en la que es troba l'usuari
*														-1 si no existeix
*
******************************************************************************************************************/
int QUEUEMSG_searchUserByPid(User *users, int pid, int num_users);

#endif //_QUEUEMSG_H_