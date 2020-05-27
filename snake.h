#include <SDL.h>
#include <deque>
#include <time.h>
#include "audio.h"
class Snake
{
public:
  Snake();
  ~Snake();
  int exec();
  bool tick();
  void draw();
  void audio();
  static const auto HeadOpenMouth = 0;
  static const auto Tail = 1;
  static const auto Turn = 2;
  static const auto Straight = 3;
  static const auto Head = 4;
  static const auto Fruit = 5;
  const static auto Width = 1300;
  const static auto Height = 650;
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *sprites;
  // lay toa do ran tu file bmp ! su dung vector
  std::deque<std::pair<int, int> > segmentsList;
  unsigned ticks = 0;
  int dx = 1;
  int dy = 0;
  int fruitX;
  int fruitY;
  void generateFruit();
  Audio effect;
  int speed = 300;
  Mix_Music *backgroundMusic;
  Mix_Chunk *Eat;
};
