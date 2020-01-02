#include "frameBase.hpp"

namespace spk
{
    bool myTypeRectangle::correctPos()
    {
        // All points should be between 0% and 100% so I check it and change them to the most similar values if necessary.

        bool correct=true;
        if(x>100.0)
        {
            x=100.0;
            correct=false;
        }
        if(x<0.0)
        {
            x=0.0;
            correct=false;
        }
        if(w<0.0)
        {
            w=0.0;
            correct=false;
        }
        if(x+w>100.0)
        {
            w=100-x;
            correct=false;
        }
        ////////////////////
        // the same for vertical parameters
        if(y>100.0)
        {
            y=100.0;
            correct=false;
        }
        if(y<0.0)
        {
            y=0.0;
            correct=false;
        }
        if(h<0.0)
        {
            h=0.0;
            correct=false;
        }
        if(y+h>100.0)
        {
            h=100-y;
            correct=false;
        }
        return correct;
    }

    //////////////////////////////////////////////////
    // FrameBase

    FrameBase::FrameBase()
    {
        // Default window is no window...
        window=nullptr;
        // It sets default values for other parameters.
        getWindowParam();

        // The background should be white and opaque (nieprzejrzysty).
        clearColor={0xFF,0xFF,0xFF,0xFF};
        // By default the viewport should be the hole parent frame.
        relViewPort={0.0,0.0,100.0,100.0};

        // There is no parent viewPort if there is no parent frame...
        parentViewPort=nullptr;
        // It sets default values.
        evaluateViewPort();
    }
    bool FrameBase::getWindowParam() //----------
    {
        if(window==nullptr)
        {
            // setting default values
            renderer=nullptr;
            windowSize.w=0;
            windowSize.h=0;
            windowID=0;
            return false;
        }
        // This object is supposed to rendering everything.
        renderer=SDL_GetRenderer(window);
        // I fought window dimensions are necessary while using viewPorts. Perhaps I should remove them.
        SDL_GetWindowSize(window,&(windowSize.w),&(windowSize.h));
        // Window ID will be used to checking events.
        windowID=SDL_GetWindowID(window);
        if(renderer==nullptr)
            return false;
        else
            return true;
    }
    bool FrameBase::setWindow(SDL_Window* w) //----------
    {
        // Setting pointer to window...
        window=w;
        // and evaluating connected with it variables.
        return getWindowParam();
    }
    bool FrameBase::setRelativeViewPort(myTypeRectangle pattern)
    {
        // Setting relative viewPort...
        relViewPort=pattern;
        bool success=true;
        // ...and checking if it is correct...
        success= relViewPort.correctPos() && success;
        // ...and evaluating new viewPort basing on relViewPort.
        success= evaluateViewPort() && success;
        return success;
    }
    bool FrameBase::setClearColor(SDL_Color c)
    {
        // setting the color of background
        clearColor=c;
        return true;
    }
    bool FrameBase::setParentViewPort(SDL_Rect* parentViewPortPoiter)
    {
        parentViewPort=parentViewPortPoiter;
        return evaluateViewPort();
    }
    bool FrameBase::evaluateViewPort()
    {
        if(parentViewPort==nullptr)
        {
            viewPort={0,0,0,0};
            return false;
        }
        viewPort.x=parentViewPort->x+parentViewPort->w*relViewPort.x/100.0;
        viewPort.y=parentViewPort->y+parentViewPort->h*relViewPort.y/100.0;
        viewPort.w=parentViewPort->w*relViewPort.w/100.0;
        viewPort.h=parentViewPort->h*relViewPort.h/100.0;
        return true;
    }

    bool FrameBase::render()
    {
        return clearRenderer();
    }
    bool FrameBase::clearRenderer()
    {
        SDL_RenderSetViewport(renderer,&viewPort);
        SDL_SetRenderDrawColor(renderer,clearColor.r,clearColor.g,clearColor.b,clearColor.a);
        SDL_RenderFillRect(renderer,nullptr/*&viewPort*/);
        return true;
    }


}
