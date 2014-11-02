#include "stdafx.h"

#include <math.h>

#include <stdio.h>
#include <GL/glut.h>
#define sqr(x) ((x)*(x))
#define ROLL 1
#define YAW 2
#define PITCH 3
#define FRONT 4
#define BACK 5
#define RIGHT 6
#define LEFT 7
#define UP 8
#define DOWN 9

class CPoint3D {
public:
	float x, y, z;
	CPoint3D()
	{};
	CPoint3D(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}
	void Set(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}
	void Normalize()
	{
		float L = sqr(x) + sqr(y) + sqr(z);
		if (L>0.0f)
		{
			x /= L; y /= L; z /= L;
		}
	}

	CPoint3D operator +(CPoint3D p)
	{
		return CPoint3D(x + p.x, y + p.y, z + p.z);
	}
	CPoint3D operator -(CPoint3D p)
	{
		return CPoint3D(x - p.x, y - p.y, z - p.z);
	}
	CPoint3D operator *(CPoint3D p)
	{
		return CPoint3D(x * p.x, y * p.y, z * p.z);
	}
	
	CPoint3D operator /(CPoint3D p)
	{
		return CPoint3D(x / p.x, y / p.y, z / p.z);
	}

};

CPoint3D operator *(const CPoint3D& p, float k)
{
	return CPoint3D(k*p.x, k*p.y, k*p.z);
}
CPoint3D operator *(float k, const CPoint3D& p)
{
	return CPoint3D(k*p.x, k*p.y, k*p.z);
}

CPoint3D CrossProduct(const CPoint3D& u, const CPoint3D& v) {
	return CPoint3D(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

class CCamera{
public:
	CPoint3D P0, At, Up;
	float AngR, AngY, AngP;
	CCamera()
	{
		P0.Set(0.0f, 0.0f, 0.0f); At.Set(0.0f, 0.0f, 1.0f); Up.Set(0.0f, 1.0f, 0.0f); AngR = AngY = AngP = 0.0f;
	}
	CCamera(CPoint3D p0, CPoint3D p, CPoint3D up)
	{
		P0 = p0; At = p; Up = up; AngR = AngY = AngP = 0.0f;
	}
	void Set(CPoint3D p0, CPoint3D p, CPoint3D up)
	{
		P0 = p0; At = p; Up = up; AngR = AngY = AngP = 0.0f;
	}
	void Set(float x0, float y0, float z0, float px0, float py0, float pz0, float upx, float upy, float upz)
	{
		P0.Set(x0, y0, z0); At.Set(px0, py0, pz0); Up.Set(upx, upy, upz); AngR = AngY = AngP = 0.0f;
	}
	void SetUp(float x1, float y1, float z1)
	{
		Up.x = x1; Up.y = y1; Up.z = z1;
	}
	void SetUp(CPoint3D up)
	{
		Up = up;
	}
	void SetPosition(float x, float y, float z)
	{
		P0.Set(x, y, z);
	}
	void SetPosition(CPoint3D p)
	{
		P0 = p;
	}
	void SetDirection(CPoint3D D)
	{
		At = D;
	}
	void SetDirection(float x, float y, float z)
	{
		At.Set(x, y, z);
	}
	CPoint3D GetPosition()
	{
		return P0;
	}
	CPoint3D GetDirection()
	{
		return At;
	}

	void MoveCamera(int direction, float delta) {
		CPoint3D v;
		switch (direction){
		case BACK: delta = -delta;
		case FRONT:
			v = At - P0;
			v.Normalize();
			SetPosition(P0 + delta*v);
			SetDirection(At + delta*v); break;
		case LEFT: delta = -delta;
		case RIGHT:
			v = CrossProduct(At - P0, Up);
			v.Normalize();
			SetPosition(P0 + delta*v);
			SetDirection(At + delta*v); break;
		case DOWN: delta = -delta;
		case UP:
			v = Up;
			v.Normalize();
			SetPosition(P0 + delta*v);
			SetDirection(At + delta*v); break;
		}
		Update();
	}
	
	void RotateCamera(int rotMode, float ang)
	{
		CPoint3D v;
		switch (rotMode){
		case ROLL: v = P0 - At; break;
		case YAW: v = Up; break;
		case PITCH: v = CrossProduct(Up, At - P0); break;
		}
		glRotatef(ang, v.x, v.y, v.z);
	}

	void AddRotate(int rotMode, float ang)
	{
		switch (rotMode){
		case ROLL: AngR += ang; break;
		case YAW: AngY += ang; break;
		case PITCH: AngP += ang;  break;
		}
		Update();
	}

	void LookAt()  // call gluLookAt
	{
		gluLookAt((GLdouble)P0.x, (GLdouble)P0.y, (GLdouble)P0.z,
			(GLdouble)At.x, (GLdouble)At.y, (GLdouble)At.z,
			(GLdouble)Up.x, (GLdouble)Up.y, (GLdouble)Up.z);
	}
	void Update()
	{
		glLoadIdentity();
		LookAt();
		RotateCamera(ROLL, AngR);
		RotateCamera(YAW, AngY);
		RotateCamera(PITCH, AngP);
	}
};

float angle = 0.0;
float x = 0.0f, y = 1.75f, z = 5.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float da = 0.5f;
float speed = 0.2;

float ratio = 1.0;
int frame, time, timebase = 0;
char s[30];
CCamera cam;
GLuint DLid;

GLuint createDL(void);

void changeSize(int w, int h)	{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//TO DO // Set camera initial position 
		// this is for the snow man scenario 
		cam.Set(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
	cam.Update();
}

float colorBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float colorWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float colorGray[] = { 0.6f, 0.6f, 0.6f, 1.0f };
float colorRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
float colorBlue[] = { 0.0f, 0.0f, 0.1f, 1.0f };
float colorYellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
float colorLightYellow[] = { .5f, .5f, 0.0f, 1.0f };

void initlights(void)
{
	GLfloat ambient[] =
	{ 0.1, 0.7, 0.7, 1.0 };
	GLfloat position[] =
	{ 0.0, 0.0, 3.0, 2.0 };

	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHT2);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	float posLight1[] = { 1.0f, 1.f, 1.f, 0.0f };
	float spotDirection[] = { -1.0f, -1.0f, 0.f };
    glLightfv(GL_LIGHT1, GL_POSITION, posLight1 );
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0F );
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection );
	glLightfv(GL_LIGHT1, GL_AMBIENT, colorGray);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, colorGray);
	glLightfv(GL_LIGHT1, GL_SPECULAR, colorWhite);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2f);

	float posLight2[] = { .5f, 1.f, 3.f, 0.0f };
	glLightfv(GL_LIGHT2, GL_POSITION, posLight2);
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, colorGray);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, colorGray);
	glLightfv(GL_LIGHT2, GL_SPECULAR, colorWhite);
	
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.8f);
}

