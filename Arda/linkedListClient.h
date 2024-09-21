#ifndef _LINKEDLIST_CLIENT_H_
#define _LINKEDLIST_CLIENT_H_

#include "../all.h"

#define LIST_NO_ERROR 0
#define LIST_ERROR_FULL 1
#define LIST_ERROR_EMPTY 2
#define LIST_ERROR_MALLOC 3
#define LIST_ERROR_END 4


typedef struct {
    char *user_name;
    char *ip;
    char *port;
    char *pid;
    int fd_client;
    pthread_t thrd;
}ClientSocket;


typedef ClientSocket ElementClient;


typedef struct _NodeClient {
    ElementClient element;
    struct _NodeClient * next;
} NodeClient;


typedef struct {
    int error;			    
    NodeClient* head;	 	// Head/First elemento o nodo Phantom
    NodeClient* previous; 	// Previous node antes del POV
} LinkedListClient;

typedef struct {
    int error;			    
    NodeClient * head;	 	// Head/First elemento o nodo Phantom
    NodeClient * previous; 	// Previous node antes del POV
} SortedList;



/************************************************************************************
 *
 * @Objetivo: Crea una linkedlist vacía. Si la lista falla al crear el nodo phantom,
 *            situará el código de error LIST_ERROR_MALLOC.
 *
 *        +---+
 *   head | o-|---------
 *        +---+         |
 *   prev | o-|---------|--
 *        +---+         | |
 *                      v v          Point of View (After the last element)
 *                    +---+----+
 *                    |   |NULL|
 *                    +---+----+
 * 
 * @Parameters: ---
 *
 * @Return:    LinkedList vacía.
 *
************************************************************************************/
LinkedListClient LINKEDLISTClient_create ();


/************************************************************************************
 *
 * @Objetivo: Inserta el elemento especificado en la lista antes del elemento donde
 *            se encuentre actualmente el POV. Shifa el elemento apuntado por el POV
 *            a la derecha. Si la lista falla al crear el nuevo nodo a ser guardado,
 *            se situará el error LIST_ERROR_MALLOC.
 *
 *        +---+
 *   head | o-|---------
 *        +---+         |
 *   prev | o-|---------|-----------  Will point to 4.
 *        +---+         |           |
 *                      v           v          Point of View
 *                    +---+---+   +---+---+     +---+---+     +---+----+
 *                    |   | o-|-->| 1 | o-|--X->| 2 | o-|---->| 3 |NULL|
 *                    +---+---+   +---+---+ |   +---+---+     +---+----+
 *										    |         ^
 *										    |         |
 *											|   +---+-|-+
 *										     -->| 4 | o | New Node
 *											    +---+---+
 *
 * @Parameters: (in/out) list    = linkedlist donde añadir el elemento.
 *				(in)     element = elemento a ser añadido.
 *
 * @Return:     ---
 *
************************************************************************************/
void 	LINKEDLISTClient_add (LinkedListClient * list, ElementClient element);


/************************************************************************************
 *
 * @Objetivo: Eliminar el elemento apuntado actualmente pot el POV. Shifta los
 *            elementos a continuación a la izquierda. Esta operación fallará si el POV
 *            se encuentra después del último elemento válido de la lista. Esto también
 *            ocurrirá en caso de lista vacía. En esta situación, se marcará el error
 *            LIST_ERROR_END.
 *
 *        +---+
 *   head | o-|---------                        aux (free aux!)
 *        +---+         |                        |
 *   prev | o-|---------|-----------             |
 *        +---+         |           |            |
 *                      v           v          	 v  POV         NEW POV
 *                    +---+---+   +---+---+     +---+---+     +---+----+
 *                    |   | o-|-->| 1 | o-|--X->| 2 | o-|---->| 3 |NULL|
 *                    +---+---+   +---+---+ |   +---+---+     +---+----+
 *										    |                   ^
 *										    |                   |
 *										     -------------------
 *
 * 
 * @Parameters: (in/out) list = lista de la que eliminar el elemento.
 * 
 * @Return:     ---
 *
************************************************************************************/
void 	LINKEDLISTClient_remove (LinkedListClient * list);


