#pragma once

#include "i_graphics_middleware.h"
#include "i_logic_middleware.h"

typedef enum
{
  FF2_STATE_LOADING_SCREEN,
  FF2_STATE_MAIN_MENU,
  FF2_STATE_GAME,
  FF2_STATE_GAME_OVER,
  FF2_STATE_OPTIONS,
  FF2_STATE_CREDITS,
  FF2_STATE_EXIT
} FF2State;

class FF2
{
private:
  IGraphicsMiddleware *graphics_;
  ILogicMiddleware *logic_;
  FF2State game_state_;

public:
  FF2(IGraphicsMiddleware *graphics, ILogicMiddleware *logic);
  ~FF2();

  void main_loop(unsigned elapsed_milliseconds);
  void switch_to_state(FF2State state);
};
