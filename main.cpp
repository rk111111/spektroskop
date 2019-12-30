#include <iostream>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

//#include "eventList.hpp"
#include "window.hpp"
#include <thread>

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    spk::EventVector ev;
    bool running=true;
    spk::EventQuit qt(&running);
    ev.push_back((spk::EventBase*)(&qt));

    spk::Window win;
    win.CreateWindow("tescik",100,100,300,300,SDL_WINDOW_RESIZABLE);
    /*cout<<"Window ViewPort"<<endl;
    cout<<"x="<<win.viewPort.x<<" y="<<win.viewPort.y<<" w="<<win.viewPort.w<<" h="<<win.viewPort.h<<endl<<endl;*/
    ev.push_back((spk::EventBase*)(&win));

    spk::Rectangle rect;
    /*cout<<"ViewPort"<<endl;
    cout<<"x="<<rect.viewPort.x<<" y="<<rect.viewPort.y<<" w="<<rect.viewPort.w<<" h="<<rect.viewPort.h<<endl;
    cout<<"relViewPort"<<endl;
    cout<<"x="<<rect.relViewPort.x<<" y="<<rect.relViewPort.y<<" w="<<rect.relViewPort.w<<" h="<<rect.relViewPort.h<<endl<<endl;*/

    spk::myTypeRectangle r={25,25,50,25};
    SDL_Color c={255,0,0,255};
    rect.setRectangle(&r,&c,false);
    //rect.setRelativeViewPort({0,0,100,100});

    win.push_back((spk::FrameBase*)&rect);
    /*cout<<"ViewPort 2"<<endl;
    cout<<"x="<<rect.viewPort.x<<" y="<<rect.viewPort.y<<" w="<<rect.viewPort.w<<" h="<<rect.viewPort.h<<endl;
    cout<<"relViewPort 2"<<endl;
    cout<<"x="<<rect.relViewPort.x<<" y="<<rect.relViewPort.y<<" w="<<rect.relViewPort.w<<" h="<<rect.relViewPort.h<<endl;
    cout<<"rect 2"<<endl;
    cout<<"x="<<rect.RectToDraw.x<<" y="<<rect.RectToDraw.y<<" w="<<rect.RectToDraw.w<<" h="<<rect.RectToDraw.h<<endl<<endl;*/

    SDL_Event e;
    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            ev.checkEvent(&e);
        }
        win.render();
    }


    win.clear();
    /*
    //SDL_Window *w=SDL_CreateWindow("tescik",100,100,300,300,0);
    spk::Window w;
    spk::Rectangle rect;
    rect.setColor({0xFF,0,0,0xFF});
    rect.setRelativeViewPort({25.0,25.0,50.0,50.0});

    w.CreateWindow("tescik",100,100,300,300,SDL_WINDOW_RESIZABLE);
    w.push_back((spk::FrameBase*)(&rect));


    spk::EventVector ev;
    bool running=true;
    spk::EventQuit qt(&running);
    ev.push_back((spk::EventBase*)(&qt));
    ev.push_back((spk::EventBase*)(&w));

    SDL_Event e;
    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            //std::cout<<e.type<<endl;
            ev.checkEvent(&e);
        }
        //cout<<"1";
        w.render();

    }
    ev.clear();
    w.DestroyWindow();
    //SDL_DestroyWindow(w);
    */
    SDL_Quit();
    cout << "Hello world!" << endl;
    return 0;
}
