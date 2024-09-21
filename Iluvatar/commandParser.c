
#define _GNU_SOURCE
#include "commandParser.h"

void COMMANDPARSER_freeFitxer(Fitxer* fitxer) {
    free(fitxer->user_name);
    free(fitxer->fileName);
    free(fitxer->fileSize);
    free(fitxer->MD5sum);
}


char *COMMANDPARSER_getServerName(char *ip){
    if (strcmp(ip, IP_MATAGALLS) == 0){
        return "matagalls";
    } else if(strcmp(ip, IP_PUIGPEDROS) == 0){
        return "puigpedros";
    } else if(strcmp(ip, IP_MONTSERRAT) == 0){
        return "montserrat";
    } else {
        return "Server unknown";
    }
}


char** COMMANDPRASER_readArguments(char* command, char* separator){

    char** res = NULL;

    char* p = strtok(command, separator);
    int n_spaces = 0;

    while(p){
        n_spaces++;
        res = realloc(res, sizeof(char*) * n_spaces);
        res[n_spaces - 1] = p;
        p = strtok (NULL, separator);
    }

    res = realloc(res, sizeof(char*) * (n_spaces + 1));
    res[n_spaces] = NULL;

    free(p);

    return res;
}

int COMMANDPARSER_checkSendMsg(char* argument4){
    int comillas = 0;
    for(int i = 0; i < (int)strlen(argument4); i++){
        if (argument4[i] == '"'){
            comillas++;
            if (i == 0 && argument4[i+1] == '"'){
                return 2;
            }   
        }
    }

    if (comillas != 2){
        return 1;
    }

    return 0;
}


int COMMANDPARSER_checkSendFile(char* argument4) {
    int extensio = 0;
    for(int i = 0; i < (int)strlen(argument4); i++) { //comprobar que hay punto y extension
        if(argument4[i] == '.' && argument4[i+1] != '\0') {
            extensio = 1;
        }
    }
    if(!extensio) {
        return 1;
    }

    return 0;
}


int COMMANDPARSER_checkUserExist(User *users, char *user_name, int num_users) {
    for(int i = 0; i < num_users; i++){
        if(strcmp(users[i].user_name, user_name) == 0){
            return i;
        }
    }
    return -1;
}   


void COMMANDPARSER_parseDataSendMsg(char *data, int length, User *users, int num_users){
    char *buff;

    char *user_name = (char*)malloc(sizeof(char));
    char *msg = (char*)malloc(sizeof(char));
    int aux = 0;
    int count = 0;

    printF("New message received!\n");

    for(int i = 0; i < length; i++){
        if(data[i] != '&'){
            if(count == 0){
                user_name[aux] = data[i];
                user_name = (char*)realloc(user_name, sizeof(char) * (++aux + 1));
            } else {
                msg[aux] = data[i];
                msg = (char*)realloc(msg, sizeof(char) * (++aux + 1));
            }
        } else {
            user_name[aux] = '\0';
            aux = 0;
            count++;
        }
    }
    msg[aux] = '\0';

    count = COMMANDPARSER_checkUserExist(users, user_name, num_users);

    if(count == -1){
        asprintf(&buff, "%s, from x.x.x.x says:\n%s", user_name, msg);
    } else {
        //  Take ip user from the user list
        asprintf(&buff, "%s, from %s says:\n%s", user_name, users[count].ip, msg);
    }

    printF(buff);

    printF("\n$ ");

    free(buff);
    free(user_name);
    free(msg);
}


