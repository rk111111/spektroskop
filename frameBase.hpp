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
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "eventVector.hpp"

//#define DEBUG

namespace spk
{
    class FrameBase: public EventBase
    {
        #ifdef DEBUG
    public:
        #else
    protected:
        #endif // DEBUG
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
        /// The constructor. It sets default values.
        FrameBase();
        /// It evaluates parameters depending on the window pointer.
        virtual bool getWindowParam();
        /// It sets window pointer and calls getWindowParam()
        virtual bool setWindow(SDL_Window* w);
        /// It sets the part of screen which will drawn to. Values are expressed as a percentage. It calls evaluateViewPort().
        virtual bool setRelativeViewPort(myTypeRectangle pattern);
        /// It sets the color of the background.
        virtual bool setClearColor(SDL_Color c);
        /// It sets pointer to part of the screen of parent frame. It calls evaluateViewPort().
        virtual bool setParentViewPort(SDL_Rect* parentViewPortPoiter);
        /// It evaluates view port basing on the relViewPort and parentViewPort
        virtual bool evaluateViewPort();

        /// It only calls clearRenderer() but it is used by ?inheritance? classes.
        virtual bool render();
        /// It sets clearColor to the background of part of the screen marked by viewPort.
        virtual bool clearRenderer();
    };


}



#endif // FRAMES_HPP_INCLUDED
