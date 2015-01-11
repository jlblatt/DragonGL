//Michelle's Spiral.cpp
//		Fun spirograph type toy using a dragon curve fractal.
//
//This code was written with help from several online OpenGL resources and is
//		not for commercial use.  If you really want to distribute this software,
//		please include the source and these comments, or a link to my site.
//
//This code is based off DragonGL v1, but with some insane bells and whistles.
//
//		-Jason Blatt
//
//Press F12 for Help


#include <gl\glut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <math.h>

int depth = 0;
double dx = 0, dy = 0, dz = 0;

bool stats = true;
bool help = false;
float loop = 0, swirl = 0, spin = 0 , radius = 0, zoom = 0, thickness = 0;
float angleX = 0, angleY = 0, angleZ = 0;

bool random[3] = {true, true, true};
float red = 0 , green = 0 , blue = 0;

const int WINX = 1280, WINY = 1024;

int frame=0,currTime,timeBase=0;
float fps=0;



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

		if(random[0] == true) red = rand()/(float)RAND_MAX;
		else red = 0;
		if(random[1] == true) green = rand()/(float)RAND_MAX;
		else green = 0;
		if(random[2] == true) blue = rand()/(float)RAND_MAX;
		else blue = 0;
		glBegin(GL_LINES);
			glColor3f(red, green, blue);
			glVertex3f(x1,y1,-thickness);
			glVertex3f(x2,y2,thickness);
		glEnd();

		return;
	}
	else {  //Recurse
		drawFractal(x1,y1,nextVertexX,nextVertexY,newstart,1);
		drawFractal(nextVertexX,nextVertexY,x2,y2,newstart,2);
	}

}  //drawFractal






////
//Draw String Function
////

void drawString(float x, float y, float z, void *font, char *string) {  
  char *c;
  glRasterPos3f(x,y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}





////
//Text Drawing Utility Functions
////

void setOrthographicProjection() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WINX, 0, WINY);
	glScalef(1, -1, 1);
	glTranslatef(0, -WINY, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerpectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}








////
//Main drawing routine
////

void draw(void)  
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

	
	glTranslatef(0+dx,-0+dy,-750+dz);
	glRotatef(angleX, 0,1,0);
	glRotatef(angleY, 1,0,0);
	glRotatef(angleZ, 0,0,1);
	drawFractal(-150,0,150,0,0,1);

	angleZ+=spin;

	dx = cos(loop) * radius;
	dy = sin(loop) * radius;
	dz = zoom;

	loop+=swirl;

	glPopMatrix();

	if(stats == true || help == true) {
		setOrthographicProjection();
		glPushMatrix();
		glLoadIdentity();

		frame++;
		currTime=glutGet(GLUT_ELAPSED_TIME);
	
		if (currTime - timeBase > 1000) {
			fps = frame*1000.0/(currTime-timeBase);
	 		timeBase = currTime;		
			frame = 0;
		}


		char c[300];
		char temp[300];
		strcpy(c, "Iterations: ");
		strcat(c, itoa(depth,temp,10));
		strcat(c, "    Rotation Speed: ");
		strcat(c, gcvt(spin,3,temp));
		strcat(c, "    Zoom: ");
		strcat(c, gcvt(zoom, 3, temp));
		strcat(c, "    Swirl Speed: ");
		strcat(c, gcvt(swirl,4,temp));
		strcat(c, "    Swirl Radius: ");
		strcat(c, gcvt(radius,3,temp));
		strcat(c, "    Fractal Thickness: ");
		strcat(c, gcvt(thickness,3,temp));
		strcat(c, "    FPS: ");
		strcat(c, gcvt(fps,3,temp));

		glColor3f(1,1,1);
		if(stats == true) {
			drawString(25,30,0,GLUT_BITMAP_9_BY_15,c);
			drawString(550,50,0,GLUT_BITMAP_9_BY_15,"Press F12 for Help");
		}

		if(help == true) {
			drawString(20,820,0,GLUT_BITMAP_9_BY_15,"F1/F2 : Increase/Decrease Fractal Complexity");
			drawString(20,840,0,GLUT_BITMAP_9_BY_15,"End : Toggle Statistics");
			drawString(20,860,0,GLUT_BITMAP_9_BY_15,"Home : Reset Parameters");
			drawString(20,880,0,GLUT_BITMAP_9_BY_15,"Left/Right Arrows : Spin");
			drawString(20,900,0,GLUT_BITMAP_9_BY_15,"Up/Down Arrows : Zoom");
			drawString(20,920,0,GLUT_BITMAP_9_BY_15,"F5/F6 : Increase/Decrease Swirling Speed");
			drawString(20,940,0,GLUT_BITMAP_9_BY_15,"F7/F8 : Increase/Decrease Swirling Radius");
			drawString(20,960,0,GLUT_BITMAP_9_BY_15,"F9/F10/F11 : Toggle Red/Blue/Green Palette Randomness");
		}

		glPopMatrix();
		resetPerpectiveProjection();
	}

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
//Process keys
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
		zoom+=3;
	}
	if(key == GLUT_KEY_DOWN) {
		zoom-=3;
	}

	if(key == GLUT_KEY_F5) {
		swirl+=.01;
	}

	if(key == GLUT_KEY_F6) {
		swirl-=.01;
	}

	if(key == GLUT_KEY_LEFT) {
		spin+=.1;
	}
	if(key == GLUT_KEY_RIGHT) {
		spin-=.1;
	}

	if(key == GLUT_KEY_F7) {
		radius++;
	}
	if(key == GLUT_KEY_F8) {
		radius--;		
	}

	if(key == GLUT_KEY_F3) {
		thickness++;
	}
	if(key == GLUT_KEY_F4) {
		thickness--;		
	}

	if(key == GLUT_KEY_HOME) {
		zoom=0;
		swirl=0;
		spin=0;
		radius=0;
		thickness=0;
	}

	if(key == GLUT_KEY_F9) {
		random[0] = !random[0];
	}
	if(key == GLUT_KEY_F10) {
		random[1] = !random[1];		
	}
	if(key == GLUT_KEY_F11) {
		random[2] = !random[2];
	}

	if(key == GLUT_KEY_F12) {
		help = !help;
	}
	if(key == GLUT_KEY_END) {
		stats = !stats;
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
    int mainwin = glutCreateWindow("Michelle's Spiral");

    glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_DEPTH_TEST);

	glutFullScreen();

	currTime = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
}



 
