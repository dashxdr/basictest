#include <SDL.h>
#include <math.h>

#include "misc.h"
#define M_PI2 (M_PI*2.0)

Uint32 maprgb(bc *bc, int r,int g,int b)
{
	return SDL_MapRGB(bc->thescreen->format,r,g,b);
}

static inline void
colordot_32(SDL_Surface *surf, unsigned int x, unsigned int y, Uint32 c, int f2)
{
Uint32 a;
	a=((f2+1) * (c>>24))>>8;
	if(a==0xff)
		*((Uint32 *)(surf->pixels)+y * surf->pitch/4+x)=c;
	else
	{
		Uint32 *p, t;
		Uint32 ai;
		
		p=(Uint32 *)(surf->pixels)+y * (surf->pitch>>2)+x;

		ai=a^255;
		t=*p;

		*p = ((a*(c&0xff) + ai*(t&0xff))>>8) |
			(((a*(c&0xff00) + ai*(t&0xff00))&0xff0000)>>8) |
			(((a*(c&0xff0000) + ai*(t&0xff0000))&0xff000000)>>8);
	}
}

void fillscreen(bc *bc, int r, int g, int b, int a)
{
int i;
Uint32 color;
int w;
SDL_Surface *scr = bc->thescreen;

	taint(bc);
	color = maprgb(bc, r, g, b);
	if(a==255)
	{
		color |= 0xff000000;
		for(i=0;i<bc->xsize;++i)
			colordot_32(scr, i, 0, color, 255);
		w=bc->xsize * 4;
		for(i=1;i<bc->ysize;++i)
		{
			memcpy(scr->pixels + i*scr->pitch,
					scr->pixels, w);
		}
	} else
	{
		int x,y;
		for(y=0;y<bc->ysize;++y)
			for(x=0;x<bc->xsize;++x)
				colordot_32(scr, x, y, color, a);
	}
}

void drawchar(bc *bc, int x, int y, unsigned char *p, Uint32 fg, Uint32 bg)
{
Uint32 *p2 = (void *)( bc->thescreen->pixels + y*bc->thescreen->pitch + x*4);
int v;
unsigned char c;
int step = bc->thescreen->pitch >> 2;

	for(v=0;v<13;++v)
	{
		c=*p++;
		p2[0]= (c&0x01) ? fg : bg;
		p2[1]= (c&0x02) ? fg : bg;
		p2[2]= (c&0x04) ? fg : bg;
		p2[3]= (c&0x08) ? fg : bg;
		p2[4]= (c&0x10) ? fg : bg;
		p2[5]= (c&0x20) ? fg : bg;
		p2 += step;
	}
}

void taint(bc *bc)
{
	bc->tainted = 1;
}

void update(bc *bc)
{
int now;
int diff;

	if(!bc->tainted) return;
	now=SDL_GetTicks();
	diff = bc->nextupdate - now;
	if(diff>0 && diff<250) return;
	bc->nextupdate=now+20;
#warning must lock
	SDL_UpdateRect(bc->thescreen, 0, 0, 0, 0);
	bc->tainted=0;
}

void lock(bc *bc)
{
	if(SDL_MUSTLOCK(bc->thescreen))
	{
		if ( SDL_LockSurface(bc->thescreen) < 0 )
		{
			fprintf(stderr, "Couldn't lock display surface: %s\n",
								SDL_GetError());
		}
	}
}
void unlock(bc *bc)
{
	if(SDL_MUSTLOCK(bc->thescreen))
		SDL_UnlockSurface(bc->thescreen);
}


