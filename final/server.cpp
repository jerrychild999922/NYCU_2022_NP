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
#define PORT 8888
using namespace std;


class Room{
	public:
		string id;
		string manager;
		string password;
		int m_r;
		int type;
		int state;
		int round;
		vector <string> player;
		vector <int> p_r;
		string answer;
		string cur;

};
class invite{
	public:
		string id;
		string m_email;
		string password;
		string manager;
		string invitee;
};

bool comparator(const Room& lhs, const Room& rhs) {
   return stoi(lhs.id) < stoi(rhs.id);
}

bool comparator1(const invite& lhs, const invite& rhs) {
   return stoi(lhs.id) < stoi(rhs.id);
}

bool mycomp (string a, string b){
    return a<b;
}


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
		}
		else{
			for(int j=0;j<4;j++){
				if(gg[i]==aa[j]&& aa[j]!=gg[j]){
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
//	int in_room[20];
	
	vector <string> ans(20);
	vector <string> user(20);
	vector <string> user_room(20);
	vector <string> name;
	vector <string> email;
	vector <string> pass;
	vector <Room> Rooms;
	vector <invite> invites;
	vector <int> online;
	vector <int> oo;
	vector <int> mute;

	for(int i=0;i<20;i++){
		client_socket[i]=0;
		log[i]=0;
		start[i]=0;
		chance[i]=0;
//		in_room[i]=0;
	}

	// Creating socket file descriptor
	if ( (sockfd_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket udp creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	

	int port = atoi(argv[1]);
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	bind(sockfd_udp, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	
	//===============================================================//

	sockfd_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_tcp == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}

	bind(sockfd_tcp, (const struct sockaddr*)&servaddr, sizeof(servaddr));

	if ((listen(sockfd_tcp, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}

	
	len=sizeof(cliaddr);
	int count;
	while(1){
		if(c3==0){
			//printf("New connection.");
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
		//cout << "test1" << endl;
		count= select(so,&s,NULL,NULL,NULL);
		//cout << "test2" << endl;
		if(FD_ISSET(sockfd_tcp,&s)){
			string o1;
            bzero(buffer, sizeof(buffer));
			new_sd = accept(sockfd_tcp,(struct sockaddr*)&cliaddr,&len);
            online.push_back(1);
			mute.push_back(0);
            int k=name.size();
            string kk="user"+to_string(k);
            name.push_back(kk);
			//cout<<new_sd<<endl;
			oo.push_back(new_sd);
            o1= "Welcome, "+kk+".\n";
            char* e ;
    		e=new char [o1.size()+1];
    		strcpy(e,o1.c_str());
            send(new_sd, (const char*)e, sizeof(o1),0);
            for(int i=0;i<20;i++){
				if(client_socket[i]==0){
					//cout<<i<<new_sd<<endl;
                    user[new_sd]=kk;
					client_socket[i]=new_sd;
					break;
				}
			}
		}


		for(int i=0;i<20;i++){
			connfd1=client_socket[i];
			if(FD_ISSET(connfd1,&s)){
				bzero(buffer, sizeof(buffer));
				//printf("Message From TCP client: ");
                int byt=read(connfd1, buffer, sizeof(buffer));
				if (true){
	
					string a(buffer);
					//puts(buffer);
					cout<<"Message From TCP client: "<<a<<endl;
					
					//cout<<a<<endl;
					vector<string> y;
					int current = 0; 
					int next;
					string o;
					while (1){
						next = a.find_first_of(" \n", current);

					if (next != current)
					{
						string tmp = a.substr(current, next - current);
						if (tmp.size() != 0) 
							y.push_back(tmp);
					}
					if (next == string::npos) break;
					current = next + 1; 

					}

					//=========================login===========================//
					if(y[0]=="yell"){
						//cout<<"yes"<<endl;
						string o1;
						o1=user[connfd1]+": ";
						for (int j=1;j<y.size();j++){
							o1+=y[j]+" ";
						}
						o1+="\n";
						char* r1 ;
						r1=new char [o1.size()+1];
						strcpy(r1,o1.c_str());

						for (int j=0;j<online.size();j++){
							if(online[j]==1){
								if(oo[j]!=connfd1){
									if(mute[j]==0){
										send(oo[j], (const char*)r1, o1.size(),0);
									}
								}
							}
						}
					}
					else if (y[0]=="mute"){
						for (int j=0;j<oo.size();j++){
							if(oo[j]==connfd1){
								if(mute[j]==1){
									o="You are already in mute mode.\n";
								}
								else{
									mute[j]=1;
									o="Mute mode.\n";
								}
							}
						}
						
					}
					else if (y[0]=="unmute"){
						for (int j=0;j<oo.size();j++){
							if(oo[j]==connfd1){
								if(mute[j]==0){
									o="You are already in unmute mode.\n";
								}
								else{
									mute[j]=0;
									o="Unmute mode.\n";
								}
							}
						}
					}
					else if(y[0]=="tell"){
						//cout<< "iriri"<<endl;
						string o1;
						o1=user[connfd1]+" told you: ";
						for (int j=2;j<y.size();j++){
							o1+=y[j]+" ";
						}
						o1+="\n";
						char* r1 ;
						r1=new char [o1.size()+1];
						strcpy(r1,o1.c_str());

						int jud3=0;
						int jud4=0;
						for (int j=0;j<user.size();j++){
							if(y[1]==user[connfd1]){
								jud4=1;
							}
							else if(user[j]==y[1]){
								//cout<<j<<endl;
								for(int k=0;k<oo.size();k++){
									//cout<< oo[k]<<endl;
									if(oo[k]==j){
										//cout<< oo[k]<<j<<endl;
										if(mute[k]==0){
											//cout<< "iriri3"<<endl;
											jud3=1;
											send(oo[k], (const char*)r1, o1.size(),0);
										}
										else{
											jud4=1;
										}
									}
								}
							}
						}
						if(jud4==1){
							o="Error\n";
						}
						else if(jud3==0){
							o=y[1]+" does not exist.\n";
						}
					}
					//=========================exit===========================//
					else if(y[0]=="exit"){
						int ii=0;
                    	for(int j=0;j<name.size();j++){
                        	if(user[connfd1]==name[j]){
                            	ii=j;
                            	break;
                        	}
                    	}
						online[ii]=0;
						oo[ii]=0;
						close(connfd1);
						client_socket[i]=0;
					}
					else {
							o="";
					}
				
					char* e ;
					e=new char [o.size()+1];
					strcpy(e,o.c_str());
					send(connfd1, (const char*)e, o.size(),0);
				}
			}
		}
		
        // if udp socket is readable receive the message.
        if (FD_ISSET(sockfd_udp, &s)) {
            len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            //printf("\nMessage from UDP client: ");
            n = recvfrom(sockfd_udp, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, &len);
			//puts(buffer);
			//cout<<"Message from UDP client: ";
			string a(buffer);
			cout<<a<<endl;
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
			//=========================register===========================//
			if(y[0]=="register"){
				if (y.size()==4){
					int error=0;
					for(int i=0;i<name.size();i++){
						if (y[1]==name[i]){
							error=1;
							o="Username or Email is already used\n";
							break;
						}
						else if(y[2]==email[i]){
							error=2;
							o="Username or Email is already used\n";
							break;
						}
					}
					if(error==0){
						name.push_back(y[1]);
						cout<<y[1]<<endl;
						email.push_back(y[2]);
						cout<<y[2]<<endl;
						pass.push_back(y[3]);
						cout<<y[3]<<endl;
						o="Register Successfully\n";
					}
				}
				else{
					o="Usage: register <username> <email> <password>";
				}
			}
			//=========================list rooms===========================//
			else if (y[0]=="list" && y[1]=="rooms"){
				if(Rooms.size()==0){
					o="List Game Rooms\nNo Rooms\n";
				}
				else {
					sort(Rooms.begin(), Rooms.end(), &comparator);
					o+="List Game Rooms\n";
					for (int j=0;j<Rooms.size();j++){

						string a1;
						string b1;
						if(Rooms[j].type==0){
							a1="Public";
						}
						else{
							a1="Private";
						}
						if(Rooms[j].state==0){
							b1=" is open for players";
						}
						else{
							b1=" has started playing";
						}
						o+= to_string(j+1)+". ("+a1+") Game Room "+Rooms[j].id+b1+"\n";
					}
				}
			}
			//=========================list users===========================//
			else if (y[0]=="list" && y[1]=="users"){
				if(name.size()==0){
					o="List Users\nNo Users\n";
				}
				else{
					sort(name.begin(), name.end(), mycomp);
					o+="List Users\n";
					for(int j=0;j<name.size();j++){
						string a2;
						a2="Offline";
						for(int k=0;k<user.size();k++){
							if(user[k]==name[j]){
								a2="Online";
								break;
							}
						}
						o+= to_string(j+1)+". "+name[j]+"<"+email[j]+"> "+a2+"\n";
					}
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
            sendto(sockfd_udp, (const char*)e, o.size(), 0,
                (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        }
		c3++;
    }
}