/**************************************************************************************
 *
 * @Propòsit:  Mòdul que s'encarrega de gestionar el fitxer de configuracio dels
 *             IluvatarSon.
 *
 * @Autor/s:   Núria Cabot  (nuria.cabot)
 *             Aran Ventura (aran.ventura)
 *
 **************************************************************************************/

#ifndef _MANAGEFILE_H_
#define _MANAGEFILE_H_

// Llibreria
#include "../all.h"

#define ERR_CONFIGFILE "Error opening the configuration file.\n"

// Estructura pròpia per enmagatzemar la informació del IluvatarSon
typedef struct{
    char *user_name;
    char *directory;
    char *ip_arda;
    char *port_arda;
    char *ip_iluvatar;
    char *port_iluvatar;
}ConfigIluvatar;


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per llegir l'arxiu de configuració indicat i enmagatzemar les dades d'aquest.
*
*   @Parameters:
*               (in) *file_name                =   Nom de l'arxiu de configuració a llegir
*               (in/out) *configIluvatar       =   Estructura de tipus ConfigIluvatar on s'enmagatzemen
*                                                  les dades que hem obtingut al llegir el fitxer indicat.
*
*   @Return:    (out) result                   =   0 : Hi ha hagut un error al obrir el fitxer indicat
*                                                  1 : El fitxer s'ha llegit correctament
*
******************************************************************************************************************/
int MANAGEFILE_readConfigIluvatar(const char *file_name, ConfigIluvatar *configIluvatar);


/****************************************************************************************************************
*
*   @Objectiu:  Mètode per alliberar la memòria del ConfigIluvatar indicat.
*
*   @Parameters:
*               (in/out) *configIluvatar       =   Estructura de tipus ConfigIluvatar de la que alliberar la
*                                                  memòria.
*
*   @Return:    ---
*
******************************************************************************************************************/
void MANAGEFILE_freeConfigIluvatar(ConfigIluvatar *configIluvatar);

#endif      //_MANAGEFILE_H_