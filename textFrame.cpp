#include "textFrame.hpp"

namespace spk
{

    TextFrame::TextFrame():FrameBase()
    {
        font=nullptr;
        textTexture=nullptr;
        wOfText=0;
        hOfText=0;
        Position={0,0,0,0};
        textColor={0x00,0x00,0x00,0xFF};
    }
    bool TextFrame::setFont(Font* font)
    {
        TextFrame::font=font;
        createTextureFromText();
        return true;
    }
    bool TextFrame::setText(std::string textString)
    {
        TextFrame::text=textString;
        createTextureFromText();
        return true;
    }
    bool TextFrame::setTextColor(SDL_Color color)
    {
        textColor=color;
        createTextureFromText();
        return true;
    }
    bool TextFrame::setTextAndTextColor(std::string textString,SDL_Color color)
    {
        TextFrame::text=textString;
        textColor=color;
        createTextureFromText();
        return true;
    }

    bool TextFrame::createTextureFromText()
    {
        destroyTexture();
        if(font==nullptr)
            return false;
        if(FrameBase::renderer==nullptr)
            return false;
        SDL_RenderSetViewport(FrameBase::renderer,nullptr);
        SDL_Surface* textSurface=nullptr;
        textSurface =TTF_RenderText_Blended(font->getFont(),text.c_str(),textColor);
        textTexture =SDL_CreateTextureFromSurface(FrameBase::renderer,textSurface);
        SDL_FreeSurface(textSurface);
        textSurface=nullptr;
        SDL_QueryTexture(textTexture,nullptr,nullptr,&wOfText,&hOfText);

        setPosition();
        return true;
    }

    bool TextFrame::destroyTexture()
    {
        if(textTexture==nullptr)
            return false;
        SDL_DestroyTexture(textTexture);
        textTexture=nullptr;
        wOfText=0;
        hOfText=0;
        setPosition();
        return true;
    }

    bool TextFrame::clear()
    {
        destroyTexture();
        font=nullptr;
        text="";
        textColor={0x00,0x00,0x00,0xFF};
        return true;
    }

    bool TextFrame::setPosition()
    {
        if(wOfText==0 || hOfText==0 || viewPort.h==0 || viewPort.w==0)
        {
            Position={0,0,0,0};
        }
        else
        {
            if(hOfText*viewPort.w<wOfText*viewPort.h)
            {
                Position.x=0;
                //Position.w=wOfText * (viewPort.w/wOfText) * (viewPort.w/viewPort.w);
                Position.w=viewPort.w;
                //Position.h=hOfText * (viewPort.w/wOfText) * (viewPort.h/viewPort.h);
                Position.h=hOfText*viewPort.w/wOfText;

                Position.y=(viewPort.h-Position.h)/2;
            }
            else
            {
                Position.y=0;

                //Position.w=wOfText * (viewPort.h/hOfText) * (viewPort.w/viewPort.w);
                Position.w=wOfText*viewPort.h/hOfText;
                //Position.h=hOfText * (viewPort.h/hOfText) * (viewPort.h/viewPort.h);
                Position.h=viewPort.h;

                Position.x=(viewPort.w-Position.w)/2;
            }
        }
        return true;
    }

    bool TextFrame::evaluateViewPort()
    {
        bool success=true;
        success= FrameBase::evaluateViewPort() && success;
        success= setPosition() && success;
        return success;
    }
    bool TextFrame::getWindowParam()
    {
        bool success=true;
        success= FrameBase::getWindowParam() && success;
        success= TextFrame::createTextureFromText() && success;
        success= setPosition() && success;
        return success;
    }

    bool TextFrame::render()
    {
        FrameBase::clearRenderer();

        SDL_RenderCopy(FrameBase::renderer,TextFrame::textTexture,nullptr, &(TextFrame::Position));
        return true;
    }

    TextFrame::~TextFrame()
    {
        clear();
    }
}
