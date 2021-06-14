#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void errHandler(const char* why, const int exitCode=1)
{
    cerr<<why<<endl;
    exit(exitCode);
}

int main()
{
    sockaddr_in* addr=new (sockaddr_in);
    addr->sin_family=AF_INET;
    addr->sin_port=0;
    addr->sin_addr.s_addr=0;


    sockaddr_in* remoteaddr=new(sockaddr_in);
    remoteaddr->sin_family=AF_INET;
    remoteaddr->sin_port = htons(13);
    remoteaddr->sin_addr.s_addr = inet_addr("82.179.90.12");

    char* buf=new char[256];
    strcpy(buf, "daytime\n");
    int msglen=strlen(buf);

    int mySocket=socket(AF_INET, SOCK_DGRAM,0);
    if(mySocket==-1)
        errHandler("Error opening socket", 11);

    int rc=bind(mySocket, (const sockaddr*)addr, sizeof(sockaddr_in));
    if(rc==-1) {
        close(mySocket);
        errHandler("Error binding socket with local address", 12);
    }
    
    rc=connect(mySocket, (const sockaddr*)remoteaddr, sizeof(sockaddr_in));
    if(rc==-1) {
        close(mySocket);
        errHandler("Error connecting socket with remote server", 13);
    }
    
    rc=send(mySocket, buf, msglen, 0);
    if(rc==-1) {
        close(mySocket);
        errHandler("Error sending message", 14);
    }
    
    rc=recv(mySocket, buf, 256, 0);
    if(rc==-1) {
        close(mySocket);
        errHandler("Error receiving message", 15);
    }
    
    buf[rc]='\0';
    cout<<"We received: "<<buf<<endl;
    close(mySocket);
    
    delete addr;
    delete remoteaddr;
    delete[] buf;
    return 0;
}
