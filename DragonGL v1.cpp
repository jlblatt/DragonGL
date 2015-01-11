//DragonGL v1.cpp
//		Draws a VERY crude dragon curve fractal which can be moved and rotated.
//
//This code was written with help from several online OpenGL resources and is
//		not for commercial use.  I wrote this while learning to code OpenGL, which
//		is why the code probably looks like crap, not to mention to the way I draw
//		the fractal is horribly wrong and loses accuracy after six or seven
//		iterations.  If you really want to distribute this software, please include
//		the source and these comments, or a link to my site.
//
//		This code is based off DragonGL, but with some added bells and whistles.
//
//		-Jason Blatt
//
//Press F1 to iterate the fractal further, F2 to deiterate. ESC quits.
//Up and down arrow keys increase the swirling speed.

#include <gl\glut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>

int depth = 0, zoom = 2;
double dx = 0, dy = 0, dz = 0;

double loop = 0, swirl = 0;

float angleX = 0, angleY = 0, angleZ = 0;

const int WINX = 1280, WINY = 1024, numStars = 1000;

float Starfield[3][numStars];


////
//Draw Fractal Routine
////

void drawFractal(double x1, double y1, double x2, double y2, int start, int id) {
	int newstart = start+1;

	double nextVertexX;
	double nextVertexY;

	if(id == 1) {  //First segment
		if(y1 == y2) {  //Line is horizontal
			if(x1<x2) {  //Line goes from left to right
				nextVertexX = ((x2-x1)/2.0F)+x1;
				nextVertexY = y1+abs(x2-x1)/2.0F;
			}
			else {  //Line goes from right ro left
				nextVertexX = ((x2-x1)/2.0F)+x1;
				nextVertexY = y1-abs(x2-x1)/2.0F;
			}
		}
		else if(x1 == x2) {  //Line is vertical
			if(y1<y2) {  //Line goes from bottom to top
				nextVertexX = x1-abs(y2-y1)/2.0F;
				nextVertexY = ((y2-y1)/2.0F)+y1;
			}
			else {  //Line goes from top to bottom
				nextVertexX = x1+abs(y2-y1)/2.0F;
				nextVertexY = ((y2-y1)/2.0F)+y1;
			}
		}
		else if(x2>x1) {
			if(y2>y1) {  //Line goes bottom left to top right
				nextVertexX = x1;
				nextVertexY = y2;
			}
			else {  //Line goes top left to bottom right
				nextVertexX = x2;
				nextVertexY = y1;
			}
		}
		else if(x2<=x1) {
			if(y2>y1) {  //Line goes bottom right to top left
				nextVertexX = x2;
				nextVertexY = y1;
			}
			else {  //Line goes top right to bottom left
				nextVertexX = x1;
				nextVertexY = y2;
			}
		}


	}

	else if(id == 2) {  //Second segment
		if(y1 == y2) {  //Line is horizontal
			if(x1>x2) {  //Line goes from right to left
				nextVertexX = ((x2-x1)/2.0F)+x1;
				nextVertexY = y1+abs(x2-x1)/2.0F;
			}
			else {  //Line goes from left ro right
				nextVertexX = ((x2-x1)/2.0F)+x1;
				nextVertexY = y1-abs(x2-x1)/2.0F;
			}
		}
		else if(x1 == x2) {  //Line is vertical
			if(y1>y2) {  //Line goes from top to bottom
				nextVertexX = x1-abs(y2-y1)/2.0F;
				nextVertexY = ((y2-y1)/2.0F)+y1;
			}
			else {  //Line goes from bottom to top
				nextVertexX = x1+abs(y2-y1)/2.0F;
				nextVertexY = ((y2-y1)/2.0F)+y1;
			}
		}
		else if(x2>x1) {
			if(y2<y1) {  //Line goes top left to bottom right
				nextVertexX = x1;
				nextVertexY = y2;
			}
			else {  //Line goes bottom left to top right
				nextVertexX = x2;
				nextVertexY = y1;
			}
		}
		else if(x2<=x1) {
			if(y2<y1) {  //Line goes top right to bottom left
				nextVertexX = x2;
				nextVertexY = y1;
			}
			else {  //Line goes bottom right to top left
				nextVertexX = x1;
				nextVertexY = y2;
			}
		}
	}


	if(start >= depth) {  //Draw line and return
		glBegin(GL_LINES);
			glColor3f(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX );
			glVertex3f(x1,y1,-5);
			glVertex3f(x2,y2,5);
		glEnd();

		return;
	}
	else {  //Recurse
		drawFractal(x1,y1,nextVertexX,nextVertexY,newstart,1);
		drawFractal(nextVertexX,nextVertexY,x2,y2,newstart,2);
	}

}  //drawFractal






////
//Rountine to draw starfield
//

void drawStars(void) {

	for(int i = 0 ; i < numStars ; i ++) {
		glTranslatef(Starfield[0][i] , Starfield[1][i] , Starfield[2][i]);
		glBegin(GL_LINE_STRIP);
			glColor3f(1,1,1);

			glVertex3f(0,0,0);
			glVertex3f(-1,0,0);
			glVertex3f(1,0,0);
			glVertex3f(0,0,0);
			glVertex3f(0,-1,0);
			glVertex3f(0,1,0);
			glVertex3f(0,0,0);
			glVertex3f(0,0,-1);
			glVertex3f(0,0,1);

		glEnd();
		glTranslatef(-Starfield[0][i] , -Starfield[1][i] , -Starfield[2][i]);
	}

}
		





////
//Main drawing routine
////

void draw(void)  
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(0+dx,-0+dy,-550+dz);
	glRotatef(angleX, 0,1,0);
	glRotatef(angleY, 1,0,0);
	glRotatef(angleZ, 0,0,1);
	drawStars();
	drawFractal(-150,0,150,0,0,1);

	angleY++;
	angleZ++;
	
	dz+=zoom;
	if(dz > 550) zoom = -3;
	if(dz < -200) zoom = 2;

	dx = cos(loop) * 75;
	dy = sin(loop) * 75;

	loop+=swirl;

	

	glPopMatrix();
	glutSwapBuffers();

}





////
//Resize window function
////

void changeSize(int w, int h)  
{
	if(h == 0) h = 1;

	double ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, w, h);

	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);
}







////
//Process keys and mouse motion
////

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
}

void processSpecialKeys(int key, int x, int y)
{
	if(key == GLUT_KEY_F1) {
		depth++;
	}

	if(key == GLUT_KEY_F2) {
		if(depth != 0) depth--;
			
	}

	if(key == GLUT_KEY_UP) {
		swirl+=.01;
	}

	if(key == GLUT_KEY_DOWN) {
		swirl-=.01;
	}

	

}





 

////
//Init
////

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitWindowPosition(0,0);
    glutInitWindowSize(WINX,WINY);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    int mainwin = glutCreateWindow("DragonGL v1");

    glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_DEPTH_TEST);

	glutFullScreen();

	srand((unsigned int)time((time_t *)NULL));
	for(int i = 0 ; i < 3 ; i++) {
		for(int j = 0 ; j < numStars ; j++) {
			Starfield[i][j] = 1000 - (rand() % 2001);
		}
	}

    glutMainLoop();
}



 
