#include <pch.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#define SHAPE_SIZE 16
using namespace concurrency;
int Sound_Play(const char *file_name);
int Sound_Init();
void fake_main(void * sdata)
{
  SDL_Window* Main_Window;
  SDL_Renderer* Main_Renderer;
  SDL_Surface* Loading_Surf;
  SDL_Texture* Background_Tx;

  /* Rectangles for drawing which will specify source (inside the texture)
  and target (on the screen) for rendering our textures. */
  SDL_Rect SrcR;
  SDL_Rect DestR;
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ,sdata);
  SrcR.x = 0;
  SrcR.y = 0;
  SrcR.w = SHAPE_SIZE;
  SrcR.h = SHAPE_SIZE;

  DestR.x = 640 / 2 - SHAPE_SIZE / 2;
  DestR.y = 580 / 2 - SHAPE_SIZE / 2;
  DestR.w = SHAPE_SIZE;
  DestR.h = SHAPE_SIZE;


  /* Before we can render anything, we need a window and a renderer */
  Main_Window = SDL_CreateWindow("SDL_RenderCopy Example",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 580, 0);
//  Main_Renderer = SDL_CreateRenderer(Main_Window, -1, SDL_RENDERER_ACCELERATED);
  string s(GAMEPATH);
  string dst="Background.png";
  string dst2="sample2.wav";
  string dst3="sample.ogg";
  //Loading_Surf = SDL_LoadBMP((s+dst).data());

  Loading_Surf=IMG_LoadPNG_RW(SDL_RWFromFile((s+dst).data(),"rb"));
    //Main_Renderer=SDL_CreateSoftwareRenderer(Loading_Surf);
  Main_Renderer=SDL_CreateRenderer(Main_Window,-1,SDL_RENDERER_SOFTWARE);
  if(!Loading_Surf)
  {
	  throw;
  }
  Background_Tx = SDL_CreateTextureFromSurface(Main_Renderer, Loading_Surf);
  
  SDL_Rect r;
  r.h=480;
  r.w=800;
  int * pitch;
      /* render background, whereas NULL for source and destination
      rectangles just means "use the default" */
      SDL_RenderCopy(Main_Renderer, Background_Tx, NULL, NULL);
 
      SDL_RenderPresent(Main_Renderer);
	  //SDL_RenderClear(Main_Renderer);
	  //SDL_RenderPresent(Main_Renderer);
	  //Sound_Init();
	  ////auto task=create_async([s,dst2](){
	  ////Sound_Play((s+dst2).data());
	  ////});
	  //Sound_Play((s+dst3).data());

	  SDL_Delay(0xFFFFFFFF);
  /* The renderer works pretty much like a big canvas:
  when you RenderCopy you are adding paint, each time adding it
  on top.
  You can change how it blends with the stuff that
  the new data goes over.
  When your 'picture' is complete, you show it
  by using SDL_RenderPresent. */

  /* SDL 1.2 hint: If you're stuck on the whole renderer idea coming
  from 1.2 surfaces and blitting, think of the renderer as your
  main surface, and SDL_RenderCopy as the blit function to that main
  surface, with SDL_RenderPresent as the old SDL_Flip function.*/
  SDL_DestroyTexture(Background_Tx);
  SDL_DestroyRenderer(Main_Renderer);
  SDL_DestroyWindow(Main_Window);
  SDL_Quit();

}
int Sound_Init()
{
        const int    TMP_FREQ = MIX_DEFAULT_FREQUENCY;
        const Uint16 TMP_FORMAT = MIX_DEFAULT_FORMAT;
        const int    TMP_CHAN = 2;
        const int    TMP_CHUNK_SIZE = 512;

        return Mix_OpenAudio(TMP_FREQ,TMP_FORMAT,TMP_CHAN,TMP_CHUNK_SIZE);
}

int Sound_Play(const char *file_name)
{
        Mix_Music *mix_music;

        if((mix_music = Mix_LoadMUS(file_name)) == NULL)
        {
			string s=Mix_GetError();
                printf("call Mix_LoadMUS failed:%s/n",Mix_GetError());
                return -1;
        }

        if(Mix_PlayMusic(mix_music,-1) == -1)
        {
                printf("call Mix_PlayMusic failed/n");
                return -1;
        }
        printf("after call Mix_PlayMusic/n");

        return 0;
}