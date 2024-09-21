#include "filesArda.h"

int FILESARDA_readConfigArda(const char *file_name, ConfigArda *configArda){

    int fd = open(file_name, O_RDONLY);

    if(fd <= 0) {
        printF(ERR_CONFIGFILE);
        return 0;
    } else {
        configArda->ip_arda = ALL_readUntil(fd, '\n');
        configArda->port_arda = ALL_readUntil(fd, '\n');
        configArda->directory_arda = ALL_readUntil(fd, '\n');
    }
    close(fd);
    return 1;
}

void FILESARDA_freeConfigArda(ConfigArda *configArda){
    free(configArda->ip_arda);
    free(configArda->port_arda);
    free(configArda->directory_arda);
}