void COMMANDPRASER_parseDataSendFile(Fitxer* fitxer, char* data, int length) {
    fitxer->user_name = (char*)malloc(sizeof(char));
    fitxer->fileName = (char*)malloc(sizeof(char));
    fitxer->fileSize = (char*)malloc(sizeof(char));
    fitxer->MD5sum = (char*)malloc(sizeof(char));
    int count = 0;
    int aux = 0;
    int i = 0;

    // Parsegem la info 
    for(i = 0; i < length; i++){
        
        if(data[i] != '&'){
            if(count == 0){
                fitxer->user_name[aux] = data[i];
                fitxer->user_name = (char*)realloc(fitxer->user_name, sizeof(char) * (++aux + 1));
            } else if(count == 1) {
                fitxer->fileName[aux] = data[i];
                fitxer->fileName = (char*)realloc(fitxer->fileName, sizeof(char) * (++aux + 1));
            } else if (count == 2) {
                fitxer->fileSize[aux] = data[i];
                fitxer->fileSize = (char*)realloc(fitxer->fileSize, sizeof(char) * (++aux + 1));
            } else if (count == 3) {
                fitxer->MD5sum[aux] = data[i];
                fitxer->MD5sum = (char*)realloc(fitxer->MD5sum, sizeof(char) * (++aux + 1));
            }
        } else {
            if(count == 0){
               fitxer->user_name[aux] = '\0';
            } else if(count == 1) {
                fitxer->fileName[aux] = '\0';
            } else if (count == 2) {
                fitxer->fileSize[aux] = '\0';
            } else if (count == 3) {
                fitxer->MD5sum[aux] = '\0';
            }
            aux = 0;
            count++;
        }
    }
}

void COMMANDPARSER_commandLinuxHandler(char* command, int num_args, ConfigIluvatar *configIluvatar, User *users, int num_users){
    
    int status_code = 0;
    char** argument_list;
    int f = fork();

    switch (f){
        case -1:
            printF("Error while creating the fork!\n");
            break;

        case 0: // child
            if (num_args == 0){
                argument_list = NULL;
                status_code = execvp(command, argument_list);
            } else {
                argument_list = COMMANDPRASER_readArguments(command, " ");
                status_code = execvp(argument_list[0], argument_list);
                free(argument_list);
            }
            if (status_code == -1){
                printF(MSG_UNKNOWN_COMMAND);
                MANAGEFILE_freeConfigIluvatar(configIluvatar);
                SOCKETS_freeUsersConnected(users, num_users);
                free(command);
                exit(127);
            }
            break;
        
        default: // dad
            wait(NULL);
            break;
    }
}


void COMMANDPARSER_listUsers(User *users, int num_users){
    char *buff;

    asprintf(&buff, MSG_LIST_USERS, num_users);
    printF(buff);
    free(buff);

    for(int i = 0; i < num_users; i++){
        asprintf(&buff, "%d.  %s   %s   %s   %s   %s\n", i+1, users[i].user_name, users[i].ip, users[i].port, COMMANDPARSER_getServerName(users[i].ip), users[i].pid);
        printF(buff);
        free(buff);
    }
    printF("\n");
}


void COMMANDPARSER_sendFile(int fd_client, char* fileToOpen) {       
    int bytes = 0;
    int bytesAEnviar = 512 - 11 - sizeof(int) - 1; 
    char* leido;     
    int fdFile = open(fileToOpen, O_RDONLY);

    do {
        leido = (char*) malloc (bytesAEnviar);
        bytes = read(fdFile, leido, bytesAEnviar);
        
        if(bytes > 0) {
            SOCKETS_sendTramaFitxer(fd_client, '4', HEADER_DATA_FILE, bytes, leido);
            usleep(300);
        }

        free(leido);  

    } while(bytes > 0); 

    //  close fd_client and free trama (ack)
    close(fdFile);
}


void COMMANDPARSER_readFile(int fd_client, TramaSocket* tramaSocketLocal, Fitxer* fitxer, ConfigIluvatar config, User *users, int num_users) {
    char* buff;
    char* fileCreated;
    int infoLeida = 0;
    int count = 0;
    int totalFileSize = atoi(fitxer->fileSize);

    asprintf(&fileCreated, "%s/%s", config.directory,fitxer->fileName);
    SOCKETS_freeTrama(tramaSocketLocal);
    
    
    int fp = open(fileCreated, O_RDONLY | O_WRONLY | O_CREAT, 0666);
    do {
        SOCKETS_getTramaFitxer(tramaSocketLocal, fd_client);
        write(fp, tramaSocketLocal->data, tramaSocketLocal->length);
        
        infoLeida += tramaSocketLocal->length;

    } while(infoLeida < totalFileSize);

    
    char* newMD5 = ALL_getMD5(fileCreated);

    if(strcmp(fitxer->MD5sum, newMD5) == 0) {
        SOCKETS_sendTrama(fd_client, '5', HEADER_CHECK_OK, 0, "");

        printF("New file recieved!\n");
        count = COMMANDPARSER_checkUserExist(users, fitxer->user_name, num_users);

        if(count == -1){
            asprintf(&buff, "%s, from x.x.x.x has sent: %s", fitxer->user_name, fitxer->fileName);
        } else {
            //  Take ip user from the user list
            asprintf(&buff, "%s, from %s has sent: %s", fitxer->user_name, users[count].ip, fitxer->fileName);
        }

        printF(buff);

        printF("\n$ ");
        free(buff);
    } else {
         SOCKETS_sendTrama(fd_client, '5', HEADER_CHECK_KO, 0, "");
    }

    free(fileCreated);
    free(newMD5);
    COMMANDPARSER_freeFitxer(fitxer);
    close(fp); 
}

