
#include <stdexcept> // thu vien check loi
#include <string>

#define SDL_CHECK(x, msg) if (!(x))                                      \
    throw std::runtime_error(std::string(msg": ") + SDL_GetError())
