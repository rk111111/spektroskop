#include "frames.hpp"

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
        return relViewPort.correctPos();
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

    //////////////////////////////////////////////////
    // Frame

    Frame::Frame():FrameBase()
    {

    }
    bool Frame::setWindow(SDL_Window* w)
    {
        return FrameBase::setWindow(w) && Frame::setWindowRecursively();
    }

    bool Frame::setWindowRecursively()
    {
        bool success=true;
        for(vecPoitFrameBase::iterator it=vecPoitFrameBase::begin(); it<vecPoitFrameBase::end();it++)
        {
            success=success && it[0]->setWindow(window);
        }
        return success;
    }

    bool Frame::setParentViewPort(SDL_Rect* parentViewPortPoiter)
    {
        return FrameBase::setParentViewPort(parentViewPortPoiter) && Frame::setParentViewPortRecursively();
    }
    bool Frame::setParentViewPortRecursively()
    {
        bool success=true;
        for(vecPoitFrameBase::iterator it=vecPoitFrameBase::begin(); it<vecPoitFrameBase::end();it++)
        {
            success=success && it[0]->setParentViewPort(&viewPort);
        }
        return success;
    }

    void Frame::push_back(FrameBase* pointer)
    {
        vecPoitFrameBase::push_back(pointer);
        pointer->setParentViewPort(&viewPort);
        pointer->setWindow(window);
    }
    bool Frame::remove(FrameBase* pointer)
    {
        vecPoitFrameBase::iterator it=begin();
        for(;it<vecPoitFrameBase::end() && *it !=pointer;it++);
        if(it==vecPoitFrameBase::end())
            return false;
        pointer->setParentViewPort(nullptr);
        pointer->setWindow(nullptr);
        for(;it+1<vecPoitFrameBase::end();it++)
        {
            it[0]=it[1];
        }
        vecPoitFrameBase::pop_back();
        return true;
    }
    void Frame::pop_back()
    {
        if(vecPoitFrameBase::size()>0)
        {
            (*(vecPoitFrameBase::end()-1))->setParentViewPort(nullptr);
            (*(vecPoitFrameBase::end()-1))->setWindow(nullptr);
            vecPoitFrameBase::pop_back();
        }
    }
    void Frame::clear()
    {
        while(vecPoitFrameBase::size()>0)
            Frame::pop_back();
    }

    bool Frame::render()
    {
        return clearRenderer() && renderRecursively();
    }
    bool Frame::renderRecursively()
    {
        bool success=true;
        for(vecPoitFrameBase::iterator it=vecPoitFrameBase::begin(); it<vecPoitFrameBase::end();it++)
        {
            success=success && it[0]->render();
        }
        return success;
    }
    bool Frame::checkEvent(const SDL_Event *e)
    {
        return checkEventRecursively(e);
    }
    bool Frame::checkEventRecursively(const SDL_Event *e)
    {
        bool success=false;
        for(vecPoitFrameBase::iterator it=vecPoitFrameBase::begin(); it<vecPoitFrameBase::end();it++)
        {
            success=success||it[0]->checkEvent(e);
        }
        return success;
    }

    //////////////////////////////////////////////////
    // Window

    Window::Window():Frame()
    {

    }
    bool Window::DestroyWindow()
    {
        if(window==nullptr)
            return false;
        SDL_Window* wbufor;
        SDL_Renderer* rbufor;
        wbufor=window;
        rbufor=renderer;
        Frame::setWindow(nullptr);
        SDL_DestroyRenderer(rbufor);
        SDL_DestroyWindow(wbufor);

        /// It should be done by "Frame::setWindow(nullptr)"
        window=nullptr;     renderer=nullptr;
        Window::setViewPort();
        return true;
    }
    bool Window::setViewPort()
    {
        if(window==nullptr)
            viewPort={0,0,0,0};
        else
        {
            viewPort.x=0;
            viewPort.y=0;
            SDL_GetWindowSize(window,&viewPort.w,&viewPort.h);
        }
        return Frame::setParentViewPortRecursively();
    }
    void Window::clear()
    {
        DestroyWindow();
        Frame::clear();
    }

    bool Window::CreateWindow(const char* title, int x, int y, int w, int h,Uint32 flags)
    {
        DestroyWindow();
        SDL_Window* wbufor;
        wbufor=SDL_CreateWindow(title,x,y,w,h,flags);
        SDL_CreateRenderer(wbufor,-1,SDL_RENDERER_ACCELERATED);
        return Frame::setWindow(window) && Window::setViewPort();

    }

    Window::~Window()
    {
        Window::clear();
    }

    bool Window::render()
    {
        SDL_RenderSetViewport(FrameBase::renderer,&(FrameBase::viewPort));
        SDL_SetRenderDrawColor(FrameBase::renderer,clearColor.r,clearColor.g,clearColor.b,clearColor.a);
        SDL_RenderClear(FrameBase::renderer);
        bool success=Frame::renderRecursively();
        SDL_RenderPresent(FrameBase::renderer);
        return success;
    }

    bool Window::checkEvent(const SDL_Event *e)
    {
        bool success=false;
        if(e->type==SDL_WINDOWEVENT
           && e->window.event==SDL_WINDOWEVENT_SIZE_CHANGED
           && e->window.windowID==FrameBase::windowID)
        {
            success=Frame::setWindow(window);
        }
        return success||Frame::checkEvent(e);
    }

}
