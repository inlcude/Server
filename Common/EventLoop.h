#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__
#include"KETTLEPlatform.h"
#include"Channel.h"
#include "ThreadMutex.h"

using namespace KETTLE;
namespace KETTLE{
    class EpollPoller;
    class EventLoop{
        
    public:
        EventLoop();
        ~EventLoop();

        void runInLoop(Channel* channel);
        void runInLoop(std::function<void()>);
        void removeChannel(Channel* channel);
        void exitInLoop(const char* Channel);
        void startLoop();
        void stopLoop();
        void loop();
    protected:
        void handRead();
        void handWrite();
        void handError();
        void wakeUp();
    private:
        bool            _running;
        std::unique_ptr<EpollPoller>            _poller;
        std::vector<std::function<void()>>      _functors;
        ThreadMutex     _mutex;
        int32                         _eventfd
        std::unique_ptr<Channel>                _eventChannel;
    };
}

#endif