#include"Acceptor.h"
#include"Log.h"
#include "TcpSocket.h"

KETTLE::Acceptor::Acceptor(const InnetAddr& address,HandleConnection connection):
_addr(new InnetAddr(address)),_connection(connection){

    int32 sockfd = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
        LOG_FATA << "socket() error,reason" << strerror(errno);

    channel = std::make_shared<Channel>(sockfd,std::bind(&Acceptor::read,this)
    ,std::bind(&Acceptor::write,this)
    ,std::bind(&Acceptor::error,this));

    _listenSocket->bindAddress(*(_addr.get()));
    _listenSocket->listen();
}

void KETTLE::Acceptor::read(){
    InnetAddr address;
    int32 sockfd = _listenSocket->accpet(address);
    if(sockfd < 0)
        LOG_ERROR << "accpet error,reason:" << strerror(errno);

    _connection(sockfd,address);
    
}

void KETTLE::Acceptor::write(){

}

void KETTLE::Acceptor::error(){

}