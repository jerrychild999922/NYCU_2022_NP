#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <cstdlib>
#include <time.h>
#include <sstream> 
#include <random>
#include <ctype.h>
#include <string>
using namespace std;
#define MAXLINE 1024
 
string print_client(int IP,int port);
vector<string> getelements(string cmdline);

int main(int argc , char *argv[]){
	int PORT = atoi(argv[1]);
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char buffer[MAXLINE];
	fd_set rset;
	// socket container
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	char message[MAXLINE];
	vector<string> port(30);
    vector<string> ip(30);
    vector<int> number(30);
    vector<bool> online(30);

	// TCP --listen
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	// binding server address & listen socket 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	// maximun listen number
	listen(listenfd, 10);

	int max_sd;
	int max_clients = 30;
	int sd;
	int client_socket[30];
	int new_socket;
	len = sizeof(cliaddr);
    for(int i=0; i<max_clients; i++){  
        client_socket[i] = 0;
        online[i] = false;
    }
    int cnt = 1;

	while(true){
		FD_ZERO(&rset);
		FD_SET(listenfd, &rset);
		max_sd = listenfd;
		for(int i=0; i<max_clients; i++){  
            sd = client_socket[i];  
            if(sd > 0){
				FD_SET( sd , &rset);  
			} 
            if(sd > max_sd){
				max_sd = sd; 
			}   
        }

		select(max_sd + 1, &rset , NULL , NULL , NULL);

		if (FD_ISSET(listenfd, &rset)){  
            if ((new_socket = accept(listenfd, (struct sockaddr *)&cliaddr, (socklen_t*)&len))<0){  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            
            for (int i=0; i<max_clients; i++){  
                if(client_socket[i] == 0){
                    int addr = cliaddr.sin_addr.s_addr;
                    cout << "New connection from " << print_client(addr, cliaddr.sin_port) << " user" << cnt << "\n";
                    string info = "";
                    for(int i = 0; i < 4; i ++){
                        info += to_string(addr % 256);
                        addr = addr >> 8;
                        if(i < 3){
                            info += ".";
                        }
                    }
                    port[i] = to_string(cliaddr.sin_port);
                    ip[i] = info;
                    number[i] = cnt;
                    online[i] = true;
                    client_socket[i] = new_socket;
                    string greet = "Welcome, you are user";
                    greet += to_string(cnt);
                    strncpy(message, greet.c_str(), sizeof(message));
                    write(client_socket[i], (const char*)message, sizeof(buffer));
                    cnt ++;
                    break;
                }  
            }  
        }

		// TCP
		for(int i=0; i<max_clients; i ++){
            sd = client_socket[i];   
            if (FD_ISSET(sd ,&rset)){
				bzero(buffer, sizeof(buffer));
				read(sd, buffer, sizeof(buffer));
				string command = buffer;
				vector<string> elements;
                elements = getelements(command);
                if(elements[0] == "exit"){
                    cout << "user" << number[i] << " " << ip[i] << ":" << port[i] << " ";
                    cout << "disconnected" << "\n";
                    client_socket[i] = 0;
                    string end = "Bye, user";
                    end += to_string(number[i]) + ".";
                    number[i] = 0;
                    ip[i] = "";
                    port[i] = "";
                    online[i] = false;
                    strncpy(message, end.c_str(), sizeof(message));
                    write(sd, (const char*)message, sizeof(buffer));
                    close(sd);
                }else if(elements[0] == "get-ip"){
                    string ipport = "IP: ";
                    ipport += ip[i] + ":" + port[i];
                    strncpy(message, ipport.c_str(), sizeof(message));
                    write(sd, (const char*)message, sizeof(buffer));
                }else if(elements[0] == "list-users"){
                    string userlist = "";
                    for(int k = 0; k < max_clients; k ++){
                        if(online[k]){
                            userlist += "user" + to_string(number[k]) + "\n";
                        }
                    }
                    userlist = userlist.substr(0,userlist.length()-1);
                    strncpy(message, userlist.c_str(), sizeof(message));
                    write(sd, (const char*)message, sizeof(buffer));
                }

            }
		}
    
	}
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

string print_client(int IP,int port){
    string info = "";
    for(int i = 0; i < 4; i ++){
        info += to_string(IP % 256);
        IP = IP >> 8;
        if(i < 3){
            info += ".";
        }
    }
    info += ":";
    info += to_string(port);
    return info;
}
