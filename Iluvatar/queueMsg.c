

#define _GNU_SOURCE
#include "queueMsg.h"


int QUEUEMSG_initQueue(int *id){
    char *aux;
	
	*id = msgget((key_t)getpid(), 0600 | IPC_CREAT);

	if( *id < 0 )
    {
		asprintf(&aux, "Error creating queue\n");
		write(1, aux, strlen(aux));
        free(aux);
		return 0;
	}

    return 1;
}

int QUEUEMSG_sendInfoMsg(int pid, int msg_length, int type){
    int id_msg_rcv_local;
    char *aux;
    InfoMsg infoMsg;
    
    id_msg_rcv_local = msgget((key_t)pid, 0600 | IPC_EXCL);

    if( id_msg_rcv_local < 0 )
    {
        //if( id_msg_rcv_local >= 0 ) msgctl ( id_msg_rcv_local, IPC_RMID, (struct msqid_ds *)NULL );
        asprintf(&aux, "Error creating queue, we couldn't send your msg\n");
        write(1, aux, strlen(aux));
        free(aux);
        return 0;
    }
    
    infoMsg.id_mailbox = getpid();
    infoMsg.data_length = msg_length;
    infoMsg.pid = pid;
    infoMsg.tipus = type;
    
    msgsnd(id_msg_rcv_local, (struct msgbuf *)&infoMsg, sizeof(int)*3, IPC_NOWAIT);

    return 1;
}

// void QUEUEMSG_sendInfoFileMsg(int pid, char *filename, int length, char* md5){
//     int id_msg_rcv_local;
//     char *aux;
//     char* msg;
//     InfoFileMsg fileInfoMsg;
    
//     id_msg_rcv_local = msgget((key_t)pid, 0600 | IPC_EXCL);

//     if( id_msg_rcv_local < 0 ) {
//         //if( id_msg_rcv_local >= 0 ) msgctl ( id_msg_rcv_local, IPC_RMID, (struct msqid_ds *)NULL );
//         asprintf(&aux, "Error creating queue, we couldn't send your msg\n");
//         write(1, aux, strlen(aux));
//         free(aux);
//     }

//     fileInfoMsg.id_mailbox = pid;
//     fileInfoMsg.total_file_size = length;
//     memcpy(fileInfoMsg.file_name, filename, strlen(filename));
//     memcpy(fileInfoMsg.md5sum, md5, strlen(md5));
    

//     int msg_size = sizeof(char)*strlen(filename) + strlen(md5) + sizeof(int);
//     asprintf(&aux, "\nmsg_size: %d, file: %s, filesize: %d, md5sum: %s\n", msg_size, fileInfoMsg.file_name, fileInfoMsg.total_file_size, fileInfoMsg.md5sum);   
//     printF(aux);
//     free(aux);
//     msgsnd(id_msg_rcv_local, (struct msgbuf *)&fileInfoMsg, msg_size, IPC_NOWAIT);
// }

void QUEUEMSG_sendDataMsg(char *msg, int pid){
    int id_msg_rcv_local;
    char *aux;
    DataMsg dataMsg;
    
    id_msg_rcv_local = msgget((key_t)pid, 0600 | IPC_EXCL);

    if( id_msg_rcv_local < 0 )
    {
        //if( id_msg_rcv_local >= 0 ) msgctl ( id_msg_rcv_local, IPC_RMID, (struct msqid_ds *)NULL );
        asprintf(&aux, "Error creating queue, we couldn't send your msg\n");
        write(1, aux, strlen(aux));
        free(aux);
    }

    dataMsg.id_mailbox = pid;
    memcpy(dataMsg.data, msg, strlen(msg));

    msgsnd(id_msg_rcv_local, (struct msgbuf *)&dataMsg, sizeof(char)*strlen(msg), IPC_NOWAIT);
}

void QUEUEMSG_sendDataFile(char *msg, int bytes, int pid){
    int id_msg_rcv_local;
    char *aux;
    DataMsg dataMsg;
    
    id_msg_rcv_local = msgget((key_t)pid, 0600 | IPC_EXCL);

    if( id_msg_rcv_local < 0 )
    {
        //if( id_msg_rcv_local >= 0 ) msgctl ( id_msg_rcv_local, IPC_RMID, (struct msqid_ds *)NULL );
        asprintf(&aux, "Error creating queue, we couldn't send your msg\n");
        write(1, aux, strlen(aux));
        free(aux);
    }

    dataMsg.id_mailbox = pid;
    for(int i = 0; i < bytes ; i++) {
        dataMsg.data[i] = msg[i];
    }
    //memcpy(dataMsg.data, msg, strlen(msg));

    msgsnd(id_msg_rcv_local, (struct msgbuf *)&dataMsg, sizeof(char)*bytes, IPC_NOWAIT);
}

int QUEUEMSG_searchUserByPid(User *users, int pid, int num_users){
    for(int i = 0; i < num_users; i++){
        if(atoi(users[i].pid) == pid){
            return i;
        }
    }
    return -1;
}
