/**************************************************************************************
 *
 * @Propòsit:  Mòdul que conté totes les funciones utilitzades durant el programa que
 *             tenen un caire més general. En general tracten sobre la lectura, parseig
 *             de paraules/frases, fer copies, obtenir informació concreta...
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#ifndef     _ALL_H
#define     _ALL_H

// Llibreries generals
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/msg.h>
#include <stdint.h>

// Constants pròpies
#define printF(X) write(1, X, strlen(X))
#define bzerO(b,len) (memset((b), '\0', (len)), (void) 0)


/****************************************************************************************************************
*
*   @Objectiu:  Mètode pròpi per realitzar la funció strdup, que el que fa és realitzar una copia de l'string
*               introduït.
*
*   @Parameters:
*               (in) *str       =   Cadena de la que realitzar la còpia
*
*   @Return:    (out) *result   =   Cadena copiada 
*
******************************************************************************************************************/
char* ALL_mystrdup(const char *str);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per obtenir l'string resultat de llegir del file descriptor indicat fins al 
*               caràcter delimitador indicat.
*
*   @Parameters:
*               (in) fd         =   Enter del File Descriptor del que llegir
*               (in) delimiter  =   Caràcter fins el que volem llegir
*
*   @Return:    (out) *string   =   String fins el caràcter indicat sense que aquest estigui inclòs 
*
******************************************************************************************************************/
char* ALL_readUntil(int fd, char delimiter);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per obtenir l'string resultat de llegir de la cadena indicada, des de la posició de begin
*               fins al delimiter.
*
*   @Parameters:
*               (in) begin          =   Enter que fa referencia a la posició inicial des de la que llegir
*               (in) *phrase        =   Cadena de la que es vol llegir per extreure l'string
*               (in) delimiter      =   Caràcter fins el que volem llegir
*
*   @Return:    (out) *string       =   String resultant 
*
******************************************************************************************************************/
char* ALL_readStringUntil(int begin, char* phrase, char delimiter);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per btenir la mida d'un fitxer. Obre el fitxer i compta byte a byte fins que no hi haguin 
*               més.
*
*   @Parameters:
*               (in) fileName   =   Nom de l'arxiu a obrir
*
*   @Return:    (out) nBytes    =   Quantitat de bytes que té l'arxiu 
*
******************************************************************************************************************/
int ALL_getSizeFitxer(char* fileName);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per obtenir el hash MD5 de l'arxiu situat en l'adreça de fileToOpen.
*
*   @Parameters:
*               (in) fileToOpen   =   Nom de l'arxiu a obtenir el hash MD5
*
*   @Return:    (out) MD5         =   MD5 de l'arxiu introduït
*
******************************************************************************************************************/
char* ALL_getMD5(char* fileToOpen);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per obtenir la quantitat de espais entre argument i argument del string indicat.
*               Només compta els salts entre arguments, es a dir, es igual si realment hi ha 1, 2 o 5 espais,
*               es comptará un "salt"/espai.
*
*   @Parameters:
*               (in) *command     =   Cadena de la que comptar els espais
*
*   @Return:    (out) count       =   Quantitat d'espais/"salts" a la cadena (= quantitat arguments - 1)
*
******************************************************************************************************************/
int ALL_countSpaces(char* command);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per obtenir la quantitat de espais que hi ha a la cadena indicada des de la posició
*               d'inici indicada fins que hi ha algun caràcter.
*
*   @Parameters:
*               (in) *command     =   Cadena de la que comptar els espais
*               (in) begin        =   Posició des de la que començar a mirar la cadena
*
*   @Return:    (out) i           =   Quantitat d'espais a la cadena fins trobar un caràcter
*
******************************************************************************************************************/
int ALL_countXSpaces(char* command, int begin);

#endif      //_ALL_H