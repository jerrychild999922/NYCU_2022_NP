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
#define MAXLINE 1024
using namespace std;

// fetch first element of line as command
string getcmd(string cmdline);
vector<string> getelements(string cmdline);

int main(int argc , char *argv[]){
	// acquire IP & port from terminal
	char* address = argv[1];
	int PORT = atoi(argv[2]);

	// UDP
	int sockfd_udp;
	char message_udp[1024];
	struct sockaddr_in servaddr_udp;

	int n_udp;
	socklen_t len_udp;
	if((sockfd_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("socket creation failed");
		exit(0);
	}

	memset(&servaddr_udp, 0, sizeof(servaddr_udp));
	servaddr_udp.sin_family = AF_INET;
	servaddr_udp.sin_port = htons(PORT);
	servaddr_udp.sin_addr.s_addr = inet_addr(address);

	while(true){
        cout << "% ";
		string command;
		getline(cin, command);
		string cmd = getcmd(command);
		if(cmd == "get-file-list" || cmd == "exit"){
			char buffer_udp[MAXLINE];
			strncpy(message_udp, command.c_str(), sizeof(message_udp));
			sendto(sockfd_udp, (const char*)message_udp, strlen(message_udp), 0, (const struct sockaddr*)&servaddr_udp, sizeof(servaddr_udp));
			n_udp = recvfrom(sockfd_udp, (char*)buffer_udp, MAXLINE, 0, (struct sockaddr*)&servaddr_udp, &len_udp);
            string response = buffer_udp;
            if(response == "exit"){
                break;
            }
			puts(buffer_udp);
		}else if(cmd == "get-file"){
			strncpy(message_udp, command.c_str(), sizeof(message_udp));
			sendto(sockfd_udp, (const char*)message_udp, strlen(message_udp), 0, (const struct sockaddr*)&servaddr_udp, sizeof(servaddr_udp));
			vector<string> elements = getelements(command);
			for(int i = 1; i < elements.size(); i ++){
				char buffer_udp[MAXLINE];
				n_udp = recvfrom(sockfd_udp, (char*)buffer_udp, MAXLINE, 0, (struct sockaddr*)&servaddr_udp, &len_udp);
				string content = buffer_udp;
				string file = elements[i];
				ofstream MyFile(file);
				MyFile << content;
				MyFile.close();	
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
