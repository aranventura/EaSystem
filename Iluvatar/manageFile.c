#include "manageFile.h"

int MANAGEFILE_readConfigIluvatar(const char *file_name, ConfigIluvatar *configIluvatar) {

    int fd = open(file_name, O_RDONLY);

    if(fd <= 0) {
        printF(ERR_CONFIGFILE);
        return 0;
    } else {
        configIluvatar->user_name = ALL_readUntil(fd,'\n');
        configIluvatar->directory = ALL_readUntil(fd,'\n');
        configIluvatar->ip_arda = ALL_readUntil(fd,'\n');
        configIluvatar->port_arda = ALL_readUntil(fd,'\n');
        configIluvatar->ip_iluvatar = ALL_readUntil(fd,'\n');
        configIluvatar->port_iluvatar = ALL_readUntil(fd,'\n');
    }
    close(fd);
    return 1;
}

void MANAGEFILE_freeConfigIluvatar(ConfigIluvatar *configIluvatar) {
    free(configIluvatar->user_name);
    free(configIluvatar->directory);
    free(configIluvatar->ip_arda);
    free(configIluvatar->port_arda);
    free(configIluvatar->ip_iluvatar);
    free(configIluvatar->port_iluvatar);
}


    