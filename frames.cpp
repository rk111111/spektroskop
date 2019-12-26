#include "frames.hpp"

//#define DEBUG
#define DEBUG2

using namespace std;

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

        // By default background color will be black...
        clearColor.r=0x00;
        clearColor.g=0x00;
        clearColor.b=0x00;
        // ... and it will cover everything by 100% (0xFF=255 => 255/255=1)
        clearColor.a=0xFF;
    }
    /// It sets renderer.
    bool FrameBase::setRenderer(SDL_Renderer* r)
    {
        renderer=r;
        return true;
    }
    bool FrameBase::setOnlyRelativeViewPort(relPosRect newRelViewPortPos)
    {
        newRelViewPortPos.correctPos();
        relViewPortPos=newRelViewPortPos;
        return true;
    }
    bool FrameBase::setRelativeViewPort(relPosRect newRelViewPortPos)
    {
        return FrameBase::setOnlyRelativeViewPort(newRelViewPortPos) && FrameBase::setViewPortPos({0,0,0,0});
    }
    bool FrameBase::setViewPortPos(SDL_Rect highterViewPort)
    {
        viewPortPos.x=highterViewPort.x+highterViewPort.w*relViewPortPos.x/100.0;
        viewPortPos.w=highterViewPort.w*relViewPortPos.w/100.0;
        viewPortPos.y=highterViewPort.y+highterViewPort.h*relViewPortPos.y/100.0;
        viewPortPos.h=highterViewPort.h*relViewPortPos.h/100.0;
        return true;
    }
    bool FrameBase::setWindowSize(windowSize winSiz)
    {
        ws=winSiz;
        return true;
    }
    bool FrameBase::setViewPort(relPosRect newRelViewPortPos, SDL_Rect highterViewPort)
    {
        return FrameBase::setOnlyRelativeViewPort(newRelViewPortPos) && FrameBase::setViewPortPos(highterViewPort);
    }
    bool FrameBase::setViewPortAndWindow(relPosRect newRelViewPortPos, SDL_Rect highterViewPort, windowSize winSiz)
    {
        return FrameBase::setOnlyRelativeViewPort(newRelViewPortPos) && FrameBase::setViewPortPos(highterViewPort) && setWindowSize(winSiz);
    }
    bool FrameBase::setViewPortPosAndWindow(SDL_Rect highterViewPort, windowSize winSiz)
    {
        return FrameBase::setViewPortPos(highterViewPort) && FrameBase::setWindowSize(winSiz);
    }
    /*
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
        #ifdef DEBUG2
        std::cout<<"ViewPort:"<<std::endl;
        std::cout<<" x="<<viewPortPos.x<<" y="<<viewPortPos.y<<" w="<<viewPortPos.w<<" h="<<viewPortPos.h<<std::endl;
        std::cout<<"End of view port"<<std::endl;
        #endif // DEBUG2
        return true;
    }
    */
    /// It is base of rendering method. It does nothing.
    bool FrameBase::render()
    {
        /*
        #ifdef DEBUG
        std::cout<<"x="<<viewPortPos.x<<" y="<<viewPortPos.y<<" w="<<viewPortPos.w<<" h="<<viewPortPos.h<<std::endl;
        #endif // DEBUG
        */
        SDL_RenderSetViewport(FrameBase::renderer,&viewPortPos);
        SDL_SetRenderDrawColor(FrameBase::renderer,clearColor.r,clearColor.g,clearColor.b,clearColor.a);
        SDL_RenderFillRect(FrameBase::renderer,&viewPortPos);

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

    bool Frame::setRelativeViewPort(relPosRect newRelViewPortPos)
    {
        return FrameBase::setRelativeViewPort(newRelViewPortPos) && Frame::setRecursivlyViewPortPos();
    }
    bool Frame::setViewPortPos(SDL_Rect highterViewPort)
    {
        return FrameBase::setViewPortPos(highterViewPort) && Frame::setRecursivlyViewPortPos();
    }
    bool Frame::setWindowSize(windowSize winSiz)
    {
        return FrameBase::setWindowSize(winSiz) && Frame::setRecursivlyWindowSize();
    }
    bool Frame::setViewPort(relPosRect newRelViewPortPos, SDL_Rect highterViewPort)
    {
        return FrameBase::setViewPort(newRelViewPortPos,highterViewPort) && Frame::setRecursivlyViewPortPos();
    }
    bool Frame::setViewPortPosAndWindow(SDL_Rect highterViewPort, windowSize winSiz)
    {
        return FrameBase::setViewPortPos(highterViewPort) && FrameBase::setWindowSize(winSiz) && Frame::setRecursivlyViewPortPosAndWindowSize();
    }
    bool Frame::setRecursivlyViewPortPos()
    {
        bool success=true;
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success= success && (*it)->setViewPortPos(FrameBase::viewPortPos);
        }
        return success;
    }
    bool Frame::setRecursivlyWindowSize()
    {
        bool success=true;
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success= success && (*it)->setWindowSize(FrameBase::ws);
        }
        return success;
    }
    bool Frame::setRecursivlyViewPortPosAndWindowSize()
    {
        bool success=true;
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success= success && (*it)->setViewPortPosAndWindow(FrameBase::viewPortPos,FrameBase::ws);
        }
        return success;
    }

    void Frame::push_back(FrameBase* pointer)
    {
        std::vector<FrameBase*>::push_back(pointer);
        if(std::vector<FrameBase*>::size()>0)
        {
            pointer->setViewPortPosAndWindow(FrameBase::viewPortPos,FrameBase::ws);
            pointer->setRenderer(FrameBase::renderer);
        }
        //cout<<" x="<<viewPortPos.x<<" y="<<viewPortPos.y<<" w="<<viewPortPos.w<<" h="<<viewPortPos.h<<endl;
        //cout<<"w="<<ws.x<<" h="<<ws.y<<endl;

    }

    /*
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
    */
    /// It clears render space and calls render() method for all child-frames.
    bool Frame::render()
    {
        return FrameBase::render() && Frame::renderRecursivly();
    }
    /// It calls render() method for all child-frames.
    bool Frame::renderRecursivly()
    {
        bool success=true;
        for(std::vector<FrameBase*>::iterator it=begin();it<end();it++)
        {
            success= success&& (*it)->render();
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
        Frame::setRelativeViewPort({0.0,0.0,100.0,100.0});
        // destroy renderer only if it is necessary
        if(FrameBase::renderer!=nullptr)
        {
            // "Frame::setRenderer(nullptr)" should set renderer value to nullptr so I avoid memory lack by keeping it values
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
        // create renderer, as far as I know SDL_RENDERER_ACCELERATED is responsible for using GPU instead of CPU
        FrameBase::renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
        if(FrameBase::renderer==nullptr) // if something goes wrong - do not continue
            return false;

        return Frame::setRenderer(FrameBase::renderer)
        && FrameBase::setViewPort({0.0,0.0,100.0,100.0},{0,0,w,h})
        && FrameBase::setWindowSize({w,h})
        && Frame::setRecursivlyViewPortPosAndWindowSize();

        // this was previous construction:
        // inform another frames about new renderer and about new frame size and return success status
        //return Frame::setRenderer(FrameBase::renderer) && Frame::setRelativeViewPort({0.0,0.0,100.0,100.0},{0,0,w,h});
    }

    /// It destroys window and renderer but do not destroys kept frames! You must take care about them yourself! You created them without Window help!
    Window::~Window()
    {
        // window and renderer are only dangerous pointers and they are taken care by DestroyWindow()
        DestroyWindow();
    }

    /// It clears background, calls recursively rendering functions of child-frames and updates screen.
    bool Window::render()
    {
        SDL_RenderSetViewport(FrameBase::renderer,&viewPortPos);
        SDL_SetRenderDrawColor(FrameBase::renderer,clearColor.r,clearColor.g,clearColor.b,clearColor.a);
        SDL_RenderClear(FrameBase::renderer);
        bool success=Frame::renderRecursivly();
        SDL_RenderPresent(FrameBase::renderer);
        return success;
    }
    /// It takes care about window resize event and calls recursively methods to take care about other events.
    bool Window::checkEvent(const SDL_Event* e)
    {
        bool success=false;
        if(e->type==SDL_WINDOWEVENT
           && e->window.event==SDL_WINDOWEVENT_SIZE_CHANGED
           && e->window.windowID==SDL_GetWindowID(window))
        {
            success=true;
            SDL_GetWindowSize(window,&FrameBase::ws.x,&FrameBase::ws.y);
            FrameBase::setViewPortPos({0,0,ws.x,ws.y});
            Frame::setRecursivlyViewPortPosAndWindowSize();
            //Frame::setRelativeViewPort({0.0,0.0,100.0,100.0},{0,0,e->window.data1,e->window.data2});
        }
        return success||Frame::checkEvent(e);
    }


    //////////////////////////////////////////////////
    // Rectangle

    bool Rectangle::setColor(SDL_Color color)
    {
        c=color;
        return true;
    }
    bool Rectangle::render()
    {
        FrameBase::render();
        #ifdef DEBUG
        std::cout<<"x="<<viewPortPos.x<<" y="<<viewPortPos.y<<" w="<<viewPortPos.w<<" h="<<viewPortPos.h<<std::endl;
        #endif // DEBUG
        //std::cout<<"a";
        SDL_RenderSetViewport(FrameBase::renderer,&viewPortPos);
        SDL_SetRenderDrawColor(FrameBase::renderer,c.r,c.g,c.b,c.a);
        SDL_Rect rysunek;

        //cout<<"Rysunek:"<<endl;
        //cout<<"x="<<viewPortPos.x<<" y="<<viewPortPos.y<<" w="<<viewPortPos.w<<" h="<<viewPortPos.h<<endl;
        //cout<<"w="<<ws.x<<" h="<<ws.y<<endl;
        //cout<<"Renderer="<<(int)FrameBase::renderer<<endl;

        rysunek.x=ws.y/4;
        rysunek.y=ws.x/4;
        rysunek.h=ws.y/4;
        rysunek.w=ws.x/4;
        //SDL_GetWindowSize(SDL_getWindow)
        SDL_RenderFillRect(FrameBase::renderer,&rysunek);
        return true;
    }

}
