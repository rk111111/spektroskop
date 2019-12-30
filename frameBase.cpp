#include "frameBase.hpp"

namespace spk
{
    bool myTypeRectangle::correctPos()
    {
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
        window=nullptr;
        getWindowParam();

        clearColor={0x00,0x00,0x00,0xFF};
        relViewPort={0.0,0.0,100.0,100.0};

        parentViewPort=nullptr;
        evaluateViewPort();
    }
    bool FrameBase::getWindowParam()
    {
        if(window==nullptr)
        {
            renderer=nullptr;
            windowSize.w=0;
            windowSize.h=0;
            windowID=0;
            return false;
        }
        renderer=SDL_GetRenderer(window);
        SDL_GetWindowSize(window,&(windowSize.w),&(windowSize.h));
        windowID=SDL_GetWindowID(window);
        if(renderer==nullptr)
            return false;
        else
            return true;
    }
    bool FrameBase::setWindow(SDL_Window* w)
    {
        window=w;
        return getWindowParam();
    }
    bool FrameBase::setRelativeViewPort(myTypeRectangle pattern)
    {
        relViewPort=pattern;
        bool success=true;
        success= relViewPort.correctPos() && success;
        success= evaluateViewPort() && success;
        return success;
    }
    bool FrameBase::setClearColor(SDL_Color c)
    {
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
        SDL_RenderFillRect(renderer,&viewPort);
        return true;
    }


}
