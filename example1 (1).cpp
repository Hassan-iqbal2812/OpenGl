/*w1493122
Hassan 
Iqbal 

* EIGA501 - Tutorial 1 - example code
* program draws a white rectangle on a black background
*/
//to view other examples comment out or exclude example 1 from the project and add a new file
//don't accidentally delete it though

#include "shared/gltools.h"	
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
// This is like my contetns page where i call all my functions
				void palette(); //<---- this is for my bar to pull up my label 
				void drawSquare(); //<----- This is to draw my square induvidually also in this function i declare the colour that they should be
				void DrawArrow(); // <----- This is Draw arrow function it takes in to paramters form get GetMagnitude to get the magintude of the line 
				void renderBitmapString(); //<--- this is to push the labels on 
				

			float WholedataArray[1500]; // <---- whole data array allows me to read in all the data.
			float UdataArray[500];// ------+
			float VdataArray[500];//       |----- This is me decalring all of my arrays as global for each piece of data.
			float TdataArray[500];//-------+

	int g = 0;//<--- is my counter
	float pi = 3.14; // <-- need to define pi for my maths of the arrows 
	float magnitudemultiplier;//This is to get my arrow stay in the box so they are able to fit in each box 
	int dateSetNumber = 0;//This quarliates to the page which will be displayed.
	bool ChangingColour = true;
/* reshape callback function
executed when window is moved or resized */
void reshape(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	glLoadIdentity();
	glOrtho(-1.0, 11.0, -3.0, 11.0, 0.0, 11.0);
	glMatrixMode(GL_MODELVIEW);
}

float GrabMagnitude(float u, float v)
{
	float a = sqrt(u * u) + (v * v);// Formula given by Phillip in the tutorail
	return a;// Returns corectly However do i need to put this as a float just think about it

}
void animation(int i){
	if (dateSetNumber != 4){
		dateSetNumber++;
			glutTimerFunc(1000, animation, 0);
		glutPostRedisplay();
	}
}
					void changeFrameSec(unsigned char key, int x, int y){
						if (key == 'c' && dateSetNumber < 4){
							animation(0);
						}
							if (key == 't'){

								if (ChangingColour == true)
									ChangingColour == false;
								else ChangingColour == true;

								glutPostRedisplay();
							}
						}
                            
					void KeysThatDoSomeRandomShixt(int key, int x, int y){
											if (key == GLUT_KEY_RIGHT && dateSetNumber < 4){
													dateSetNumber++;      }
											if (key == GLUT_KEY_LEFT && dateSetNumber > 0){
													dateSetNumber--;     }
													glutPostRedisplay();

																		}
			//		void cPressed(char spec){//<----> does this really work for me 
			//if (spec == 'c'){
			//changeFrameSec(0);}
			//					}

void readDataFile()
{	float FitItIn = 1; 
	float TheLargestMagnitude = 0;
	float MagnitudeAtm;

	std::ifstream infile;
	/*note make sure you  declare your array where you need it - ideally this function
	should return the array*/
	
	float WholedataArray[1500]; //float dataArray[5000]; //should be the size of your expected data so 1500 for the CW
	int i = 0;
	char cNum[10];//must be no bigger than 5 characters
	infile.open("H:\\3d graphics\\tutorial 1-2\\Debug\\data.txt", std::ifstream::in); //name of your data file - use either full address
	// or place file in the same directory as the executable (usually Debug folder on PC)
	if (infile.is_open())
	{
		while (infile.good())
		{
			infile.getline(cNum, 256, ',');
			WholedataArray[i] = atof(cNum);
			i++;
			//output to console for check
			std::cout << cNum << " ";
		}
		infile.close(); //always close the file
	}
	else
	{
		std::cout << "Error opening file";
	}
	//This Part is fine my sorting works good NICE!!!
	int j = 0;
	for (int k = 0; k < 500; k++){
		UdataArray[k] = WholedataArray[j];
		j++;
		VdataArray[k] = WholedataArray[j];
		j++;
		TdataArray[k] = WholedataArray[j];
		j++;



	}
	
	
	

		for (int i = 0; i < 500; i++) {

			MagnitudeAtm = GrabMagnitude(UdataArray[i], VdataArray[i]);

			if (MagnitudeAtm > TheLargestMagnitude)
				TheLargestMagnitude = MagnitudeAtm;

		}
		
		magnitudemultiplier = FitItIn / TheLargestMagnitude;
		//The multiply all magnitudes by this multiplier to make them all fit inside my boxes
	




 }

void renderBitmapString(
	double x,
	double y,
	float z,
	void *font,
	char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void DrawArrow(float a, float angleInDegrees)
{   int MakingThemLonger = 2;
	glPushMatrix;
	glTranslatef(0.5,0.5, 0);
	glLineWidth(10);
	glRotatef(angleInDegrees, 0, 0, -1);
	//colour is black 
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, -a / MakingThemLonger);
	glVertex2f(0, a / MakingThemLonger);
	glEnd();
	glPopMatrix();
}


