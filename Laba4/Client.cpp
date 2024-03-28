#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	struct sockaddr_in peer;
	int s,t,t1;
	int rc;
	char buf[256],p,p1,b[256];;

	peer.sin_family=AF_INET;
	peer.sin_port=htons(7500);
	peer.sin_addr.s_addr=inet_addr("127.0.0.1");

	s=socket(AF_INET,SOCK_STREAM,0);
	rc=connect(s,(struct sockaddr *)&peer,sizeof(peer));
while(true){
	//Выбор пункта меню и отправка его серверу			
	puts("Choose:");
	puts("\t1 - Select");
	puts("\t2 - Edit");
	puts("\t3 - View");
	puts("\t4 - Exit");
	scanf("%s",buf);
	buf[1]='\0';
	send(s,buf,sizeof(buf),0);
	p=buf[0];

	switch (p){
	case '1': //Выбрать
		puts("kol-vo months (1-12) :");scanf("%s",buf);
		send(s,buf,sizeof(buf),0);
		puts("Symbol: ");scanf("%s",buf);
		send(s,buf,sizeof(buf),0);

		printf("Sum of taxes: ");
		recv(s,buf,sizeof(buf),0);
		for (t=0;buf[t+3];t++) printf("%c",buf[t]);
		printf(".");
		for (t1=t;buf[t1];t1++) printf("%c",buf[t1]);
		printf("\n");
		break;
	case '2': //Подредактировать
	
		puts("What number (1-5) to edit");
		scanf("%s",buf); //Какой номер будем редактировать			
		send(s,buf,sizeof(buf),0);
		puts("What field (1-4) to edit");
		puts("\t1 - Name");
		puts("\t2 - Number");
		puts("\t3 - Income");
		puts("\t4 - Tax");
		scanf("%s",buf);
		send(s,buf,sizeof(buf),0);
		p1=buf[0];
		buf[0]='\0';		
		switch(p1){		//Введите новые поля
			case '1': printf("Name: ");
			  fflush(stdin);fflush(stdout);
			  scanf("%s",b);strcat(buf,b);strcat(buf," ");
			  scanf("%s",b);strcat(buf,b);strcat(buf," ");				  
			  scanf("%s",b);strcat(buf,b);strcat(buf,"\0");
			  //puts(buf);
			  send(s,buf,sizeof(buf),0);
			  break;
			case '2':fflush(stdin);fflush(stdout);
			  printf("Tab Number: ");scanf("%s",buf);
			  send(s,buf,sizeof(buf),0);
			  break;
			case '3':fflush(stdin);fflush(stdout);
			  printf("Income per month: ");scanf("%s",buf);
			  send(s,buf,sizeof(buf),0);
			  break;
			case '4':
				fflush(stdin);
				fflush(stdout);
				printf("Tax rate (%%) per month: ");
				scanf("%s", buf);
				send(s, buf, sizeof(buf), 0);
				break;

  scanf("%s",buf);
 			  send(s,buf,sizeof(buf),0);
		}
		break;
	case '3'://Просмотреть 5 записей
		recv(s,buf,sizeof(buf),0);printf("%s",buf);
		recv(s,buf,sizeof(buf),0);printf("%s",buf);
		recv(s,buf,sizeof(buf),0);printf("%s",buf);
		recv(s,buf,sizeof(buf),0);printf("%s",buf);
		recv(s,buf,sizeof(buf),0);printf("%s",buf);
		break;	
	case '4'://Выход
		exit(0);
	}
}
}
