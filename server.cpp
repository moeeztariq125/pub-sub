#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include "fileops.h"
#include <cstring>

std:: list <int> csds;
std:: list <int> signed_in;
void remove_client(int a)
{
    close(a);
    csds.remove(a);
    pthread_exit(0);
}

char* recv_msg(int a){
    char* r=new char[1024];
    int i;
    while(true){
    recv(a,r,1024,0);
    std::cout<<r<<'\n';
 
}
}






// void send_msg(int a){
//     char r[]="chawal";
//     std::cout<<a<<'\n';
//    send(a,r,6,0);

// }

bool authenticate(int a){
    char buf[1024];
    int check;
    check=recv(a,buf,sizeof(buf),0);
    if(check==0 or check==-1)
    {
        return 0;
    }
    if(buf[0]=='0')
    {   std::cout<<"new client \n";
        int x=new_client();
        std::string s=std::to_string(x);
        char* r= new char[sizeof(s)];
        std::strncat(r,&s[0],sizeof(r));
        send(a,r,sizeof(r),0);
        signed_in.push_back(x);
        return 1;
    }
    else if (buf[0]=='1')
    {   std::cout<<"existing client"<<std::endl;
        int x=existing_users(buf[1]);
        if(x)
       { 
           signed_in.push_back(x);
            return 1;
       }
       else
        return 0;
    }
    else
    {   
        char r[]="bhaag chawal \n";
        send(a,r,sizeof(r),0);
         return 0;
    }
    

}

void* dispatcher(void* i)
{
    int a=*((int *) i);
    pthread_exit(0);
}





void* new_connections(void* a)
    { 
        int * socketDescriptor=new int;
        *socketDescriptor=*((int*) a);

        pthread_t* client_acceptance;
        int i=0;
        int* csd=new int;
    while(1)
    {
        i++;
        socklen_t clientLength;
        struct sockaddr_in clientAddress;
        *csd=accept(*socketDescriptor,(struct sockaddr*)&clientAddress,&clientLength);
        if(!authenticate(*csd))
        {   std::cout<<"nae hua authenticate \n";
            close(*csd);
            continue;
        }
        csds.push_back(*csd);
        std::cout<<"new client connected with id: "<<*csd<<"\n";
        client_acceptance=new pthread_t;
        pthread_create(client_acceptance,0,dispatcher,csd);

    }
}


int main(int argc,char *argv[])
{
    int* socketDescriptor=new int;
    struct sockaddr_in serverAddress;
    bzero(&serverAddress,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddress.sin_port=htons(5500);


    *socketDescriptor=socket(AF_INET,SOCK_STREAM,0);


    bind(*socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));


    listen(*socketDescriptor,5);
    printf("%s\n","Server is running ...");

    int i=0;
    pthread_t listening;
    pthread_create(&listening,0,new_connections,socketDescriptor);


    std::cout<<"shut down server? \n";
    std::string a;
    std::cin>>a;
    if(a=="yes"){
    for (auto v : csds)
        {   std::cout <<"client "<< v << " disconnected. \n";
            remove_client(v);
        }

    close(*socketDescriptor);
    std::cout<<"shutting down system \n";
    }    
    _exit;
    return 0;
}
