#include "frames.hpp"

namespace spk
{
    /// Position is presented using percent so the values cant be smaller than 0 and bigger than 100
    bool relPosRect::correctPos()
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

    /// It sets default values.
    FrameBase::FrameBase()
    {
        // The renderer is unknown.
        renderer=nullptr;
        // By default frame contents all available space.
        relViewPortPos.x=0;
        relViewPortPos.y=0;
        relViewPortPos.w=100;
        relViewPortPos.h=100;

        // By default parent frame is empty.
        viewPortPos.x=0;
        viewPortPos.y=0;
        viewPortPos.w=0;
        viewPortPos.h=0;
    }
    /// It sets renderer.
    bool FrameBase::setRenderer(SDL_Renderer* r)
    {
        renderer=r;
        return true;
    }
    /// It sets relative viewport and assumes that parent frame is empty.
    bool FrameBase::setRelativeViewPort(relPosRect relViewPortPosition)
    {
        return FrameBase::setRelativeViewPort(relViewPortPosition,{0,0,0,0});
    }
    /// It sets relative viewport and evaluates new viewport
    bool FrameBase::setRelativeViewPort(relPosRect relViewPortPosition, SDL_Rect highterViewPort)
    {
        // It ensures that values will be correct.
        relViewPortPosition.correctPos();
        // It copies values.
        relViewPortPos=relViewPortPosition;

        // It evaluates new view port coordinates.
        FrameBase::setViewPortPos(highterViewPort);
        return true;
    }
    /// It evaluates new view port coordinates, basing on the new parent frame and relative view port coordinates.
    bool FrameBase::setViewPortPos(SDL_Rect highterViewPort)
    {
        viewPortPos.x=highterViewPort.x+highterViewPort.w*relViewPortPos.x/100.0;
        viewPortPos.w=highterViewPort.w*relViewPortPos.w/100.0;
        viewPortPos.y=highterViewPort.y+highterViewPort.h*relViewPortPos.y/100.0;
        viewPortPos.h=highterViewPort.h*relViewPortPos.h/100.0;
        return true;
    }

    //////////////////////////////////////////////////
    // Frame

    /// Constructor does almost nothing.
    Frame::Frame():FrameBase()
    {

    }
    /// It checks if child-frames should do anything with this event.
    bool Frame::checkEvent(const SDL_Event* e)
    {
        bool success=false;
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success=success||it[0]->checkEvent(e);
        }
        return success;
    }
    /// It copies the pointer to the renderer and do so recursively with child-frames.
    bool Frame::setRenderer(SDL_Renderer* r)
    {
        FrameBase::renderer=r;
        bool success=true;
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success= success&& (*it)->setRenderer(FrameBase::renderer);
        }
        return success;
    }
    /// It sets relative viewport and assumes that parent frame is empty.
    bool Frame::setRelativeViewPort(relPosRect relViewPortPosition)
    {
        return Frame::setRelativeViewPort(relViewPortPosition,{0,0,0,0});
    }
    /// It sets relative viewport and evaluates new viewport
    bool Frame::setRelativeViewPort(relPosRect relViewPortPosition, SDL_Rect highterViewPort)
    {
        relViewPortPosition.correctPos();
        relViewPortPos=relViewPortPosition;
        return Frame::setViewPortPos(highterViewPort);
    }
    /// It evaluates new view port coordinates, basing on the new parent frame and relative view port coordinates, and it do so recursively with child-frames.
    bool Frame::setViewPortPos(SDL_Rect highterViewPort)
    {
        bool success=true;
        success= success&&FrameBase::setViewPortPos(highterViewPort);
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success= success&& (*it)->setViewPortPos(viewPortPos);
        }
        return success;
    }

    //////////////////////////////////////////////////
    // Window

    /// Constructor only calls parent constructors and set default values.
    Window::Window():Frame()
    {
        window=nullptr;
    }

    /// Destroys window and renderer and looses connection to all child frames
    bool Window::clear()
    {
        DestroyWindow();
        std::vector<FrameBase*>::clear();
        return true;
    }
    /// Destroys window and renderer and informs child-frames that renderer is no longer up-to-date
    bool Window::DestroyWindow()
    {
        // destroy renderer only if it is necessary
        if(FrameBase::renderer!=nullptr)
        {
            // Frame::setRenderer(nullptr); should set renderer value to nullptr so avoid memory lack by keeping it values
            SDL_Renderer* bufor=FrameBase::renderer;
            // inform another frames that renderer is no longer up-to-date
            Frame::setRenderer(nullptr);
            // destroy renderer
            SDL_DestroyRenderer(bufor);
            // it should be done by Frame::setRenderer(nullptr); but if something goes wrong...
            FrameBase::renderer=nullptr;
        }
        // destroy window only if it is necessary
        if(window!=nullptr)
        {
            // destroy window
            SDL_DestroyWindow(window);
            // ...and ensure no one will access it again
            window=nullptr;
        }
        return true;
    }

    /// It safety creates new window.
    bool Window::CreateWindow(const char* title, int x, int y, int w, int h,Uint32 flags)
    {
        // destroy old window, ensure there will be no memory leak
        DestroyWindow();
        // create window
        window=SDL_CreateWindow(title,x,y,w,h,flags);
        if(window==nullptr) // if something goes wrong - do not continue
            return false;
        // create renderer
        FrameBase::renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
        if(FrameBase::renderer==nullptr) // if something goes wrong - do not continue
            return false;
        // inform another frames about new renderer and return success status
        return Frame::setRenderer(FrameBase::renderer);
    }

    /// It destroys window and renderer but do not destroys kept frames! You must take care about them yourself! You created them without Window help!
    Window::~Window()
    {
        // window and renderer are only dangerous pointers and they are taken care by DestroyWindow()
        DestroyWindow();
    }
}
