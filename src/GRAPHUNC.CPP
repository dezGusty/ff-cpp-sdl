int mxa(int i)
{int aux;
 if(i>63) aux=63;
 else aux=i;
 return aux;
}
void load_palette(char nume[30],float intens)
{FILE *f;
 int i,a,b,c;
 char d;
 f=fopen(nume,"r");
 for(i=0;i<18;i++)fscanf(f,"%c",&d);
 for(i=0;i<256;i++)
  {fscanf(f,"%d%d%d",&a,&b,&c);
   setrgbpalette(i,mxa(a/4*intens),mxa(b/4*intens),mxa(c/4*intens));
  }
 fclose(f);
}

void printfile(int x,int y,char untext[30],int dm1,int dm2)
{ FILE *f=NULL;
  unsigned char c;
  int i,j;
  for(i=0;j<640;j++) a[i]=0;
  f=fopen(untext,"r");
  for(i=0;i<dm2;i++)
   {for(j=0;j<dm1;j++)
     {fscanf(f,"%c",&c);
      if(c>=33)a[j]=c;
     }
    for(j=0;j<dm1;j++)
      if(c>=33)putpixel(x+j,y+i,a[j]);
    fscanf(f,"%c",&c);
   }
  fclose(f);
}

void putmem(char* untext,long lin,unsigned char a[45][45])
{ FILE *f=NULL;
  unsigned char c;
  int i,j;
  f=fopen(untext,"r");
//  for(j=0;j<lin;j++)fscanf(f,"%c",&c);
  fseek(f,lin,0);
  for(i=0;i<45;i++)
   {for(j=0;j<45;j++)
     {fscanf(f,"%c",&c);
      if(c>33)a[i][j]=c;
      else a[i][j]=0;
     }
    fscanf(f,"%c",&c);
   }
  fclose(f);
}
void scotmem(unsigned char a[45][45],int x,int y)
{ int i,j;
  for(i=0;i<45;i++)
    for(j=0;j<45;j++)
      if(a[i][j]>33) putpixel(x+j,y+i,a[i][j]);
}
void negru(unsigned char a[45][45],int x,int y)
{int i,j;
 for(i=0;i<45;i++)
  for(j=0;j<45;j++)
   if(a[i][j]>33)putpixel(x+j,y+i,0);
}
void x_negru45(unsigned char a[45][45],int x,int y,int dist)
{int i,j;
 for(i=0;i<dist;i++)
  for(j=0;j<45;j++)
   if(a[i][j])putpixel(x+j,y+i,0);
 for(i=dist;i<45;i++)
  for(j=0;j<45;j++)
   if(a[i][j] && !a[i-dist][j])
     putpixel(x+j,y+i,0);
}

void putmem15(char* untext,long lin,unsigned char a[15][15])
{ FILE *f=NULL;
  unsigned char c;
  int i,j;
  f=fopen(untext,"r");
//  for(j=0;j<lin;j++)fscanf(f,"%c",&c);
  fseek(f,lin,0);
  for(i=0;i<15;i++)
   {for(j=0;j<15;j++)
     {fscanf(f,"%c",&c);
      if(c>33)a[i][j]=c;
      else a[i][j]=0;
     }
    fscanf(f,"%c",&c);
   }
  fclose(f);
}
void scotmem15(unsigned char a[15][15],int x,int y)
{ int i,j;
  for(i=0;i<15;i++)
    for(j=0;j<15;j++)
      if(a[i][j]>33) putpixel(x+j,y+i,a[i][j]);
}
void negru15(unsigned char a[15][15],int x,int y)
{int i,j;
 for(i=0;i<15;i++)
  for(j=0;j<15;j++)
   if(a[i][j]>33)putpixel(x+j,y+i,0);
}
void x_negru15(unsigned char a[15][15],int x,int y,int dist)
{int i,j;
 for(i=0;i<dist && i<15;i++)
  for(j=0;j<15;j++)
   if(a[i][j])putpixel(x+j,y+i,0);
 for(i=dist;i<15;i++)
  for(j=0;j<15;j++)
   if(a[i][j] && !a[i-dist][j])
     putpixel(x+j,y+i,0);
}


void luminare(int viteza)
{float fl;
 for(fl=0.0;fl<luminozitate;fl+=0.1)
  {load_palette(paleta,fl);
   delay(10*viteza);
  }
 load_palette(paleta,luminozitate);
}
void intunecare(int viteza)
{float fl;
 for(fl=luminozitate;fl>0.0;fl-=0.1)
  {load_palette(paleta,fl);
   delay(10*viteza);
  }
}

void text_shadow(int x,int y,char mesaj[50],int culoare)
{setcolor(0);
 outtextxy(x+1,y+1,mesaj);
 setcolor(culoare);
 outtextxy(x,y,mesaj);
}
void printfile0(int x,int y,char untext[30],int dm1,int dm2)
{ FILE *f=NULL;
  unsigned char c;
  int i,j;
  for(i=0;j<640;j++) a[i]=0;
  f=fopen(untext,"r");
  for(i=0;i<dm2;i++)
   {for(j=0;j<dm1;j++)
     {fscanf(f,"%c",&c);
      if(c)a[j]=c;
     }
    for(j=0;j<dm1;j++)
      if(c>=33)putpixel(x+j,y+i,a[j]);
    fscanf(f,"%c",&c);
   }
  fclose(f);
}

void write_file(char *fisier,int x,int y,int dim1,int dim2)
{int i,j;
 FILE *f;
 char c;
 f=fopen(fisier,"w");
 for(j=y;j<y+dim2;j++)
  {for(i=x;i<x+dim1;i++)
    {c=getpixel(i,j);
     if(c==0)c=255;
     fprintf(f,"%c",c);
    }
   fprintf(f,"\n");
  }
 fclose(f);
}