void setMaterial(float color[])
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colorBlack);
}

void initScene() {

	glEnable(GL_DEPTH_TEST);
	// Load or call scenario
	DLid = createDL();
	initlights();
}


void drawSnowMan() {

	setMaterial(colorWhite);
	
	// Draw Body	
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);


	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	setMaterial(colorBlack);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	setMaterial(colorYellow);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}

void drawTeaPot() {

	setMaterial(colorRed);
	glTranslatef(0.0f, 0.5f, 0.0f);
	glutSolidTeapot(1.0f);

}

void drawUmbrella() {

	// Draw cabe	
	setMaterial(colorBlack);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -1.0f);
	GLUquadric* quadObj = gluNewQuadric();
	gluCylinder(quadObj, 0.02f, 0.02f, 1.0f, 10, 10);

	// Draw top
	setMaterial(colorBlue);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(1.0f, 0.15f, 10, 10);
}


GLuint createDL() {
	GLuint snowManDL, loopDL, teaPotDL, umbrellaDL;

	snowManDL = glGenLists(1);
	teaPotDL = glGenLists(1);
	umbrellaDL = glGenLists(1);

	loopDL = glGenLists(1);

	glNewList(snowManDL, GL_COMPILE);
	drawSnowMan();
	glEndList();
	
	glNewList(teaPotDL, GL_COMPILE);
	drawTeaPot();
	glEndList();
	
	glNewList(umbrellaDL, GL_COMPILE);
	drawUmbrella();
	glEndList();

	glNewList(loopDL, GL_COMPILE);
	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++) {
		glPushMatrix();
		glTranslatef(i*10.0f, 0, j * 10.0f);
		switch ((i+j) % 3) {
		case 0: glCallList(umbrellaDL); break;
		case 1: glCallList(teaPotDL); break;
		case 2: glCallList(snowManDL); break;
		}
		
		glPopMatrix();
		}
	glEndList();

	return(loopDL);
}


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw ground

	setMaterial(colorWhite);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	glCallList(DLid);
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(s, "FPS:%4.2f", frame*1000.0 / (time - timebase));
		glutSetWindowTitle(s);
		timebase = time;
		frame = 0;
	}
	glutSwapBuffers();
}

void inputKey(unsigned char c, int x, int y) {
	float ang = speed;
	switch (c) {
	case 'd': cam.MoveCamera(RIGHT, speed); break;
	case 'a': cam.MoveCamera(LEFT, speed); break;
	case 'w': cam.MoveCamera(FRONT, speed); break;
	case 's': cam.MoveCamera(BACK, speed); break;
	case 'z': cam.MoveCamera(UP, speed); break;
	case 'x': cam.MoveCamera(DOWN, speed); break;
	case '+': speed *= 2; break;
	case '-': speed /= 2; break;
	case 'r': cam.AddRotate(ROLL, ang); break;
	case 't': cam.AddRotate(ROLL, -ang); break;
	case 'y': cam.AddRotate(YAW, ang); break;
	case 'u': cam.AddRotate(YAW, -ang); break;
	case 'i': cam.AddRotate(PITCH, ang); break;
	case 'o': cam.AddRotate(PITCH, -ang); break;
	default: break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Lab4");
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	initScene();

	glutKeyboardFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);
	glutMainLoop();

	return(0);
}

