#ifndef FRAMES_HPP_INCLUDED
#define FRAMES_HPP_INCLUDED

#include <vector>

namespace spk
{
    class FrameBase;
    /// relative position, value should not be less than 0 and more than 100, because it expresses percent value.
    typedef float myTypeLenght;

    typedef struct myTypeRectangle
    {
        myTypeLenght x; myTypeLenght y; myTypeLenght w; myTypeLenght h;
    public:
        bool correctPos();
    } myTypeRectangle;

    typedef struct RectangleSide
    {
        int w;
        int h;
    }RectangleSide;

    typedef std::vector<FrameBase*> vecPoitFrameBase;

}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "eventVector.hpp"

namespace spk
{
    class FrameBase: public EventBase
    {
    protected:
    /// Depends on the window. Those parameters can be evaluated basing on the SDL_Window pointer, but I hope that remembering them might be easier and faster.
        SDL_Window* window;
        SDL_Renderer* renderer;
        RectangleSide windowSize;
        Uint32 windowID;

    /// Depends on the previous frame:
        SDL_Rect viewPort;
        SDL_Rect* parentViewPort;

    /// Its own parameters:
        myTypeRectangle relViewPort;
        SDL_Color clearColor;
    public:
        FrameBase();
        virtual bool getWindowParam();
        virtual bool setWindow(SDL_Window* w);
        virtual bool setRelativeViewPort(myTypeRectangle pattern);
        virtual bool setClearColor(SDL_Color c);
        virtual bool setParentViewPort(SDL_Rect* parentViewPortPoiter);
        virtual bool evaluateViewPort();

        virtual bool render();
        virtual bool clearRenderer();
    };


}



#endif // FRAMES_HPP_INCLUDED
