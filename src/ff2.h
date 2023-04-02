#pragma once

#include "i_graphics_middleware.h"
#include "i_logic_middleware.h"

class FF2
{
private:
  IGraphicsMiddleware *graphics_;
  ILogicMiddleware *logic_;

public:
  FF2(IGraphicsMiddleware *graphics, ILogicMiddleware *logic);
  ~FF2();

  void main();
};