void COMMANDPARSER_sendFileQueue(int pid, char* fileToOpen, int total_file_size) {
    int enviat = 0;
    int bytes = 0;
    char* leido;
    int fdFile = open(fileToOpen, O_RDONLY);

    do {
        leido = (char*) malloc (512);
        bytes = read(fdFile, leido, 512);

        QUEUEMSG_sendDataFile(leido, bytes, pid);
        enviat += bytes;
        free(leido);

    } while(enviat < total_file_size);
    
}

void COMMANDPARSER_readFileQueue(int id, int pid, Fitxer* fitxer, ConfigIluvatar configIluvatar, User *users, int num_users) {
    char* buff;
    char* fileCreated;
    int leido = 0;
    int leer = 0;
    int count = 0;
    DataMsg dataMsg;
    int totalFileSize = atoi(fitxer->fileSize);

    asprintf(&fileCreated, "%s/%s", configIluvatar.directory, fitxer->fileName);
    int fp = open(fileCreated, O_RDONLY | O_WRONLY | O_CREAT, 0666);
    memset(dataMsg.data, 0, 512);

    do {
        leer = totalFileSize - leido;
        if(leer > 512) {
            msgrcv(id, (struct msgbuf *)&dataMsg, sizeof(char)*512, 0, 0);
            leido += 512;
            write(fp, dataMsg.data, 512);
       
        } else {
            msgrcv(id, (struct msgbuf *)&dataMsg, sizeof(char)*leer, 0, 0);
            leido += leer;
            write(fp, dataMsg.data, leer);
        } 

    } while(leido < totalFileSize);    
    
    char* newMD5 = ALL_getMD5(fileCreated);
    
    if(strcmp(fitxer->MD5sum, newMD5) == 0) {

        printF("New file recieved!\n");
        count = COMMANDPARSER_checkUserExist(users, fitxer->user_name, num_users);

        if(count == -1){
            asprintf(&buff, "%s, from x.x.x.x has sent: %s", fitxer->user_name, fitxer->fileName);

        } else {
            //  Take ip user from the user list
            asprintf(&buff, "%s, from %s has sent: %s", fitxer->user_name, users[count].ip, fitxer->fileName);
        }

        QUEUEMSG_sendInfoMsg(pid, 0, ACK_FILE_OK);

        printF(buff);

        printF("\n$ ");
        free(buff);

    } else {
        QUEUEMSG_sendInfoMsg(pid, 0, ACK_FILE_KO);
    }

    free(fileCreated);
    free(newMD5);
    close(fp); 
}


void COMMANDPRASER_checkMD5 (int fd_client) {
    TramaSocket trama;

    SOCKETS_getTrama(&trama, fd_client);

    if(strcmp(trama.header, HEADER_CHECK_OK)) {
        printF("File correctly sent\n");
    } else if (strcmp(trama.header, HEADER_CHECK_OK)) {
        printF("File NOT correctly sent\n");
    }

    SOCKETS_freeTrama(&trama);

}


