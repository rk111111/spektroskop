#include "button.hpp"

#include <iostream>

#define DEBUG3
using namespace std;

namespace spk
{


    //////////////////////////////////////////////////
    // ButtonBase


    ButtonBase::ButtonBase():Frame(),text()
    {
        pressed=false;
        hover=false;
        color[NORMAL][BACKGROUND]={0x99,0x99,0x99,0xFF};
        color[NORMAL][TEXT]={0x00,0x00,0x00,0xFF};
        color[NORMAL][BORDER]={0xBB,0xBB,0xBB,0xFF};
        color[HOVER][BACKGROUND]={0xBB,0xBB,0xBB,0xFF};
        color[HOVER][TEXT]={0x22,0x22,0x22,0xFF};
        color[HOVER][BORDER]={0xDD,0xDD,0xDD,0xFF};
        color[PRESSED][BACKGROUND]={0xDD,0xDD,0xDD,0xFF};
        color[PRESSED][TEXT]={0x44,0x44,0x44,0xFF};
        color[PRESSED][BORDER]={0xFF,0xFF,0xFF,0xFF};
        Frame::push_back((FrameBase*)&text);
        setColor();
        text.setRelativeViewPort({5.0,5.0,90.0,90.0});
        text.setParentViewPort(&(FrameBase::viewPort));
    }

    bool ButtonBase::setColor()
    {
        if(pressed)
        {
            text.setClearColor(color[PRESSED][BACKGROUND]);
            text.setTextColor(color[PRESSED][TEXT]);
            FrameBase::clearColor =color[PRESSED][BORDER];
        }
        else if(hover)
        {
            text.setClearColor(color[HOVER][BACKGROUND]);
            text.setTextColor(color[HOVER][TEXT]);
            FrameBase::clearColor =color[HOVER][BORDER];
        }
        else
        {
            text.setClearColor(color[NORMAL][BACKGROUND]);
            text.setTextColor(color[NORMAL][TEXT]);
            FrameBase::clearColor =color[NORMAL][BORDER];
        }
        return true;
    }

    bool ButtonBase::setFont(spk::Font* font)
    {
        return text.setFont(font);
    }
    bool ButtonBase::setText(std::string textString)
    {
        return text.setText(textString);
    }

    bool ButtonBase::checkEvent(const SDL_Event* e)
    {
        if(e->type==SDL_MOUSEMOTION)
        {
            if(e->motion.windowID==windowID)
            {
                setHover();
            }
        }
        else if(e->type==SDL_MOUSEBUTTONDOWN)
        {
            if(!pressed)
            {
                if(hovered())
                {
                    pressed=true;
                    if(!setHover())
                        setColor();
                    return onClick();
                }
            }
        }
        else if(e->type==SDL_MOUSEBUTTONUP)
        {
            if(pressed)
            {
                pressed=false;
                setColor();
                return true;
            }
        }
        return false;
    }
    bool ButtonBase::hovered()
    {
        int x,y;
        SDL_GetMouseState(&x,&y);
        if(viewPort.x<x &&
                      x<viewPort.x+viewPort.w &&
           viewPort.y<y &&
                      y<viewPort.y+viewPort.h)
        {
            return true;
        }
        return false;
    }
    bool ButtonBase::setHover()
    {
        if(!hover && hovered())
        {
            hover=true;
            setColor();
            return true;
        }
        else if(hover && !hovered())
        {
            hover=false;
            setColor();
            return true;
        }
        return false;
    }
    bool ButtonBase::onClick()
    {
        return true;
    }


}
