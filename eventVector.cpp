#include "eventVector.hpp"
using namespace std;
namespace spk
{
    //////////////////////////////////////////////////
    // EventBase

    /// Default constructor. It just is.
    EventBase::EventBase()
    {

    }
    /// This is virtual function. It does nothing, but child classes may use it to operate on events.
    bool EventBase::checkEvent(const SDL_Event *e)
    {
        return false;
    }

    //////////////////////////////////////////////////
    // EventQuit

    /// Constructor require pointer to bool variable hold the main loop.
    EventQuit::EventQuit(bool* runningFlag):EventBase()
    {
        running=runningFlag;
    }
    /// If the user clicked on X button the running variable will be set to false, so the main loop should be broken.
    bool EventQuit::checkEvent(const SDL_Event* e)
    {
        if(e->type==SDL_QUIT)
        {
            *running=false;
            return true;
        }
        return false;
    }

    //////////////////////////////////////////////////
    // EventVector

    /// Default constructor - it does nothing.
    EventVector::EventVector()
    {

    }
    /// It calls all pointed objects to deal with current event. It returns true if any object succeeded.
    bool EventVector::checkEvent(const SDL_Event *e)
    {
        bool succec=false;
        for(vector<EventBase*>::iterator it=begin();it<end();it++)
        {
            succec=succec || (*it)->checkEvent(e);
        }
        return succec;
    }
}