void arc_piece(bc *bc, double xc, double yc, double r, double a, double da)
{
double x1,y1,x2,y2,x3,y3,x4,y4;
double q1,q2,k2,ax,ay,bx,by;
#define AP_STEPS 3
int i;

	a*=M_PI/180.0;
	da*=M_PI/180.0;

	if(da>M_PI2) da=M_PI2;
	if(da<-M_PI2) da=-M_PI2;
	da/=AP_STEPS;
	for(i=0;i<AP_STEPS;++i)
	{

		ax=r*cos(a);
		ay=-r*sin(a);
		a+=da;
		bx=r*cos(a);
		by=-r*sin(a);

		x1=xc+ax;
		y1=yc+ay;
		x4=xc+bx;
		y4=yc+by;

		q1=ax*ax + ay*ay;
		q2=q1 + ax*bx + ay*by;
		k2 = 4.0/3.0*((sqrt(2.0*q1*q2)-q2)/(ax*by-ay*bx));
		x2=x1 - k2*ay;
		y2=y1 + k2*ax;
		x3=x4 + k2*by;
		y3=y4 - k2*bx;

		shape_add(bc, x1, y1, TAG_ONPATH);
		shape_add(bc, x2, y2, TAG_CONTROL3);
		shape_add(bc, x3, y3, TAG_CONTROL3);
		if(i+1==AP_STEPS)
			shape_add(bc, x4, y4, TAG_ONPATH);
	}
}

void spot(bc *bc)
{
	taint(bc);
	disc(bc, bc->gx, bc->gy, bc->pen/2.0);	
}


void stroke(bc *bc, double x, double y)
{
//Uint32 color;
double dx,dy, r, pen2;
double a;

	taint(bc);

	dx=bc->gx - x;
	dy=bc->gy - y;
	if(dx || dy)
	{
		a=atan2(-dy, dx)*180.0/M_PI;
		pen2=bc->pen/2.0;
		r=sqrt(dx*dx+dy*dy);
		dx=dx*pen2/r;
		dy=dy*pen2/r;
		shape_init(bc);
		shape_add(bc, bc->gx-dy, bc->gy+dx, TAG_ONPATH);
		shape_add(bc, x-dy, y+dx, TAG_ONPATH);
		arc_piece(bc, x, y, pen2, a-90, -180);
		shape_add(bc, x+dy, y-dx, TAG_ONPATH);
		shape_add(bc, bc->gx+dy, bc->gy-dx, TAG_ONPATH);
		arc_piece(bc, bc->gx, bc->gy, pen2, a+90, -180);
		shape_done(bc);
	}


	bc->gx = x;
	bc->gy = y;
}

void circle(bc *bc, double cx, double cy, double radius)
{
	taint(bc);
	shape_init(bc);
#define T1 0
#define T2 360
	arc_piece(bc, cx, cy, radius+bc->pen/2.0, T1,T2);
	arc_piece(bc, cx, cy, radius-bc->pen/2.0, T1+T2, -T2);
	shape_done(bc);
}

void disc(bc *bc, double cx, double cy, double radius)
{
	taint(bc);
	shape_init(bc);
	arc_piece(bc, cx, cy, radius, 0, 360);
	shape_done(bc);
}

void myspanner(int y, int count, FT_Span *spans, void *user)
{
bc *bc=user;
int x, w, coverage;
Uint32 color;
SDL_Surface *surf = bc->thescreen;

// renders spans in reverse order, but why not? They don't overlap
	spans += count;
	color = bc->temp;
	while(count--)
	{
		--spans;
		x=spans->x;
		w=spans->len;
		coverage=spans->coverage;
		while(w--)
			colordot_32(surf, x++, y, color, coverage);
	}
}


#define IFACTOR 64  // used to fix coords to the grays rendering engine 

