#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#define MAX 10
struct mesg_buffer{
	long mesg_type;
	char mesg_text[100];
}message;

int main()
{
	//Code for Sender
	key_t key;
	int msgid, ch;
	
	key = ftok("progfile", 65);
	
	msgid = msgget (key, 0666 | IPC_CREAT);
	message.mesg_type = 1;
	
	printf("Write data: ");
	fgets(message.mesg_text, MAX, stdin);
	
	msgsnd(msgid, &message, sizeof(message), 0);
	
	printf("Data send is: %s \n", message.mesg_text);
	
	//Code for Receiver
	char sdel[]="$";
  char sdata[200]="";
  int i=0,j=0;
	key_t key;
	int msgid;
	
	key = ftok("progfile", 65);
	
	msgid = msgget(key, 0666 | IPC_CREAT);
	
	msgrcv(msgid, &message, sizeof(message), 1, 0);
	
	printf("Data Received is: %s \n", message.mesg_text);
	
	printf("Character count: %ld\n", strlen(message.mesg_text));
	
	while(message.mesg_text[i]!='\0'){
         	if(message.mesg_text[i]=='D' && message.mesg_text[i+1]=='L' && message.mesg_text[i+2]=='E'){
                	strcat(sdata,"$");
                	i=i+3;
               
                	j=j+6;
                	continue;
               
            }
             
	sdata[j]=message.mesg_text[i];
         j++;           
         i++; 
	}
         
         
        strcat(sdel,sdata);
        strcat(sdel,"$");
        printf("Message after stuffing is: %s \n",sdel);
	
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
