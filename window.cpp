#include "window.hpp"

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
