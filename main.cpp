#include <iostream>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

//#include "eventList.hpp"
#include "window.hpp"
#include "button.hpp"
#include <thread>

using namespace std;

void renderWindow(spk::Window* window, bool* running)
{
    if(running==nullptr)
    {
        cerr<<"Przekazano pusty wskaznik na zmienna podtzrymujaca pentle glowna"<<endl;
        return;
    }
    while(*running)
    {
        window->render();
        std::this_thread::sleep_for(16ms);
    }
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    spk::EventVector ev;
    bool running=true;
    spk::EventQuit qt(&running);
    ev.push_back((spk::EventBase*)(&qt));

    spk::Window win;
    win.CreateWindow("tescik",100,100,300,300,SDL_WINDOW_RESIZABLE);
    ev.push_back((spk::EventBase*)(&win));

    spk::Rectangle rect;

    spk::myTypeRectangle r={25,25,50,25};
    SDL_Color c={255,0,0,255};
    rect.setRectangle(&r,&c,false);

    win.push_back((spk::FrameBase*)&rect);

    thread renderingThread(renderWindow,&win,&running);

    SDL_Event e;
    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            ev.checkEvent(&e);
        }
    }
    renderingThread.join();


    win.clear();
    SDL_Quit();
    cout << "Hello world!" << endl;
    return 0;
}
