#ifndef FONT_HPP_INCLUDED
#define FONT_HPP_INCLUDED

#include <string>
#include <SDL2/SDL_ttf.h>

namespace spk
{

    class Font
    {
        TTF_Font* font;
        //FontQuality quality;
        //int style;
    public:
        Font();
        Font(const std::string& path, int size);
        //Font(const std::string& path, int size, FontQuality quality, int style);
        bool setFont(const std::string& path, int size);
        //void setQuality(FontQuality quality);
        //void setStyle(int style);
        TTF_Font* getFont();
        bool clear();

        ~Font();
    };


}

#endif // FONT_HPP_INCLUDED
