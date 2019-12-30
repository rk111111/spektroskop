#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

//#define DEBUG

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

    class Rectangle: public FrameBase
    {
        #ifdef DEBUG
    public:
        #else
    protected:
        #endif // DEBUG
        myTypeRectangle relRect;
        SDL_Rect RectToDraw;
        SDL_Color rectColor;
    public:
        virtual bool setRectangle(myTypeRectangle *rect, SDL_Color *color, bool setDefault=true);
        virtual bool setRelativeViewPort(myTypeRectangle pattern);
        virtual bool setParentViewPort(SDL_Rect* parentViewPortPointer);

        virtual bool render();

    };

    bool evaluateRectangle(const SDL_Rect *viewPort, const myTypeRectangle *base, SDL_Rect *target);
}

#endif // WINDOW_HPP_INCLUDED