void rendertest(bc *bc)
{
	printf("render test!\n");
FT_Vector points[1024];
char tags[1024];
short pathstops[100];
int res;
FT_Raster myraster;
FT_Raster_Params myparams;
FT_Outline myoutline;
int i;
float a,r;

	for(i=0;i<100;++i)
	{
		a=i*3.1415928*2/100;
		r=220.0*(1.0+sin(a*6));
		points[i].x = IFACTOR*(512+r*cos(a));
		points[i].y = IFACTOR*(384-r*sin(a));
		tags[i] = TAG_CONTROL2; // (i&1) ? TAG_ONPATH: TAG_CONTROL2;
	}
	pathstops[0] = i-1;

	myoutline.n_contours = 1;
	myoutline.n_points = 100;
	myoutline.points = points;
	myoutline.tags = tags;
	myoutline.contours = pathstops;
	myoutline.flags = FT_OUTLINE_IGNORE_DROPOUTS |
			FT_OUTLINE_EVEN_ODD_FILL;

	myparams.target = 0;
	myparams.source = &myoutline;
	myparams.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA |
		FT_RASTER_FLAG_CLIP;
	myparams.gray_spans = myspanner;
	myparams.user = bc;
	myparams.clip_box.xMin = 0;
	myparams.clip_box.xMax = bc->xsize;
	myparams.clip_box.yMin = 0;
	myparams.clip_box.yMax = bc->ysize;
	res=SDL_basic_ft_grays_raster.raster_new(0, &myraster);

	bc->temp = maprgb(bc, bc->gred, bc->ggreen, bc->gblue) |
		(bc->galpha<<24);

	SDL_basic_ft_grays_raster.raster_reset(myraster, bc->pool, sizeof(bc->pool));

	res=SDL_basic_ft_grays_raster.raster_render(myraster, &myparams);

	SDL_basic_ft_grays_raster.raster_done(myraster);

	taint(bc);
}

void shape_init(bc *bc)
{
	bc->shape_numpoints = 0;
	bc->shape_numcontours = 0;
}

void shape_add(bc *bc, double x, double y, int tag)
{
	if(bc->shape_numpoints < MAX_SHAPE_POINTS)
	{
		bc->shape_points[bc->shape_numpoints].x = IFACTOR *x;
		bc->shape_points[bc->shape_numpoints].y = IFACTOR *y;
		bc->shape_tags[bc->shape_numpoints] = tag;
		++bc->shape_numpoints;
	}
}
void shape_end(bc *bc)
{
	if(bc->shape_numcontours < MAX_SHAPE_CONTOURS &&
		bc->shape_numpoints &&
		(!bc->shape_numcontours || 
		bc->shape_numpoints > bc->shape_pathstops[bc->shape_numcontours-1]+1))
	{
		bc->shape_pathstops[bc->shape_numcontours++] = bc->shape_numpoints-1;
	}
}
void shape_done(bc *bc)
{
int res;
FT_Raster myraster;
FT_Raster_Params myparams;
FT_Outline myoutline;

	shape_end(bc);

	myoutline.n_contours = bc->shape_numcontours;;
	myoutline.n_points = bc->shape_numpoints;
	myoutline.points = bc->shape_points;
	myoutline.tags = bc->shape_tags;;
	myoutline.contours = bc->shape_pathstops;
	myoutline.flags = FT_OUTLINE_IGNORE_DROPOUTS |
			FT_OUTLINE_EVEN_ODD_FILL;

	myparams.target = 0;
	myparams.source = &myoutline;
	myparams.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA |
		FT_RASTER_FLAG_CLIP;
	myparams.gray_spans = myspanner;
	myparams.user = bc;
	myparams.clip_box.xMin = 0;
	myparams.clip_box.xMax = bc->xsize;
	myparams.clip_box.yMin = 0;
	myparams.clip_box.yMax = bc->ysize;
	res=SDL_basic_ft_grays_raster.raster_new(0, &myraster);

	bc->temp = maprgb(bc, bc->gred, bc->ggreen, bc->gblue) |
		(bc->galpha<<24);

	SDL_basic_ft_grays_raster.raster_reset(myraster, bc->pool, sizeof(bc->pool));

	res=SDL_basic_ft_grays_raster.raster_render(myraster, &myparams);

	SDL_basic_ft_grays_raster.raster_done(myraster);


	taint(bc);
}


