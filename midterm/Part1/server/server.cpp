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
#include <filesystem>
#include <dirent.h>
using namespace std;
#define MAXLINE 1024
 

string list_dir(const char *path);
string read_file(string filename);
vector<string> getelements(string cmdline);

int main(int argc , char *argv[]){
	int PORT = atoi(argv[1]);
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char buffer[MAXLINE];
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	char message[1024];

    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// UDP
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	while(true){
        FD_ZERO(&rset);
        FD_SET(udpfd, &rset);
		// UDP
		if (FD_ISSET(udpfd, &rset)) {
			bool judge = true;
			strncpy(message, "UDP", sizeof(message));
			len = sizeof(cliaddr);
			bzero(buffer, sizeof(buffer));
			cout << "UDP server is waiting." << endl;
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);
			string command = buffer;
			vector<string> elements;
			elements = getelements(command);
			if(elements[0] == "get-file-list"){
                string filename;
                char buffer[PATH_MAX];
                getcwd(buffer, sizeof(buffer));
                filename = list_dir(buffer);
				strncpy(message, filename.c_str(), sizeof(message));
			}else if(elements[0] == "get-file"){
                for(int i = 1; i < elements.size(); i ++){
                    string file= elements[i];
                    string content = read_file(file);
                    strncpy(message, content.c_str(), sizeof(message));
                    sendto(udpfd, (const char*)message, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
                }
				judge = false;
            }else if(elements[0] == "exit"){
                strncpy(message, "exit", sizeof(message));
            }
			puts(buffer);
			if(judge){
				sendto(udpfd, (const char*)message, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
			}
		}
	}
}


string list_dir(const char *path) {
    struct dirent *entry;
    string list = "Files: ";
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return list;
    }
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            list += entry->d_name;
            list += " ";
        }
    }
    closedir(dir);
    return list;
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


string read_file(string filename){
	ifstream myfile; 
	myfile.open(filename);
	string content;
	if(myfile.is_open()){
		while(myfile){
			content += myfile.get();
		}
	}
	content.pop_back();
	return content;
}
