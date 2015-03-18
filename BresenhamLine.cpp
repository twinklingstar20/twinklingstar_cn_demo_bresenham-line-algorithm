#include <stdlib.h>
#include <GL/glut.h> 
#include <stdio.h>
#include <algorithm>
#include <math.h>


typedef unsigned long	DWORD;
typedef unsigned short	WORD;
typedef unsigned char	BYTE;

#define  RGB(r,g,b) ((DWORD)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
int g_inHeight;
int g_inWidth;
int g_inPixelArray[] = {1,2,4,8,16,32};
int g_inPixelIndx;
unsigned char	g_ptrScreen[1080*1920*3];
int g_inX1,g_inY1,g_inX2,g_inY2;


void setPixel(int x,int y,DWORD color)
{
	int iStart = y*g_inPixelArray[g_inPixelIndx];
	int jStart = x*g_inPixelArray[g_inPixelIndx];
	int iEnd = iStart + g_inPixelArray[g_inPixelIndx];
	int jEnd = jStart + g_inPixelArray[g_inPixelIndx];
	if( iEnd>g_inHeight || jEnd>g_inWidth )
		return ;
	int i , j , lines;
	int w = (g_inWidth / g_inPixelArray[g_inPixelIndx]) * g_inPixelArray[g_inPixelIndx];
	for(i=iStart ; i<iEnd ; i++ )
	{
		lines = i*w*3;
		for( j=jStart; j<jEnd ; j++ )
		{
			*(g_ptrScreen + lines + j*3)	 = (color>>0)&0xff;
			*(g_ptrScreen + lines + j*3 + 1) = (color>>8)&0xff;
			*(g_ptrScreen + lines + j*3 + 2) = (color>>16)&0xff;
		}
	}

}


void bresenhamLine(int x1,int y1,int x2,int y2,DWORD color)
{
	int w = x2 - x1;
	int h = y2 - y1;
	int dx = ((w>0)<<1) - 1;
	int dy = ((h>0)<<1) - 1;
	w = abs(w);
	h = abs(h);
	int f , y , x, delta1,delta2;
	if( w > h )
	{
		f = 2*h - w;
		delta1 = 2*h;
		delta2 = (h-w)*2;
		for( x = x1 , y = y1 ; x!=x2 ; x += dx )
		{
			setPixel(x,y,color);
			if( f < 0 )
			{
				f += delta1;
			}
			else
			{
				y += dy;
				f += delta2;
			}
		}
		setPixel(x, y,color);
	}
	else
	{
		f = 2*w - h;
		delta1 = w*2;
		delta2 = (w-h)*2;
		for( x = x1 , y = y1 ; y!=y2 ; y += dy )
		{
			setPixel(x,y,color);
			if( f < 0 )
			{
				f += delta1;
			}
			else
			{
				x += dx;
				f += delta2;
			}
		}
		setPixel(x, y,color);
	}
}



void setBackground( int inW, int inH)
{
	int i , j ,lines;

	for( i=0 ; i<inH ; i ++ )
	{
		lines = i*inW*3;
		for( j=0 ; j<inW ; j ++ )
		{
			if( (j/g_inPixelArray[g_inPixelIndx] + i/g_inPixelArray[g_inPixelIndx])%2==0 )
			{
				*(g_ptrScreen + lines + j*3) = 0;
				*(g_ptrScreen + lines + j*3 + 1) = 0;
				*(g_ptrScreen + lines + j*3 + 2) = 0;
			}
			else
			{
				*(g_ptrScreen + lines + j*3) = 128;
				*(g_ptrScreen + lines + j*3 + 1) = 128;
				*(g_ptrScreen + lines + j*3 + 2) = 128;

			}
		}
	}
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	int gridH = (g_inHeight / g_inPixelArray[g_inPixelIndx]) * g_inPixelArray[g_inPixelIndx];
	int gridW = (g_inWidth / g_inPixelArray[g_inPixelIndx]) * g_inPixelArray[g_inPixelIndx];

	setBackground(gridW,gridH);

	bresenhamLine(g_inX1,g_inY1,g_inX2,g_inY2,RGB(255,0,0));

	glDrawPixels(gridW,gridH,GL_RGB,GL_UNSIGNED_BYTE,g_ptrScreen);

	glutSwapBuffers();
	glFlush();
}

void myMouseButton(int button, int state, int x, int y)
{
	if( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN )
	{
		g_inX1 = x / g_inPixelArray[g_inPixelIndx];
		g_inY1 = (g_inHeight-y) / g_inPixelArray[g_inPixelIndx];
	}
	else if( button==GLUT_LEFT_BUTTON && state==GLUT_UP )
	{
		g_inX2 = x / g_inPixelArray[g_inPixelIndx];
		g_inY2 = (g_inHeight-y) / g_inPixelArray[g_inPixelIndx];
		myDisplay();
	}

}

void init()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f); 
}

void myKeys(unsigned char key, int x, int y) 
{
	switch(key) 
	{
	case 'u':
	case 'U':
		if( g_inPixelIndx<5 )
			g_inPixelIndx += 1;
		break;
	case 'l':
	case 'L':
		if( g_inPixelIndx>0 )
			g_inPixelIndx -= 1;
		break;
	}
	myDisplay();
}

void myReshape(GLsizei w,GLsizei h)
{
	g_inHeight = h;
	g_inWidth = w;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc,char ** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500,600);

	glutCreateWindow("Brensenham");

	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeys);
	glutMouseFunc(myMouseButton);
	glutMainLoop();


	return(0);
}