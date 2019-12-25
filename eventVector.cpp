#include "eventVector.hpp"
using namespace std;
namespace spk
{
    //////////////////////////////////////////////////
    // EventBase

    EventBase::EventBase()
    {

    }

    bool EventBase::checkEvent(const SDL_Event *e)
    {
        return false;
    }

    //////////////////////////////////////////////////
    // EventQuit

    EventQuit::EventQuit(bool* runningFlag):EventBase()
    {
        running=runningFlag;
    }
    bool EventQuit::checkEvent(const SDL_Event* e)
    {
        //cout<<"Event quit checks event"<<endl;
        if(e->type==SDL_QUIT)
        {
            cout<<"The program will be closed"<<endl;
            *running=0;
            return true;
        }
        return false;
    }

    //////////////////////////////////////////////////
    // EventVector

    EventVector::EventVector()
    {

    }

    bool EventVector::checkEvent(const SDL_Event *e)
    {
        //cout<<"Event type "<<e->type<<endl;
        bool succec=false;
        for(vector<EventBase*>::iterator it=begin();it<end();it++)
        {
            succec=succec || (*it)->checkEvent(e);
        }
        return succec;
    }
}
