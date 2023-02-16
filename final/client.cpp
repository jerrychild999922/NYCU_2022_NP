#include<iostream>
#include<thread>
#include<unistd.h>
#include<cstring>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define MAX_MASSAGE_LEN 1024

using namespace std;

class SOCKET_{
    private:
        struct sockaddr_in server_addr;
        int tcp_socket;
        int recv_num,send_num;
        char send_buff[MAX_MASSAGE_LEN],recv_buff[MAX_MASSAGE_LEN];
    public:
        SOCKET_();
        void ip_port_(char*,char*);
        int TCP_init();
        int TCP_connect();
        int TCP_recv();
        int TCP_send();
        void write_in_send_buff();
        string get_send_buff();
        string get_recv_buff();
};

SOCKET_::SOCKET_(){
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
}
void SOCKET_::ip_port_(char*ip,char*port){
    server_addr.sin_port=htons(stoi(port));
    inet_pton(AF_INET,ip,&server_addr.sin_addr);
}
int SOCKET_::TCP_init(){
    tcp_socket=socket(AF_INET,SOCK_STREAM,0);
    if(tcp_socket==-1){
        cout<<"tcp socket error\n";
        return 0;
    }
    return 1;
}
int SOCKET_::TCP_connect(){
    if(connect(tcp_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
        cout<<"tcp connect error\n";
        return 0;
    }
    return 1;
}
int SOCKET_::TCP_recv(){
    memset(&recv_buff,0,sizeof(recv_buff));
    recv_num=recv(tcp_socket,recv_buff,MAX_MASSAGE_LEN,0);
    if(recv_num==-1){
        cout<<"recv error\n";
        return 0;
    }
    recv_buff[recv_num]='\0';
    return 1;
}
int SOCKET_::TCP_send(){
    send_num=send(tcp_socket,send_buff,strlen(send_buff),0);
    if(send_num==-1){
        cout<<"send error\n";
        return 0;
    }
    return 1;
}
void SOCKET_::write_in_send_buff(){
    memset(&send_buff,0,sizeof(send_buff));
    fgets(send_buff,MAX_MASSAGE_LEN,stdin);
}
string SOCKET_::get_send_buff(){
    return send_buff;
}
string SOCKET_::get_recv_buff(){
    return recv_buff;
}


SOCKET_ SOCKET;

void* tcp_recv_thread(void*){
    while(true){
        SOCKET.TCP_recv();
        cout<<SOCKET.get_recv_buff();
    }
}

int main(int argc,char**argv){
    if(argc!=3){
        cout<<"IP or PORT error\n";
        return 0;
    }

    SOCKET.ip_port_(argv[1],argv[2]);

    if(!SOCKET.TCP_init()){
        return 0;
    }

    if(!SOCKET.TCP_connect()){
        return 0;
    }

    pthread_t th;
    pthread_create(&th,NULL,tcp_recv_thread,NULL);

    cout<<"**************************\n* Welcome to the BBS server. *\n**************************\n";
    while(true){
        SOCKET.write_in_send_buff();
        SOCKET.TCP_send();
        if(SOCKET.get_send_buff()=="exit\n"){
            break;
        }

    }
    return 0;

}