int COMMANDPARSER_parseCommand(char* command, ConfigIluvatar *configIluvatar, User *users, int num_users){
    int num_args = 0, num_spaces = 0, error = 0; //, child;
    //char** argument_list;
    char* argument1;
    char* argument2;
    char* argument3;
    char* argument4;
    char* buff;
    asprintf(&buff, "Nothing");

    num_args = ALL_countSpaces(command); // Obtenim el número d'arguments de la comanda que ens han passat

    if (num_args == 0){
        if (strcasecmp(command, COMMAND_EXIT) == 0){
            free(buff);
            return NUM_COMMAND_EXIT;
        }
    } else if (num_args == 1){
        argument1 = ALL_readStringUntil(0, command, ' ');
        num_spaces = ALL_countXSpaces(command, strlen(argument1));
        argument2 = ALL_readStringUntil(strlen(argument1) + num_spaces, command, '\0');
        free(buff);
        asprintf(&buff, "%s %s", argument1, argument2);
        free(argument1);
        free(argument2);
    } else if (num_args == 3){
        argument1 = ALL_readStringUntil(0, command, ' ');
        num_spaces = ALL_countXSpaces(command, strlen(argument1));
        argument2 = ALL_readStringUntil(strlen(argument1) + num_spaces, command, ' ');
        num_spaces += ALL_countXSpaces(command, strlen(argument1) + strlen(argument2) + num_spaces);
        argument3 = ALL_readStringUntil(strlen(argument1) + strlen(argument2) + num_spaces, command, ' ');
        num_spaces += ALL_countXSpaces(command, strlen(argument1) + strlen(argument2) + strlen(argument3) + num_spaces);
        argument4 = ALL_readStringUntil(strlen(argument1) + strlen(argument2) + strlen(argument3) + num_spaces, command, '\0');
        free(buff);
        asprintf(&buff, "%s %s", argument1, argument2);
        free(argument1);
        free(argument2);
    }
    //write(1, buff, strlen(buff));

    if (strcasecmp(buff, COMMAND_UPDATE_USERS) == 0 && num_args == 1){
        //write(1, "FUNCTION UPDATE USERS\n", strlen("FUNCTION UPDATE USERS\n"));
        free(buff);
        return NUM_COMMAND_UPDATE_USERS;

    } else if (strcasecmp(buff, COMMAND_LIST_USERS) == 0 && num_args == 1){
        //write(1, "FUNCTION LIST USERS\n", strlen("FUNCTION LIST USERS\n"));
        COMMANDPARSER_listUsers(users, num_users);
        free(buff);
        return NUM_COMMAND_LIST_USERS;

    } else if (strcasecmp(buff, COMMAND_SEND_FILE) == 0 && num_args == 3){
        free(buff);

        error = COMMANDPARSER_checkSendFile(argument4);
        if (error == 1){
            printF("Error on the extension of file. File has not been sent\n");
        } else{
            // Comprobem que el usuari estigui a la llista (argument3 = nom d'usuari)
            int pos = COMMANDPARSER_checkUserExist(users, argument3, num_users);
            
            if (pos > -1){
                // Comparem ip's, d'aquesta manera sabem quin metode hem d'utilitzar
                if(strcmp(users[pos].ip, configIluvatar->ip_iluvatar) == 0){
                     //  Fase 4
                     char* fileToOpen;
                    //Intentem obrir el fitxer per comprovar que existeix
                    asprintf(&fileToOpen, "%s/%s", configIluvatar->directory, argument4);
                    int fdFile = open(fileToOpen, O_RDONLY);

                    if(fdFile <= 0) {
                        printF("File does not exist\n");
                    } else {
                        
                        close(fdFile); // El tanquem per poder obrir-lo a getSizeFitxer un altre cop
                        // Obtenim la informacio pel missatge
                        int fileSize = ALL_getSizeFitxer(fileToOpen);
                        char* MD5SUM = ALL_getMD5(fileToOpen); 
                        char* fileInfo;
                        asprintf(&fileInfo,"%s&%s&%d&%s", configIluvatar->user_name, argument4, fileSize, MD5SUM);

                        if (QUEUEMSG_sendInfoMsg(atoi(users[pos].pid), strlen(fileInfo), FILE_TYPE)) { 
                            //Creem el missatge amb la informacio del file i enviem
                            QUEUEMSG_sendDataMsg(fileInfo, atoi(users[pos].pid));
                            //Enviem tot el file
                            COMMANDPARSER_sendFileQueue(atoi(users[pos].pid), fileToOpen, fileSize);

                        } else {
                            printF("ERROR: Couldn't send msg, user is disconnected\n");
                        }
                        free(MD5SUM);
                        free(fileInfo);
                    } 
                    free(fileToOpen); 

                } else {
                    int fd_client = SOCKETSILUVATAR_connect_to_server(users[pos].ip, users[pos].port);
                    
                    if (fd_client > 0){     // Comprovem que el fd sigui correcte, sino vol dir que el usuari no esta connectat

                        char* fileToOpen;
                        //Intentem obrir el fitxer per comprovar que existeix
                        asprintf(&fileToOpen, "%s/%s", configIluvatar->directory, argument4);
                        int fdFile = open(fileToOpen, O_RDONLY);

                        if(fdFile <= 0) {
                            printF("File does not exist\n");
                        } else {
                        
                            close(fdFile); // El tanquem per poder obrir-lo a getSizeFitxer un altre cop

                            // Obtenim la informacio per la trama
                            int fileSize = ALL_getSizeFitxer(fileToOpen);
                            char* MD5SUM = ALL_getMD5(fileToOpen);
                            char* aux;
                            asprintf(&aux, "%d", fileSize);

                            //Creem la trama i la enviem
                            asprintf(&buff, "%s&%s&%s&%s", configIluvatar->user_name, argument4, aux, MD5SUM);
                            SOCKETS_sendTrama(fd_client, '4', HEADER_NEW_FILE, strlen(buff), buff);
                            
                            // Enviem tota la informació del fitxer
                            COMMANDPARSER_sendFile(fd_client, fileToOpen);

                            // Obtenim ACK
                            COMMANDPRASER_checkMD5(fd_client);

                            free(aux);
                            free(buff);
                            free(MD5SUM);
                            free(fileToOpen);
                        }  

                    } else {
                        printF("The user is disconnected\n");
                    }
                }
            } else {
                printF("The file can't be send beacause the user name does not exist\n");
            }
        }

        free(argument3);
        free(argument4);
        //free(buff);
        return NUM_COMMAND_SEND_FILE;

    } else if(strcasecmp(buff, COMMAND_SEND_MSG) == 0 && num_args == 3){ 

        error = COMMANDPARSER_checkSendMsg(argument4);
        if (error == 1){
            printF("Error on the type of message. Message has not been sent\n");
        } else if(error == 2){
            printF("Error, the message is empty. Message has not been sent\n");
        } else {
            free(buff);
            
            //  Comprobem que el usuari estigui a la llista
            //  Argument3 = el nom d'usuari a qui li envies la informacio
            int pos = COMMANDPARSER_checkUserExist(users, argument3, num_users);
            
            if (pos > -1){

                //  Comparem ip's, d'aquesta manera sabem quin metode em d'utilitzar
                if(strcmp(users[pos].ip, configIluvatar->ip_iluvatar) == 0){
                    
                    //  Fase 4
                    if (QUEUEMSG_sendInfoMsg(atoi(users[pos].pid), strlen(argument4), MSG_TYPE)){
                        QUEUEMSG_sendDataMsg(argument4, atoi(users[pos].pid));
                    } else {
                        printF("ERROR: Couldn't send msg, user is disconnected\n");
                    }
                } else {
                    //  Comprobem que el fd sigui correcte, sino vol dir que el usuari no esta connectat
                    int fd_client = SOCKETSILUVATAR_connect_to_server(users[pos].ip, users[pos].port);

                    if (fd_client > 0){
                        asprintf(&buff, "%s&%s", configIluvatar->user_name, argument4);
                        // printF(buff);
                        SOCKETS_sendTrama(fd_client, '3', HEADER_MSG, strlen(buff), buff);
                        free(buff);
                        int ack = SOCKETSILUVATAR_checkACK(fd_client);
                        if (ack){
                            printF("Message correctly sent\n\n");
                        } else {
                            printF("ERROR: Something went wrong, no ACK recived\n");
                        }
                        close(fd_client);
                    } else {
                        printF("ERROR: Couldn't send msg, user is disconnected\n");
                    }  
                } 
            } else {
                printF("The message can't be send beacause the user name does not exist\n");
            }
        }

        free(argument3);
        free(argument4);
        return NUM_COMMAND_SEND_MSG;

    } else {
        
        if (num_spaces == 3){
            free(argument3);
            free(argument4);
        }
        free(buff);
        COMMANDPARSER_commandLinuxHandler(command, num_spaces, configIluvatar, users, num_users);
        return NUM_COMMAND_LINUX;
    }
    return 0;
}
