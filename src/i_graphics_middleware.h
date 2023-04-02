#pragma once

class IGraphicsMiddleware
{
private:
public:
  virtual void cleardevice() = 0;
  virtual void setrgbpalette(int i, int r, int g, int b) = 0;

  virtual void putpixel(int x, int y, int color_code) = 0;
  virtual int getpixel(int x, int y) = 0;
};
