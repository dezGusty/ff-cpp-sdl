#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "mouse.h"

#include "i_graphics_middleware.h"
#include "ff2.h"

FF2::FF2(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
    : graphics_(graphics), logic_(logic)
{
  game_state_ = FF2_STATE_LOADING_SCREEN;
}

FF2::~FF2()
{
}

char c, *paleta;
unsigned char global_a_line[640], caz[10], aux[45][45];
int mouse_btn = 0, x = 0, y = 0, i = 0, j = 0;
int goodbye = 0, game_over = 0, victory = 0, shipx = 0, shiplife = 100, exshiplife = 100;
int tipnav, extipnav, waiter, fwaiter; // pt mutarile navei &foc
long mun1, mun2, cash, exmun1, exmun2, excash, kills, timer, ltimp;
int temp_bye = 0;     // quit pt joc in desfasurare
int player_terminate; // idem

long state_count_timer = 0;

unsigned char shp1[45][45], shp2[45][45], shp3[45][45], shp0[45][45];
unsigned char enm1[45][45], enm2[45][45], enm3[45][45], enm4[45][45], enm5[45][45];
unsigned char bm1[45][45], bm2[45][45], bm3[45][45], bm4[45][45], bm5[45][45], bm6[45][45];
unsigned char blt1[15][15], blt2[15][15], blt3[15][15], blt4[15][15], blt5[15][15], aux15[15][15];
unsigned char bns1[15][15], bns2[15][15], bns3[15][15], bns4[15][15];
unsigned char ast1[45][45], ast2[45][45];
float luminozitate = 1.0;
// tipul ce va defini stelele
typedef struct
{
  int cul, x, y, vit, ok;
} STELAR;
// tipul ce va defini navele inamice
typedef struct
{
  int fel, x, y, hlt, vit, foc;
} ENEMY;
// tipul de va defini gloantele
typedef struct
{
  int fel, vit, x, y, dir;
} GLONT;
// tipul ce va defini exploziile
typedef struct
{
  int faz, x, y;
} EXPLOZ;
typedef struct
{
  int x, y, fel, vit;
} BONUS;
typedef struct
{
  int x, y, fel, vit, hlt;
} ASTEROID;
STELAR st1[40], st2[40], st3[40];
ENEMY enemy[10];
EXPLOZ explo[10];
GLONT bullet[20];
BONUS bonus;
ASTEROID aster[7];
int ship_types[10], ship_often, bonus_often, max_enemy, cur_enemy, fire_often;
int max_aster, cur_aster, aster_often, mission_type, limit;

#if 0
void initializari(void)
{
  int g1 = installuserdriver("VESA", NULL), g2 = 1;
  randomize();
  initgraph(&g1, &g2, "");
  if (graphresult() != grOk)
  {
    clrscr();
    printf("Error working with file \"VESA.BGI\"\n");
    exit(1);
  }
  load_palette("Imagdata\\menu.pal", 0);
  luminozitate = 1.1;
  mouseinit();
  mousemove(100, 0);
}
#endif

void give(unsigned char a[45][45], unsigned char b[45][45])
{
  for (int i = 0; i < 45; i++)
    for (int j = 0; j < 45; j++)
      b[i][j] = a[i][j];
}
void give15(unsigned char a[15][15], unsigned char b[15][15])
{
  for (int i = 0; i < 15; i++)
    for (int j = 0; j < 15; j++)
      b[i][j] = a[i][j];
}

void printfile(IGraphicsMiddleware *graphics, int x, int y, char file_name[30], int dm1, int dm2)
{
  FILE *f = NULL;
  unsigned char c;
  unsigned char a_line[640];
  for (int i = 0; i < 640; i++)
  {
    a_line[i] = 0;
  }

  f = fopen(file_name, "r");
  for (int i = 0; i < dm2; i++)
  {
    for (int j = 0; j < dm1; j++)
    {
      fscanf(f, "%c", &c);
      if (c >= 33)
        a_line[j] = c;
    }
    for (int j = 0; j < dm1; j++)
      if (c >= 33)
        graphics->putpixel(x + j, y + i, a_line[j]);
    fscanf(f, "%c", &c);
  }
  fclose(f);
}

void putmem(char *untext, long lin, unsigned char a[45][45])
{
  FILE *f = NULL;
  unsigned char c;
  int i, j;
  f = fopen(untext, "r");
  fseek(f, lin, 0);
  for (i = 0; i < 45; i++)
  {
    for (j = 0; j < 45; j++)
    {
      fscanf(f, "%c", &c);
      if (c > 33)
        a[i][j] = c;
      else
        a[i][j] = 0;
    }
    fscanf(f, "%c", &c);
  }
  fclose(f);
}
void scotmem(IGraphicsMiddleware *graphics, unsigned char a[45][45], int x, int y)
{
  int i, j;
  for (i = 0; i < 45; i++)
    for (j = 0; j < 45; j++)
      if (a[i][j] > 33)
        graphics->putpixel(x + j, y + i, a[i][j]);
}
void negru(IGraphicsMiddleware *graphics, unsigned char a[45][45], int x, int y)
{
  int i, j;
  for (i = 0; i < 45; i++)
    for (j = 0; j < 45; j++)
      if (a[i][j] > 33)
        graphics->putpixel(x + j, y + i, 0);
}
void x_negru45(IGraphicsMiddleware *graphics, unsigned char a[45][45], int x, int y, int dist)
{
  int i, j;
  for (i = 0; i < dist; i++)
    for (j = 0; j < 45; j++)
      if (a[i][j])
        graphics->putpixel(x + j, y + i, 0);
  for (i = dist; i < 45; i++)
    for (j = 0; j < 45; j++)
      if (a[i][j] && !a[i - dist][j])
        graphics->putpixel(x + j, y + i, 0);
}

void putmem15(char *untext, long lin, unsigned char a[15][15])
{
  FILE *f = NULL;
  unsigned char c;
  int i, j;
  f = fopen(untext, "r");
  fseek(f, lin, 0);
  for (i = 0; i < 15; i++)
  {
    for (j = 0; j < 15; j++)
    {
      fscanf(f, "%c", &c);
      if (c > 33)
        a[i][j] = c;
      else
        a[i][j] = 0;
    }
    fscanf(f, "%c", &c);
  }
  fclose(f);
}
void scotmem15(IGraphicsMiddleware *graphics, unsigned char a[15][15], int x, int y)
{
  int i, j;
  for (i = 0; i < 15; i++)
    for (j = 0; j < 15; j++)
      if (a[i][j] > 33)
        graphics->putpixel(x + j, y + i, a[i][j]);
}
void negru15(IGraphicsMiddleware *graphics, unsigned char a[15][15], int x, int y)
{
  int i, j;
  for (i = 0; i < 15; i++)
    for (j = 0; j < 15; j++)
      if (a[i][j] > 33)
        graphics->putpixel(x + j, y + i, 0);
}
void x_negru15(IGraphicsMiddleware *graphics, unsigned char a[15][15], int x, int y, int dist)
{
  int i, j;
  for (i = 0; i < dist && i < 15; i++)
    for (j = 0; j < 15; j++)
      if (a[i][j])
        graphics->putpixel(x + j, y + i, 0);
  for (i = dist; i < 15; i++)
    for (j = 0; j < 15; j++)
      if (a[i][j] && !a[i - dist][j])
        graphics->putpixel(x + j, y + i, 0);
}

int mxa(int i)
{
  int aux;
  if (i > 63)
    aux = 63;
  else
    aux = i;
  return aux;
}

void load_palette(IGraphicsMiddleware *graphics, char nume[30], float intens)
{
  FILE *f;
  int i, a, b, c;
  char d;
  f = fopen(nume, "r");
  for (i = 0; i < 18; i++)
    fscanf(f, "%c", &d);
  for (i = 0; i < 256; i++)
  {
    fscanf(f, "%d%d%d", &a, &b, &c);
    graphics->setrgbpalette(i, mxa(a / 4 * intens), mxa(b / 4 * intens), mxa(c / 4 * intens));
  }
  fclose(f);
}
void intunecare(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, int viteza)
{
  float fl;
  for (fl = luminozitate; fl > 0.0; fl -= 0.1)
  {
    load_palette(graphics, paleta, fl);
    logic->delay(10 * viteza);
  }
}

void luminare(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, int viteza)
{
  float fl;
  for (fl = 0.0; fl < luminozitate; fl += 0.1)
  {
    load_palette(graphics, paleta, fl);
    logic->delay(10 * viteza);
  }
  load_palette(graphics, paleta, luminozitate);
}

void text_shadow(IGraphicsMiddleware *graphics, int x, int y, char mesaj[50], int culoare)
{
  graphics->setcolor(0);
  graphics->outtextxy(x + 1, y + 1, mesaj);
  graphics->setcolor(culoare);
  graphics->outtextxy(x, y, mesaj);
}

void write_file(IGraphicsMiddleware *graphics, char *fisier, int x, int y, int dim1, int dim2)
{
  int i, j;
  FILE *f;
  char c;
  f = fopen(fisier, "w");
  for (j = y; j < y + dim2; j++)
  {
    for (i = x; i < x + dim1; i++)
    {
      c = graphics->getpixel(i, j);
      if (c == 0)
        c = 255;
      fprintf(f, "%c", c);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

// procedurile legate de stele
void init_culori_stele(IGraphicsMiddleware *graphics)
{
  graphics->setrgbpalette(1, 30, 30, 30);
  graphics->setrgbpalette(2, 30, 0, 0);
  graphics->setrgbpalette(3, 0, 0, 30);
  graphics->setrgbpalette(4, 30, 0, 30);
  graphics->setrgbpalette(5, 45, 45, 45);
  graphics->setrgbpalette(6, 45, 0, 0);
  graphics->setrgbpalette(7, 0, 0, 45);
  graphics->setrgbpalette(8, 45, 0, 45);
  graphics->setrgbpalette(9, 63, 63, 63);
  graphics->setrgbpalette(10, 63, 45, 0);
  graphics->setrgbpalette(11, 0, 50, 3);
  graphics->setrgbpalette(12, 63, 50, 63);
}
void init_stea(ILogicMiddleware *logic, STELAR &p, int zon)
{
  p.cul = logic->random(4) + 1 + 4 * zon;
  p.x = logic->random(450);
  p.y = 0;
  p.vit = logic->random(2) + zon * 2 + 1;
  p.ok = 1;
}
void init_stele(ILogicMiddleware *logic)
{
  int i;
  for (i = 0; i < 40; i++)
  {
    init_stea(logic, st1[i], 0);
    init_stea(logic, st2[i], 0);
    init_stea(logic, st3[i], 0);
    st1[i].y = logic->random(400);
    st2[i].y = logic->random(400);
    st3[i].y = logic->random(400);
  }
}
void muta_stele(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int i;
  for (i = 0; i < 40; i++)
  {
    if (st1[i].ok)
      graphics->putpixel(st1[i].x, st1[i].y, 0);
    if (st2[i].ok)
      graphics->putpixel(st2[i].x, st2[i].y, 0);
    if (st3[i].ok)
      graphics->putpixel(st3[i].x, st3[i].y, 0);
    st1[i].y += st1[i].vit;
    st2[i].y += st2[i].vit;
    st3[i].y += st3[i].vit;
    if (graphics->getpixel(st1[i].x, st1[i].y))
      st1[i].ok = 0;
    else
      st1[i].ok = 1;
    if (graphics->getpixel(st2[i].x, st2[i].y))
      st2[i].ok = 0;
    else
      st2[i].ok = 1;
    if (graphics->getpixel(st3[i].x, st3[i].y))
      st3[i].ok = 0;
    else
      st3[i].ok = 1;
    if (st1[i].ok)
      graphics->putpixel(st1[i].x, st1[i].y, st1[i].cul);
    if (st2[i].ok)
      graphics->putpixel(st2[i].x, st2[i].y, st2[i].cul);
    if (st3[i].ok)
      graphics->putpixel(st3[i].x, st3[i].y, st3[i].cul);
  }
  for (i = 0; i < 40; i++)
  {
    if (st1[i].y > 400)
      init_stea(logic, st1[i], 0);
    if (st2[i].y > 400)
      init_stea(logic, st2[i], 1);
    if (st3[i].y > 400)
      init_stea(logic, st3[i], 2);
  }
}
void xstardel(IGraphicsMiddleware *graphics)
{
  int i;
  for (i = 0; i < 40; i++)
  {
    if (st1[i].ok)
      graphics->putpixel(st1[i].x, st1[i].y, 0);
    if (st2[i].ok)
      graphics->putpixel(st2[i].x, st2[i].y, 0);
    if (st3[i].ok)
      graphics->putpixel(st3[i].x, st3[i].y, 0);
  }
}
void xstarchk()
{
  int i;
  for (i = 0; i < 40; i++)
  {
    if (st1[i].ok)
      st1[i].ok = 0;
    if (st2[i].ok)
      st2[i].ok = 0;
    if (st3[i].ok)
      st3[i].ok = 0;
  }
}
//----"Moovfc2.cpp"
// procedurile legate de nava
void muta_nava(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int dif, i, fostx = shipx;
  logic->mouseState(&x, &y, &mouse_btn);
  extipnav = tipnav;
  if (x != 96)
  {
    dif = (x - 96) / 4;
    if (dif > 0 && dif <= 15)
      dif = 5;
    if (dif > 15)
      dif = 10;
    if (dif < 0 && dif >= -15)
      dif = -5;
    if (dif < -15)
      dif = -10;
    if (dif > 0)
      tipnav = 1;
    if (dif < 0)
      tipnav = -1;
    if (dif == 0)
      tipnav = 0;
    //^clarificarea diferentelor^
    logic->mousemove(100, 100);
    negru(graphics, shp0, shipx, 350);
    shipx = shipx + dif;
    if (shipx < 1)
      shipx = 1;
    if (shipx > 405)
      shipx = 405;
  }
  else if (waiter == 12)
    tipnav = 0;
  else
    waiter++;
  if (x != 96 || tipnav != extipnav)
    negru(graphics, shp0, fostx, 350);
  if (extipnav != tipnav)
  {
    switch (tipnav)
    {
    case 0:
      give(shp1, shp0);
      break;
    case 1:
      give(shp2, shp0);
      break;
    case -1:
      give(shp3, shp0);
      break;
    }
    waiter = 0;
  }
  if (x != 96 || tipnav != extipnav)
    scotmem(graphics, shp0, shipx, 350);
}

// procedurile legate de inamici
void init_inamic(ILogicMiddleware *logic, ENEMY &p)
{
  int i, k, uniq = 1;
  k = logic->random(405) + 1;
  k = k / 5 * 5;
  for (i = 1; i < 10; i++)
    if (uniq && enemy[i].fel)
      if (k < enemy[i].x + 40 && k > enemy[i].x - 40)
        if (enemy[i].y < 0)
          uniq = 0;
  if (uniq)
  {
    p.x = k;
    p.y = logic->random(100) - 150;
    k = logic->random(ship_types[0]) + 1;
    p.fel = ship_types[k];
    switch (p.fel)
    {
    case 1:
      p.vit = 7;
      p.hlt = 2;
      break;
    case 2:
      p.vit = 9;
      p.hlt = 1;
      break;
    case 3:
      p.vit = 6;
      p.hlt = 3;
      break;
    case 4:
      p.vit = 7;
      p.hlt = 2;
      break;
    case 5:
      p.vit = 8;
      p.hlt = 2;
      break;
    }
    cur_enemy++;
  }
}
void init_inamici(ILogicMiddleware *logic)
{
  int i, k;
  putmem("Gamedata\\enemy1.dta", 0, enm1);
  putmem("Gamedata\\enemy2.dta", 0, enm2);
  putmem("Gamedata\\enemy3.dta", 0, enm3);
  putmem("Gamedata\\enemy4.dta", 0, enm4);
  putmem("Gamedata\\enemy5.dta", 0, enm5);
  for (i = 1; i <= ship_types[0]; i++)
    if (cur_enemy < max_enemy)
    {
      k = logic->random(ship_often) + 1;
      if (k == 1)
        init_inamic(logic, enemy[i]);
    }
}
void muta_inamici(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int i, k;
  for (i = 1; i < 10; i++)
    if (enemy[i].fel)
    {
      switch (enemy[i].fel)
      {
      case 1:
        x_negru45(graphics, enm1, enemy[i].x, enemy[i].y, enemy[i].vit);
        break;
      case 2:
        x_negru45(graphics, enm2, enemy[i].x, enemy[i].y, enemy[i].vit);
        break;
      case 3:
        x_negru45(graphics, enm3, enemy[i].x, enemy[i].y, enemy[i].vit);
        break;
      case 4:
        x_negru45(graphics, enm4, enemy[i].x, enemy[i].y, enemy[i].vit);
        break;
      case 5:
        x_negru45(graphics, enm5, enemy[i].x, enemy[i].y, enemy[i].vit);
        break;
      }
      enemy[i].y += enemy[i].vit;
      if (enemy[i].y > 400)
      {
        enemy[i].fel = 0;
        cur_enemy--;
      }
      if (enemy[i].y > -45 && enemy[i].y < 400)
        switch (enemy[i].fel)
        {
        case 1:
          scotmem(graphics, enm1, enemy[i].x, enemy[i].y);
          break;
        case 2:
          scotmem(graphics, enm2, enemy[i].x, enemy[i].y);
          break;
        case 3:
          scotmem(graphics, enm3, enemy[i].x, enemy[i].y);
          break;
        case 4:
          scotmem(graphics, enm4, enemy[i].x, enemy[i].y);
          break;
        case 5:
          scotmem(graphics, enm5, enemy[i].x, enemy[i].y);
          break;
        }
    }
    else if (cur_enemy < max_enemy)
    {
      k = logic->random(ship_often) + 1;
      if (k == 1)
        init_inamic(logic, enemy[i]);
    }
}
// procedurile legate de aasteroizi
void init_aster(ILogicMiddleware *logic, ASTEROID &p)
{
  int i, k, uniq = 1;
  k = logic->random(405) + 1;
  k = k / 5 * 5;
  for (i = 1; i < 7; i++)
    if (uniq && aster[i].fel)
      if (k < aster[i].x + 40 && k > aster[i].x - 40)
        if (aster[i].y < 0)
          uniq = 0;
  if (uniq)
  {
    p.x = k;
    p.y = logic->random(150) - 200;
    p.fel = logic->random(2) + 1;
    p.vit = logic->random(2) + 6;
    p.hlt = 5;
    cur_aster++;
  }
}
void muta_aster(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int i, k;
  for (i = 1; i < 7; i++)
    if (aster[i].fel)
    {
      switch (aster[i].fel)
      {
      case 1:
        x_negru45(graphics, ast1, aster[i].x, aster[i].y, aster[i].vit);
        break;
      case 2:
        x_negru45(graphics, ast2, aster[i].x, aster[i].y, aster[i].vit);
        break;
      }
      aster[i].y += aster[i].vit;
      if (aster[i].y > 400)
      {
        aster[i].fel = 0;
        cur_aster--;
      }
      if (aster[i].y > -45 && aster[i].y < 400)
        switch (aster[i].fel)
        {
        case 1:
          scotmem(graphics, ast1, aster[i].x, aster[i].y);
          break;
        case 2:
          scotmem(graphics, ast2, aster[i].x, aster[i].y);
          break;
        }
    }
    else if (cur_aster < max_aster)
    {
      k = logic->random(aster_often) + 1;
      if (k == 1)
        init_aster(logic, aster[i]);
    }
}

// procedurile legate de bonusuri
void init_bonus(ILogicMiddleware *logic)
{
  int k;
  k = logic->random(bonus_often) + 1;
  if (k == 1)
  {
    bonus.fel = logic->random(4) + 1;
    bonus.x = logic->random(405) + 1;
    bonus.y = logic->random(150) - 200;
    bonus.vit = logic->random(3) + 5;
  }
}
void muta_bonus(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  if (bonus.fel)
  {
    switch (bonus.fel)
    {
    case 1:
      give15(bns1, aux15);
      break;
    case 2:
      give15(bns2, aux15);
      break;
    case 3:
      give15(bns3, aux15);
      break;
    case 4:
      give15(bns4, aux15);
      break;
    }
    x_negru15(graphics, aux15, bonus.x, bonus.y, bonus.vit);
    bonus.y += bonus.vit;
    if (bonus.y < 400)
      scotmem15(graphics, aux15, bonus.x, bonus.y);
    else
      bonus.fel = 0;
  }
  else if (!victory && !goodbye)
    init_bonus(logic);
}
// procedurile legate de gloante
void init_glont_en(ILogicMiddleware *logic, GLONT &p, int i)
{
  int k;
  k = logic->random(fire_often) + 1;
  if (k == 1)
    if (enemy[i].fel && enemy[i].foc == 0)
    {
      p.x = enemy[i].x + 15;
      p.y = enemy[i].y + 45;
      switch (enemy[i].fel)
      {
      case 1:
        p.fel = 1;
        p.vit = 10;
        break;
      case 2:
        p.fel = 2;
        p.vit = 14;
        break;
      case 3:
        p.fel = 1;
        p.vit = 12;
        break;
      case 4:
        p.fel = 2;
        p.vit = 14;
        break;
      case 5:
        p.fel = 1;
        p.vit = 10;
        break;
      }
      p.dir = 1;
      enemy[i].foc = 1;
    }
}
void init_gloante()
{
  int i;
  for (i = 1; i < 20; i++)
  {
    bullet[i].fel = 0;
    bullet[i].x = 0;
    bullet[i].y = 0;
    bullet[i].vit = 0;
    bullet[i].dir = 0;
  }
}
void muta_gloante_en(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int i, k;
  for (i = 1; i < 10; i++)
    if (bullet[i].fel)
    {
      switch (bullet[i].fel)
      {
      case 1:
        x_negru15(graphics, blt1, bullet[i].x, bullet[i].y, bullet[i].vit);
        break;
      case 2:
        x_negru15(graphics, blt2, bullet[i].x, bullet[i].y, bullet[i].vit);
        break;
      case 3:
        x_negru15(graphics, blt3, bullet[i].x, bullet[i].y, bullet[i].vit);
        break;
      case 4:
        x_negru15(graphics, blt4, bullet[i].x, bullet[i].y, bullet[i].vit);
        break;
      case 5:
        x_negru15(graphics, blt5, bullet[i].x, bullet[i].y, bullet[i].vit);
        break;
      }
      bullet[i].y += bullet[i].vit * bullet[i].dir;
      if (bullet[i].y > 400)
      {
        bullet[i].fel = 0;
        enemy[i].foc = 0;
      }
      if (bullet[i].y > -15 && bullet[i].y < 400)
        switch (bullet[i].fel)
        {
        case 1:
          scotmem15(graphics, blt1, bullet[i].x, bullet[i].y);
          break;
        case 2:
          scotmem15(graphics, blt2, bullet[i].x, bullet[i].y);
          break;
        case 3:
          scotmem15(graphics, blt3, bullet[i].x, bullet[i].y);
          break;
        case 4:
          scotmem15(graphics, blt4, bullet[i].x, bullet[i].y);
          break;
        case 5:
          scotmem15(graphics, blt5, bullet[i].x, bullet[i].y);
          break;
        }
    }
    else
      init_glont_en(logic, bullet[i], i);
}
void init_glont_sh(int gl)
{
  int i, k = 0;
  if (fwaiter > 6)
  {
    for (i = 10; i < 20; i++)
      if (bullet[i].fel == 0)
        k = i;
    bullet[k].x = shipx + 15;
    bullet[k].y = 336;
    switch (gl)
    {
    case 1:
      bullet[k].fel = 1;
      bullet[k].vit = 10;
      mun1--;
      break;
    case 2:
      bullet[k].fel = 3;
      bullet[k].vit = 16;
      mun2--;
      break;
    }
    bullet[k].dir = -1;
    fwaiter = 0;
  }
}
void muta_gloante_sh(IGraphicsMiddleware *graphics)
{
  for (i = 10; i < 20; i++)
    if (bullet[i].fel)
    {
      switch (bullet[i].fel)
      {
      case 1:
        negru15(graphics, blt1, bullet[i].x, bullet[i].y);
        break;
      case 2:
        negru15(graphics, blt2, bullet[i].x, bullet[i].y);
        break;
      case 3:
        negru15(graphics, blt3, bullet[i].x, bullet[i].y);
        break;
      }
      bullet[i].y += bullet[i].vit * bullet[i].dir;
      if (bullet[i].y < -200)
      {
        bullet[i].fel = 0;
      }
      if (bullet[i].y > -15 && bullet[i].y < 400)
        switch (bullet[i].fel)
        {
        case 1:
          scotmem15(graphics, blt1, bullet[i].x, bullet[i].y);
          break;
        case 2:
          scotmem15(graphics, blt2, bullet[i].x, bullet[i].y);
          break;
        case 3:
          scotmem15(graphics, blt3, bullet[i].x, bullet[i].y);
          break;
        }
    }
}
void init_explozie(int xx, int yy)
{
  int i, k;
  for (i = 0; i < 10; i++)
    if (explo[i].faz == 0)
    {
      k = i;
      break;
    }
  explo[k].faz = 1;
  explo[k].x = xx;
  explo[k].y = yy;
}
void anim_explozii(IGraphicsMiddleware *graphics)
{
  int i;
  for (i = 0; i < 10; i++)
    if (explo[i].faz)
    {
      switch (explo[i].faz)
      {
      case 4:
        negru(graphics, bm1, explo[i].x, explo[i].y);
        break;
      case 7:
        negru(graphics, bm2, explo[i].x, explo[i].y);
        break;
      case 10:
        negru(graphics, bm3, explo[i].x, explo[i].y);
        break;
      case 13:
        negru(graphics, bm4, explo[i].x, explo[i].y);
        break;
      case 16:
        negru(graphics, bm5, explo[i].x, explo[i].y);
        break;
      case 19:
        negru(graphics, bm6, explo[i].x, explo[i].y);
        break;
      }
      explo[i].faz++;
      if (explo[i].faz > 20)
        explo[i].faz = 0;
      switch (explo[i].faz)
      {
      case 2:
        scotmem(graphics, bm1, explo[i].x, explo[i].y);
        break;
      case 5:
        scotmem(graphics, bm2, explo[i].x, explo[i].y);
        break;
      case 8:
        scotmem(graphics, bm3, explo[i].x, explo[i].y);
        break;
      case 11:
        scotmem(graphics, bm4, explo[i].x, explo[i].y);
        break;
      case 14:
        scotmem(graphics, bm5, explo[i].x, explo[i].y);
        break;
      case 17:
        scotmem(graphics, bm6, explo[i].x, explo[i].y);
        break;
      }
    }
}
void show_initial_ship_life(IGraphicsMiddleware *graphics)
{
  int i;
  graphics->setcolor(100);
  for (i = 1; i < shiplife; i++)
    graphics->line(504 + i, 203, 504 + i, 225);
}
void regl_ship_life() // va regla pct. de viata
{
  if (shiplife < 0)
    shiplife = 0;
  if (shiplife > 115)
    shiplife = 115;
}
void show_ship_life(IGraphicsMiddleware *graphics)
{
  int i;
  if (shiplife < exshiplife)
  {
    graphics->setcolor(255);
    for (i = exshiplife; i > shiplife; i--)
      graphics->line(504 + i, 203, 504 + i, 225);
  }
  else
  {
    graphics->setcolor(100);
    for (i = exshiplife; i < shiplife; i++)
      graphics->line(504 + i, 203, 504 + i, 225);
  }
}
void show_cash(IGraphicsMiddleware *graphics)
{
  char p[10] = "\0";
  sprintf(p, "%d", excash);
  graphics->setcolor(255);
  graphics->outtextxy(510, 11, p);
  sprintf(p, "%d", cash);
  graphics->setcolor(34);
  graphics->outtextxy(510, 11, p);
}
void show_mun1(IGraphicsMiddleware *graphics)
{
  char p[10] = "\0";
  sprintf(p, "%d", exmun1);
  graphics->setcolor(255);
  graphics->outtextxy(520, 110, p);
  sprintf(p, "%d", mun1);
  graphics->setcolor(34);
  graphics->outtextxy(520, 110, p);
}
void show_mun2(IGraphicsMiddleware *graphics)
{
  char p[10] = "\0";
  sprintf(p, "%d", exmun2);
  graphics->setcolor(255);
  graphics->outtextxy(520, 150, p);
  sprintf(p, "%d", mun2);
  graphics->setcolor(34);
  graphics->outtextxy(520, 150, p);
}
void show_ship(IGraphicsMiddleware *graphics)
{
  scotmem(graphics, shp0, shipx, 350);
}
int impact45(unsigned char matr1[45][45], unsigned char matr2[45][45], int difx, int dify)
{
  int vl_ret = 0, i, j;
  if (difx >= 0 && dify >= 0)
    for (j = 0; j < 45; j++)
      for (i = 0; i < 45; i++)
        if (matr2[i][j] && matr1[i + difx][j + dify])
        {
          vl_ret = 1;
          break;
        }
  if (difx >= 0 && dify < 0)
    for (j = 0; j < 45; j++)
      for (i = 0; i < 45; i++)
        if (j >= abs(dify))
          if (matr2[i][j] && matr1[i + difx][j + dify])
          {
            vl_ret = 1;
            break;
          }
  if (difx < 0 && dify >= 0)
    for (j = 0; j < 45; j++)
      for (i = 0; i < 45; i++)
        if (i >= abs(difx))
          if (matr2[i][j] && matr1[i + difx][j + dify])
          {
            vl_ret = 1;
            break;
          }
  if (difx < 0 && dify < 0)
    for (j = 0; j < 45; j++)
      for (i = 0; i < 45; i++)
        if (j >= abs(dify) && i >= abs(difx))
          if (matr2[i][j] && matr1[i + difx][j + dify])
          {
            vl_ret = 1;
            break;
          }
  return vl_ret;
}
int impact15(unsigned char matr1[45][45], unsigned char matr2[15][15], int difx, int dify)
{
  int vl_ret = 0, i, j;
  if (difx >= 0 && dify >= 0)
    for (j = 0; j < 15; j++)
      for (i = 0; i < 15; i++)
        if (matr2[i][j] && matr1[i + difx][j + dify])
        {
          vl_ret = 1;
          break;
        }
  if (difx >= 0 && dify < 0)
    for (j = 0; j < 15; j++)
      for (i = 0; i < 15; i++)
        if (j >= abs(dify))
          if (matr2[i][j] && matr1[i + difx][j + dify])
          {
            vl_ret = 1;
            break;
          }
  if (difx < 0 && dify >= 0)
    for (j = 0; j < 15; j++)
      for (i = 0; i < 15; i++)
        if (i >= abs(difx))
          if (matr2[i][j] && matr1[i + difx][j + dify])
          {
            vl_ret = 1;
            break;
          }
  if (difx < 0 && dify < 0)
    for (j = 0; j < 15; j++)
      for (i = 0; i < 15; i++)
        if (j >= abs(dify) && i >= abs(difx))
          if (matr2[i][j] && matr1[i + difx][j + dify])
          {
            vl_ret = 1;
            break;
          }
  return vl_ret;
}
void impact_nava(IGraphicsMiddleware *graphics)
{
  int difx = 0, dify = 0;
  for (i = 0; i < 10; i++)
    if (enemy[i].fel)
    {
      difx = shipx - enemy[i].x;
      dify = 350 - enemy[i].y;
      if (abs(difx) < 45 && abs(dify) < 45)
      {
        switch (enemy[i].fel)
        {
        case 1:
          give(enm1, aux);
          break;
        case 2:
          give(enm2, aux);
          break;
        case 3:
          give(enm3, aux);
          break;
        case 4:
          give(enm4, aux);
          break;
        case 5:
          give(enm5, aux);
          break;
        }
        if (impact45(aux, shp0, difx, dify))
        {
          exshiplife = shiplife;
          shiplife -= 4 * enemy[i].hlt;
          regl_ship_life();
          enemy[i].hlt = 0;
          show_ship_life(graphics);
        }
      }
    }
}
void impact_bullets_nava(IGraphicsMiddleware *graphics)
{
  int i, difx = 0, dify = 0;
  for (i = 0; i < 10; i++)
    if (bullet[i].fel)
    {
      difx = bullet[i].x - shipx;
      dify = bullet[i].y - 350;
      if (difx > -15 && difx < 45)
        if (dify > -15 && dify < 45)
        {
          switch (bullet[i].fel)
          {
          case 1:
            give15(blt1, aux15);
            break;
          case 2:
            give15(blt2, aux15);
            break;
          case 3:
            give15(blt3, aux15);
            break;
          case 4:
            give15(blt4, aux15);
            break;
          case 5:
            give15(blt5, aux15);
            break;
          }
          if (impact15(shp0, aux15, difx, dify))
          {
            negru15(graphics, aux15, bullet[i].x, bullet[i].y);
            exshiplife = shiplife;
            if (bullet[i].fel == 1)
              shiplife -= 2;
            else
              shiplife -= 3;
            regl_ship_life();
            show_ship_life(graphics);
            bullet[i].fel = 0;
            enemy[i].foc = 0;
          }
        }
    }
}
void impact_bullets(IGraphicsMiddleware *graphics)
{
  int i, j, difx = 0, dify = 0;
  for (j = 10; j < 20; j++)
    for (i = 0; i < 10; i++)
      if (enemy[i].fel && bullet[j].fel)
      {
        difx = bullet[j].x - enemy[i].x;
        dify = bullet[j].y - enemy[i].y;
        if (difx > -15 && difx < 45)
          if (dify > -15 && dify < 45)
          {
            if (bullet[j].fel == 1)
              enemy[i].hlt--;
            else
              enemy[i].hlt -= 2;
            switch (bullet[j].fel)
            {
            case 1:
              negru15(graphics, blt1, bullet[j].x, bullet[j].y);
              break;
            case 2:
              negru15(graphics, blt2, bullet[j].x, bullet[j].y);
              break;
            case 3:
              negru15(graphics, blt3, bullet[j].x, bullet[j].y);
              break;
            case 4:
              negru15(graphics, blt4, bullet[j].x, bullet[j].y);
              break;
            case 5:
              negru15(graphics, blt5, bullet[j].x, bullet[j].y);
              break;
            }
            bullet[j].fel = 0;
          }
      }
}
void impact_bullets_aster(IGraphicsMiddleware *graphics)
{
  int i, j, difx = 0, dify = 0;
  for (j = 10; j < 20; j++)
    for (i = 0; i < 7; i++)
      if (aster[i].fel && bullet[j].fel)
      {
        difx = bullet[j].x - aster[i].x;
        dify = bullet[j].y - aster[i].y;
        if (difx > -15 && difx < 45)
          if (dify > -15 && dify < 45)
          {
            if (bullet[j].fel == 1)
              aster[i].hlt--;
            else
              aster[i].hlt -= 2;
            switch (bullet[j].fel)
            {
            case 1:
              negru15(graphics, blt1, bullet[j].x, bullet[j].y);
              break;
            case 2:
              negru15(graphics, blt2, bullet[j].x, bullet[j].y);
              break;
            case 3:
              negru15(graphics, blt3, bullet[j].x, bullet[j].y);
              break;
            case 4:
              negru15(graphics, blt4, bullet[j].x, bullet[j].y);
              break;
            case 5:
              negru15(graphics, blt5, bullet[j].x, bullet[j].y);
              break;
            }
            bullet[j].fel = 0;
          }
      }
}
void impact_aster(IGraphicsMiddleware *graphics)
{
  int difx = 0, dify = 0;
  for (i = 0; i < 7; i++)
    if (aster[i].fel)
    {
      difx = shipx - aster[i].x;
      dify = 350 - aster[i].y;
      if (abs(difx) < 45 && abs(dify) < 45)
      {
        switch (aster[i].fel)
        {
        case 1:
          give(ast1, aux);
          break;
        case 2:
          give(ast2, aux);
          break;
        }
        if (impact45(aux, shp0, difx, dify))
        {
          exshiplife = shiplife;
          shiplife -= 4 * aster[i].hlt;
          regl_ship_life();
          aster[i].hlt = 0;
          show_ship_life(graphics);
        }
      }
    }
}
void impact_boom(IGraphicsMiddleware *graphics) // impact intre nava si explozii
{
  int difx = 0, dify = 0;
  if (shiplife > 0)
    for (i = 0; i < 10; i++)
      if (explo[i].faz == 20)
      {
        difx = shipx - explo[i].x;
        dify = 350 - explo[i].y;
        if (abs(difx) < 45 && abs(dify) < 45)
          show_ship(graphics);
      }
}
void impact_bonus(IGraphicsMiddleware *graphics)
{
  int i, difx = 0, dify = 0;
  for (i = 0; i < 10; i++)
    if (bullet[i].fel)
    {
      difx = bonus.x - shipx;
      dify = bonus.y - 350;
      if (difx > -15 && difx < 45)
        if (dify > -15 && dify < 45)
        {
          switch (bonus.fel)
          {
          case 1:
            give15(bns1, aux15);
            break;
          case 2:
            give15(bns2, aux15);
            break;
          case 3:
            give15(bns3, aux15);
            break;
          case 4:
            give15(bns4, aux15);
            break;
          }
          if (impact15(shp0, aux15, difx, dify))
          {
            negru15(graphics, aux15, bonus.x, bonus.y);
            show_ship(graphics);
            switch (bonus.fel)
            {
            case 1:
              exshiplife = shiplife;
              shiplife += 30;
              regl_ship_life();
              show_ship_life(graphics);
              break;
            case 2:
              exshiplife = shiplife;
              shiplife += 15;
              regl_ship_life();
              show_ship_life(graphics);
              break;
            case 3:
              if (mun1 < 32000)
                exmun1 = mun1;
              mun1 += 70;
              show_mun1(graphics);
              break;
            case 4:
              if (mun2 < 32000)
                exmun2 = mun2;
              mun2 += 35;
              show_mun2(graphics);
              break;
            }
            bonus.fel = 0;
          }
        }
    }
}

//----"Moovfc2.cpp"
void destroy_dead(IGraphicsMiddleware *graphics)
{
  for (i = 0; i < 10; i++)
    if (enemy[i].fel && enemy[i].hlt <= 0)
    {
      switch (enemy[i].fel)
      {
      case 1:
        negru(graphics, enm1, enemy[i].x, enemy[i].y);
        excash = cash;
        cash += 2;
        break;
      case 2:
        negru(graphics, enm2, enemy[i].x, enemy[i].y);
        excash = cash;
        cash += 1;
        break;
      case 3:
        negru(graphics, enm3, enemy[i].x, enemy[i].y);
        excash = cash;
        cash += 3;
        break;
      case 4:
        negru(graphics, enm4, enemy[i].x, enemy[i].y);
        excash = cash;
        cash += 2;
        break;
      case 5:
        negru(graphics, enm5, enemy[i].x, enemy[i].y);
        excash = cash;
        cash += 2;
        break;
      }
      show_cash(graphics);
      enemy[i].fel = 0;
      init_explozie(enemy[i].x, enemy[i].y);
      cur_enemy--;
      kills++;
    }
  for (i = 0; i < 7; i++)
    if (aster[i].fel && aster[i].hlt <= 0)
    {
      switch (aster[i].fel)
      {
      case 1:
        negru(graphics, ast1, aster[i].x, aster[i].y);
        break;
      case 2:
        negru(graphics, ast2, aster[i].x, aster[i].y);
        break;
      }
      excash = cash;
      cash++;
      show_cash(graphics);
      aster[i].fel = 0;
      init_explozie(aster[i].x, aster[i].y);
      cur_aster--;
    }
  if (shiplife <= 0)
  {
    game_over = 1;
    init_explozie(shipx, 350);
    negru(graphics, shp0, shipx, 350);
    aster_often = max_enemy = max_aster = 0;
    bonus_often = 1000;
  }
}
void timpul_trece()
{
  ltimp++;
  if (ltimp > 125)
  {
    ltimp = 0;
    timer++;
  }
}
void conditii_victorie(IGraphicsMiddleware *graphics)
{
  switch (mission_type)
  {
  case 0:
    if (timer >= limit)
      victory = 1;
    break;
  case 1:
    if (kills >= limit)
      victory = 1;
    break;
  case 2:
    if (cash >= limit)
      victory = 1;
  }
  if (victory)
  {
    for (i = 0; i < 10; i++)
      if (enemy[i].fel)
      {
        switch (enemy[i].fel)
        {
        case 1:
          negru(graphics, enm1, enemy[i].x, enemy[i].y);
          break;
        case 2:
          negru(graphics, enm2, enemy[i].x, enemy[i].y);
          break;
        case 3:
          negru(graphics, enm3, enemy[i].x, enemy[i].y);
          break;
        case 4:
          negru(graphics, enm4, enemy[i].x, enemy[i].y);
          break;
        case 5:
          negru(graphics, enm5, enemy[i].x, enemy[i].y);
          break;
        }
        enemy[i].fel = 0;
        init_explozie(enemy[i].x, enemy[i].y);
        cur_enemy--;
      }
    for (i = 0; i < 7; i++)
      if (aster[i].fel)
      {
        switch (aster[i].fel)
        {
        case 1:
          negru(graphics, ast1, aster[i].x, aster[i].y);
          break;
        case 2:
          negru(graphics, ast2, aster[i].x, aster[i].y);
          break;
        }
        aster[i].fel = 0;
        init_explozie(aster[i].x, aster[i].y);
        cur_aster--;
      }
    negru(graphics, shp0, shipx, 350);
    give(shp1, shp0);
    scotmem(graphics, shp1, shipx, 350);
    aster_often = max_enemy = max_aster = 0;
    bonus_often = 1000;
  }
}
void load_map(char *untext)
{
  int i;
  FILE *f;
  f = fopen(untext, "r");
  fscanf(f, "%d", &max_enemy);
  fscanf(f, "%d", &ship_types[0]);
  for (i = 1; i <= ship_types[0]; i++)
    fscanf(f, "%d", &ship_types[i]);
  fscanf(f, "%d", &ship_often);
  fscanf(f, "%d", &fire_often);
  fscanf(f, "%d", &bonus_often);
  fscanf(f, "%d", &max_aster);
  fscanf(f, "%d", &aster_often);
  fscanf(f, "%d%d", &mission_type, &limit);
  fscanf(f, "%d%d", &mun1, &mun2);
  exmun1 = mun1;
  exmun2 = mun2;
  fclose(f);
}
void init_joc_nou(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, char *harta)
{
  int i;
  cur_enemy = 0;
  cur_aster = 0;
  putmem("Ship1.dta", 0, shp1);
  putmem("Ship1.dta", 2115, shp2);
  putmem("Ship1.dta", 2115 * 2, shp3);
  give(shp1, shp0);
  tipnav = 0;
  shipx = 225;
  putmem("Gamedata\\Asteroid.dta", 0, ast1); // asteroizi
  putmem("Gamedata\\Asteroid.dta", 2115, ast2);
  putmem("Gamedata\\Boom.dta", 0, bm1); // boomuri
  putmem("Gamedata\\Boom.dta", 2115, bm2);
  putmem("Gamedata\\Boom.dta", 2 * 2115, bm3);
  putmem("Gamedata\\Boom.dta", 3 * 2115, bm4);
  putmem("Gamedata\\Boom.dta", 4 * 2115, bm5);
  putmem("Gamedata\\Boom.dta", 5 * 2115, bm6);
  putmem15("Gamedata\\Bonus.dta", 0, bns1); // bonusuri
  putmem15("Gamedata\\Bonus.dta", 255, bns2);
  putmem15("Gamedata\\Bonus.dta", 255 * 2, bns3);
  putmem15("Gamedata\\Bonus.dta", 255 * 3, bns4);
  putmem15("GameData\\Bullets.dta", 0, blt1); // gloante
  putmem15("GameData\\Bullets.dta", 255, blt2);
  putmem15("GameData\\Bullets.dta", 255 * 2, blt3);
  exshiplife = shiplife = 115;
  bonus.fel = 0;
  graphics->settextstyle(2, 0, 9);
  cash = 0;
  kills = 0;
  timer = 0;
  ltimp = 0;
  // incepe partea de tiparire
  paleta = "Freedom.pal";
  printfile(graphics, 450, 0, "Imagdata\\In_menu.dta", 190, 400);
  load_map(harta);
  show_cash(graphics);
  show_mun1(graphics);
  show_mun2(graphics);
  luminare(graphics, logic, 2);
  for (i = 1; i < 10; i++)
  {
    enemy[i].fel = enemy[i].x = enemy[i].y = enemy[i].vit = enemy[i].foc = 0;
    explo[i].faz = explo[i].x = explo[i].y = 0;
  }
  for (i = 0; i < 20; i++)
    bullet[i].fel = bullet[i].x = bullet[i].y = bullet[i].vit = bullet[i].dir = 0;
  bonus.x = bonus.y = bonus.fel = bonus.vit = 0;
  for (i = 0; i < 7; i++)
    aster[i].x = aster[i].y = aster[i].fel = aster[i].vit = aster[i].hlt = 0;
  goodbye = 0;
  game_over = 0;
  temp_bye = 0;
  player_terminate = 0;
  victory = 0;
}
void tell_mission_type(IGraphicsMiddleware *graphics)
{
  char s[20], s1[20];
  switch (mission_type)
  {
  case 0:
    sprintf(s, "Rezista %d", limit);
    sprintf(s1, "secunde de joc.");
    break;
  case 1:
    sprintf(s, "Distruge %d", limit);
    sprintf(s1, "inamici.");
    break;
  case 2:
    sprintf(s, "Acumuleaza %d", limit);
    sprintf(s1, "credite(bani).");
    break;
  }
  graphics->settextstyle(2, 0, 6);
  graphics->outtextxy(480, 260, "Scopul misiunii");
  graphics->outtextxy(480, 270, "_______________");
  graphics->outtextxy(480, 310, s);
  graphics->outtextxy(475, 325, s1);
  graphics->settextstyle(2, 0, 9);
}
void quit_question(IGraphicsMiddleware *graphics)
{
  int i, go_on = 0;
  char c;
  write_file(graphics, "SWAP.dta", 135, 125, 270, 150);
  printfile(graphics, 135, 125, "Imagdata\\Quit.dta", 270, 150);
  do
  {
    c = getch();
    if (c == 13)
    {
      temp_bye = 1;
      go_on = 1;
      player_terminate = 1;
    }
    if (c == 27)
      go_on = 1;
  } while (!go_on);
  printfile(graphics, 135, 125, "SWAP.dta", 270, 150);
}

void game(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, char *harta)
{
  char tasta;
  init_joc_nou(graphics, logic, harta);
  init_stele(logic);
  init_culori_stele(graphics);
  init_inamici(logic);
  init_gloante();
  show_initial_ship_life(graphics);
  tell_mission_type(graphics);
  scotmem(graphics, shp0, shipx, 350);
  do
  {
    muta_stele(graphics, logic);
    muta_nava(graphics, logic);
    muta_gloante_en(graphics, logic);
    muta_inamici(graphics, logic);
    muta_aster(graphics, logic);
    muta_gloante_sh(graphics);
    muta_bonus(graphics, logic);
    impact_nava(graphics);
    impact_aster(graphics);
    impact_bullets(graphics);
    impact_bullets_aster(graphics);
    impact_bullets_nava(graphics);
    impact_bonus(graphics);
    destroy_dead(graphics);
    conditii_victorie(graphics);
    anim_explozii(graphics);
    impact_boom(graphics);
    if (mouse_btn == 1 && mun1 > 0)
    {
      exmun1 = mun1;
      init_glont_sh(1);
      show_mun1(graphics);
    }
    if (mouse_btn == 2 && mun2 > 0)
    {
      exmun2 = mun2;
      init_glont_sh(2);
      show_mun2(graphics);
    }
    if (fwaiter < 100)
      fwaiter++;
    logic->delay(8);
    timpul_trece();
    if (kbhit())
    {
      c = getch();
      if (c == 27)
        quit_question(graphics);
    }
  } while (!game_over && !temp_bye && !goodbye && !victory);
  if (game_over)
  {
    for (int kk = 1; kk <= 100; kk++)
    {
      muta_stele(graphics, logic);
      muta_gloante_en(graphics, logic);
      muta_inamici(graphics, logic);
      muta_aster(graphics, logic);
      muta_gloante_sh(graphics);
      muta_bonus(graphics, logic);
      anim_explozii(graphics);
      logic->delay(8);
    }
    xstardel(graphics);
    printfile(graphics, 135, 125, "Imagdata\\Youdied.dta", 270, 150);
    xstarchk();
    do
    {
      muta_stele(graphics, logic);
      logic->delay(8);
      if (kbhit())
      {
        c = getch();
        if (c == 13)
          temp_bye = 1;
      }
    } while (!temp_bye);
    temp_bye = 0;
  }
  if (victory)
  {
    for (int kk = 1; kk <= 100; kk++)
    {
      muta_stele(graphics, logic);
      muta_gloante_en(graphics, logic);
      muta_inamici(graphics, logic);
      muta_aster(graphics, logic);
      muta_gloante_sh(graphics);
      muta_bonus(graphics, logic);
      impact_bullets_nava(graphics);
      anim_explozii(graphics);
      show_ship(graphics);
      logic->delay(8);
    }
    xstardel(graphics);
    printfile(graphics, 135, 125, "Imagdata\\Youwon.dta", 270, 150);
    xstarchk();
    do
    {
      muta_stele(graphics, logic);
      logic->delay(8);
      if (kbhit())
      {
        c = getch();
        if (c == 13)
          temp_bye = 1;
      }
    } while (!temp_bye);
    temp_bye = 0;
  }
}
void ecran_intro(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, int cm, char s_prim[25])
{
  char s[25], c;
  load_palette(graphics, "Imagdata\\loading.pal", 0);
  graphics->cleardevice();
  graphics->setcolor(254);
  graphics->rectangle(149, 149, 450, 250);
  paleta = "Imagdata\\loading.pal";
  printfile(graphics, 150, 150, "Imagdata\\Loading.dta", 300, 100);
  sprintf(s, "Misiunea cu numarul:%d", cm);
  graphics->settextstyle(2, 0, 5);
  graphics->outtextxy(160, 160, s);
  graphics->outtextxy(160, 175, s_prim);
  graphics->outtextxy(160, 190, "Apasa <Enter> pt. a continua");
  luminare(graphics, logic, 2);
  do
  {
    c = getch();
  } while (c != 13);
  intunecare(graphics, logic, 2);
  paleta = "Freedom.pal";
  graphics->cleardevice();
}
void ecran_victorie(IGraphicsMiddleware *graphics)
{
  char c;
  graphics->cleardevice();
  graphics->setcolor(100);
  graphics->rectangle(150, 150, 400, 250);
  graphics->settextstyle(2, 0, 5);
  graphics->outtextxy(160, 160, "Ai terminat toate misiunile.");
  graphics->outtextxy(160, 175, "Acesta a fost DEMO-ul jocului.");
  graphics->outtextxy(160, 190, "Vrei mai mult? Incearca tot jocul!");
  graphics->outtextxy(160, 205, "Apasa <Enter> pt. a continua.");
  do
  {
    c = getch();
  } while (c != 13);
}

void jocnou(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  ecran_intro(graphics, logic, 1, "Maps\\Map1.map");
  game(graphics, logic, "Maps\\Map1.map");
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 2, "Maps\\Map2.map");
    game(graphics, logic, "Maps\\Map2.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 3, "Maps\\Map3.map");
    game(graphics, logic, "Maps\\Map3.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 4, "Maps\\Map4.map");
    game(graphics, logic, "Maps\\Map4.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 5, "Maps\\Map5.map");
    game(graphics, logic, "Maps\\Map5.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 6, "Maps\\Map6.map");
    game(graphics, logic, "Maps\\Map6.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 7, "Maps\\Map7.map");
    game(graphics, logic, "Maps\\Map7.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 8, "Maps\\Map8.map");
    game(graphics, logic, "Maps\\Map8.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 9, "Maps\\Map9.map");
    game(graphics, logic, "Maps\\Map9.map");
  }
  if (!game_over && !player_terminate)
  {
    ecran_intro(graphics, logic, 10, "Maps\\Map10.map");
    game(graphics, logic, "Maps\\Map10.map");
  }
  if (!game_over && !player_terminate)
    ecran_victorie(graphics);
}

// #include "Logo.cpp"
void init_logo_state(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  load_palette(graphics, "imagdata/logo.pal", 1.0);
  printfile(graphics, 0, 0, "imagdata/logo.dta", 640, 400);
  state_count_timer = 0;
}

FF2State logo_state_loop(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, unsigned elapsed_milliseconds)
{
  state_count_timer += elapsed_milliseconds;
  if (state_count_timer < 1000)
  {
    double luminosity = 1.0 - (double)(1000 - state_count_timer) / 1000.0;
    load_palette(graphics, "imagdata/logo.pal", luminosity);
  }
  else if (state_count_timer >= 1000 && state_count_timer < 3000)
  {
    load_palette(graphics, "imagdata/logo.pal", 1.0);
  }
  else if (state_count_timer >= 3000 && state_count_timer < 4000)
  {
    double luminosity = 1.0 - (double)(state_count_timer - 3000) / 1000.0;
    load_palette(graphics, "imagdata/logo.pal", luminosity);
  }
  else if (state_count_timer > 4000)
  {
    // switch to menu state
    return FF2_STATE_MAIN_MENU;
  }

  if (logic->anyKeyPressed())
  {
    return FF2_STATE_MAIN_MENU;
  }

  printfile(graphics, 0, 0, "imagdata/logo.dta", 640, 400);
  graphics->settextstyle(2, 0, 5);
  graphics->setcolor(252);
  graphics->outtextxy(5, 5, "O productie Gusty-2001");

  return FF2_STATE_LOADING_SCREEN;
}
// Logo.cpp

//------

void optiuni(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int i, xex, byenow = 0;
  char c, *s;
  printfile(graphics, 230, 185, "Imagdata\\Nobuts.dta", 180, 215);
  putmem("Imagdata\\Sistem.dta", 0, shp0);
  graphics->setcolor(0);
  for (i = 35; i < 90; i++)
    graphics->line(110, i, 500, i);
  graphics->setcolor(255);
  graphics->rectangle(110, 35, 500, 90);
  xex = 120 + (luminozitate - 0.7) * 10 * 40;
  scotmem(graphics, shp0, xex, 40);
  graphics->setcolor(255);
  graphics->settextstyle(2, 0, 5);
  graphics->outtextxy(35, 250, "Apasa <ESC> pt. �ntoarcere �n meniul principal.");
  graphics->outtextxy(25, 110, "Apasa tastele 1..9 pentru a modifica luminozitatea imaginii.");
  graphics->outtextxy(50, 145, "Control:");
  graphics->outtextxy(30, 155, "Buton mouse stanga=> foc primar (bila energica).");
  graphics->outtextxy(30, 165, "Buton mouse dreapta=> foc secundar (tun laser).");
  graphics->outtextxy(30, 175, "Miscare mouse stanga-dreapta => miscare nava");
  graphics->outtextxy(450, 385, "O productie Gusty-2001");
  luminare(graphics, logic, 2);
  for (i = 1; i < 10; i++)
  {
    sprintf(s, "%d", i);
    graphics->outtextxy(100 + 40 * i, 90, s);
  }
  do
  {
    c = getch();
    if (c == 27)
      byenow = 1;
    else
      switch (c)
      {
      case '1':
        negru(graphics, shp0, xex, 40);
        luminozitate = 0.7;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '2':
        negru(graphics, shp0, xex, 40);
        luminozitate = 0.8;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '3':
        negru(graphics, shp0, xex, 40);
        luminozitate = 0.9;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '4':
        negru(graphics, shp0, xex, 40);
        luminozitate = 1;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '5':
        negru(graphics, shp0, xex, 40);
        luminozitate = 1.1;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '6':
        negru(graphics, shp0, xex, 40);
        luminozitate = 1.2;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '7':
        negru(graphics, shp0, xex, 40);
        luminozitate = 1.3;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '8':
        negru(graphics, shp0, xex, 40);
        luminozitate = 1.4;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      case '9':
        negru(graphics, shp0, xex, 40);
        luminozitate = 1.5;
        xex = 120 + (luminozitate - 0.7) * 10 * 40;
        scotmem(graphics, shp0, xex, 40);
        load_palette(graphics, paleta, luminozitate);
        break;
      }
  } while (!byenow);
  intunecare(graphics, logic, 2);
}
void credits(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  printfile(graphics, 230, 185, "Imagdata\\Nobuts.dta", 180, 215);
  graphics->settextstyle(2, 0, 5);
  graphics->setcolor(252);
  graphics->outtextxy(5, 5, "O productie Gusty-2001");
  graphics->outtextxy(65, 80, "Programul acesta a fost realizat integral de Preda Livius Augustin.");
  graphics->outtextxy(25, 90, "...cunoscut si sub numele de Gusty.");
  graphics->outtextxy(25, 100, "Doresc pe aceasta cale sa �mi salut colegii din Lic. Info.");
  graphics->outtextxy(25, 110, "(salut!).");
  graphics->outtextxy(30, 200, "Grafica jocului a fost realizata cu ajutorul diferitor programe");
  graphics->outtextxy(30, 210, "de specialitate si cu ajutorul motorului grafic propriu bazat pe");
  graphics->outtextxy(30, 220, "Converter 2000-(copyright Gusty 2001).");
  graphics->outtextxy(30, 230, "Multumiri speciale lui Mocanu Bogdan, fara a carui contributie la");
  graphics->outtextxy(30, 240, "dezvoltarea motorului grafic, calitatea imaginii ar fi avut de suferit");
  graphics->outtextxy(40, 250, "Program realizat pt. ATESTAT 2002");
  graphics->outtextxy(230, 380, "...Apasati orice tasta pt. a reveni la meniul principal.");
  luminare(graphics, logic, 2);
  getch();
}

void pune_fundal_meniu(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  int i;
  paleta = "imagdata/menu.pal";
  printfile(graphics, 0, 0, "imagdata/mainmenu.dta", 640, 400);
  for (i = 185; i <= 335; i += 50)
    printfile(graphics, 230, i, "imagdata/buton.dta", 180, 40);
  graphics->settextstyle(2, 0, 8);
  text_shadow(graphics, 250, 190, "(N)ew Game", 200);
  text_shadow(graphics, 250, 240, " (O)ptions", 200);
  text_shadow(graphics, 250, 290, " (C)redits", 200);
  text_shadow(graphics, 250, 340, "  E(x)it", 200);
}

void init_main_menu_state(IGraphicsMiddleware *graphics, ILogicMiddleware *logic)
{
  state_count_timer = 0;
}

FF2State main_menu_loop(IGraphicsMiddleware *graphics, ILogicMiddleware *logic, unsigned elapsed_milliseconds)
{
  state_count_timer += elapsed_milliseconds;

  pune_fundal_meniu(graphics, logic);

  if (logic->isKeyPressed('n'))
  {
    return FF2_STATE_GAME;
  }
  else if (logic->isKeyPressed('o'))
  {
    return FF2_STATE_OPTIONS;
  }
  else if (logic->isKeyPressed('c'))
  {
    return FF2_STATE_CREDITS;
  }
  else if (logic->isKeyPressed('x'))
  {
    return FF2_STATE_EXIT;
  }

  return FF2_STATE_MAIN_MENU;
}

//------

void printfile0(IGraphicsMiddleware *graphics, int x, int y, char untext[30], int dm1, int dm2)
{
  FILE *f = NULL;
  unsigned char c;
  int i, j;
  for (i = 0; j < 640; j++)
    global_a_line[i] = 0;
  f = fopen(untext, "r");
  for (i = 0; i < dm2; i++)
  {
    for (j = 0; j < dm1; j++)
    {
      fscanf(f, "%c", &c);
      if (c)
        global_a_line[j] = c;
    }
    for (j = 0; j < dm1; j++)
      if (c >= 33)
        graphics->putpixel(x + j, y + i, global_a_line[j]);
    fscanf(f, "%c", &c);
  }
  fclose(f);
}

//---

void FF2::main_loop(unsigned elapsed_milliseconds)
{
  if (this->game_state_ == FF2_STATE_LOADING_SCREEN)
  {
    FF2State new_state = logo_state_loop(graphics_, logic_, elapsed_milliseconds);
    if (new_state != this->game_state_)
    {
      switch_to_state(new_state);
    }
  }
  else if (this->game_state_ == FF2_STATE_MAIN_MENU)
  {
    FF2State new_state = main_menu_loop(graphics_, logic_, elapsed_milliseconds);
    if (new_state != this->game_state_)
    {
      switch_to_state(new_state);
    }
  }
  else if (this->game_state_ == FF2_STATE_GAME)
  {
  }
}

void FF2::switch_to_state(FF2State state)
{
  this->game_state_ = state;
  switch (state)
  {
  case FF2_STATE_LOADING_SCREEN:
    init_logo_state(graphics_, logic_);
    break;

  case FF2_STATE_MAIN_MENU:
    init_main_menu_state(graphics_, logic_);
    break;

  default:
    break;
  }
}
