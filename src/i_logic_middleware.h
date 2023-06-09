#pragma once

class ILogicMiddleware
{
private:
public:
  virtual unsigned random(unsigned upper_bound) = 0;
  virtual void randomize() = 0;
  virtual void delay(unsigned milliseconds) = 0;
  virtual void mouseState(int *x, int *y, int *button) = 0;
  virtual void mousemove(int x, int y) = 0;

  // virtual void clearKeys() = 0;
  // virtual void setKeyPressed(int key) = 0;
  virtual bool anyKeyPressed() = 0;
  virtual bool isKeyPressed(int key) = 0;
};
