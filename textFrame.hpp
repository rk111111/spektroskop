#ifndef TEXTFRAME_H_INCLUDED
#define TEXTFRAME_H_INCLUDED

#include "frameBase.hpp"
#include "font.hpp"
namespace spk
{
    enum FontQuality
    {
        TTF_EXTENSION_SOLID=0,
        TTF_EXTENSION_SHADED=1,
        TTF_EXTENSION_BLENDED=2
    };
    class TextFrame: public FrameBase
    {
    protected:
        Font* font;

        SDL_Color textColor;
        std::string text;
        //FontQuality quality;

        SDL_Texture* textTexture;
        int wOfText;
        int hOfText;
        SDL_Rect Position;
    public:
        TextFrame();
        ~TextFrame();
        virtual bool setFont(spk::Font* font);
        virtual bool setText(std::string textString);
        virtual bool setTextColor(SDL_Color color);
        virtual bool setTextAndTextColor(std::string textString,SDL_Color color);

        virtual bool createTextureFromText();
        virtual bool destroyTexture();
        virtual bool clear();

        virtual bool evaluateViewPort();
        virtual bool getWindowParam();

        virtual bool setPosition();

        virtual bool render();
    };
}

#endif // TEXTFRAME_H_INCLUDED
