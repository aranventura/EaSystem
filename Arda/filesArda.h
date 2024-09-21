#ifndef _FILESARDA_H_
#define _FILESARDA_H_

#include "../all.h"

#define ERR_CONFIGFILE "Error opening the configuration file.\n"

typedef struct{
    char *ip_arda;
    char *port_arda;
    char *directory_arda;
} ConfigArda;


/****************************************************************************************************************
*
*   @Objectiu:  Mètode que llegeix i guarda el fitxer de configuració Arda
*
*   @Parameters:
*               (in) *file_name                          =   nom del fitxer   
*                     *configArda                        =   fitxer on volem guardar l'informació del fitxer             
*
*   @Return:    (out) correct                         =   0 : Error, el fitxer no existeix
*                                                         1 : Correcte
*
******************************************************************************************************************/
int FILESARDA_readConfigArda(const char *file_name, ConfigArda *configArda);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per alliberar memoria del tipus configArda
*
*   @Parameters:
*               (in) configArda                           =   struct que volem alliberar                                          
*
*   @Return:    
*
******************************************************************************************************************/
void FILESARDA_freeConfigArda(ConfigArda *configArda);

#endif      //_MANAGEFILE_H_