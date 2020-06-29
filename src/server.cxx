 /*

SERVER-receives messages from clients

*/
 #include "local.h"
 #include <iostream>
 #include <cstdio>
 #include <ctype.h>
 #include <stdlib.h>
#include<signal.h>

 using namespace std;

char name[BUFSIZ][BUFSIZ];
char pid_numstr [BUFSIZ][BUFSIZ];
long int pid_num[BUFSIZ];
int size;
int mid;
int
 main(int argc, char *argv[ ]) {

//FUNCTION PROTOTYPES 
void process_msg(char *, int);
//READ NAME AND PID NUMBER FROM FILE
int read_dns();
//WHEN THE NEW CONNECTION COMES TO THE END OF THE FILE, THIS IS THE SAME-NAME LINK, THAT IS A LIVING PROCESS, THERE IS ALWAYS REVERSE THE FILE TO TAKE THE LATEST.
void reverse_domains(int);
//BETWEEN THE NAME AND PID REGISTER - TOKENI WAS USED BEFORE THE LINE IS RECEIVED AS NAME-PID, AND THE NAME IS RECORDED TO THE LIST, THEN THE LINE IS REVERSE (PID-NAME) PID IS RECORDED TO THE SEQUENCE.
char *strev(char *);
//THE NAME IN THE MESSAGE FROM THE NAME SERVER IS ALSO SEARCHED AND TURNS INDEX NUMBER
char search_name(char *,int);
void signal_handler(int);



int  n;
int key;
MESSAGE msg;
key = ftok(".",SEED);

//MSGGET CALLING
if((mid=msgget(key,0))==-1)
	mid=msgget(key,IPC_CREAT | 0660);

//STARTING MOVIE
while (1) {
//SIGNAL
signal(SIGINT,signal_handler);

memset( msg.buffer, 0x0, BUFSIZ );
//MSG RECV
cout<<"****LISTENING CLIENTS****\n"<<endl;
if ((n=msgrcv(mid, &msg, BUFSIZ, SERVER, 0)) == -1 ) {
perror("Server: msgrcv");
return 2;
} else if (n == 0) break;
//END MSG RECV



size = read_dns();

reverse_domains(size);

int index=search_name(msg.receiver_name,size);


process_msg(msg.buffer,strlen(msg.buffer));

//MSG SEND
msg.msg_to = pid_num[index];
msg.msg_fm = SERVER;



n += sizeof(msg.msg_fm);
cout<<"****MSG GONE "<<"to "<<msg.receiver_name<<" FROM "<<msg.sender_name<<"****\n"<<index<<endl;
if(index!=-1)
{
	if (msgsnd(mid, &msg, BUFSIZ, 0) == -1 ) { 
	perror("Server: msgsnd");
	return 3;
	}
}
else{
	cout<<"***PLEASE CORRECT NAME***\n"<<endl;
}

//END MSG SEND



}
msgctl(mid,IPC_RMID,(struct msqid_ds *) 0);
exit(0);
}
void signal_handler(int sig){
	msgctl(mid,IPC_RMID,(struct msqid_ds *) 0);
	exit(0);
}


void
process_msg(char *b, int len){
for (int i = 0; i < len; ++i)
if (isalpha(*(b + i)))
*(b + i) = toupper(*(b + i));
}

char *strev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

int read_dns(){

	FILE *dnsfile;
	dnsfile = fopen("/tmp/nameserver","r");
	char line[BUFSIZ];
	char temp[BUFSIZ];
	int i=-1;
	char *str1;
	char *str2;
	char *str3;
	char *pidrev;

	while(!feof(dnsfile)){
		i++;	
		fscanf(dnsfile,"%s",&line);
		for(int j=0;j<strlen(line);j++)
			temp[j] = line[j];

		str1 = strtok(line,"-");
		strcpy(name[i],str1);
		str2 = strev(temp);
		
		str3 = strtok(temp,"-");
		pidrev = strev(str3);
		strcpy(pid_numstr[i],pidrev);
		
		
	str1=NULL;
	str2=NULL;
	str3=NULL;
	pidrev = NULL;
	}

	for(int a =0;a<i;a++)
		pid_num[a] = atoi(pid_numstr[a]);

	fclose(dnsfile);
	return i;
}
void reverse_domains(int s){
	char temp[BUFSIZ];
	long int temp1;
	for(int i = 0;i<s/2;i++){
		//FOR PIDS
		temp1=pid_num[i];
		pid_num[i] = pid_num[s-i-1];
		pid_num[s-i-1] = temp1;
		//FOR NAMES
		strcpy(temp,name[i]);
		strcpy(name[i],name[s-i-1]);
		strcpy(name[s-i-1],temp);

	}

}
int search_name(char *kime,int s){

	int i;
	int c;
	char *name1;


	for(i=0;i<s;++i){
		if((c=strcmp(name[i],kime))==0){
		
			return i;
		}
	}
	return -1;
}
