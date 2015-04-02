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
bool  g_bHelpInfo = false;

//save model view matrix
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

	float ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
	float diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
	float specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
	float shininess[] = { 51.2f };
	float emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightPos[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
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
	glPushMatrix();

	//compute rotation matrix
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
	
	//render scene
	glutSolidTeapot(0.3f);
	
	glPopMatrix();
	if (g_bHelpInfo)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 0.2f);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
			glVertex3f(-1, 0, 1);
			glVertex3f(0, 0, 1);
			glVertex3f(0, 1, 1);
			glVertex3f(-1, 1, 1);
		glEnd();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		glColor3f(1, 1, 1);
		glListBase(1000);
		glRasterPos2f(-1, 0.9);
		char sz[] = "hello, this is a test!\0\0";
		glCallLists(22, GL_UNSIGNED_BYTE, sz);
		glEnable(GL_LIGHTING);
	}
}


