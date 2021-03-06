#ifndef FRAMES_HPP_INCLUDED
#define FRAMES_HPP_INCLUDED

namespace spk
{
    class FrameBase;
    /// relative position, value should not be less than 0 and more than 100, because it expresses percent value.
    typedef float relPos;
    typedef struct relPosRect
    {
        relPos x; relPos y; relPos w; relPos h;
    public:
        bool correctPos();
    } relPosRect;
    typedef struct windowSize
    {
        int x;
        int y;
    }windowSize;
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
#include <vector>
#include "eventVector.hpp"

namespace spk
{
    /**
        This class will be used by vector of pointers to frames, so every frame can have its own child-frames (frames inside this frame).
    */
    class FrameBase: public EventBase
    {
    protected:
        /// renderer will be used for rendering
        SDL_Renderer *renderer;
        /// relative viewPort position
        relPosRect relViewPortPos;
        /// coordinates of viewPort which will be used to rendering
        SDL_Rect viewPortPos;
        ///
        SDL_Color clearColor;
        /// Size of the window
        windowSize ws;

        virtual bool setOnlyRelativeViewPort(relPosRect newRelViewPortPos);
    public:
        /// It sets default values.
        FrameBase();
        /// It sets renderer.
        virtual bool setRenderer(SDL_Renderer* r);

        virtual bool setRelativeViewPort(relPosRect newRelViewPortPos);
        virtual bool setViewPortPos(SDL_Rect highterViewPort);
        virtual bool setWindowSize(windowSize winSiz);
        virtual bool setViewPort(relPosRect newRelViewPortPos, SDL_Rect highterViewPort);
        virtual bool setViewPortAndWindow(relPosRect newRelViewPortPos, SDL_Rect highterViewPort, windowSize winSiz);
        virtual bool setViewPortPosAndWindow(SDL_Rect highterViewPort, windowSize winSiz);
        /*
        /// It sets relative viewport and assumes that parent frame is empty.
        virtual bool setRelativeViewPort(relPosRect relViewPortPosition);
        /// It sets relative viewport and evaluates new viewport
        virtual bool setRelativeViewPort(relPosRect relViewPortPosition, SDL_Rect highterViewPort);
        /// It evaluates new view port coordinates, basing on the new parent frame and relative view port coordinates.
        virtual bool setViewPortPos(SDL_Rect highterViewPort);
        ///
        virtual bool setWindowSize(windowSize winSiz);
        */
        /// It is base of rendering method. It does nothing.
        virtual bool render();
    };

    /**
        This class will be used to hold frame and all its child-frames
        (By the way, I am not certain if dividing FrameBase and Frame into two different classes was good idea.)
    */
    class Frame:
        /// This will be used to hold frame
        public FrameBase,
        /// This will be used to hold pointers to all child-frames.
        public std::vector<FrameBase*>
    {
    protected:
    public:
        /// Constructor does almost nothing.
        Frame();
        /// It checks if child-frames should do anything with this event.
        virtual bool checkEvent(const SDL_Event* e);
        /// It copies the pointer to the renderer and do so recursively with child-frames.
        virtual bool setRenderer(SDL_Renderer* r);


        virtual bool setRelativeViewPort(relPosRect newRelViewPortPos);
        virtual bool setViewPortPos(SDL_Rect highterViewPort);
        virtual bool setWindowSize(windowSize winSiz);
        virtual bool setViewPort(relPosRect newRelViewPortPos, SDL_Rect highterViewPort);
        //virtual bool setViewPortAndWindow(relPosRect newRelViewPortPos, SDL_Rect highterViewPort, windowSize winSiz);
        virtual bool setViewPortPosAndWindow(SDL_Rect highterViewPort, windowSize winSiz);

        virtual bool setRecursivlyViewPortPos();
        virtual bool setRecursivlyWindowSize();
        virtual bool setRecursivlyViewPortPosAndWindowSize();

        virtual void push_back(FrameBase* pointer);

        /*
        /// It sets relative viewport and assumes that parent frame is empty.
        virtual bool setRelativeViewPort(relPosRect relViewPortPosition);
        /// It sets relative viewport and evaluates new viewport
        virtual bool setRelativeViewPort(relPosRect relViewPortPosition, SDL_Rect highterViewPort);
        /// It evaluates new view port coordinates, basing on the new parent frame and relative view port coordinates, and it do so recursively with child-frames.
        virtual bool setViewPortPos(SDL_Rect highterViewPort);
        */
        /// It clears render space and calls render() method for all child-frames.
        virtual bool render();
        /// It calls render() method for all child-frames.
        virtual bool renderRecursivly();
    };

    /**
        This class is responsible for holding the window and holding pointers to all frames which will draw on this window.
    */
    class Window: public Frame
    {
    protected:
        /// This variable holds the window.
        SDL_Window *window;

    public:
        /// Constructor only calls parent constructors and set default values.
        Window();
        /// Destroys window and renderer and looses connection to all child frames
        bool clear();
        /// Destroys window and renderer and informs child-frames that renderer is no longer up-to-date
        bool DestroyWindow();
        /// It safety creates new window.
        bool CreateWindow(const char* title, int x, int y, int w, int h,Uint32 flags);
        /// It destroys window and renderer but do not destroys kept frames! You must take care about them yourself! You created them without Window help!
        ~Window();


        /// It clears background, calls recursively rendering functions of child-frames and updates screen.
        virtual bool render();
        /// It takes care about window resize event and calls recursively methods to take care about other events.
        virtual bool checkEvent(const SDL_Event* e);


    };

    class Rectangle: public FrameBase
    {
        SDL_Color c;
    public:
        bool setColor(SDL_Color color);
        bool render();
    };
}


#endif // FRAMES_HPP_INCLUDED
