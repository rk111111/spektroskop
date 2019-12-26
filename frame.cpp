#include "frame.hpp"

namespace spk
{
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
}
