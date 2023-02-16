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

int main() {
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
	//int port = atoi(argv[1]);
    // assign IP, PORT

	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

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
				//printf("Message From TCP client: ");
                int byt=read(connfd1, buffer, sizeof(buffer));
				if(byt==0){
					if(user_room[i]!=""){
						int c =-1;
						for(int j=0;j<Rooms.size();j++){
							if(Rooms[j].id==user_room[i]){
								c=j;
								break;
							}
						}
						if(Rooms[c].manager==user[i]){
							//cout<<user_room[i]<<"no"<<i;
							for(int k=0;k<Rooms[c].p_r.size();k++){
								user_room[Rooms[c].p_r[k]]="";
							}
							for(int k=0;k<invites.size();k++){
								if(invites[k].id==user_room[i]){
									invites.erase(invites.begin()+k);
									k--;
								}
							}
							user_room[i]="";
							Rooms.erase(Rooms.begin()+c);
						}
						else if(Rooms[c].state==0){
							for(int k=0;k<Rooms[c].p_r.size();k++){
								if(i==Rooms[c].p_r[k]){
									Rooms[c].player.erase(Rooms[c].player.begin()+k);
									Rooms[c].p_r.erase(Rooms[c].p_r.begin()+k);
									k--;
								}
							}
							user_room[i]="";
						}
						else{
							for(int k=0;k<Rooms[c].p_r.size();k++){
								if(i==Rooms[c].p_r[k]){
									Rooms[c].player.erase(Rooms[c].player.begin()+k);
									Rooms[c].p_r.erase(Rooms[c].p_r.begin()+k);
									k--;
								}
							}
							Rooms[c].state=0;
							user_room[i]="";
						}
					}
					if(user[i]!=""){
						int c1;
						for (int j=0;j<name.size();j++){
							if(user[i]==name[j]){
								c1=j;
								break;
							}
						}
						user[i]="";
						log[c1]=0;
					}
					close(connfd1);
					client_socket[i]=0;
				}
				else{
					//puts(buffer);
					cout<<"Message From TCP client: ";
					string a(buffer);
					cout<<a<<endl;
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

					//cout<<y[0]<<endl;
					//=========================login===========================//
					if(y[0]=="login"){
						//cout<<"123";
						if (y.size()==3){
							//cout<<"12345";
							int error=0;
							int c;
							for (int j=0;j<name.size();j++){
								if(y[1]==name[j]){
									c=j;
									error=1;
									break;
								}
							}
							if(error==0){
								o="Username does not exist\n";
							}
							else if(user[i]!=""){
								o="You already logged in as "+user[i]+"\n";
							}
							else if(log[c]==1){
								o="Someone already logged in as "+y[1]+"\n";
							}
							else if(y[2]==pass[c]){
								//cout<<"hi";
								o="Welcome, "+y[1]+"\n";
								log[c]=1;
								user[i]=y[1];
							}
							else{
								o="Wrong password\n";
							}
						}
						else{
							o="Usage: login <username> <password>";
						}
					}
					//=========================exit===========================//
					else if(y[0]=="exit"){
						
						if(user_room[i]!=""){
							int c =-1;
							for(int j=0;j<Rooms.size();j++){
								if(Rooms[j].id==user_room[i]){
									c=j;
									break;
								}
							}
							if(Rooms[c].manager==user[i]){
								//cout<<user_room[i]<<"no"<<i;
								for(int k=0;k<Rooms[c].p_r.size();k++){
									user_room[Rooms[c].p_r[k]]="";
								}
								for(int k=0;k<invites.size();k++){
									if(invites[k].id==user_room[i]){
										invites.erase(invites.begin()+k);
										k--;
									}
								}
								user_room[i]="";
								Rooms.erase(Rooms.begin()+c);
							}
							else if(Rooms[c].state==0){
								for(int k=0;k<Rooms[c].p_r.size();k++){
									if(i==Rooms[c].p_r[k]){
										Rooms[c].player.erase(Rooms[c].player.begin()+k);
										Rooms[c].p_r.erase(Rooms[c].p_r.begin()+k);
										k--;
									}
								}
								user_room[i]="";
							}
							else{
								for(int k=0;k<Rooms[c].p_r.size();k++){
									if(i==Rooms[c].p_r[k]){
										Rooms[c].player.erase(Rooms[c].player.begin()+k);
										Rooms[c].p_r.erase(Rooms[c].p_r.begin()+k);
										k--;
									}
								}
								Rooms[c].state=0;
								user_room[i]="";
							}
						}
						if(user[i]!=""){
							int c1;
							for (int j=0;j<name.size();j++){
								if(user[i]==name[j]){
									c1=j;
									break;
								}
							}
							user[i]="";
							log[c1]=0;
						}
						close(connfd1);
						client_socket[i]=0;
					}
					//=========================logout===========================//
					else if(y[0]=="logout"){
						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]!=""){
							o="You are already in game room "+user_room[i]+", please leave game room\n";
						}
						else{
							int c;
							for (int j=0;j<name.size();j++){
								if(user[i]==name[j]){
									c=j;
									break;
								}
							}
							o="Goodbye, "+user[i]+"\n";
							user[i]="";
							log[c]=0;
						}
					}
					//=========================start-game===========================//
					else if(y[0]=="start" && y[1]=="game"){

						int c =-1;
						if(user_room[i]!=""){
							for(int j=0;j<Rooms.size();j++){
								if(Rooms[j].id==user_room[i]){
									c=j;
									break;
								}
							}
						}
						string four;
						int ee=0;
						if(y.size()==3){
							srand(time(NULL));
							string f;
							for (int i=0;i<4;i++){
								int x=rand()%10;
								string s=to_string(x);
								f=f+s;
							}
							four=f;
						}
						else if(y.size()==4){
							int cc=0;
							for(int j=0;j<y[3].length();j++){
								if(isdigit(y[3][j])){
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
							if(ee!=1){
								four=y[3];
							}
						}


						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]==""){
							o="You did not join any game room\n";
						}
						else if(Rooms[c].manager!=user[i]){
							o="You are not game room manager, you can't start game\n";
						}
						else if(Rooms[c].state==1){
							o="Game has started, you can't start again\n";
						}
						else if(ee==1){
							o="Please enter 4 digit number with leading zero\n";
						}
						else{
							string o1="Game start! Current player is "+user[i]+"\n";
							for(int k=0;k<Rooms[c].p_r.size();k++){					
								char* r1 ;
								r1=new char [o1.size()+1];
								strcpy(r1,o1.c_str());
								write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
							}
							o=o1;
							Rooms[c].round=stoi(y[2])*(Rooms[c].player.size()+1);
							Rooms[c].answer=four;
							Rooms[c].state=1;
							Rooms[c].cur=user[i];
							//cout<<"jojojoj"<<Rooms[c].answer;

						}
					}
					//=========================guess===========================//
					else if(y[0]=="guess"){
						int c =-1;
						if(user_room[i]!=""){
							for(int j=0;j<Rooms.size();j++){
								if(Rooms[j].id==user_room[i]){
									c=j;
									break;
								}
							}
						}

						int ee=0;
						if(y.size()==2){
							int cc=0;
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
						}

						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]==""){
							o="You did not join any game room\n";
						}
						else if(Rooms[c].state!=1){
							if(Rooms[c].manager==user[i]){
								o="You are game room manager, please start game first\n";
							}
							else{
								o="Game has not started yet\n";
							}
						}
						else if(Rooms[c].cur!=user[i]){
							o="Please wait..., current player is "+Rooms[c].cur+"\n";
						}
						else if(ee==1){
							o="Please enter 4 digit number with leading zero\n";
						}
						else{
							string s;
							//cout<<y[1];
							///cout<<Rooms[c].answer;
							s=game(y[1],Rooms[c].answer);
							if(s=="4A0B"){
								string o1=user[i]+" guess \'"+y[1]+"\' and got Bingo!!! "+user[i]+" wins the game, game ends\n";
								if(user[i]==Rooms[c].manager){
									for(int k=0;k<Rooms[c].p_r.size();k++){					
										char* r1 ;
										r1=new char [o1.size()+1];
										strcpy(r1,o1.c_str());
										write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
									}
									o=o1;
								}
								else{
									for(int k=0;k<Rooms[c].p_r.size();k++){
										if(Rooms[c].p_r[k]!=i){
											char* r1 ;
											r1=new char [o1.size()+1];
											strcpy(r1,o1.c_str());
											write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
										}
									}
									char* r1 ;
									r1=new char [o1.size()+1];
									strcpy(r1,o1.c_str());
									write(client_socket[Rooms[c].m_r], (const char*)r1, o1.size());
									o=o1;
								}
								Rooms[c].state=0;
							}
							else{
								//cout<<"jjjj"<<Rooms[c].round;
								Rooms[c].round--;
								if(Rooms[c].round==0){
									string o1=user[i]+" guess \'"+y[1]+"\' and got \'"+s+"\'\nGame ends, no one wins\n";
									if(user[i]==Rooms[c].manager){
										for(int k=0;k<Rooms[c].p_r.size();k++){					
											char* r1 ;
											r1=new char [o1.size()+1];
											strcpy(r1,o1.c_str());
											write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
										}
										o=o1;
									}
									else{
										for(int k=0;k<Rooms[c].p_r.size();k++){
											if(Rooms[c].p_r[k]!=i){
												char* r1 ;
												r1=new char [o1.size()+1];
												strcpy(r1,o1.c_str());
												write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
											}
										}
										char* r1 ;
										r1=new char [o1.size()+1];
										strcpy(r1,o1.c_str());
										write(client_socket[Rooms[c].m_r], (const char*)r1, o1.size());
										o=o1;
									}
									Rooms[c].state=0;
								}
								else{
									string o1=user[i]+" guess \'"+y[1]+"\' and got \'"+s+"\'\n";
									if(user[i]==Rooms[c].manager){
										for(int k=0;k<Rooms[c].p_r.size();k++){					
											char* r1 ;
											r1=new char [o1.size()+1];
											strcpy(r1,o1.c_str());
											write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
										}
										o=o1;
									}
									else{
										for(int k=0;k<Rooms[c].p_r.size();k++){
											if(Rooms[c].p_r[k]!=i){
												char* r1 ;
												r1=new char [o1.size()+1];
												strcpy(r1,o1.c_str());
												write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
											}
										}
										char* r1 ;
										r1=new char [o1.size()+1];
										strcpy(r1,o1.c_str());
										write(client_socket[Rooms[c].m_r], (const char*)r1, o1.size());
										o=o1;
									}

									if(user[i]==Rooms[c].manager){
										Rooms[c].cur=Rooms[c].player[0];
									}
									else if(user[i]==Rooms[c].player[Rooms[c].player.size()-1]){
										Rooms[c].cur=Rooms[c].manager;
									}
									else{
										int c3 =-1;
										for(int j=0;j<Rooms[c].player.size();j++){
											if(Rooms[c].player[j]==user[i]){
												c3=j;
												break;
											}
										}
										Rooms[c].cur=Rooms[c].player[c3+1];
									}
								}
							}
						}
					}
					//=========================create public room===========================//
					else if(y[0]=="create" && y[1]=="public" && y[2]=="room"){
						int jud1=0;
						for(int j=0;j<Rooms.size();j++){
							if(y[3]==Rooms[j].id){
								jud1=1;
								break;
							}
						}
						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]!=""){
							o="You are already in game room "+user_room[i]+", please leave game room\n";
						}
						else if(jud1==1){
							o="Game room ID is used, choose another one\n";
						}
						else{
							Room new_room;
							new_room.id=y[3];
							new_room.manager=user[i];
							new_room.type=0;
							new_room.state=0;
							new_room.m_r=i;
							new_room.round=0;
							Rooms.push_back(new_room);
							user_room[i]=y[3];
							o="You create public game room "+y[3]+"\n";
						}
					}
					//=========================create private room===========================//
					else if(y[0]=="create" && y[1]=="private" && y[2]=="room"){
						int jud1=0;
						for(int j=0;j<Rooms.size();j++){
							if(y[3]==Rooms[j].id){
								jud1=1;
								break;
							}
						}
						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]!=""){
							o="You are already in game room "+user_room[i]+", please leave game room\n";
						}
						else if(jud1==1){
							o="Game room ID is used, choose another one\n";
						}
						else{
							Room new_room;
							new_room.id=y[3];
							new_room.manager=user[i];
							new_room.type=1;
							new_room.m_r=i;
							new_room.password=y[4];
							new_room.state=0;
							new_room.round=0;
							Rooms.push_back(new_room);
							user_room[i]=y[3];
							o="You create private game room "+y[3]+"\n";
						}
					}
					//=========================join room===========================//
					else if(y[0]=="join" && y[1]=="room"){
						int c=-1;
						for(int j=0;j<Rooms.size();j++){
							if(y[2]==Rooms[j].id){
								c=j;
								break;
							}
						}
						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]!=""){
							o="You are already in game room "+user_room[i]+", please leave game room\n";
						}
						else if(c==-1){
							o="Game room "+y[2]+" is not exist\n";
						}
						else if(Rooms[c].type==1){
							o="Game room is private, please join game by invitation code\n";
						}
						else if(Rooms[c].state==1){
							o="Game has started, you can't join now\n";
						}
						else {
							user_room[i]=y[2];
							Rooms[c].player.push_back(user[i]);
							Rooms[c].p_r.push_back(i);
							o="You join game room "+y[2]+"\n";
							
							string o1="Welcome, "+user[i]+" to game!\n";
							char* r1 ;
							r1=new char [o1.size()+1];
							strcpy(r1,o1.c_str());

							write(client_socket[Rooms[c].m_r], (const char*)r1, o1.size());
							for (int k=0;k<Rooms[c].p_r.size();k++){	
								if(Rooms[c].p_r[k]!=i){
									write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
								}
							}
						}
					}
					//=========================invite===========================//
					else if(y[0]=="invite"){
						int c =-1;
						if(user_room[i]!=""){
							for(int j=0;j<Rooms.size();j++){
								if(Rooms[j].id==user_room[i]){
									c=j;
									break;
								}
							}
						}
						int c1 =-1;
						for (int j=0;j<email.size();j++){
							if(email[j]==y[1]){
								c1=j;
								break;
							}
						}
						int c2=-1;
						if(log[c1]!=0){
							for (int j=0;j<user.size();j++){
								if(user[j]==name[c1]){
									c2=j;
									break;
								}
							}
						}
						int c3=-1;
						for (int j=0;j<name.size();j++){
							if(user[i]==name[j]){
								c3=j;
								break;
							}
						}

						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]==""){
							o="You did not join any game room\n";
						}
						else if(Rooms[c].manager!=user[i]){
							o="You are not private game room manager\n";
						}
						else if(log[c1]==0){
							o="Invitee not logged in\n";
						}
						else{
							int e1=0;
							for (int j=0;j<invites.size();j++){
								if(invites[j].manager==user[i] && invites[j].invitee==name[c1]){
									e1=1;
								}
							}

							
							invite new_invite;
							new_invite.id=Rooms[c].id;
							new_invite.manager=user[i];
							new_invite.password=Rooms[c].password;
							new_invite.invitee=name[c1];
							new_invite.m_email=email[c3];
							if(e1!=1){
								invites.push_back(new_invite);
							}

							string o1="You receive invitation from "+user[i]+"<"+email[c3]+">\n";
							char* r1 ;
							r1=new char [o1.size()+1];
							strcpy(r1,o1.c_str());
							write(client_socket[c2], (const char*)r1, o1.size());
							o="You send invitation to "+name[c1]+"<"+y[1]+">\n";
							
						}
					}
					//=========================list invitations===========================//
					else if(y[0]=="list" && y[1]=="invitations"){
						int c=-1;
						for(int j=0;j<invites.size();j++){
							if(invites[j].invitee==user[i]){
								c=j;
								break;
							}
						}
						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(c==-1){
							o="List invitations\nNo Invitations\n";
						}
						else{
							sort(invites.begin(), invites.end(), &comparator1);
							o+="List invitations\n";
							int count=1;
							for(int j=0;j<invites.size();j++){
								if(invites[j].invitee==user[i]){
									o+=to_string(count)+". "+invites[j].manager+"<"+invites[j].m_email+"> invite you to join game room "+invites[j].id+", invitation code is "+invites[j].password+"\n";
								}
								count++;
							}
						}
					}
					//=========================leave room===========================//
					else if(y[0]=="leave" && y[1]=="room"){
						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]==""){
							o="You did not join any game room\n";
						}
						else{
							int c =-1;
							for(int j=0;j<Rooms.size();j++){
								if(Rooms[j].id==user_room[i]){
									c=j;
									break;
								}
							}
							if(Rooms[c].manager==user[i]){
								//cout<<user_room[i]<<"no"<<i;
								for(int k=0;k<Rooms[c].p_r.size();k++){
									string o1="Game room manager leave game room "+user_room[i]+", you are forced to leave too\n";
									char* r1 ;
									r1=new char [o1.size()+1];
									strcpy(r1,o1.c_str());
									write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
									user_room[Rooms[c].p_r[k]]="";
								}
								for(int k=0;k<invites.size();k++){
									if(invites[k].id==user_room[i]){
										invites.erase(invites.begin()+k);
										k--;
									}
								}
								o="You leave game room "+user_room[i]+"\n";
								user_room[i]="";
								Rooms.erase(Rooms.begin()+c);
							}
							else if(Rooms[c].state==0){
								string o1=user[i]+" leave game room "+user_room[i]+"\n";
								for(int k=0;k<Rooms[c].p_r.size();k++){
									if(Rooms[c].p_r[k]!=i){
										char* r1 ;
										r1=new char [o1.size()+1];
										strcpy(r1,o1.c_str());
										write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
									}
								}
								char* r1 ;
								r1=new char [o1.size()+1];
								strcpy(r1,o1.c_str());
								write(client_socket[Rooms[c].m_r], (const char*)r1, o1.size());

								for(int k=0;k<Rooms[c].p_r.size();k++){
									if(i==Rooms[c].p_r[k]){
										Rooms[c].player.erase(Rooms[c].player.begin()+k);
										Rooms[c].p_r.erase(Rooms[c].p_r.begin()+k);
										k--;
									}
								}
								o="You leave game room "+user_room[i]+"\n";
								user_room[i]="";
							}
							else{
								string o1=user[i]+" leave game room "+user_room[i]+", game ends\n";
								for(int k=0;k<Rooms[c].p_r.size();k++){
									if(Rooms[c].p_r[k]!=i){
										char* r1 ;
										r1=new char [o1.size()+1];
										strcpy(r1,o1.c_str());
										write(client_socket[Rooms[c].p_r[k]], (const char*)r1, o1.size());
									}
								}
								char* r1 ;
								r1=new char [o1.size()+1];
								strcpy(r1,o1.c_str());
								write(client_socket[Rooms[c].m_r], (const char*)r1, o1.size());
								o="You leave game room "+user_room[i]+", game ends\n";
								for(int k=0;k<Rooms[c].p_r.size();k++){
									if(i==Rooms[c].p_r[k]){
										Rooms[c].player.erase(Rooms[c].player.begin()+k);
										Rooms[c].p_r.erase(Rooms[c].p_r.begin()+k);
										k--;
									}
								}
								Rooms[c].state=0;
								user_room[i]="";
							}
						}
					}
					//=========================accept===========================//
					else if(y[0]=="accept"){
						int c=-1;
						for(int j=0;j<invites.size();j++){
							if(y[1]==invites[j].m_email){
								c=j;
								break;
							}
						}
						int c1=0;
						int c2=0;
						if(c!=-1){
							for(int j=0;j<Rooms.size();j++){
								if(Rooms[j].id==invites[c].id){
									c2=j;
									if(Rooms[j].state==1){
										c1=1;
										break;
									}
								}
							}
						}



						if (user[i]==""){
							o="You are not logged in\n";
						}
						else if(user_room[i]!=""){
							o="You are already in game room "+user_room[i]+", please leave game room\n";
						}
						else if(c==-1){
							o="Invitation not exist\n";
						}
						else if(invites[c].password!=y[2]){
							o="Your invitation code is incorrect\n";
						}
						else if(c1==1){
							o="Game has started, you can't join now\n";
						}
						else{
							user_room[i]=invites[c].id;
							Rooms[c2].player.push_back(user[i]);
							Rooms[c2].p_r.push_back(i);
							o="You join game room "+invites[c].id+"\n";
							
							string o1="Welcome, "+user[i]+" to game!\n";
							char* r1 ;
							r1=new char [o1.size()+1];
							strcpy(r1,o1.c_str());

							write(client_socket[Rooms[c2].m_r], (const char*)r1, o1.size());
							for (int k=0;k<Rooms[c2].p_r.size();k++){	
								if(Rooms[c2].p_r[k]!=i){
									write(client_socket[Rooms[c2].p_r[k]], (const char*)r1, o1.size());
								}
							}
						}

					}

					else {
							o="invalid input.";
					}
				
					char* e ;
					e=new char [o.size()+1];
					strcpy(e,o.c_str());
					write(connfd1, (const char*)e, o.size());
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
			cout<<"Message from UDP client: ";
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
						//cout<<y[1]<<endl;
						email.push_back(y[2]);
						//cout<<y[2]<<endl;
						pass.push_back(y[3]);
						//cout<<y[3]<<endl;
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