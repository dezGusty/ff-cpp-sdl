//procedurile legate de nava
void muta_nava()
{int dif,i,fostx=shipx;
 mousedata(but,x,y);
 extipnav=tipnav;
 if(x!=96)
  {dif=(x-96)/4;
   if(dif>0 && dif<=15)dif=5;
   if(dif>15)dif=10;
   if(dif<0 && dif>=-15)dif=-5;
   if(dif<-15)dif=-10;
   if(dif>0)tipnav=1;
   if(dif<0)tipnav=-1;
   if(dif==0)tipnav=0;
   //^clarificarea diferentelor^
   mousemove(100,100);
   negru(shp0,shipx,350);
   shipx=shipx+dif;
   if(shipx<1)shipx=1;
   if(shipx>405)shipx=405;
  }
 else if(waiter==12)tipnav=0;
      else waiter++;
 if(x!=96 || tipnav!=extipnav)negru(shp0,fostx,350);
 if(extipnav!=tipnav)
  {switch(tipnav)
   {case 0:give(shp1,shp0);break;
    case 1:give(shp2,shp0);break;
    case -1:give(shp3,shp0);break;
   }
   waiter=0;
  }
 if(x!=96 || tipnav!=extipnav)scotmem(shp0,shipx,350);
}

//procedurile legate de inamici
void init_inamic(ENEMY &p)
{int i,k,uniq=1;
 k=random(405)+1;
 k=k/5*5;
 for(i=1;i<10;i++)
   if(uniq && enemy[i].fel)
    if(k<enemy[i].x+40 && k>enemy[i].x-40)
     if(enemy[i].y<0)uniq=0;
 if(uniq)
  {p.x=k;
   p.y=random(100)-150;
   k=random(ship_types[0])+1;
   p.fel=ship_types[k];
   switch(p.fel)
   {case 1:p.vit=7;p.hlt=2;break;
    case 2:p.vit=9;p.hlt=1;break;
    case 3:p.vit=6;p.hlt=3;break;
    case 4:p.vit=7;p.hlt=2;break;
    case 5:p.vit=8;p.hlt=2;break;
   }
   cur_enemy++;
  }
}
void init_inamici()
{int i,k;
 putmem("Gamedata\\enemy1.dta",0,enm1);
 putmem("Gamedata\\enemy2.dta",0,enm2);
 putmem("Gamedata\\enemy3.dta",0,enm3);
 putmem("Gamedata\\enemy4.dta",0,enm4);
 putmem("Gamedata\\enemy5.dta",0,enm5);
 for(i=1;i<=ship_types[0];i++)
  if(cur_enemy<max_enemy)
  {k=random(ship_often)+1;
   if(k==1) init_inamic(enemy[i]);
  }
}
void muta_inamici()
{int i,k;
 for(i=1;i<10;i++)
  if(enemy[i].fel)
   {switch(enemy[i].fel)
    {case 1:x_negru45(enm1,enemy[i].x,enemy[i].y,enemy[i].vit);break;
     case 2:x_negru45(enm2,enemy[i].x,enemy[i].y,enemy[i].vit);break;
     case 3:x_negru45(enm3,enemy[i].x,enemy[i].y,enemy[i].vit);break;
     case 4:x_negru45(enm4,enemy[i].x,enemy[i].y,enemy[i].vit);break;
     case 5:x_negru45(enm5,enemy[i].x,enemy[i].y,enemy[i].vit);break;
    }
    enemy[i].y+=enemy[i].vit;
    if(enemy[i].y>400){enemy[i].fel=0;cur_enemy--;}
    if(enemy[i].y>-45 && enemy[i].y<400)
    switch(enemy[i].fel)
    {case 1:scotmem(enm1,enemy[i].x,enemy[i].y);break;
     case 2:scotmem(enm2,enemy[i].x,enemy[i].y);break;
     case 3:scotmem(enm3,enemy[i].x,enemy[i].y);break;
     case 4:scotmem(enm4,enemy[i].x,enemy[i].y);break;
     case 5:scotmem(enm5,enemy[i].x,enemy[i].y);break;
    }
   }
  else
  if(cur_enemy<max_enemy)
   {k=random(ship_often)+1;
    if(k==1) init_inamic(enemy[i]);
   }
}
//procedurile legate de aasteroizi
void init_aster(ASTEROID &p)
{int i,k,uniq=1;
 k=random(405)+1;
 k=k/5*5;
 for(i=1;i<7;i++)
   if(uniq && aster[i].fel)
    if(k<aster[i].x+40 && k>aster[i].x-40)
     if(aster[i].y<0)uniq=0;
 if(uniq)
  {p.x=k;
   p.y=random(150)-200;
   p.fel=random(2)+1;
   p.vit=random(2)+6;
   p.hlt=5;
   cur_aster++;
  }
}
void muta_aster()
{int i,k;
 for(i=1;i<7;i++)
  if(aster[i].fel)
   {switch(aster[i].fel)
    {case 1:x_negru45(ast1,aster[i].x,aster[i].y,aster[i].vit);break;
     case 2:x_negru45(ast2,aster[i].x,aster[i].y,aster[i].vit);break;
    }
    aster[i].y+=aster[i].vit;
    if(aster[i].y>400){aster[i].fel=0;cur_aster--;}
    if(aster[i].y>-45 && aster[i].y<400)
    switch(aster[i].fel)
    {case 1:scotmem(ast1,aster[i].x,aster[i].y);break;
     case 2:scotmem(ast2,aster[i].x,aster[i].y);break;
    }
   }
  else
  if(cur_aster<max_aster)
   {k=random(aster_often)+1;
    if(k==1) init_aster(aster[i]);
   }
}

