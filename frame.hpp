#ifndef FRAME_HPP_INCLUDED
#define FRAME_HPP_INCLUDED

namespace spk
{
    class Frame;
}

#include "frameBase.hpp"

namespace spk
{
    class Frame: public FrameBase, protected std::vector<FrameBase*>
    {
    protected:

    public:
        Frame();
        virtual bool setWindow(SDL_Window* w);
        virtual bool setWindowRecursively();
        virtual bool setParentViewPort(SDL_Rect* parentViewPortPoiter);
        virtual bool setParentViewPortRecursively();
        virtual void push_back(FrameBase* pointer);
        virtual bool remove(FrameBase* pointer);
        virtual void pop_back();
        virtual void clear();

        virtual bool render();
        virtual bool renderRecursively();
        virtual bool checkEvent(const SDL_Event *e);
        virtual bool checkEventRecursively(const SDL_Event *e);
    };
}

#endif // FRAME_HPP_INCLUDED
