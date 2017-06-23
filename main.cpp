//Windows - uncomment the includes below

#include "shared/gltools.h"				//OpenGL toolkit - in the local shared folder
#include <math.h>						//These were included in glm.cpp so I kept them here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "shared/glm.h"
#include <iostream>			
#include <string>//Needed for console output (debugging)


//
//  texture.cpp
//
//
//  Created by Philip Trwoga on 09/11/2012.
//  Copyright (c) 2012 __Philip Trwoga__. All rights reserved.
// DO NOT DISTRIBUTE WITHOUT THE AUTHOR'S PERMISSION

//Windows uncomment the includes below


//Mac includes + frameworks OpenGL and GLUT - comment out for Windows
//#include <iostream>
//#include <stdio.h>
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
//#include "gltools.h"
//#include <OpenGL/gl.h>
//#include <OpenGL/glext.h>
//#include <OpenGL/gl3ext.h>

//note that this needs gltools.h and gltools.cpp to be included in the shared directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such
#define IMAGE1       0//GRASS
#define IMAGE2       1//bRICK
#define	IMAGE3		 2//TARGET BLUE
#define	IMAGE4		 3//TARGET GREEN
#define	IMAGE5		 4//TARGET RED
#define IMAGE6		 5//TARGET DULL
#define IMAGE7		 6//FLOWER
#define IMAGE8		 7//SMALL FLOWER
#define IMAGE9		 8//skybox
#define TEXTURE_BALL 9//FOOTBALL

#define TEXTURE_COUNT 10
GLuint  textures[TEXTURE_COUNT];
//GLuint  Sky[IMAGE9];
//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below
const char *textureFiles[TEXTURE_COUNT] = {"grass_diff(1).tga", "brick_texture_lo_res.tga","targetBlue.tga","targetGreen.tga","targetRed.tga","targetDull.tga","palmBranchA.tga","yellowFlowerFinal.tga","stormydays_large.tga","FootballCompleteMap.tga"};


//for lighting if you want to experiment with these
float x;
float y;
float z;
float XinMakingMovement;
float YinMakingMovement;
float ZinMakingMovement;
float speed = 4;
float r = 0;
float phi = 0.0;
float theta = 0.0;
bool shot = false;
bool spacebar;
bool g_bButton1Down;
int red,green,blue;
GLdouble wz_temp = 0.0;
GLdouble wx, wy, wz;
GLfloat mKa[4] = {0.11f,0.06f,0.11f,1.0f}; //ambient
GLfloat mKd[4] = {0.43f,0.47f,0.54f,1.0f}; //diffuse
GLfloat mKs[4] = {1.0f,1.0f,1.0f,1.0f}; //specular
GLfloat mKe[4] = {0.5f,0.5f,0.0f,1.0f}; //emission




// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	 lightPos[] = { 100.0f, 100.0f, 50.0f, 1.0f };
GLUquadricObj *quadricFootball;


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

//end of intialisation 

//------------------------------------//
typedef struct CollisionRect {

	float xUpper;
	float xLower;
	float yUpper;
	float yLower;
	int score;
	bool hit = false;
	int identifier; //note you can use this structure to position the hoops also

} CollisionRect;
CollisionRect BlueTarget, GreenTarget, RedTarget;
void SetCollision()
{
	BlueTarget.xLower = -50; 
	BlueTarget.xUpper = -30;
	BlueTarget.yLower = -30;
	BlueTarget.yUpper = -10;

	GreenTarget.xLower = -20;
	GreenTarget.xUpper = 0;
	GreenTarget.yLower = -10;
	GreenTarget.yUpper = 10;

	RedTarget.xLower = 30;
	RedTarget.xUpper = 50;
	RedTarget.yLower = -30;
	RedTarget.yUpper = -10;


}