void display(void)
{g = 0  + dateSetNumber *100 ;
	for (int y = 0; y < 10; y++)
	{
		for (int x= 0; x < 10; x++)
		{
			glPushMatrix();
			glTranslatef(x, 10 - y, 0); 
			drawSquare();
										float magnitude = GrabMagnitude(UdataArray[g], VdataArray[g]);
										//find out what wind angle is
										float angleInOfWindRadians = atan2(VdataArray[g], UdataArray[g]); // this is  to find out the values of rotation of all our 
										DrawArrow(magnitude * magnitudemultiplier, angleInOfWindRadians * (180 / pi));

			
			g++;
			glPopMatrix();
		}
	}
	



	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.5, -0.4, 0.0);
	palette(); 
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	palette();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.0, -0.4, 0.0);
	palette();
	glColor3f(1.0, 0.0, 1.0);
	glTranslatef(0.0, -0.4, 0.0);
	palette();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	palette();
	glColor3f(0.0, 1.0, 1.0);
	glTranslatef(0.0, -0.4, 0.0);
	palette();
	glColor3f(0.0, 1.0, 0.5);
	glTranslatef(0.0, -0.4, 0.0);
	palette();
	

	glFlush();     /* execute drawing commands in buffer */
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	

	renderBitmapString(1.0, -0.4, 0.0, GLUT_BITMAP_HELVETICA_12, "4 - 6");

	renderBitmapString(1.0, -0.8, 0.0, GLUT_BITMAP_HELVETICA_12, "6 - 8");
	

	renderBitmapString(1.0, -1.2, 0.0, GLUT_BITMAP_HELVETICA_12, "8 - 10");

	renderBitmapString(1.0, -1.6, 0.0, GLUT_BITMAP_HELVETICA_12, "10 - 12");

	renderBitmapString(1.0, -2.0, 0.0, GLUT_BITMAP_HELVETICA_12, "12 - 14");

	renderBitmapString(1.0, -2.4, 0.0, GLUT_BITMAP_HELVETICA_12, "14 - 16");

	renderBitmapString(1.0, -2.8, 0.0, GLUT_BITMAP_HELVETICA_12, "16 - 17");

	renderBitmapString(1.0, -3.2, 0.0, GLUT_BITMAP_HELVETICA_12, "17 - 20");
	
	renderBitmapString(5.0, -2.2, 0.0, GLUT_BITMAP_HELVETICA_12, "3D Graphics Coursework 1");

	if (dateSetNumber == 0)
	{
		renderBitmapString(3.0, -1.2, 0.0, GLUT_BITMAP_HELVETICA_12, "day 1");
		
	}
	if (dateSetNumber == 1)
	{
		
		renderBitmapString(4.0, -1.2, 0.0, GLUT_BITMAP_HELVETICA_12, "day 2");
		
	}
	if (dateSetNumber == 2)
	{
		renderBitmapString(5.0, -1.2, 0.0, GLUT_BITMAP_HELVETICA_12, "day 3");
		
	}
	if (dateSetNumber == 3)
	{
		renderBitmapString(6.0, -1.2, 0.0, GLUT_BITMAP_HELVETICA_12, "day 4");
		
	}
	if (dateSetNumber == 4)
	{
		renderBitmapString(7.0, -1.2, 0.0, GLUT_BITMAP_HELVETICA_12, "day 5");
		
	}

	glutSwapBuffers();
}

void palette()
{

	glBegin(GL_POLYGON);
	glVertex2f(0.2, 0.2);
	glVertex2f(0.2, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 0.2);
	glEnd();
}

void drawSquare()
{
	if (ChangingColour == true){
		//The Below Colour is red
		if (TdataArray[g] > 4 && TdataArray[g] <= 6){
			glColor3f(1.0, 0.0, 0.0);
		}//g++;
		// The Below coulour is green
		else if (TdataArray[g] > 6 && TdataArray[g] <= 8){
			glColor3f(0.0, 1.0, 0.0);
		}
		// The below colour is Blue
		else if (TdataArray[g] > 8 && TdataArray[g] <= 13){
			glColor3f(0.0, 0.0, 1.0);
		}
		// The Below colour is purple
		else if (TdataArray[g] > 13 && TdataArray[g] <= 15){
			glColor3f(1.0, 0.0, 1.0);
		}
		//This Is yellow
		else if (TdataArray[g] > 15 && TdataArray[g] <= 17){
			glColor3f(1.0, 1.0, 0.0);
		}
		//this teal
		else if (TdataArray[g] > 17 && TdataArray[g] <= 19){
			glColor3f(0.0, 1.0, 1.0);
		}
		//this is lighter green
		else if (TdataArray[g] > 19 && TdataArray[g] <= 21){
			glColor3f(0.0, 1.0, 0.5);
		}
	}
	else glColor3f(1, 1, 1);
	//Drawing my squares from the points
	glBegin(GL_POLYGON);
	glVertex2f(1.0, 1.0);
	glVertex2f(1.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 1.0);
	glEnd();
}










/* display callback function
called whenever contents of window need to be re-displayed */
//this is the all important drawing method - all drawing code goes in here



/* graphics initialisation */
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	readDataFile();
	/* window will be cleared to black */
	//DataSorting();//CalldataSortingHere.

}

//rename this to main(...) and change example 2 to run this main function
int main(int argc, char** argv)
{

	
	/* window management code ... */
	/* initialises GLUT and processes any command line arguments */
	glutInit(&argc, argv);
	/* use single-buffered window and RGBA colour model */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	/* window width = 400 pixels, height = 400 pixels */
	glutInitWindowSize(700, 700);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(100, 100);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("Example 1");
	glutKeyboardFunc(changeFrameSec);
	glutSpecialFunc(KeysThatDoSomeRandomShixt);
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

