#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku


//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <ostream>
#include <iostream>


//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX= 0.0;
double kameraZ = 0.0;
double kameraD = -10.0;
double kameraPredkosc;
double kameraKat = 0;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;


//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy


float vertices[] = {
	0.0,0.5,0.0, //0
	0.5,0.0,0.0, //1
	0.0,-0.5,0.0,//2
	-0.5,0.0,0.0,//3
	0.0,0.0,-0.5,//4
	0.0,0.0,0.5, //5
};

float vertices2[] = {
	0.0,0.0,0.0,
	0.0,1.0,0.0,
	1.0,1.0,0.0,
	1.0,0.0,0.0,
};

float texCoord[] = {

	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	
};

GLuint elements[] = 
{ 0, 1, 5,
0, 1, 4,
0, 3, 5,
0, 3, 4,
2, 1, 5,
2, 1, 4,
2, 3, 5,
2, 3, 4,
};
GLuint elements2[] ={ 0, 1, 2, 3,4,5 };
//shaders
GLuint programID = 0;

unsigned int VBO, VBO2, vtex;
unsigned int ebo, ebo2;
unsigned int VAO[2];
bool onoff = false;


/*###############################################################*/
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
	}

}
/******************************************/



void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case 'x':
		std::cout << "switch" << std::endl;
		onoff = !onoff;
		break;
	case '1':

		break;
	case '2':

		break;
	}
	
	
}
/*###############################################################*/
void rysuj(void)
{

	//GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, color);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera	

	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(0, 0, kameraD)); 
	MV = glm::rotate(MV, (float)glm::radians(kameraZ), glm::vec3(1, 0, 0)); 
	MV = glm::rotate(MV, (float)glm::radians(kameraX), glm::vec3(0, 1, 0)); 

	glm::mat4 MVP = P * MV;

	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));	   // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4	


	float time = glutGet(GLUT_ELAPSED_TIME);

	GLuint Time_id = glGetUniformLocation(programID, "Time");
	glUniform1f(Time_id, time);


	glBindVertexArray(VAO[1]);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawElements(GL_QUADS, 8, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glDrawElements(GL_POLYGON, 12, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_POLYGON, 0,100 );

	glFlush();
	glutSwapBuffers();

}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/
GLfloat k = 0.05;
GLfloat ad = 0.0;

void timer(int value) {

	
	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 5");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
    glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)
									
	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(2, VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);

	glGenBuffers(1, &vtex);
	glBindBuffer(GL_ARRAY_BUFFER, vtex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vtex);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, vtex);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");


	glutMainLoop();					
	
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &ebo2);
	glDeleteBuffers(2, VAO);
	return(0);
}

