#include"TcpServer.h"
#include"EventLoop.h"
#include"Log.h"

TcpServer::TcpServer(const char* ip,uint16 port,int32 threadNum)
:_eventLoop(new EventLoop()) 
,_acceptor(new Acceptor(_eventLoop.get()
                        ,*(_address.get())
                        ,std::bind(&TcpServer::handlAccept
                                    ,this
                                    ,std::placeholders::_1
                                    ,std::placeholders::_2)))
,_running(false)
,_address(new InnetAddr(ip,port))
,_eventPool(new ThreadEventPool(threadNum)){
}

TcpServer::~TcpServer(){

}

void TcpServer::loop(){
    // dothing;
    _eventLoop->loop();
}

void TcpServer::start(){
    LOG_INFO << "Tcp server start,listen on " << _address->getIP() << " listen on port:" << _address->getPort();
    _eventLoop->runInLoop(_acceptor->get_channel());
    _running = true;
    _eventPool->start();
    loop();
}

void TcpServer::handlAccept(int32 sockfd,const InnetAddr& address){
    
    TcpConnectionPtr connection = std::make_shared<TcpConnection>(_eventLoop.get(),sockfd,*(_address.get()),address);
    if(_connectionHashMap.find(sockfd) != _connectionHashMap.end())
        _connectionHashMap[sockfd] = connection;
    else
        _connectionHashMap.insert(std::make_pair(sockfd,connection));

    EventLoop* pEventLoop = _eventPool->getNextLoop();
    pEventLoop->runInLoop(connection->getChannel());

    LOG_TRACE << "remote address connected,ip:" << connection->getRemoteAddress().getIP()
    << " port:" << connection->getRemoteAddress().getPort();
    
}

void TcpServer::handleRemoveConnection(TcpConnectionPtr connection){
    EventLoop* loop = connection->getLoop();
}