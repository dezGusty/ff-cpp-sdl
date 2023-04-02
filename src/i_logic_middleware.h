#pragma once

class ILogicMiddleware
{
private:
public:
  virtual unsigned random(unsigned upper_bound) = 0;
  virtual void randomize() = 0;
  virtual void delay(unsigned milliseconds) = 0;
};
