#include "window.hpp"

using namespace std;


namespace spk
{
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
        {
            viewPort={0,0,0,0};
        }
        else
        {
            viewPort.x=0;
            viewPort.y=0;
            SDL_GetWindowSize(window,&(viewPort.w),&(viewPort.h));
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
        bool success=true;
        success= Frame::setWindow(wbufor) && success;
        success= Window::setViewPort() && success;
        return success;
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
            success=Window::setViewPort() && success;
        }
        success = Frame::checkEvent(e) || success;
        return success;
    }

    //////////////////////////////////////////////////
    // Rectangle

    bool Rectangle::setRectangle(myTypeRectangle *rect, SDL_Color *color, bool setDefault)
    {
        if(rect!=nullptr)
        {
            relRect.x=rect->x;
            relRect.y=rect->y;
            relRect.w=rect->w;
            relRect.h=rect->h;
        }
        else if(setDefault)
        {
            relRect.x=0.0;
            relRect.y=0.0;
            relRect.w=0.0;
            relRect.h=0.0;
        }
        if(color!=nullptr)
        {
            rectColor.r=color->r;
            rectColor.g=color->g;
            rectColor.b=color->b;
            rectColor.a=color->a;
        }
        else if(setDefault)
        {
            rectColor.r=0x00;
            rectColor.g=0x00;
            rectColor.b=0x00;
            rectColor.a=0xFF;
        }
        return true;
    }
    bool Rectangle::setRelativeViewPort(myTypeRectangle pattern)
    {
        bool success=true;
        success= FrameBase::setRelativeViewPort(pattern) && success;
        success= spk::evaluateRectangle(&viewPort, &relRect, &RectToDraw) && success;
        return success;
    }
    bool Rectangle::setParentViewPort(SDL_Rect* parentViewPortPointer)
    {
        bool success=true;
        success= FrameBase::setParentViewPort(parentViewPortPointer) && success;
        success= spk::evaluateRectangle(&viewPort, &relRect, &RectToDraw) && success;
        return  success;
    }
    bool Rectangle::render()
    {
        clearRenderer();
        SDL_SetRenderDrawColor(FrameBase::renderer,rectColor.r,rectColor.g,rectColor.b,rectColor.a);
        SDL_RenderFillRect(FrameBase::renderer,&RectToDraw);
        return true;
    }

    bool evaluateRectangle(const SDL_Rect *viewPort, const myTypeRectangle *base, SDL_Rect *target)
    {
        if(target==nullptr)
            return false;
        if(viewPort==nullptr)
        {
            *target={0,0,0,0};
            return false;
        }
        if(base==nullptr)
        {
            target->x=viewPort->x;
            target->y=viewPort->y;
            target->w=viewPort->w;
            target->h=viewPort->h;
            return false;
        }
        target->x=viewPort->x+viewPort->w*base->x/100.0;
        target->y=viewPort->y+viewPort->h*base->y/100.0;
        target->w=viewPort->w*base->w/100.0;
        target->h=viewPort->h*base->h/100.0;
        return true;
    }





}
