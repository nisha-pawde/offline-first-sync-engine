
 #ifndef IPC_H
 #define IPC_H
 #define MSG_DATA 1
 #define MSG_ACK  2

 void create_fifo();
 void write_fifo(const char *msg);
 void read_fifo(char *buffer);

 #endif
