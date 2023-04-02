#pragma once

class IGraphicsMiddleware
{
private:
public:
  virtual void cleardevice() = 0;
  virtual void setrgbpalette(int i, int r, int g, int b) = 0;

  virtual void setcolor(int color_code) = 0;
  virtual void rectangle(int x1, int y1, int x2, int y2) = 0;
  virtual void line(int x1, int y1, int x2, int y2) = 0;
  virtual void putpixel(int x, int y, int color_code) = 0;
  virtual int getpixel(int x, int y) = 0;

  virtual void outtextxy(int x, int y, const char* text) = 0;
  virtual void settextstyle(int font_style, int something, int size) = 0;
};
