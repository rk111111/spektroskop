#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include <string>
#include "frame.hpp"
#include "font.hpp"
#include "textFrame.hpp"

namespace spk
{
    class ButtonBase: public Frame
    {

        TextFrame text;
    protected:
        SDL_Color color[3][3];
        // borderColor - get from FrameBase
        // textColor - get from TextFrame
        // backgroundColor==clearColor - get from TextFrame::FrameBase
        bool pressed;
        bool hover;

    public:
        enum
        {
            NORMAL=0,
            HOVER=1,
            PRESSED=2,
            TEXT=0,
            BACKGROUND=1,
            BORDER=2
        };

    public:
        ButtonBase();


        virtual bool setColor();

        virtual bool setFont(spk::Font* font);
        virtual bool setText(std::string textString);
        virtual bool checkEvent(const SDL_Event* e);
        virtual bool hovered();
        virtual bool setHover();

        virtual bool onClick();
    };
}

#endif // BUTTON_HPP_INCLUDED
