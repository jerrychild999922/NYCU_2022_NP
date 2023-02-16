#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <sys/select.h>
#include <cctype>
#include <time.h>
#include <algorithm>



#define MAX 1024
#define PORT 8080
using namespace std;
bool isNumertic(string const &str){
	return !str.empty()&&str.find_first_of("0123456789")==std::string::npos;
	}
string game(string guess, string ans){
	string r;
	string gg=guess;
	string aa=ans;
	int a=0;
	int b=0;
	for(int i=0;i<4;i++){
		if(gg[i]==aa[i]){
			a++;
		}else{
			for(int j=0;j<4;j++){
				if(gg[i]==aa[j]){
					b++;
					break;
				}
			}
		}
	}
	r=to_string(a)+"A"+to_string(b)+"B";
	return r;
}

int main(int argc,char* argv[]) {
	int sockfd_udp;
	int sockfd_tcp;
	int sockff;
	socklen_t len;
    ssize_t n;
	int connfd;
	int connfd1;
	fd_set s;
	pid_t pid;
	char buffer[MAX];
	int client_socket[20];
	int master;
	int sd;
	int c3=0;
//	char* m="tcp";
//	char* m1="udp";
	struct sockaddr_in servaddr, cliaddr;
//	int log=0;
	int new_sd;
	int log[20];
	int start[20];
	int chance[20];
	
	vector <string> ans(20);
	vector <string> user(20);
	vector <string> name;
	vector <string> email;
	vector <string> pass;

	for(int i=0;i<20;i++){
		client_socket[i]=0;
		log[i]=0;
		start[i]=0;
		chance[i]=0;
	}

	// Creating socket file descriptor
	if ( (sockfd_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket udp creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	int port = atoi(argv[1]);
    // assign IP, PORT

	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);




		
	if ( bind(sockfd_udp, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	//===============================================================//

	sockfd_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_tcp == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}

	if ((bind(sockfd_tcp, (const struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}

	if ((listen(sockfd_tcp, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}

	/*
	if ( (sockff = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket udp creation failed");
		exit(EXIT_FAILURE);
	}

	if ( bind(sockff, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	len = sizeof(cliaddr);
    bzero(buffer, sizeof(buffer));
    //printf("\nMessage from UDP client: ");
    n = recvfrom(sockff, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, &len);
	puts(buffer);
	char* e1="*****Welcome to Game 1A2B*****" ;

    sendto(sockff, (const char*)e1, sizeof(buffer), 0,
            (struct sockaddr*)&cliaddr, sizeof(cliaddr));
	*/
	//=================================================================//
	
	/*
	FD_ZERO(&s);
	int socket;
	if (sockfd_udp>sockfd_tcp){
		socket=sockfd_udp;
	}
	else{
		socket=sockfd_tcp;
	}
	socket=socket+1;*/
	len=sizeof(cliaddr);
	int count;
	while(1){
		if(c3==0){
			printf("New connection.");
		}
		FD_ZERO(&s);
		int so;
		FD_SET(sockfd_udp,&s);
		FD_SET(sockfd_tcp,&s);
		master=sockfd_tcp;
		for(int i=0;i<20;i++){
			sd=client_socket[i];
			if(sd>0){
				FD_SET(sd,&s);
			}
			if(sd>master){
				master=sd;
			}
		}
		if (sockfd_udp>master){
		so=sockfd_udp;
		}
		else{
			so=master;
		}
		so=so+1;

		count= select(so,&s,NULL,NULL,NULL);
		if(FD_ISSET(sockfd_tcp,&s)){
			new_sd = accept(sockfd_tcp,(struct sockaddr*)&cliaddr,&len);
			for(int i=0;i<20;i++){
				if(client_socket[i]==0){
					client_socket[i]=new_sd;
					break;
				}
			}
		}


		for(int i=0;i<20;i++){
			connfd1=client_socket[i];
			if(FD_ISSET(connfd1,&s)){
				bzero(buffer, sizeof(buffer));
                printf("%dMessage From TCP client: ",start[i]);
                read(connfd1, buffer, sizeof(buffer));
                puts(buffer);
				string a(buffer);
				vector<string> y;
				int current = 0; 
				int next;
				string o;
				while (1){
					next = a.find_first_of(" ", current);

				if (next != current)
				{
					string tmp = a.substr(current, next - current);
					if (tmp.size() != 0) 
						y.push_back(tmp);
				}
				if (next == string::npos) break;
				current = next + 1; 
				}
				if (start[i]!=1){
					if(y[0]=="login"){
						if (y.size()==3){
							int error=0;
							int c;
							for (int j=0;j<name.size();j++){
								if(y[1]==name[j]){
									c=j;
									error=1;
									break;
								}
							}
							if(log[c]==1){
								o="Please logout first.";
							}
							else if(error==0){
								o="Username not found.";
							}
							else if(y[2]==pass[c]){
								o="Welcome, "+y[1]+".";
								log[c]=1;
								user[i]=y[1];
							}
							else{
								o="Password not correct.";
							}
						}
						else{
							o="Usage: login <username> <password>";
						}
					}
					else if(y[0]=="exit"){
						c3=0;
						int c;
						for (int j=0;j<name.size();j++){
							if(user[i]==name[j]){
								c=j;
								break;
							}
						}
						close(connfd1);
						user[i]="";
						client_socket[i]=0;
						log[c]=0;
						start[i]=0;
					}
					else if(y[0]=="logout"){
						if(user[i]!=""){
							int c;
							for (int j=0;j<name.size();j++){
								if(user[i]==name[j]){
									c=j;
									break;
								}
							}
							o="Bye, "+user[i]+".";
							user[i]="";
							log[c]=0;
							start[i]=0;
						}
						else{
							o="Please login first.";
						}
					}
					else if(y[0]=="start-game"){
						if (user[i]==""){
							o="Please login first.";
						}
						else if(y.size()==1){
							srand(time(NULL));
							string f;
							for (int i=0;i<4;i++){
								int x=rand()%10;
								string s=to_string(x);
								f=f+s;
							}
							ans[i]=f;
							start[i]=1;
							chance[i]=5;
							o="Please typing a 4-digit number.";

						}

						else if(y.size()==2){
							int cc=0;
							int ee=0;
							for(int j=0;j<y[1].length();j++){
								if(isdigit(y[1][j])){
									cc++;
								}
								else{
									ee=1;
									break;
								}
							}
							if(cc!=4){
								ee=1;
							}
							
						
							if(ee==1){
								o="Usage: start-game <4-digit number>";
							}
							else{
								ans[i]=y[1];
								start[i]=1;
								chance[i]=5;
								o="Please typing a 4-digit number.";
							}
							
						}
						else{
							o="Usage: start-game <4-digit number>";
						}
					}
					else {
						o="invalid input.";
					}
				}
				else{
					if(y.size()==1){
						int cc=0;
						int ee=0;
						for(int j=0;j<y[0].length();j++){
							if(isdigit(y[0][j])){
								cc++;
							}
							else{
								ee=1;
								break;
							}
						}
						if(cc!=4){
							ee=1;
						}
						if(ee==1){
							o="Your guess should be a 4-digit number.";
						}
						else{
							string s;
							s=game(y[0],ans[i]);
							chance[i]=chance[i]-1;
							if(s=="4A0B"){
								start[i]=0;
								o="You got the answer!";
							}
							else if(chance[i]==0){
								start[i]=0;
								o=s+"\nYou lose the game!";
							}
							else{
								o=s;
							}
						}
					}
					else{
						o="Your guess should be a 4-digit number.";
					}
				}














				char* e ;
    			e=new char [o.size()+1];
    			strcpy(e,o.c_str());
                write(connfd1, (const char*)e, sizeof(buffer));
			}
		}
		
        // if udp socket is readable receive the message.
        if (FD_ISSET(sockfd_udp, &s)) {
            len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            printf("\nMessage from UDP client: ");
            n = recvfrom(sockfd_udp, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, &len);
			puts(buffer);
			string a(buffer);
			vector<string> y;
			int current = 0; 
			int next;
			int i=0;
			string o;
			while (1){
				next = a.find_first_of(" ", current);

			if (next != current)
			{
				string tmp = a.substr(current, next - current);
				if (tmp.size() != 0) 
					y.push_back(tmp);
			}
			if (next == string::npos) break;
			current = next + 1; 
			i++;
			}
			if(y[0]=="register"){
				if (y.size()==4){
					int error=0;
					for(int i=0;i<name.size();i++){
						if (y[1]==name[i]){
							error=1;
							o="Username is already used.";
							break;
						}
						else if(y[2]==email[i]){
							error=2;
							o="Email is already used.";
							break;
						}
					}
					if(error==0){
						name.push_back(y[1]);
						email.push_back(y[2]);
						pass.push_back(y[3]);
						o="Register successfully.";
					}
				}
				else{
					o="Usage: register <username> <email> <password>";
				}
			}
			if(y.size()==1&&y[0]=="game-rule"){
				o="1. Each question is a 4-digit secret number.\n\
2. After each guess, you will get a hint with the following information:\n\
2.1 The number of \"A\", which are digits in the guess that are in the correct position.\n\
2.2 The number of \"B\", which are digits in the guess that are in the answer but are in the wrong position.\n\
The hint will be formatted as \"xAyB\".\n\
3. 5 chances for each question.";
			}
			char* e ;
    		e=new char [o.size()+1];
    		strcpy(e,o.c_str());
            sendto(sockfd_udp, (const char*)e, sizeof(buffer), 0,
                (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        }
		c3++;
    }
}