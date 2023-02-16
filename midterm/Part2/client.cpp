#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;
#define MAXLINE 1024

// fetch first element of line as command
string getcmd(string cmdline);
vector<string> getelements(string cmdline);

int main(int argc , char *argv[]){
	// acquire IP & port from terminal
	char* address = argv[1];
	int PORT = atoi(argv[2]);

	//TCP
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr;

	int n; 
	socklen_t len;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket creation failed");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr(address);
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("\n Error : Connect Failed \n");
	}
    read(sockfd, buffer, sizeof(buffer));
    puts(buffer);

	while(true){
		cout << "% ";
		string command;
		getline(cin, command);
		string cmd = getcmd(command);
		if(cmd == "exit" || cmd == "get-ip" || cmd == "list-users"){
			memset(buffer, 0, sizeof(buffer));
			strncpy(buffer, command.c_str(), sizeof(buffer));
			write(sockfd, buffer, sizeof(buffer));
			read(sockfd, buffer, sizeof(buffer));
            string tmp = buffer;
			vector<string> response = getelements(tmp);
			puts(buffer);
            if(response[0] == "Bye,"){
				close(sockfd);
				break;
			}
		}
	}
}


string getcmd(string command){
	string cmd;
	int cnt = 0;
	int pre = 0;
	string a[100];
	for(int i=0 ; i<command.length() ; i++){
		if(command[i] == ' '){
			a[cnt] = command.substr(pre,i-pre);
			pre = i+1;
			cnt ++;
		}
	}
	a[cnt] = command.substr(pre,command.length());
	return a[0];
}

vector<string> getelements(string command){
	string cmd;
	int cnt = 0;
	int pre = 0;
	vector<string> a;
	for(int i=0 ; i<command.length() ; i++){
		if(command[i] == ' '){
			a.push_back(command.substr(pre,i-pre));
			pre = i+1;
			cnt ++;
		}
	}
	a.push_back(command.substr(pre,command.length()));
	return a;
}