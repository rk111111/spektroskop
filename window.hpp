#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

namespace spk
{
    class Window;
}

#include "frame.hpp"

namespace spk
{
    class Window: public Frame
    {
    protected:

    public:
        Window();
        virtual bool DestroyWindow();
        virtual bool setViewPort();
        virtual void clear();
        virtual bool CreateWindow(const char* title, int x, int y, int w, int h,Uint32 flags);

        ~Window();

        virtual bool render();
        virtual bool checkEvent(const SDL_Event *e);
    };
}

#endif // WINDOW_HPP_INCLUDED
