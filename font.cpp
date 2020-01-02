#include "font.hpp"

namespace spk
{
    Font::Font()
    {
        font=nullptr;
        //quality=TTF_EXTENSION_SOLID;
        //style=0;
    }
    Font::Font(const std::string& path, int size)
    {
        font=TTF_OpenFont(path.c_str(),size);  //setFont(path,size);
        //quality=TTF_EXTENSION_SOLID;
        //style=0;
    }
    /*Font::Font(const std::string& path, int size, FontQuality quality, int style)
    {
        //Font::quality=quality;
        //Font::style=style;
        //setFont(path,size);
    }*/
    bool Font::setFont(const std::string& path, int size)
    {
        clear();
        font=TTF_OpenFont(path.c_str(),size);
        if(font==nullptr)
            return false;
        else
        //TTF_SetFontStyle(font,style);
            return true;
    }
    /*void Font::setQuality(FontQuality quality)
    {
        Font::quality=quality;
    }
    void Font::setStyle(int style)
    {
        Font::style=style;
        if(font!=nullptr)
        {
            TTF_SetFontStyle(font,style);
        }
    }*/
    TTF_Font* Font::getFont()
    {
        return font;
    }
    bool Font::clear()
    {
        if(font!=nullptr)
        {
            TTF_CloseFont(font);
            font=nullptr;
        }
        return true;
    }

    Font::~Font()
    {
        if(font!=nullptr)
        {
            TTF_CloseFont(font);
            font=nullptr;
        }
    }
}
