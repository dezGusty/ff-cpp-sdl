#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "mouse.h"
char c,*paleta;
unsigned char a[640],caz[10],aux[45][45];
int but,x,y,i,j,goodbye,game_over,victory,shipx,shiplife,exshiplife,kk;
int tipnav,extipnav,waiter,fwaiter;//pt mutarile navei &foc
long mun1,mun2,cash,exmun1,exmun2,excash,kills,timer,ltimp;
int temp_bye=0;//quit pt joc in desfasurare
int player_terminate;//idem

unsigned char shp1[45][45],shp2[45][45],shp3[45][45],shp0[45][45];
unsigned char enm1[45][45],enm2[45][45],enm3[45][45],enm4[45][45],enm5[45][45];
unsigned char bm1[45][45],bm2[45][45],bm3[45][45],bm4[45][45],bm5[45][45],bm6[45][45];
unsigned char blt1[15][15],blt2[15][15],blt3[15][15],blt4[15][15],blt5[15][15],aux15[15][15];
unsigned char bns1[15][15],bns2[15][15],bns3[15][15],bns4[15][15];
unsigned char ast1[45][45],ast2[45][45];
float luminozitate;
//tipul ce va defini stelele
typedef struct
	  {int cul,x,y,vit,ok;
	  }STELAR;
//tipul ce va defini navele inamice
typedef struct
	  {int fel,x,y,hlt,vit,foc;
	  }ENEMY;
//tipul de va defini gloantele
typedef struct
	  {int fel,vit,x,y,dir;
	  }GLONT;
//tipul ce va defini exploziile
typedef struct
	  {int faz,x,y;
	  }EXPLOZ;
typedef struct
	  {int x,y,fel,vit;
	  }BONUS;
typedef struct
	  {int x,y,fel,vit,hlt;
	  }ASTEROID;
STELAR st1[40],st2[40],st3[40];
ENEMY enemy[10];
EXPLOZ explo[10];
GLONT bullet[20];
BONUS bonus;
ASTEROID aster[7];
int ship_types[10],ship_often,bonus_often,max_enemy,cur_enemy,fire_often;
int max_aster,cur_aster,aster_often,mission_type,limit;

#include "Graphunc.cpp"
void initializari(void)
{int g1=installuserdriver("VESA",NULL),g2=1;
 randomize();
 initgraph(&g1,&g2,"");
 if(graphresult()!=grOk)
 {
  clrscr();
  printf("Error working with file \"VESA.BGI\"\n");
  exit(1);
 }
 load_palette("Imagdata\\menu.pal",0);
 luminozitate=1.1;
 mouseinit();
 mousemove(100,0);
}
void give(unsigned char a[45][45],unsigned char b[45][45])
{int i,j;
 for(i=0;i<45;i++)for(j=0;j<45;j++)
   b[i][j]=a[i][j];
}
void give15(unsigned char a[15][15],unsigned char b[15][15])
{int i,j;
 for(i=0;i<15;i++)for(j=0;j<15;j++)
   b[i][j]=a[i][j];
}

