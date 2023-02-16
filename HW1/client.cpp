#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <vector>
    
#define MAXLINE 1024 


using namespace std;
void func(int sockfd,string a)
{
	
    //printf("Enter the string : ");
    char* e ;
    e=new char [a.size()+1];
    strcpy(e,a.c_str());
    write(sockfd, e, strlen(e));
 
    char buff[MAXLINE];
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s\n", buff);
    /*if ((strncmp(buff, "exit", 4)) == 0) {
        printf("Client Exit...\n");
        break;
    }*/
}

// Driver code 
int main(int argc,char* argv[]) { 
    int sockfd1;
    int sockff;
    char buffer1[MAXLINE]; 
    struct sockaddr_in servaddr;
    
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd1 == -1) {
            printf("socket creation failed...\n");
            exit(0);
        }
    bzero(&servaddr, sizeof(servaddr));
    string adr(argv[1]);
    int port = atoi(argv[2]);
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(adr.c_str());
    servaddr.sin_port = htons(port);
    // connect the client socket to server socket
    if (connect(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else{
        printf("*****Welcome to Game 1A2B*****\n");
    }

    /*
    if ( (sockff = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
                perror("socket creation failed"); 
                exit(EXIT_FAILURE); 
            } 
            
    memset(&servaddr, 0, sizeof(servaddr)); 
            
    
    int n ;
    char* e1="New connection.";
    socklen_t len;
    sendto(sockff, (const char *)e1, strlen(e1), 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
    sizeof(servaddr)); 

    n = recvfrom(sockff, (char *)buffer1, MAXLINE,  
            MSG_WAITALL, (struct sockaddr *) &servaddr, 
            &len); 
    buffer1[n] = '\0'; 
    printf("%s\n", buffer1);
    */
    

    while (1){
        string a;
        getline(cin ,a);
        vector<string> y;
        int current = 0; 
        int next;
        int i=0;
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

        int judge=0; // 0-error 1-tcp 2-udp;
        if (y[0]=="register"){
            judge=2;
        }
        else if(y[0]=="game-rule"){
            judge=2;
        }
        else if(y[0]=="login"){
            judge=1;
        }
        else if(y[0]=="logout"){
            judge=1;
        }
        else if(y[0]=="start-game"){
            judge=1;
        }
        else if(y[0]=="exit"){
            judge=1;
        }
        else{
            judge=1;
        }
        
        
        if(judge==2){
            int sockfd; 
            char buffer[MAXLINE]; 
            struct sockaddr_in  servaddr ;
            // Creating socket file descriptor 
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
                perror("socket creation failed"); 
                exit(EXIT_FAILURE); 
            } 
            
            memset(&servaddr, 0, sizeof(servaddr)); 
            
            // Filling server information 
            servaddr.sin_family = AF_INET; 
            servaddr.sin_port = htons(port); 
            servaddr.sin_addr.s_addr = inet_addr(adr.c_str()); 

            char* e ;
            e=new char [a.size()+1];
            strcpy(e,a.c_str());
            int n ;
            socklen_t len;

            sendto(sockfd, (const char *)e, strlen(e), 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
                sizeof(servaddr)); 

            n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                    MSG_WAITALL, (struct sockaddr *) &servaddr, 
                    &len); 
            buffer[n] = '\0'; 
            printf("%s\n", buffer);
            //close(sockfd); 
        }
        else if (judge==1){
            //int sockfd1;
            int connfd;
            struct sockaddr_in servaddr;

            // socket create and verification
            /*sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd1 == -1) {
                printf("socket creation failed...\n");
                exit(0);
            }

            bzero(&servaddr, sizeof(servaddr));

            // assign IP, PORT
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
            servaddr.sin_port = htons(PORT);

            // connect the client socket to server socket
            if (connect(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr))
                != 0) {
                printf("connection with the server failed...\n");
                exit(0);
            }*/

            // function for chat
            func(sockfd1,a);
            if (y[0]=="exit"){
                //printf("exit");
                close(sockfd1);
                break;
            }
            // close the socket
        }
    }
    
    return 0; 
}