#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class SDLTest
{
public:
    SDLTest() : window(nullptr), renderer(nullptr) {}

    ~SDLTest()
    {
        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0){
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }
        if(!createWindow()) return false;
        if(!createRenderer()) return false;
        if(!initSDLImage()) return false;
        return true;
    }

    SDL_Texture* loadTexture(const std::string& filename)
    {
        SDL_Texture* imgTexture = IMG_LoadTexture_RW(renderer,
                                                     SDL_RWFromFile(filename.c_str(), "rb"),
                                                     1);
        if(nullptr == imgTexture){
            printf( "File not found: %s SDL_image Error: %s\n", filename.c_str(), IMG_GetError() );
        }
        return imgTexture;
    }

    // just to check SDL's bitmap loader
    SDL_Texture* loadBMP(const std::string& filename)
    {
        SDL_Surface *bmp = SDL_LoadBMP(filename.c_str());
        if (bmp == nullptr){
            std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
        SDL_FreeSurface(bmp);
        if (tex == nullptr){
            std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        }
        return tex;
    }

    void getWindowSize(int& width, int& height)
    {
        SDL_GetWindowSize(window, &width, &height);
    }

    SDL_Renderer* getRenderer(){return renderer;}
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool createWindow()
    {
        window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
        if (window == nullptr){
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
    }
    bool createRenderer()
    {
        if(!window) return false;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr){
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
    }
    bool initSDLImage()
    {
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags) & imgFlags)) {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            return false;
        }
        return true;
    }
};

int main(int, char**){
    SDLTest test;
    test.init();

    // load pictures
    SDL_Texture* pic00 = test.loadBMP("../pic/test1.bmp");
    SDL_Texture* pic01 = test.loadTexture("../pic/test1.png");

    // get window sizes for te loop
    int windowWidth;
    int windowHeight;
    test.getWindowSize(windowWidth, windowHeight);

    // setup a size for the second picture
    SDL_Rect r;
    r.x = 0; r.y = 0;
    r.w = 100; r.h = 100;

    SDL_Delay(100);

    bool quit = false;
    while(!quit){
        //Event handler
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }



        //First clear the renderer
        SDL_RenderClear(test.getRenderer());
        //Draw the textures
        SDL_RenderCopy(test.getRenderer(), pic00, NULL, NULL);
        SDL_RenderCopy(test.getRenderer(), pic01, NULL, &r);

        // draw the png texture
        r.x += 5; r.y += 5;
        if( r.x > windowWidth &&
            r.y > windowHeight)
        {
            r.x = 0;
            r.y = 0;
        }

        //Update the screen
        SDL_RenderPresent(test.getRenderer());
        //Take a quick break after all that hard work
        SDL_Delay(32);
    }

    
    SDL_DestroyTexture(pic00);
    SDL_DestroyTexture(pic01);
    
    // the other destroys (window, renderer, SDL_Quit() are called from the SDLTest destructor
    
    return 0;
}
