#define WIN32_LEAN_AND_MEAN    
#include <Windows.h>
#include <gl\GL.h>
#include <gl\glut.h>

#include "RotateHelper.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")


//////////////////////////////////////////////////
//rotate, scale and move parameters
float g_fScale = 1.0f;
float g_rotAngle = 45.0f;
float g_xOffSet = 0.0f;
float g_yOffSet = 0.0f;

float g_modelview[16]= {
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f 
						};
rthlp::CVector3<float> g_axis(1.0f, 0.0f, 0.0f);

void Init()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 1);

	//enable lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	float ambient[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
	float diffuse[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
	float specular[] = {0.5082373f, 0.5082373f, 0.5082373f, 1.0f};
	float shininess = 51.2f;
	float emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	float lightPos[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}


void Paint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(g_rotAngle, g_axis.m_data[0], g_axis.m_data[1], g_axis.m_data[2]);
	g_rotAngle = 0.0f;
	glMultMatrixf(g_modelview);
	glGetFloatv(GL_MODELVIEW_MATRIX, g_modelview);

	glLoadIdentity();
	glTranslatef(g_xOffSet, g_yOffSet, 0.0f);
	glScalef(g_fScale, g_fScale, g_fScale);
	glMultMatrixf(g_modelview);

	float lightPos[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
	glutSolidTeapot(0.3f);
}