//procedurile legate de stele
void init_culori_stele()
{setrgbpalette(1,30,30,30);
 setrgbpalette(2,30,0,0);
 setrgbpalette(3,0,0,30);
 setrgbpalette(4,30,0,30);
 setrgbpalette(5,45,45,45);
 setrgbpalette(6,45,0,0);
 setrgbpalette(7,0,0,45);
 setrgbpalette(8,45,0,45);
 setrgbpalette(9,63,63,63);
 setrgbpalette(10,63,45,0);
 setrgbpalette(11,0,50,3);
 setrgbpalette(12,63,50,63);
}
void init_stea(STELAR &p,int zon)
{p.cul=random(4)+1+4*zon;
 p.x=random(450);
 p.y=0;
 p.vit=random(2)+zon*2+1;
 p.ok=1;
}
void init_stele()
{int i;
 for(i=0;i<40;i++)
   {init_stea(st1[i],0);
    init_stea(st2[i],0);
    init_stea(st3[i],0);
    st1[i].y=random(400);
    st2[i].y=random(400);
    st3[i].y=random(400);
   }
}
void muta_stele()
{int i;
 for(i=0;i<40;i++)
   {if(st1[i].ok)putpixel(st1[i].x,st1[i].y,0);
    if(st2[i].ok)putpixel(st2[i].x,st2[i].y,0);
    if(st3[i].ok)putpixel(st3[i].x,st3[i].y,0);
    st1[i].y+=st1[i].vit;
    st2[i].y+=st2[i].vit;
    st3[i].y+=st3[i].vit;
    if(getpixel(st1[i].x,st1[i].y))st1[i].ok=0;else st1[i].ok=1;
    if(getpixel(st2[i].x,st2[i].y))st2[i].ok=0;else st2[i].ok=1;
    if(getpixel(st3[i].x,st3[i].y))st3[i].ok=0;else st3[i].ok=1;
    if(st1[i].ok)putpixel(st1[i].x,st1[i].y,st1[i].cul);
    if(st2[i].ok)putpixel(st2[i].x,st2[i].y,st2[i].cul);
    if(st3[i].ok)putpixel(st3[i].x,st3[i].y,st3[i].cul);
   }
 for(i=0;i<40;i++)
   {if(st1[i].y>400)
     init_stea(st1[i],0);
    if(st2[i].y>400)
     init_stea(st2[i],1);
    if(st3[i].y>400)
     init_stea(st3[i],2);
   }
}
void xstardel()
{int i;
 for(i=0;i<40;i++)
   {if(st1[i].ok)putpixel(st1[i].x,st1[i].y,0);
    if(st2[i].ok)putpixel(st2[i].x,st2[i].y,0);
    if(st3[i].ok)putpixel(st3[i].x,st3[i].y,0);
   }
}
void xstarchk()
{int i;
 for(i=0;i<40;i++)
   {if(st1[i].ok)st1[i].ok=0;
    if(st2[i].ok)st2[i].ok=0;
    if(st3[i].ok)st3[i].ok=0;
   }
}
#include "Moovfc2.cpp"
void destroy_dead()
{for(i=0;i<10;i++)
 if(enemy[i].fel && enemy[i].hlt<=0)
  {switch(enemy[i].fel)
    {case 1:negru(enm1,enemy[i].x,enemy[i].y);excash=cash;cash+=2;break;
     case 2:negru(enm2,enemy[i].x,enemy[i].y);excash=cash;cash+=1;break;
     case 3:negru(enm3,enemy[i].x,enemy[i].y);excash=cash;cash+=3;break;
     case 4:negru(enm4,enemy[i].x,enemy[i].y);excash=cash;cash+=2;break;
     case 5:negru(enm5,enemy[i].x,enemy[i].y);excash=cash;cash+=2;break;
    }
   show_cash();
   enemy[i].fel=0;
   init_explozie(enemy[i].x,enemy[i].y);
   cur_enemy--;
   kills++;
  }
 for(i=0;i<7;i++)
 if(aster[i].fel && aster[i].hlt<=0)
  {switch(aster[i].fel)
    {case 1:negru(ast1,aster[i].x,aster[i].y);break;
     case 2:negru(ast2,aster[i].x,aster[i].y);break;
    }
   excash=cash;cash++;
   show_cash();
   aster[i].fel=0;
   init_explozie(aster[i].x,aster[i].y);
   cur_aster--;
  }
 if(shiplife<=0)
  {game_over=1;
   init_explozie(shipx,350);
   negru(shp0,shipx,350);
   aster_often=max_enemy=max_aster=0;
   bonus_often=1000;
   }
}
void timpul_trece()
{ltimp++;
 if(ltimp>125)
   {ltimp=0;
    timer++;
   }
}
void conditii_victorie()
{switch(mission_type)
  {case 0:if(timer>=limit)victory=1;break;
   case 1:if(kills>=limit)victory=1;break;
   case 2:if(cash>=limit)victory=1;
  }
 if(victory)
  {for(i=0;i<10;i++)
    if(enemy[i].fel)
    {switch(enemy[i].fel)
      {case 1:negru(enm1,enemy[i].x,enemy[i].y);break;
       case 2:negru(enm2,enemy[i].x,enemy[i].y);break;
       case 3:negru(enm3,enemy[i].x,enemy[i].y);break;
       case 4:negru(enm4,enemy[i].x,enemy[i].y);break;
       case 5:negru(enm5,enemy[i].x,enemy[i].y);break;
      }
     enemy[i].fel=0;
     init_explozie(enemy[i].x,enemy[i].y);
     cur_enemy--;
    }
   for(i=0;i<7;i++)
    if(aster[i].fel)
      {switch(aster[i].fel)
	{case 1:negru(ast1,aster[i].x,aster[i].y);break;
	 case 2:negru(ast2,aster[i].x,aster[i].y);break;
	}
       aster[i].fel=0;
       init_explozie(aster[i].x,aster[i].y);
       cur_aster--;
      }
    negru(shp0,shipx,350);
    give(shp1,shp0);
    scotmem(shp1,shipx,350);
    aster_often=max_enemy=max_aster=0;
    bonus_often=1000;
  }
}
void load_map(char *untext)
{int i;
 FILE *f;
 f=fopen(untext,"r");
 fscanf(f,"%d",&max_enemy);
 fscanf(f,"%d",&ship_types[0]);
 for(i=1;i<=ship_types[0];i++)
   fscanf(f,"%d",&ship_types[i]);
 fscanf(f,"%d",&ship_often);
 fscanf(f,"%d",&fire_often);
 fscanf(f,"%d",&bonus_often);
 fscanf(f,"%d",&max_aster);
 fscanf(f,"%d",&aster_often);
 fscanf(f,"%d%d",&mission_type,&limit);
 fscanf(f,"%d%d",&mun1,&mun2);
 exmun1=mun1;exmun2=mun2;
 fclose(f);
}
void init_joc_nou(char *harta)
{int i;
 cur_enemy=0;
 cur_aster=0;
 putmem("Ship1.dta",0,shp1);
 putmem("Ship1.dta",2115,shp2);
 putmem("Ship1.dta",2115*2,shp3);
 give(shp1,shp0);
 tipnav=0;
 shipx=225;
 putmem("Gamedata\\Asteroid.dta",0,ast1);//asteroizi
 putmem("Gamedata\\Asteroid.dta",2115,ast2);
 putmem("Gamedata\\Boom.dta",0,bm1);//boomuri
 putmem("Gamedata\\Boom.dta",2115,bm2);
 putmem("Gamedata\\Boom.dta",2*2115,bm3);
 putmem("Gamedata\\Boom.dta",3*2115,bm4);
 putmem("Gamedata\\Boom.dta",4*2115,bm5);
 putmem("Gamedata\\Boom.dta",5*2115,bm6);
 putmem15("Gamedata\\Bonus.dta",0,bns1);//bonusuri
 putmem15("Gamedata\\Bonus.dta",255,bns2);
 putmem15("Gamedata\\Bonus.dta",255*2,bns3);
 putmem15("Gamedata\\Bonus.dta",255*3,bns4);
 putmem15("GameData\\Bullets.dta",0,blt1);//gloante
 putmem15("GameData\\Bullets.dta",255,blt2);
 putmem15("GameData\\Bullets.dta",255*2,blt3);
 exshiplife=shiplife=115;
 bonus.fel=0;
 settextstyle(2,0,9);
 cash=0;
 kills=0;
 timer=0;
 ltimp=0;
 //incepe partea de tiparire
 paleta="Freedom.pal";
 printfile(450,0,"Imagdata\\In_menu.dta",190,400);
 load_map(harta);
 show_cash();
 show_mun1();
 show_mun2();
 luminare(2);
 for(i=1;i<10;i++)
  {enemy[i].fel=enemy[i].x=enemy[i].y=enemy[i].vit=enemy[i].foc=0;
   explo[i].faz=explo[i].x=explo[i].y=0;
  }
 for(i=0;i<20;i++)
  bullet[i].fel=bullet[i].x=bullet[i].y=bullet[i].vit=bullet[i].dir=0;
 bonus.x=bonus.y=bonus.fel=bonus.vit=0;
 for(i=0;i<7;i++)
   aster[i].x=aster[i].y=aster[i].fel=aster[i].vit=aster[i].hlt=0;
 goodbye=0;
 game_over=0;
 temp_bye=0;
 player_terminate=0;
 victory=0;
}
void tell_mission_type()
{char s[20],s1[20];
 switch(mission_type)
  {case 0:sprintf(s,"Rezista %d",limit);
	  sprintf(s1,"secunde de joc.");
	  break;
   case 1:sprintf(s,"Distruge %d",limit);
	  sprintf(s1,"inamici.");
	  break;
   case 2:sprintf(s,"Acumuleaza %d",limit);
	  sprintf(s1,"credite(bani).");break;
  }
 settextstyle(2,0,6);
 outtextxy(480,260,"Scopul misiunii");
 outtextxy(480,270,"_______________");
 outtextxy(480,310,s);
 outtextxy(475,325,s1);
 settextstyle(2,0,9);
}
void quit_question()
{int i,go_on=0;
 char c;
 write_file("SWAP.dta",135,125,270,150);
 printfile(135,125,"Imagdata\\Quit.dta",270,150);
 do
 {c=getch();
  if(c==13){temp_bye=1;go_on=1;player_terminate=1;}
  if(c==27) go_on=1;
 }
 while(!go_on);
 printfile(135,125,"SWAP.dta",270,150);
}
void game(char *harta)
{char tasta;
 init_joc_nou(harta);
 init_stele();
 init_culori_stele();
 init_inamici();
 init_gloante();
 show_initial_ship_life();
 tell_mission_type();
 scotmem(shp0,shipx,350);
 do
 {muta_stele();
  muta_nava();
  muta_gloante_en();
  muta_inamici();
  muta_aster();
  muta_gloante_sh();
  muta_bonus();
  impact_nava();
  impact_aster();
  impact_bullets();
  impact_bullets_aster();
  impact_bullets_nava();
  impact_bonus();
  destroy_dead();
  conditii_victorie();
  anim_explozii();
  impact_boom();
  if(but==1 && mun1>0){exmun1=mun1;init_glont_sh(1);show_mun1();}
  if(but==2 && mun2>0){exmun2=mun2;init_glont_sh(2);show_mun2();}
  if(fwaiter<100)fwaiter++;
  delay(8);
  timpul_trece();
  if(kbhit())
    {c=getch();
     if(c==27) quit_question();
    }
 }
 while(!game_over && !temp_bye && !goodbye && !victory);
 if(game_over)
  {for(kk=1;kk<=100;kk++)
   {muta_stele();
    muta_gloante_en();
    muta_inamici();
    muta_aster();
    muta_gloante_sh();
    muta_bonus();
    anim_explozii();
    delay(8);
   }
   xstardel();
   printfile(135,125,"Imagdata\\Youdied.dta",270,150);
   xstarchk();
   do
    {muta_stele();
     delay(8);
     if(kbhit()){c=getch();if(c==13)temp_bye=1;}
    }
   while(!temp_bye);
   temp_bye=0;
  }
 if(victory)
  {for(kk=1;kk<=100;kk++)
   {muta_stele();
    muta_gloante_en();
    muta_inamici();
    muta_aster();
    muta_gloante_sh();
    muta_bonus();
    impact_bullets_nava();
    anim_explozii();
    show_ship();
    delay(8);
   }
   xstardel();
   printfile(135,125,"Imagdata\\Youwon.dta",270,150);
   xstarchk();
   do
    {muta_stele();
     delay(8);
     if(kbhit()){c=getch();if(c==13)temp_bye=1;}
    }
   while(!temp_bye);
   temp_bye=0;
  }
}
void ecran_intro(int cm,char s_prim[25])
{char s[25],c;
 load_palette("Imagdata\\loading.pal",0);
 cleardevice();
 setcolor(254);
 rectangle(149,149,450,250);
 paleta="Imagdata\\loading.pal";
 printfile(150,150,"Imagdata\\Loading.dta",300,100);
 sprintf(s,"Misiunea cu numarul:%d",cm);
 settextstyle(2,0,5);
 outtextxy(160,160,s);
 outtextxy(160,175,s_prim);
 outtextxy(160,190,"Apasa <Enter> pt. a continua");
 luminare(2);
 do
  {c=getch();
  }
 while(c!=13);
 intunecare(2);
 paleta="Freedom.pal";
 cleardevice();
}
void ecran_victorie()
{char c;
 cleardevice();
 setcolor(100);
 rectangle(150,150,400,250);
 settextstyle(2,0,5);
 outtextxy(160,160,"Ai terminat toate misiunile.");
 outtextxy(160,175,"Acesta a fost DEMO-ul jocului.");
 outtextxy(160,190,"Vrei mai mult? Incearca tot jocul!");
 outtextxy(160,205,"Apasa <Enter> pt. a continua.");
 do
  {c=getch();
  }
 while(c!=13);
}
void jocnou()
{
 ecran_intro(1,"Maps\\Map1.map");
 game("Maps\\Map1.map");
 if(!game_over && !player_terminate)
  {ecran_intro(2,"Maps\\Map2.map");
   game("Maps\\Map2.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(3,"Maps\\Map3.map");
   game("Maps\\Map3.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(4,"Maps\\Map4.map");
   game("Maps\\Map4.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(5,"Maps\\Map5.map");
   game("Maps\\Map5.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(6,"Maps\\Map6.map");
   game("Maps\\Map6.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(7,"Maps\\Map7.map");
   game("Maps\\Map7.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(8,"Maps\\Map8.map");
   game("Maps\\Map8.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(9,"Maps\\Map9.map");
   game("Maps\\Map9.map");
  }
 if(!game_over && !player_terminate)
  {ecran_intro(10,"Maps\\Map10.map");
   game("Maps\\Map10.map");
  }
 if(!game_over && !player_terminate)
  ecran_victorie();
}
#include "Logo.cpp"
#include "Meniu2.cpp"
void main(void)
{char tasta;
 initializari();
 logofunc();
 meniu_principal();
 closegraph();
}