//procedurile legate de bonusuri
void init_bonus()
{int k;
 k=random(bonus_often)+1;
 if(k==1)
  {bonus.fel=random(4)+1;
   bonus.x=random(405)+1;
   bonus.y=random(150)-200;
   bonus.vit=random(3)+5;
  }
}
void muta_bonus()
{if(bonus.fel)
 {switch(bonus.fel)
  {case 1:give15(bns1,aux15);break;
   case 2:give15(bns2,aux15);break;
   case 3:give15(bns3,aux15);break;
   case 4:give15(bns4,aux15);break;
  }
  x_negru15(aux15,bonus.x,bonus.y,bonus.vit);
  bonus.y+=bonus.vit;
  if(bonus.y<400)
    scotmem15(aux15,bonus.x,bonus.y);
  else bonus.fel=0;
 }
 else if(!victory && !goodbye)init_bonus();
}
//procedurile legate de gloante
void init_glont_en(GLONT &p,int i)
{int k;
 k=random(fire_often)+1;
 if(k==1)
   if(enemy[i].fel && enemy[i].foc==0)
     {p.x=enemy[i].x+15;
      p.y=enemy[i].y+45;
      switch(enemy[i].fel)
       {case 1:p.fel=1;p.vit=10;break;
	case 2:p.fel=2;p.vit=14;break;
	case 3:p.fel=1;p.vit=12;break;
	case 4:p.fel=2;p.vit=14;break;
	case 5:p.fel=1;p.vit=10;break;
       }
      p.dir=1;
      enemy[i].foc=1;
     }
}
void init_gloante()
{int i;
 for(i=1;i<20;i++)
  {bullet[i].fel=0;
   bullet[i].x=0;
   bullet[i].y=0;
   bullet[i].vit=0;
   bullet[i].dir=0;
  }
}
void muta_gloante_en()
{int i,k;
 for(i=1;i<10;i++)
  if(bullet[i].fel)
   {switch(bullet[i].fel)
    {case 1:x_negru15(blt1,bullet[i].x,bullet[i].y,bullet[i].vit);break;
     case 2:x_negru15(blt2,bullet[i].x,bullet[i].y,bullet[i].vit);break;
     case 3:x_negru15(blt3,bullet[i].x,bullet[i].y,bullet[i].vit);break;
     case 4:x_negru15(blt4,bullet[i].x,bullet[i].y,bullet[i].vit);break;
     case 5:x_negru15(blt5,bullet[i].x,bullet[i].y,bullet[i].vit);break;
    }
    bullet[i].y+=bullet[i].vit*bullet[i].dir;
    if(bullet[i].y>400){bullet[i].fel=0;enemy[i].foc=0;}
    if(bullet[i].y>-15 && bullet[i].y<400)
    switch(bullet[i].fel)
    {case 1:scotmem15(blt1,bullet[i].x,bullet[i].y);break;
     case 2:scotmem15(blt2,bullet[i].x,bullet[i].y);break;
     case 3:scotmem15(blt3,bullet[i].x,bullet[i].y);break;
     case 4:scotmem15(blt4,bullet[i].x,bullet[i].y);break;
     case 5:scotmem15(blt5,bullet[i].x,bullet[i].y);break;
    }
   }
  else
   init_glont_en(bullet[i],i);
}
void init_glont_sh(int gl)
{int i,k=0;
 if(fwaiter>6)
 {for(i=10;i<20;i++)
    if(bullet[i].fel==0)k=i;
  bullet[k].x=shipx+15;
  bullet[k].y=336;
  switch(gl)
  {case 1:bullet[k].fel=1;
	  bullet[k].vit=10;
	  mun1--;break;
   case 2:bullet[k].fel=3;
	  bullet[k].vit=16;
	  mun2--;break;
  }
  bullet[k].dir=-1;
  fwaiter=0;
 }
}
void muta_gloante_sh()
{for(i=10;i<20;i++)
  if(bullet[i].fel)
   {switch(bullet[i].fel)
    {case 1:negru15(blt1,bullet[i].x,bullet[i].y);break;
     case 2:negru15(blt2,bullet[i].x,bullet[i].y);break;
     case 3:negru15(blt3,bullet[i].x,bullet[i].y);break;
    }
    bullet[i].y+=bullet[i].vit*bullet[i].dir;
    if(bullet[i].y<-200){bullet[i].fel=0;}
    if(bullet[i].y>-15 && bullet[i].y<400)
    switch(bullet[i].fel)
    {case 1:scotmem15(blt1,bullet[i].x,bullet[i].y);break;
     case 2:scotmem15(blt2,bullet[i].x,bullet[i].y);break;
     case 3:scotmem15(blt3,bullet[i].x,bullet[i].y);break;
    }
   }
}
void init_explozie(int xx,int yy)
{int i,k;
 for(i=0;i<10;i++)
   if(explo[i].faz==0){k=i;break;}
 explo[k].faz=1;
 explo[k].x=xx;
 explo[k].y=yy;
}
void anim_explozii()
{int i;
 for(i=0;i<10;i++)
  if(explo[i].faz)
   {switch(explo[i].faz)
    {case 4:negru(bm1,explo[i].x,explo[i].y);break;
     case 7:negru(bm2,explo[i].x,explo[i].y);break;
     case 10:negru(bm3,explo[i].x,explo[i].y);break;
     case 13:negru(bm4,explo[i].x,explo[i].y);break;
     case 16:negru(bm5,explo[i].x,explo[i].y);break;
     case 19:negru(bm6,explo[i].x,explo[i].y);break;
    }
    explo[i].faz++;
    if(explo[i].faz>20)explo[i].faz=0;
    switch(explo[i].faz)
    {case 2:scotmem(bm1,explo[i].x,explo[i].y);break;
     case 5:scotmem(bm2,explo[i].x,explo[i].y);break;
     case 8:scotmem(bm3,explo[i].x,explo[i].y);break;
     case 11:scotmem(bm4,explo[i].x,explo[i].y);break;
     case 14:scotmem(bm5,explo[i].x,explo[i].y);break;
     case 17:scotmem(bm6,explo[i].x,explo[i].y);break;
    }
   }
}
void show_initial_ship_life()
{int i;
 setcolor(100);
 for(i=1;i<shiplife;i++)
   line(504+i,203,504+i,225);
}
void regl_ship_life()//va regla pct. de viata
{if(shiplife<0)shiplife=0;
 if(shiplife>115)shiplife=115;
}
void show_ship_life()
{int i;
 if(shiplife<exshiplife)
  {setcolor(255);
   for(i=exshiplife;i>shiplife;i--)
     line(504+i,203,504+i,225);
  }
 else
  {setcolor(100);
   for(i=exshiplife;i<shiplife;i++)
     line(504+i,203,504+i,225);
  }
}
void show_cash()
{char p[10]="\0";
 sprintf(p,"%d",excash);
 setcolor(255);
 outtextxy(510,11,p);
 sprintf(p,"%d",cash);
 setcolor(34);
 outtextxy(510,11,p);
}
void show_mun1()
{char p[10]="\0";
 sprintf(p,"%d",exmun1);
 setcolor(255);
 outtextxy(520,110,p);
 sprintf(p,"%d",mun1);
 setcolor(34);
 outtextxy(520,110,p);
}
void show_mun2()
{char p[10]="\0";
 sprintf(p,"%d",exmun2);
 setcolor(255);
 outtextxy(520,150,p);
 sprintf(p,"%d",mun2);
 setcolor(34);
 outtextxy(520,150,p);
}
void show_ship()
{scotmem(shp0,shipx,350);
}
int impact45(unsigned char matr1[45][45],unsigned char matr2[45][45],int difx,int dify)
{int vl_ret=0,i,j;
 if(difx>=0 && dify>=0)
  for(j=0;j<45;j++)for(i=0;i<45;i++)
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 if(difx>=0 && dify<0)
  for(j=0;j<45;j++)for(i=0;i<45;i++)
   if(j>=abs(dify))
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 if(difx<0 && dify>=0)
  for(j=0;j<45;j++)for(i=0;i<45;i++)
   if(i>=abs(difx))
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 if(difx<0 && dify<0)
  for(j=0;j<45;j++)for(i=0;i<45;i++)
   if(j>=abs(dify) && i>=abs(difx))
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 return vl_ret;
}
int impact15(unsigned char matr1[45][45],unsigned char matr2[15][15],int difx,int dify)
{int vl_ret=0,i,j;
 if(difx>=0 && dify>=0)
  for(j=0;j<15;j++)for(i=0;i<15;i++)
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 if(difx>=0 && dify<0)
  for(j=0;j<15;j++)for(i=0;i<15;i++)
   if(j>=abs(dify))
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 if(difx<0 && dify>=0)
  for(j=0;j<15;j++)for(i=0;i<15;i++)
   if(i>=abs(difx))
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 if(difx<0 && dify<0)
  for(j=0;j<15;j++)for(i=0;i<15;i++)
   if(j>=abs(dify) && i>=abs(difx))
    if(matr2[i][j] && matr1[i+difx][j+dify])
      {vl_ret=1;break;}
 return vl_ret;
}
void impact_nava()
{int difx=0,dify=0;
 for(i=0;i<10;i++)
  if(enemy[i].fel)
  {difx=shipx-enemy[i].x;
   dify=350-enemy[i].y;
   if(abs(difx)<45 && abs(dify)<45)
    {switch(enemy[i].fel)
     {case 1:give(enm1,aux);break;
      case 2:give(enm2,aux);break;
      case 3:give(enm3,aux);break;
      case 4:give(enm4,aux);break;
      case 5:give(enm5,aux);break;
     }
     if(impact45(aux,shp0,difx,dify))
     {exshiplife=shiplife;
      shiplife-=4*enemy[i].hlt;
      regl_ship_life();
      enemy[i].hlt=0;
      show_ship_life();
     }
    }
  }
}
void impact_bullets_nava()
{int i,difx=0,dify=0;
 for(i=0;i<10;i++)
   if(bullet[i].fel)
    {difx=bullet[i].x-shipx;
     dify=bullet[i].y-350;
     if(difx>-15 && difx<45)
      if(dify>-15 && dify<45)
       {switch(bullet[i].fel)
	{case 1:give15(blt1,aux15);break;
	 case 2:give15(blt2,aux15);break;
	 case 3:give15(blt3,aux15);break;
	 case 4:give15(blt4,aux15);break;
	 case 5:give15(blt5,aux15);break;
	}
	if(impact15(shp0,aux15,difx,dify))
	 {negru15(aux15,bullet[i].x,bullet[i].y);
	  exshiplife=shiplife;
	  if(bullet[i].fel==1)shiplife-=2;
	  else shiplife-=3;
	  regl_ship_life();
	  show_ship_life();
	  bullet[i].fel=0;
	  enemy[i].foc=0;
	 }
       }
  }
}
void impact_bullets()
{int i,j,difx=0,dify=0;
 for(j=10;j<20;j++)
  for(i=0;i<10;i++)
   if(enemy[i].fel && bullet[j].fel)
    {difx=bullet[j].x-enemy[i].x;
     dify=bullet[j].y-enemy[i].y;
     if(difx>-15 && difx<45)
      if(dify>-15 && dify<45)
       {if(bullet[j].fel==1)enemy[i].hlt--;
	else enemy[i].hlt-=2;
	switch(bullet[j].fel)
	{case 1:negru15(blt1,bullet[j].x,bullet[j].y);break;
	 case 2:negru15(blt2,bullet[j].x,bullet[j].y);break;
	 case 3:negru15(blt3,bullet[j].x,bullet[j].y);break;
	 case 4:negru15(blt4,bullet[j].x,bullet[j].y);break;
	 case 5:negru15(blt5,bullet[j].x,bullet[j].y);break;
	}
	bullet[j].fel=0;
       }
  }
}
void impact_bullets_aster()
{int i,j,difx=0,dify=0;
 for(j=10;j<20;j++)
  for(i=0;i<7;i++)
   if(aster[i].fel && bullet[j].fel)
    {difx=bullet[j].x-aster[i].x;
     dify=bullet[j].y-aster[i].y;
     if(difx>-15 && difx<45)
      if(dify>-15 && dify<45)
       {if(bullet[j].fel==1)aster[i].hlt--;
	else aster[i].hlt-=2;
	switch(bullet[j].fel)
	{case 1:negru15(blt1,bullet[j].x,bullet[j].y);break;
	 case 2:negru15(blt2,bullet[j].x,bullet[j].y);break;
	 case 3:negru15(blt3,bullet[j].x,bullet[j].y);break;
	 case 4:negru15(blt4,bullet[j].x,bullet[j].y);break;
	 case 5:negru15(blt5,bullet[j].x,bullet[j].y);break;
	}
	bullet[j].fel=0;
       }
  }
}
void impact_aster()
{int difx=0,dify=0;
 for(i=0;i<7;i++)
  if(aster[i].fel)
  {difx=shipx-aster[i].x;
   dify=350-aster[i].y;
   if(abs(difx)<45 && abs(dify)<45)
    {switch(aster[i].fel)
     {case 1:give(ast1,aux);break;
      case 2:give(ast2,aux);break;
     }
     if(impact45(aux,shp0,difx,dify))
     {exshiplife=shiplife;
      shiplife-=4*aster[i].hlt;
      regl_ship_life();
      aster[i].hlt=0;
      show_ship_life();
     }
    }
  }
}
void impact_boom()//impact intre nava si explozii
{int difx=0,dify=0;
 if(shiplife>0)
 for(i=0;i<10;i++)
  if(explo[i].faz==20)
  {difx=shipx-explo[i].x;
   dify=350-explo[i].y;
   if(abs(difx)<45 && abs(dify)<45)
     show_ship();
  }
}
void impact_bonus()
{int i,difx=0,dify=0;
 for(i=0;i<10;i++)
   if(bullet[i].fel)
    {difx=bonus.x-shipx;
     dify=bonus.y-350;
     if(difx>-15 && difx<45)
      if(dify>-15 && dify<45)
       {switch(bonus.fel)
	{case 1:give15(bns1,aux15);break;
	 case 2:give15(bns2,aux15);break;
	 case 3:give15(bns3,aux15);break;
	 case 4:give15(bns4,aux15);break;
	}
	if(impact15(shp0,aux15,difx,dify))
	 {negru15(aux15,bonus.x,bonus.y);
	  show_ship();
	  switch(bonus.fel)
	  {case 1:exshiplife=shiplife;shiplife+=30;regl_ship_life();show_ship_life();break;
	   case 2:exshiplife=shiplife;shiplife+=15;regl_ship_life();show_ship_life();break;
	   case 3:if(mun1<32000)exmun1=mun1;mun1+=70;show_mun1();break;
	   case 4:if(mun2<32000)exmun2=mun2;mun2+=35;show_mun2();break;
	  }
	  bonus.fel=0;
	 }
       }
  }
}
