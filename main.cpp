#include <iostream>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

//#include "eventList.hpp"
#include "eventVector.hpp"
#include <thread>

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *w=SDL_CreateWindow("tescik",100,100,300,300,0);

    spk::EventVector ev;
    bool running=true;
    spk::EventQuit qt(&running);
    ev.push_back((spk::EventBase*)(&qt));

    SDL_Event e;
    while(running)
    {
        //std::cout<<"runnung="<<(int)running<<endl;
        while(SDL_PollEvent(&e))
        {
            std::cout<<e.type<<endl;
            ev.checkEvent(&e);
        }
    }

    SDL_DestroyWindow(w);
    SDL_Quit();
    cout << "Hello world!" << endl;
    return 0;
}