/************************************************************************************
 *
 * @Objetivo: Retorna el elemento que se encuentra actualmente en el POV de la lista.
 *            Esta operación fallará si el POB se encuentra después del último elemento
 *            válido de la lista. Esto también ocurrirá para una lista vacía. En esta
 *            situación, esta operación verá el código de error: LIST_ERROR_END.
 * 
 * @Parameters: (in/out) list = lista de la que queremos obtener el elemento.
 * 
 * @Return:     ---
 *
************************************************************************************/
ElementClient LINKEDLISTClient_get (LinkedListClient * list);


/************************************************************************************
 *
 * @Objetivo: Retorna true (!0) si la lista contiene ningún elemento.
 * @Parameters: (in)    list = lista que queremos chequear.
 * @Return:    true (!0) si la lista contiene ningún elemento, false (0) al contrario.
 *
************************************************************************************/
int 	LINKEDLISTClient_isEmpty (LinkedListClient list);


/************************************************************************************
 *
 * @Objetivo: Mueve el POV al primer elemento de la lista-
 * @Parameters: (in/out) list = lista de la que mover el POV.
 * @Return:     ---
 *
************************************************************************************/
void 	LINKEDLISTClient_goToHead (LinkedListClient * list);


/************************************************************************************
 *
 * @Objetivo: Mueve el POV al siguiente elemento de la lista. Si el POV se encuentra
 *            después del último elemento de la lista (o cuando la lista está vacía), 
 *            esta función situará el error LIST_ERROR_END.
 * 
 * @Parameters: (in/out) list = lista de la que mover el POV.
 * 
 * @Return:     ---
 *
************************************************************************************/
void 	LINKEDLISTClient_next (LinkedListClient * list);


/************************************************************************************
 *
 * @Objetivo: Retorna true (!0) si el POV se encuentra después del último elemento 
 *            de la lista.
 * 
 * @Parameters: (in) list = Lista a chequear.
 * 
 * @Return:     true (!0) si el POV se encuentra después del último elemento de la lista.
 *
************************************************************************************/
int 	LINKEDLISTClient_isAtEnd (LinkedListClient list);


/****************************************************************************
 *
 * @Objective: Removes all the elements from the list and frees any dynamic
 *				memory block the list was using. The list must be created
 *				again before usage.
 * @Parameters: (in/out) list = the linked list to destroy.
 * @Return: ---
 *
 ****************************************************************************/
void 	LINKEDLISTClient_destroy (LinkedListClient * list);


/************************************************************************************
 *
 * @Objetivo: Esta función retorna el código de error de la última operación que
 *            se ha ejecutado.
 * 
 * @Parameters: (in) list = lista a chequear.
 * 
 * @Return:     El último código de error definido.
 *
************************************************************************************/
int		LINKEDLISTClient_getErrorCode (LinkedListClient list);


/************************************************************************************
 *
 * @Objetivo: Retorna la longitud de la lista.
 * 
 * @Parameters: (in) list = lista de la que queremos obtener su cantidad de elementos.
 * 
 * @Return:    count  = cantidad de elementos que se encuentran en la lista.
 *
************************************************************************************/
int LINKEDLISTClient_getLength (LinkedListClient list);


/************************************************************************************
 *
 * @Objetivo: Situa el POV de la linked list en la posición indicada.
 * 
 * @Parameters: (in/out) list     = lista de la que queremos modificar el POV.
 *              (in)     position = posición en la que queremos situar el POV.
 * 
 * @Return:     ---
 *
************************************************************************************/
int LINKEDLISTClient_setPOV (LinkedListClient *list, int position);


void LINKEDLISTClient_freeClientSocket(ClientSocket *miUsuario);


#endif //_LINKEDLIST_CLIENT_H_


