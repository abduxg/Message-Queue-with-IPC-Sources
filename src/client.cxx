 /*

CLIENT ... sends messages to the server

*/
#include "local.h"
#include <cstdio>
#include<iostream>
#include<sys/file.h>
#include<sys/types.h>
#include<signal.h>
using namespace std;
 int
 main(int argc,char *argv[]){
//WRITE CLIENT PID NUMBER on /tmp/nameserver
void write_dns(char *,int);
void signal_handler(int);

	if(argc!=2 || strlen(argv[1])>20){
		cout<<"usage: ./client {name<20char}"<<endl;
		return 0;

	}

key_t key;

pid_t cli_pid;

int mid, n,m;
char *sender = argv[1];
MESSAGE msg;
char *command;

cli_pid = getpid( );


write_dns(argv[1],cli_pid);
if ((key = ftok(".", SEED)) == -1) {


perror("Client: key generation");

return 1;

}

mid=msgget(key, IPC_CREAT | 0660);

bool b = true;
int a;

if((a=fork())==0){
	b= false;
	while(1){
		if((n = msgrcv(mid,&msg,BUFSIZ,cli_pid,0))!=-1)
		{
			cout<<"\nMessage From "<<msg.sender_name<<":"<<msg.buffer<<endl;
			write(fileno(stdout),"Receiver>",10);
		}
	}
}

while (1&&b==true) {

signal(SIGINT,signal_handler);
//MESSAGE TO

msg.msg_to = SERVER;
//MESSAGE FROM
msg.msg_fm = cli_pid;
//SENDER NAME
strcpy(msg.sender_name,sender);
strcpy(msg.receiver_name,"");

	write(fileno(stdout),"Receiver>",10);
	scanf("%s",&msg.receiver_name);
	write(fileno(stdout), "msg>" , 4);

	memset(msg.buffer, 0x0, BUFSIZ);

	if ( (n=read(fileno(stdin), msg.buffer, BUFSIZ)) == 0 )

	break;

	n += sizeof(msg.msg_fm);
	//MSG SND
	if (msgsnd(mid, &msg, BUFSIZ, 0) == -1 ) {

	perror("Client: msgsend");
	cout<<"Make sure that the server is running"<<endl;

	return 4;

}
}
msgsnd(mid, &msg, 0, 0);
//kill(a,9);
return 0;
}

void signal_handler(int sig){

	exit(0);
}
void write_dns(char *name,int pid){

	FILE *dnsfile;

	dnsfile = fopen("/tmp/nameserver","a");

	fprintf(dnsfile,"%s-%d\n",name,pid);
	fclose(dnsfile);
}

