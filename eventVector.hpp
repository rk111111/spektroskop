#ifndef EVENTVECTOR_HPP_INCLUDED
#define EVENTVECTOR_HPP_INCLUDED
#include <cstdlib>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

namespace spk
{
    class EventBase
    {

    public:
        EventBase();
        virtual bool checkEvent(const SDL_Event *e);
    };

    class EventQuit: public EventBase
    {
        bool *running;
    public:
        EventQuit(bool *runningFlag);
        virtual bool checkEvent(const SDL_Event *e);
    };

    class EventVector: public std::vector<EventBase*>
    {
    public:
        EventVector();
        bool checkEvent(const SDL_Event *e);
    };
}





#endif // EVENTVECTOR_HPP_INCLUDED
