void MOUSE(int &a1, int &a2, int &a3, int &a4)
{struct REGPACK regs;
 regs.r_ax=a1;
 regs.r_bx=a2;
 regs.r_cx=a3;
 regs.r_dx=a4;
 intr(0x33,&regs);
 a1=regs.r_ax;
 a2=regs.r_bx;
 a3=regs.r_cx;
 a4=regs.r_dx;
}
void mouseinit(void)
{int a=1,b,c,d;
 MOUSE(a,b,c,d);
}
void mousehide(void)
{int a,b,c,d;
 a=2;
 MOUSE(a,b,c,d);
}
void mouseshow(void)
{int a=1,b,c,d;
 MOUSE(a,b,c,d);
}
void mousedata(int &but,int &x, int &y)
{int a=3,b,c,d;
 MOUSE(a,b,c,d);
 but=b;x=c;y=d;
}
void mousemove(int x,int y)
{int a=4,b,c=x,d=y;
 MOUSE(a,b,c,d);
}