void CheckCollision()
{
	if (XinMakingMovement >= BlueTarget.xLower && XinMakingMovement <= BlueTarget.xUpper && YinMakingMovement >= BlueTarget.yLower && YinMakingMovement <= BlueTarget.yUpper && ZinMakingMovement <= 200){
		BlueTarget.hit = true;
		BlueTarget.score = 20;
		
		//std::cout << "It will hit";
	}
	if (XinMakingMovement >= GreenTarget.xLower && XinMakingMovement <= GreenTarget.xUpper && YinMakingMovement >= GreenTarget.yLower && YinMakingMovement <= GreenTarget.yUpper &&!YinMakingMovement == 0 && ZinMakingMovement<=200){
		GreenTarget.hit = true;
		GreenTarget.score = 10;
	}
	if (XinMakingMovement >= RedTarget.xLower && XinMakingMovement <= RedTarget.xUpper && YinMakingMovement >= RedTarget.yLower && YinMakingMovement <= RedTarget.yUpper && ZinMakingMovement <=200){
		RedTarget.hit = true;
		RedTarget.score = 30;
	}
	
}

void handleMouse(int button, int state, int x, int y)
{
	//get world coordinates
	GLint viewport[4];
	GLdouble mvmatrix[16], projectmatrix[16];
	GLint real_y;
	GLfloat z;
	// Respond to mouse button presses.
	if (button == GLUT_LEFT_BUTTON)
	{
		//do something
		if (state == GLUT_DOWN){
			g_bButton1Down = TRUE;
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projectmatrix);
			real_y = viewport[3] - (GLint)y - 1;
			//get the z coordinate for the mouse position
			glReadPixels(x, real_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

			printf("Pixel coordinates at cursor are: (%4d, %4d)\n", x, real_y);

			gluUnProject((GLdouble)x, (GLdouble)real_y, z, mvmatrix, projectmatrix, viewport, &wx, &wy, &wz);

			printf("World coordinates are: (%f, %f, %f)\n", wx, wy, wz);
			//preserve the z coordinate
			wz_temp = wz;
			glutPostRedisplay();
		}
	}

}

