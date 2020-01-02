#include <iostream>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

//#include "eventList.hpp"
#include "window.hpp"
#include "button.hpp"
#include <thread>
#include <ctime>

using namespace std;

unsigned long long int getProgramTime()
{
    return clock()*1000/CLOCKS_PER_SEC;
}

#include "button.hpp"


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    //IMG_Init();

{
    spk::Font f;
    f.setFont("Arial.ttf",100);

    spk::EventVector ev;
    bool running=true;
    spk::EventQuit qt(&running);
    ev.push_back((spk::EventBase*)(&qt));

    spk::Window win;
    win.CreateWindow("tescik",100,100,500,500,SDL_WINDOW_RESIZABLE);
    ev.push_back((spk::EventBase*)(&win));

    spk::Rectangle rect;

    spk::myTypeRectangle r={5,55,90,40};
    SDL_Color c={255,0,0,255};
    rect.setRectangle(&r,&c,false);

    win.push_back((spk::FrameBase*)&rect);


    spk::TextFrame tekscik;
    tekscik.setFont(&f);
    tekscik.setRelativeViewPort({5,30,90,15});
    tekscik.setClearColor({0,0,0xFF,0xFF});
    tekscik.setTextColor({0,0xFF,0,0xFF});
    tekscik.setText("hihi");

    win.push_back((spk::FrameBase*)&tekscik);

    spk::ButtonBase buton;
    buton.setFont(&f);
    buton.setRelativeViewPort({5,5,90,15});
    //buton.setClearColor({0,0,0xFF,0xFF});
    //tekscik.setTextColor({0,0xFF,0,0xFF});
    buton.setText("przycisk");

    win.push_back((spk::FrameBase*)&buton);
    ev.push_back((spk::EventBase*)&buton);


    //thread renderingThread(renderWindow,&win,&running);

    SDL_Event e;
    unsigned int frameRate=60;
    unsigned int deltaTime=1000/frameRate;
    unsigned int czas=getProgramTime()+deltaTime;
    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            ev.checkEvent(&e);
        }
        if(czas<getProgramTime())
        {
            //cout<<czas<<endl;
            czas+=deltaTime;
            win.render();
        }

    }
    //renderingThread.join();


    win.clear();
    f.clear();
}
    //IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    //cout << "Hello world!" << endl;

    return 0;
}
