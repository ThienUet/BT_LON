#include "snake.hpp"
#include "sdl_check.hpp" // ham doi 2 dau
#include "audio.h"

Snake::Snake()
{
  auto res = SDL_Init(SDL_INIT_EVERYTHING);
  effect.load("C:/Users/Admin/Desktop/BT_GAME/EatSound.wav");
  SDL_CHECK(res == 0, "SDL_Init"); // check init
  SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_BORDERLESS, &window, &renderer); // khoi tao cua so
  SDL_CHECK(window, "SDL_CreateWindowAndRenderer"); // check window
  SDL_CHECK(renderer, "SDL_CreateWindowAndRenderer"); // check renderer
  SDL_SetWindowPosition(window, 3, 3);
  auto surface = SDL_LoadBMP("sprites.bmp");
  SDL_CHECK(surface, "SDL_LoadBMP(\"sprites.bmp\")");
  sprites = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_CHECK(sprites, "SDL_CreateTextureFromSurface");
  SDL_FreeSurface(surface);

  // them doan than vao dot sau cung cua con ran
  segmentsList.push_back(std::make_pair(5, 5));
  segmentsList.push_back(std::make_pair(5, 6));
  segmentsList.push_back(std::make_pair(4, 6));
  // con moi
  generateFruit();
}

void Snake::generateFruit()
{
  auto done = false;
  do
  {
      srand(time(NULL));
      // tao toa do moi random
    fruitX = rand() % (Width / 64);
    fruitY = rand() % (Height / 64);
    done = true;
    for (const auto &segment: segmentsList)
    {
      if (fruitX == segment.first && fruitY == segment.second)
      {
        done = false;
        break;
      }
    }
  } while (!done);
}


Snake::~Snake()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int Snake::exec()
{
  auto oldTick = SDL_GetTicks();
  for (auto done = false; !done;)
  {
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
      switch (e.type)
      {
      case SDL_KEYDOWN:
        {
          switch (e.key.keysym.sym)
          {
          case SDLK_UP: // khi di len thi set cac toa do de di len
            dx = 0;  // di len thi gia tri toa do x = 0;
            dy = -1;// di xuong thi gia tri toa do y giam dan
            break;
          case SDLK_RIGHT:
            dx = 1;
            dy = 0;
            break;
          case SDLK_DOWN:
            dx = 0;
            dy = 1;
            break;
          case SDLK_LEFT:
            dx = -1;
            dy = 0;
            break;
          case  'w':
            speed -= 20; break;
          case 's':
            speed +=20; break;
          }
        }
        break;
      case SDL_QUIT:
        done = true;
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer,0, 0, 255, 255); // man hinh mau xanh
    SDL_RenderClear(renderer);
    // vong lap cho game chay
    auto currentTick = SDL_GetTicks();
    for (auto t = oldTick; t < currentTick; ++t)
      if (!tick())
        return 1;
    oldTick = currentTick;
    // ve ran
    draw();
    // in len man hinh hien tai
    SDL_RenderPresent(renderer);
  }
  return 0;
}

bool Snake::tick()
{
  if (ticks++ % speed == 0)
  {
      // cho game chay
      // cac bien tang so luong
    auto p = segmentsList.front(); // tien len cac toa do tang dan
    p.first += dx;
    p.second += dy;
    if (p.first < 0 || p.first >= Width / 64 || // chia cho 64 ra toa do
        p.second < 0 || p.second >= Height / 64) //
      return false; // cham vao tuong thi game over
    for (const auto &segment: segmentsList)
      if (p == segment)
      {
            return false;
      }
    segmentsList.push_front(p);
    if (p.first != fruitX || p.second != fruitY) // neu
    {
      segmentsList.pop_back(); // neu ko truyen vao bien p thi giu ngiuyen
    }
    else
      generateFruit();
  }
  return true;
}

void Snake::draw()
{
//    ve ran
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 64;
  src.h = 64;
  SDL_Rect dest;
  dest.w = 64;
  dest.h = 64;
  // lay cac toa do cac phan than cua ran dc ve trong file BMP _cot 3 the hien gia trii goc do
  int ds[][3] = {
    { -1, 0, 0 },
    { 0, -1, 90 },
    { 1, 0, 180 },
    { 0, 1, -90 },
  };
  for (auto iter = std::begin(segmentsList); iter != std::end(segmentsList); ++iter)
  {
    float direction = 0;
    const auto &segment = *iter;
    if (iter == std::begin(segmentsList))
    {
        // gan den moi thi in hinh mo mom *****
      if ((iter->first + dx == fruitX - 1 || iter->first + dx == fruitX + 1 ) && ( iter->second + dy == fruitY - 1) || ( iter->second + dy == fruitY +1))
      {
        src.x = Head * 64; // neu toa do cua dau ran bang toa do moi thi mo mom
      }
      else
         src.x = HeadOpenMouth * 64;
       // k mo mom
      if (iter + 1 != std::end(segmentsList))
      {
        const auto &nextSegment = *(iter + 1);
        for (const auto &d: ds)
        {
          if (segment.first + d[0] == nextSegment.first && segment.second + d[1] == nextSegment.second) // xet than ran theo hinh bmp
          {
            direction = d[2];
            break;
          }
        }
      }
    }
    else if (iter + 1 == std::end(segmentsList))
    {
      src.x = Tail * 64;
      const auto &prevSegment = *(iter - 1);
      for (const auto &d: ds)
      {
        if (segment.first == prevSegment.first + d[0] && segment.second == prevSegment.second + d[1])
        {
          direction = d[2];
          break;
        }
      }
    }
    else
    {
      const auto &nextSegment = *(iter + 1);
      const auto &prevSegment = *(iter - 1);
      if (nextSegment.first == prevSegment.first)
      {
        src.x = Straight * 65;
        direction = 90;
      }
      else if (nextSegment.second == prevSegment.second)
      {
        src.x = Straight * 64;
        direction = 0;
      }
      else
      {
        src.x = Turn * 64;
        // tu dau di len  = 0
        bool up = false;
        if (segment.first == nextSegment.first && segment.second - 1 == nextSegment.second)
          up = true;
        if (segment.first == prevSegment.first && segment.second - 1 == prevSegment.second)
          up = true;
        // sang phai
        bool right = false;
        if (segment.first + 1 == nextSegment.first && segment.second == nextSegment.second)
          right = true;
        if (segment.first + 1 == prevSegment.first && segment.second == prevSegment.second)
          right = true;

        bool down = false;
        if (segment.first == nextSegment.first && segment.second + 1 == nextSegment.second)
          down = true;
        if (segment.first == prevSegment.first && segment.second + 1 == prevSegment.second)
          down = true;

        bool left = false;
        if (segment.first - 1 == nextSegment.first && segment.second == nextSegment.second)
          left = true;
        if (segment.first - 1 == prevSegment.first && segment.second == prevSegment.second)
          left = true;

        if (up && right) // cac toa do cho hinh thanh mot khuon
          direction = 0;
        else if (right && down)
          direction = 90;
        else if (down && left)
          direction = 180;
        else if (left && up)
          direction = 270;
      }
    }

    dest.x = 64 * segment.first;
    dest.y = 64 * segment.second;
    SDL_RenderCopyEx(renderer, sprites, &src, &dest, direction, nullptr, SDL_FLIP_NONE);
  }
  src.x = Fruit * 64;
  dest.x = fruitX * 64;
  dest.y = fruitY * 64;
  SDL_RenderCopyEx(renderer, sprites, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}