void DrawBigFlowers(int image){
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0, 70.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0, 70.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

void DrawSmallFlowes(int image){
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0, 20.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0, 20.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

void DrawTargetDull(int image){
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0, 20.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0, 20.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void drawTargets(int image){
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0, 20.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0, 20.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void drawTexturedBackwall(int image)
{

	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-80.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(80.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(80.0, 75.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-80.0, 75.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void drawTexturedGrass(int image)
{
	
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
   glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable( GL_TEXTURE_2D );
    //bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-110.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(50.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(50.0, 120.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-110.0, 120.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
}

void drawTexturedQuad(int image)
{

	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-80.0, 0.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(50.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(50.0, 100.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-80.0, 100.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void drawFootBall(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
	// MAYBE I NEED TO PUSH THE MATRIX 
	//glPushMatrix();
	glFrontFace(GL_CCW);
	// Create a texture the ball
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BALL]);
	glColor3f(0.5, 0.5, 0.5);
	quadricFootball = gluNewQuadric();
	gluQuadricDrawStyle(quadricFootball, GLU_FILL);
	gluQuadricNormals(quadricFootball, GLU_SMOOTH);
	gluQuadricOrientation(quadricFootball, GLU_OUTSIDE);
	gluQuadricTexture(quadricFootball, GL_TRUE);
	gluSphere(quadricFootball, r, 50, 35);
	glDisable(GL_TEXTURE_2D);
	//glPopMatrix();
	// MAYBE I NEED TO POP BUT I POP IN THE DRAW
}

void MakingMovements(int key, int x, int y){
	if (key == GLUT_KEY_RIGHT){
		std::cout << "You Pressed the right button \n";
		phi = phi + 0.1;
	}
	if (key == GLUT_KEY_LEFT){
		std::cout << "You Pressed the Left button\n ";
		phi= phi - 0.1;
	}
	if (key == GLUT_KEY_UP){
		std::cout << "You Pressed the UP button \n";
		theta = theta + 0.1;
	}
	if (key == GLUT_KEY_DOWN){
		std::cout << "You Pressed the DOWN button\n ";
		theta= theta - 0.1;
	}
	

	XinMakingMovement = r * sin (theta)* cos(phi);
	YinMakingMovement = r * cos(theta)* sin(phi) -40;
	ZinMakingMovement = -r * cos(theta)+ 300;
	std::cout << XinMakingMovement; std::cout << "This is X\n";
	std::cout << YinMakingMovement; std::cout << "This is Y\n";
	std::cout << ZinMakingMovement; std::cout << "This is z\n";
}

void Keyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case' ':
			if(!spacebar){
				spacebar = true;
				shot = true;
				std::cout << "You pressed space";
			}
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		spacebar = false;
		break;
	}
}

void TimerFunc(int value)
{
	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
	if (shot)
	{
		MakingMovements(0, 0, 0);
		r += 2;
		if (ZinMakingMovement <= 200){
			r = 0;
			shot = false;
			//std::cout << "Shot is false";
		}
	}
	
}

void drawCursor(GLdouble x, GLdouble y, GLdouble z)
{
	std::cout << RedTarget.score;
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(x, y, z);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(0.0, 0.0, 200.0);
	glEnd();
	glutSolidSphere(4.0, 10, 5);
	glPopMatrix();
}

GLfloat cubeVertexData[108] =
{
	//object vertex coordinates for cube made from triangles
	// Data layout for each line below is:
	// positionX, positionY, positionZ
	//wind counter-clockwise

	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,

	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,

	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	//4
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	//5
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	//6
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f

};

//the texture coordinates - work will skybox texture of this shape +--

GLfloat textureCoordsSkyBox[72] = {
	//face 1
	0.75, 0.33,      //    0,1,
	0.75, 0.67,     //    1,1,
	0.5, 0.33,     //    0,0,
	0.5, 0.33,     //    0,0,
	0.75, 0.67,    //    1,0,
	0.5, 0.67,   //    1,1,

	//face 2
	0.5, 1.0, //    1,1,
	0.25, 1, //    0,1,
	0.5, 0.67, //    1,0,
	0.5, 0.67, //    1,0,
	0.25, 1.0, //    0,1,
	0.25, 0.67, //    1,1,
	//face 3
	0, 0.67,//    1,1,
	0, 0.33,//    0,1,
	0.25, 0.67,//    1,0,
	0.25, 0.67,//    1,0,
	0, 0.33,//    0,1,
	0.25, 0.33,//    0,0,
	//face 4
	0.25, 0.0,//    0,1,
	0.5, 0.0,//    1,1,
	0.25, 0.33,//    0,0,
	0.25, 0.33,//    0,0,
	0.5, 0.0,//    1,1,
	0.5, 0.33,//    0,0,
	//face 5
	0.5, 0.67,//    1,0,
	0.25, 0.67,//    0,0,
	0.5, 0.33,//    1,1,
	0.5, 0.33,//    1,1,
	0.25, 0.67,//    0,0,
	0.25, 0.33,//    0,1,
	//face 6
	0.75, 0.33,//    1,1,
	1.0, 0.33,//    0,1,
	0.75, 0.67,//    1,0,
	0.75, 0.67,//    1,0,
	1.0, 0.33,//    0,1,
	1.0, 0.67//    0,0


};

//the lighting normals - all facing out from each face
GLfloat gCubeVertexdataNormals[108] =
{
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f

};






void draw_SkyBox(GLuint textures[],int image){

	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW); //texture the inside

	//bind your texture here
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, gCubeVertexdataNormals);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cubeVertexData);
	//   glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsSkyBox);
	// draw a cube - type - start number - number of vertices to draw (so 3 for single triangle)
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);


}

float ang = 30.0f;
void Skyrotation(int value){
	
	ang += 0.2f;
	glutPostRedisplay();
	glutTimerFunc(25, Skyrotation, 0);
	
}
// Called to draw scene
void RenderScene()
{drawCursor(wx, wy, wz);


	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// a test point
	//use for locating points in your view
	/*glEnable(GL_POINT_SMOOTH);
   glPointSize(10.0);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
   glVertex3f(0.0f, 0.0f, 100.0f);
   glEnd();
   */
	//starting from left to right the floor squares.
	// this is for the 
	/* for (x = -300; x <= 300; x++){
		glPushMatrix();
		glTranslatef(x, 50.0, 0.0);
		glRotatef(0.0, 0.0, 0.0, 0.0);
		drawTexturedQuad(IMAGE2);
		glPopMatrix();
		x = x + 50;}*/

	glPushMatrix();
	glTranslated(0.0, 150.0, 0.0);
	glScalef(1000, 1000, 1000);
	draw_SkyBox(textures,IMAGE9);//skybox
	glPopMatrix();
	Skyrotation(0);

	glPushMatrix();
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(wx, wy, wz);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -50.0, 200.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	drawTexturedGrass(IMAGE1);//grass
	glPopMatrix();

	glPushMatrix();
	glTranslatef(79.0, -75.0, 240.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTexturedQuad(IMAGE2);//right sidewall
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-79.0, -75.0, 240.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTexturedQuad(IMAGE2);//right sidewall
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-74.0, -50.0, 295.0);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	DrawBigFlowers(IMAGE7);//FLOWERS
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-74.0, -50.0, 260.0);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	DrawBigFlowers(IMAGE7);//FLOWER
	glPopMatrix();


	glPushMatrix();
	glTranslatef(74.5, -50.0, 260.0);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	DrawBigFlowers(IMAGE7);//FLOWERS
	glPopMatrix();

	glPushMatrix();
	glTranslatef(74.5, -50.0, 295.0);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	DrawBigFlowers(IMAGE7);//FLOWERS
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.0, -50.0, 200.0);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	drawTexturedBackwall(IMAGE2);//backwall
	glPopMatrix();
	for (x = -60; x <= 80; x++){
		glPushMatrix();
		glTranslatef(x, -50.0, 200.5);
		glRotatef(0.0, 0.0, 0.0, 0.0);
		DrawSmallFlowes(IMAGE8);//small flowers
		glPopMatrix();
		x = x + 20;
	}
	if (BlueTarget.hit == true){
		glPushMatrix();
		glTranslatef(-50, -30, 201);
		drawTargets(IMAGE6); // in here needs to go the target blue
		glPopMatrix();
		CheckCollision();
		//score = score + 20;
		//std::cout << "hit\n";
	}
	if (BlueTarget.hit == false)
	{
		CheckCollision();
		glPushMatrix();
		glTranslatef(-50, -30, 201);
		drawTargets(IMAGE3); // in here needs to go the target blue
		glPopMatrix();
		//std::cout << "not hit\n";
	}
	if (GreenTarget.hit == true){
		glPushMatrix();
		glTranslatef(0, -10, 201);
		drawTargets(IMAGE6); // in here needs to go the texture for dull
		glPopMatrix();
		CheckCollision();
		//score = score + 10;
		//std::cout << "hit\n";
	}
	if (GreenTarget.hit == false)
	{
		CheckCollision();
		glPushMatrix();
		glTranslatef(0, -10, 201);
		drawTargets(IMAGE4); // in here needs to go the target Green
		glPopMatrix();
		//std::cout << "not hit\n";
	}
	if (RedTarget.hit == true){
		glPushMatrix();
		glTranslatef(40, -30, 201);
		drawTargets(IMAGE6); // in here needs to go the texture for dull
		glPopMatrix();
		CheckCollision();
		// std::cout << "Hit is true";
	}
	if (RedTarget.hit == false)
	{
		CheckCollision();
		glPushMatrix();
		glTranslatef(40, -30, 201);
		drawTargets(IMAGE5); // in here needs to go the target red
		glPopMatrix();
		//score = score + 10;
		//std::cout << "Hit is False";
		//std::cout << "not hit\n";
	}
	if (shot) 
	{
		glPushMatrix();
		glTranslatef(XinMakingMovement, YinMakingMovement, ZinMakingMovement);
		glRotatef(XinMakingMovement * 50, 0.0, 1, 0.0);
		drawFootBall(0, 0, 0, 5);// in here needs to go the drawFOOTBALL function.
		glPopMatrix();
		//std::cout << "Shot is True";
	}
	else
	{
	glPushMatrix();
	glTranslatef(0, -40, 300);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	drawFootBall(0, 0, 0, 5);// in here needs to go the drawFOOTBALL function.
	glPopMatrix();
	//std::cout << "Shot is false";
	}

	//mb a for loop here
	
		//TimerFunc(1);
		/*glTranslatef(XinMakingMovement, YinMakingMovement, ZinMakingMovement);
		glRotatef(0.0, 0.0, 0.0, 0.0);
		drawFootBall(0, 0, 0, 5);*/// in here needs to go the drawFOOTBALL function.
		

		//for (x = 300; x >= ZinMakingMovement; x++){
		//	glPushMatrix();
		//	glTranslatef(XinMakingMovement, YinMakingMovement, x);
		//	glRotatef(0.0, 0.0, 0.0, 0.0);
		//	drawFootBall(0, 0, 0, 5);//small flowers
		//	glPopMatrix();
		//	x = x + 20;
		//}
		
		glPushMatrix();
		glEnable(GL_POINT_SMOOTH);
		glPointSize(10.0);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		glVertex3f(wx, wy, wz);//this is for the world coridnates
		glEnd();
		glPopMatrix();

		//std::cout << score <<" <- is your score"  ;

	
}
// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    //textures
	SetCollision();
    GLuint texture;
    // allocate a texture name
    glGenTextures( 1, &texture );
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	//make sure any TGA has no alpha channel - photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is from the OpenGL SuperBible book
	//there are quite a few ways of loading images
    // Load textures in a for loop
    glGenTextures(TEXTURE_COUNT, textures);
    //this puts the texture into OpenGL texture memory
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    for(int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        // Bind to next texture object
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);
        
        // Load texture data, set filter and wrap modes
        //note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
        pBytes0 = gltLoadTGA(textureFiles[iLoop],&iWidth, &iHeight,
                             &iComponents, &eFormat);
        
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);
        
            //set up texture parameters
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //try these too
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        free(pBytes0);
    }
    
	//enable textures
    glEnable(GL_TEXTURE_2D);

    
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
    glFrontFace(GL_CW);// Counter clock-wise polygons face out
    //glEnable(GL_CULL_FACE);		// Do not calculate inside

    glCullFace(GL_FRONT_AND_BACK);
    
	// Enable lighting
	//glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLightLessBright);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,redLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
	// Black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f );
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;
    
    // Prevent a divide by zero
    if(h == 0)
        h = 1;
    
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // Calculate aspect ratio of the window
    fAspect = (GLfloat)w/(GLfloat)h;
    
    // Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // field of view of 45 degrees, near and far planes 1.0 and 1000
    //that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
    gluPerspective(55.0f, fAspect, 1.0, 1000.0);
    // Modelview matrix reset
    glMatrixMode(GL_MODELVIEW);
    //pull the eye position back to view the scene
    gluLookAt(0.00,0.00,400.0,//eye
              0.00,0.00,0.00,//centre
              0.00,1.00,0.00);//up
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);// a 4:3 ratio
	glutCreateWindow("Textures Tutorial");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutMouseFunc(handleMouse);
    glutTimerFunc(25, TimerFunc, 1);
	glutTimerFunc(25, Skyrotation, 1);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(MakingMovements);
	SetupRC();
	glutMainLoop();
	
	
	
	return 0;
}






