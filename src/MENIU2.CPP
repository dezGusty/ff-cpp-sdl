void optiuni()
{int i,xex,byenow=0;
 char c,*s;
 printfile(230,185,"Imagdata\\Nobuts.dta",180,215);
 putmem("Imagdata\\Sistem.dta",0,shp0);
 setcolor(0);
 for(i=35;i<90;i++)
   line(110,i,500,i);
 setcolor(255);
 rectangle(110,35,500,90);
 xex=120+(luminozitate-0.7)*10*40;
 scotmem(shp0,xex,40);
 setcolor(255);
 settextstyle(2,0,5);
 outtextxy(35,250,"Apasa <ESC> pt. �ntoarcere �n meniul principal.");
 outtextxy(25,110,"Apasa tastele 1..9 pentru a modifica luminozitatea imaginii.");
 outtextxy(50,145,"Control:");
 outtextxy(30,155,"Buton mouse stanga=> foc primar (bila energica).");
 outtextxy(30,165,"Buton mouse dreapta=> foc secundar (tun laser).");
 outtextxy(30,175,"Miscare mouse stanga-dreapta => miscare nava");
 outtextxy(450,385,"O productie Gusty-2001");
 luminare(2);
 for(i=1;i<10;i++)
  {sprintf(s,"%d",i);outtextxy(100+40*i,90,s);}
 do
 {c=getch();
  if(c==27)byenow=1;
  else
   switch(c)
   {case '1':negru(shp0,xex,40);luminozitate=0.7;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '2':negru(shp0,xex,40);luminozitate=0.8;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '3':negru(shp0,xex,40);luminozitate=0.9;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '4':negru(shp0,xex,40);luminozitate=1;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '5':negru(shp0,xex,40);luminozitate=1.1;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '6':negru(shp0,xex,40);luminozitate=1.2;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '7':negru(shp0,xex,40);luminozitate=1.3;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '8':negru(shp0,xex,40);luminozitate=1.4;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
    case '9':negru(shp0,xex,40);luminozitate=1.5;xex=120+(luminozitate-0.7)*10*40;scotmem(shp0,xex,40);load_palette(paleta,luminozitate);break;
   }
 }
 while(!byenow);
 intunecare(2);
}
void credits()
{printfile(230,185,"Imagdata\\Nobuts.dta",180,215);
 settextstyle(2,0,5);
 setcolor(252);
 outtextxy(5,5,"O productie Gusty-2001");
 outtextxy(65,80,"Programul acesta a fost realizat integral de Preda Livius Augustin.");
 outtextxy(25,90,"...cunoscut si sub numele de Gusty.");
 outtextxy(25,100,"Doresc pe aceasta cale sa �mi salut colegii din Lic. Info.");
 outtextxy(25,110,"(salut!).");
 outtextxy(30,200,"Grafica jocului a fost realizata cu ajutorul diferitor programe");
 outtextxy(30,210,"de specialitate si cu ajutorul motorului grafic propriu bazat pe");
 outtextxy(30,220,"Converter 2000-(copyright Gusty 2001).");
 outtextxy(30,230,"Multumiri speciale lui Mocanu Bogdan, fara a carui contributie la");
 outtextxy(30,240,"dezvoltarea motorului grafic, calitatea imaginii ar fi avut de suferit");
 outtextxy(40,250,"Program realizat pt. ATESTAT 2002");
 outtextxy(230,380,"...Apasati orice tasta pt. a reveni la meniul principal.");
 luminare(2);
 getch();

}

void pune_fundal_meniu()
{int i;
 cleardevice();
 paleta="Imagdata\\Menu.pal";
 intunecare(2);
 printfile(0,0,"Imagdata\\Mainmenu.dta",640,400);
 for(i=185;i<=335;i+=50)
   printfile(230,i,"Imagdata\\Buton.dta",180,40);
 settextstyle(2,0,8);
 text_shadow(250,190,"(N)ew Game",200);
 text_shadow(250,240," (O)ptions",200);
 text_shadow(250,290," (C)redits",200);
 text_shadow(250,340,"  E(x)it",200);
 luminare(2);
}

void meniu_principal()
{ char cht;
  int comanda,gameexit;
  do
   {gameexit=0;
    pune_fundal_meniu();
    do
    {comanda=0;
     cht=getch();
     if(cht=='N'||cht=='n') comanda=1;
     if(cht=='O'||cht=='o') comanda=2;
     if(cht=='C'||cht=='c') comanda=3;
     if(cht=='X'||cht=='x') comanda=4;
    }
    while(!comanda);
    intunecare(2);
    switch(comanda)
    {case 1:cleardevice();jocnou();break;
     case 2:optiuni();break;
     case 3:credits();break;
     case 4:gameexit=1;
    }
   }
  while(!gameexit);
}