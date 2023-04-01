void logofunc()
{int waiter=0;
 paleta="Imagdata\\Logo.pal";
 printfile(0,0,"Imagdata\\Logo.dta",640,400);
 settextstyle(2,0,5);
 setcolor(252);
 outtextxy(5,5,"O productie Gusty-2001");
 luminare(2);
 do
 {waiter++;
  delay(20);
 }
 while(!kbhit()&& waiter<350);
 if(kbhit())getch();
 intunecare(2);
 cleardevice();
}