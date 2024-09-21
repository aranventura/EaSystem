#include "linkedListClient.h"


LinkedListClient LINKEDLISTClient_create () {
    LinkedListClient list;

    // Request a Node. This node will be the auxiliary "Phantom" node.
    // The list's head now is the phantom node.
    list.head = (NodeClient *) malloc(sizeof(NodeClient));
    if (NULL != list.head) {
        // There is noone after the phantom node, so next is NULL.
        list.head->next = NULL;
        // We set the previous pointer to the phantom node. Now the point
        //  of view is after the last valid element in the list (there are
        //  no valid elements as the list is empty).
        list.previous = list.head;

        // Everything was fine, so we set the error code to NO_ERROR
        list.error = LIST_NO_ERROR;
    }
    else {
        // Could not get dynamic memory for the phantom node, so we set the
        //  error to malloc error code.
        list.error = LIST_ERROR_MALLOC;
    }

    return list;
}

void LINKEDLISTClient_add (LinkedListClient * list, ElementClient element) {
    // 1- Create a new node to store the new element.
    NodeClient * new_node = (NodeClient *) malloc (sizeof(NodeClient));
    if (NULL != new_node) {
        // 2- Set the element field in the new node with the provided element.
        new_node->element = element;
        // 3- Set the next field in the new node.
        //    The next node will be the node in the point of view.
        new_node->next = list->previous->next;

        // 4- Link the new node to the list. The new node will go before the
        //    point of view, so
        list->previous->next = new_node;
        // 5- Move the previous pointer.
        list->previous = new_node;

        // As everything was fine, set the error code to NO_ERROR
        list->error = LIST_NO_ERROR;
    }
    else {
        // Could not get dynamic memory for the new node, so we set the
        //  error to malloc error code.
        list->error = LIST_ERROR_MALLOC;
    }
}

void 	LINKEDLISTClient_remove (LinkedListClient * list) {
    NodeClient * aux = NULL;
    // We cannot remove an element if the POV is not valid.
    // The POV will not be valid when the previous pointer points to the last
    //  node in the list (there is noone after PREVIOUS).
    if (LINKEDLISTClient_isAtEnd(*list)) {
        list->error = LIST_ERROR_END;
    }
    else {
        // We need to set an auxiliary pointer to point the element we want
        //  to remove (the POV).
        aux = list->previous->next;

        // "Remove" the POV. The element after the PREVIOUS node will be the
        //  element after the POV.
        list->previous->next = list->previous->next->next;

        // Free the POV. Remove the element.
        free(aux);

        // If there are no errors, set error code to NO_ERROR.
        list->error = LIST_NO_ERROR;
    }
}

ElementClient LINKEDLISTClient_get (LinkedListClient * list) {
    ElementClient element;

    // We cannot return an element if the POV is not valid.
    // The POV will not be valid when the previous pointer points to the last
    //  node in the list (there is noone after PREVIOUS).
    if (LINKEDLISTClient_isAtEnd(*list)) {
        list->error = LIST_ERROR_END;
    }
    else {
        // The element to return is the element stored in the POV.
        element = list->previous->next->element;

        // If there are no errors, set error code to NO_ERROR.
        list->error = LIST_NO_ERROR;
    }

    return element;
}

int 	LINKEDLISTClient_isEmpty (LinkedListClient list) {
    // The list will be empty if there are no nodes after the phantom node.
    return NULL == list.head->next;
}

void 	LINKEDLISTClient_goToHead (LinkedListClient * list) {
    // To move the POV to the first element in the list, we need to point
    //  whoever is before the first element. That is the phantom node.
    list->previous = list->head;
}

void 	LINKEDLISTClient_next (LinkedListClient * list) {
    // We cannot move to the next element if the POV is not valid.
    // The POV will not be valid when the previous pointer points to the last
    //  node in the list (there is noone after PREVIOUS).
    if (LINKEDLISTClient_isAtEnd(*list)) {
        list->error = LIST_ERROR_END;
    }
    else {
        // Move the POV to the next element.
        list->previous = list->previous->next;

        // If there are no errors, set error code to NO_ERROR.
        list->error = LIST_NO_ERROR;
    }
}

int 	LINKEDLISTClient_isAtEnd (LinkedListClient list) {
    // To check if the list is at the end (POV after the last element) we
    //  need to check if there is any Node after the previous pointer.
    return NULL == list.previous->next;
}

void 	LINKEDLISTClient_destroy (LinkedListClient * list) {
    NodeClient * aux;
    // While there are still NODEs in the list.
    while (NULL != list->head) {
        // Take the first node.
        aux = list->head;
        // Now the first node is the next node.
        list->head = list->head->next;
        // Free who was the first node;
        free(aux);
    }
    // Set the pointers to NULL (best practice).
    list->head = NULL;
    list->previous = NULL;
}

int		LINKEDLISTClient_getErrorCode (LinkedListClient list) {
    return list.error;
}

int LINKEDLISTClient_getLength (LinkedListClient list) {

    int count = 0;

    if (!LINKEDLISTClient_isEmpty(list)) {
        list.previous = list.head;

        while (!LINKEDLISTClient_isAtEnd(list)) {
            LINKEDLISTClient_next(&list);
            count++;
        }
    }
    return count;
}

int LINKEDLISTClient_setPOV (LinkedListClient *list, int position) {
    int i, length, error = 0;

    length = LINKEDLISTClient_getLength(*list);

    if (position > length) {
        error = 1;
        printf("Error. The list has fewer elements than %d.", position);
    }
    else {
        list->previous = list->head;

        for (i = 0; i < position - 1; i++) {
            LINKEDLISTClient_next(list);
        }
    }

    return error;
}

void LINKEDLISTClient_freeClientSocket(ClientSocket *miUsuario){
    free(miUsuario->user_name);
    free(miUsuario->ip);
    free(miUsuario->port);
    free(miUsuario->pid);
}