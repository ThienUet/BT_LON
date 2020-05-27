#include "snake.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
  try
  {
    Snake s;
    return s.exec(); // check loi
  } catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
