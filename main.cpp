#include <iostream>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

//#include "eventList.hpp"
#include "frames.hpp"
#include <thread>

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

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

    SDL_Quit();
    cout << "Hello world!" << endl;
    return 0;
}
