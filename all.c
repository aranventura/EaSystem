
#include "all.h"

char *ALL_mystrdup(const char *str) {
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    memcpy(result, str, len + 1);
    return result;
}


char* ALL_readUntil(int fd, char delimiter) {
    int i = 0, size;
    char buff = '\0';
    char* string = (char*)malloc(sizeof(char));

    while (1) {
        size = read(fd, &buff, sizeof(char));
        if (buff != delimiter && size > 0) {
            if (buff != '&'){
                string[i] = buff;
                string = (char*)realloc(string, sizeof(char) * (++i + 1));
            }
        } else {
            break;
        }
    }
    string[i] = '\0';
    return string;
}


char* ALL_readStringUntil(int begin, char* phrase, char delimiter){
    char buff = '\0';
    int i = 0;

    char* string = (char*)malloc(sizeof(char));

    while (1) {
        buff = phrase[begin];
        if (buff != delimiter) {
            string[i] = buff;
            string = (char*)realloc(string, sizeof(char) * (++i + 1));
            begin++;
        } else {
            break;
        }
    }
    string[i] = '\0';
    return string;
}


int ALL_getSizeFitxer(char* fileName) {
    int fd;
    int b_read = 0;
    int nBytes = 0;
    char aux;

    fd = open(fileName, O_RDONLY);
    
    if(fd <= 0) {
        printF("Error al obrir el fitxer");
    } else {
        do {
            b_read =  read(fd, &aux, 1);
                
            if(b_read > 0) {  //Si hi ha bytes per llegir
                nBytes++;
            }
        } while(b_read > 0);
    }

    close(fd);

    return nBytes;
}


char* ALL_getMD5(char* fileToOpen) {
    int pipeFd[2];
    pid_t pid;
    char* cmd[3];
    char aux[32];  // Auxiliar per guardar la info del MD5
    //char* MD5;
    int i = 0;

    //Guardem la informacio necessaria per executar la comanda md5
    cmd[0] = ALL_mystrdup("/bin/md5sum");
    cmd[1] = ALL_mystrdup(fileToOpen);
    cmd[2] = NULL;
    //aux = (char*)malloc(sizeof(char)*32);
    //aux = (char*) malloc(sizeof(char) * 32);
    
    if(pipe(pipeFd) == -1) {
        printF("Error a l'hora de la creacio del pipe");
    }
    pid = fork();
    switch(pid) {
        case -1:    // El fork no s'ha creat be
            printF("Error en la creacio del fork");
            break;

        case 0:     // Guardem el que es printa per pantalla al executar el MD5Sum
            dup2(pipeFd[1], STDOUT_FILENO);
            close(pipeFd[0]);
            close(pipeFd[1]);
            execv(cmd[0], cmd);

            exit(0);
            break;
        
        default:    // Obtenim la informació (només els 32 primers bytes, que és el que es la cadena del MD5Sum )
            close(pipeFd[1]);

            while(i < 32) {
                read(pipeFd[0], &(aux[i]), sizeof(char));
                i++;
            }

            aux[i] = '\0';
            wait(NULL);
            close(pipeFd[0]);
            break;
    }

    //MD5 = ALL_mystrdup(aux);
    free(cmd[0]);
    free(cmd[1]);
    //free(aux);

    return ALL_mystrdup(aux);
}


int ALL_countSpaces(char* command){
    int count = 0;
    for(int i = 0; i < (int)strlen(command); i++){
        if (command[i] == ' '){
            count++;
            while(command[i+1] == ' '){
                i++;
            }
        } else if (command[i] == '"'){
            return count;
        }
    }

    return count;
}


int ALL_countXSpaces(char* command, int begin){
    int i = 0;
    while(command[begin] == ' '){
        i++;
        begin++;
    }
    return i;
}
