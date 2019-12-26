#ifndef EVENTVECTOR_HPP_INCLUDED
#define EVENTVECTOR_HPP_INCLUDED

namespace spk
{
    class EventBase;
    class EventQuit;
    class EventVector;
}

#include <cstdlib>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

namespace spk
{
    //////////////////////////////////////////////////
    // EventBase

    /**
        This will be used by EventVector to keep vector of pointers to objects operating on events.
    */
    class EventBase
    {

    public:
        /// Default constructor. It just is.
        EventBase();
        /// This is a virtual function. It does nothing, but child classes may use it to operate on events.
        virtual bool checkEvent(const SDL_Event *e);
    };

    //////////////////////////////////////////////////
    // EventQuit

    /**
        This class is responsible for dealing with quit events.
    */
    class EventQuit: public EventBase
    {
        /// This holds pointer to bool variable keeping the main loop.
        bool *running;
    public:
        /// Constructor require pointer to bool variable hold the main loop.
        EventQuit(bool *runningFlag);
        /// If the user clicked on X button the running variable will be set to false, so the main loop should be broken.
        virtual bool checkEvent(const SDL_Event *e);
    };

    //////////////////////////////////////////////////
    // EventVector

    /**
        This class is responsible for holding pointers to all objects responsible for dealing with events. Remember! It only holds pointers so it will not destroy any of those objects (I think it will not) - you have to destroy them yourself!
    */
    class EventVector: public std::vector<EventBase*>
    {
    public:
        /// Default constructor - it does nothing.
        EventVector();
        /// It calls all pointed objects to deal with current event. It returns true if any object succeeded.
        bool checkEvent(const SDL_Event *e);
    };
}





#endif // EVENTVECTOR_HPP_INCLUDED
