#include "stdafx.h"
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

CCamera cam;
float speed = 1;

GLint n = 1;
GLint fill = 0;
GLint TOL = 10;
GLdouble tolerance = sqr(TOL);
typedef struct { GLfloat x, y, z; } point3D_type;

GLint viewport[4];
GLdouble mvmatrix[16], projmatrix[16];

float vAng = 40.0f, asp = 1.0f, nearD = 0.2f, farD = 40.0f;
int w = 300, h = 300;

//GLfloat ctrlpoints[4][4][3] =
point3D_type ctrlpoints[4][4] =

{
	{
		{ -1.5, -1.5, 4.0 },
		{ -0.5, -1.5, 2.0 },
		{ 0.5, -1.5, -1.0 },
		{ 1.5, -1.5, 2.0 } },
		{
			{ -1.5, -0.5, 1.0 },
			{ -0.5, -0.5, 3.0 },
			{ 0.5, -0.5, 0.0 },
			{ 1.5, -0.5, -1.0 } },
			{
				{ -1.5, 0.5, 4.0 },
				{ -0.5, 0.5, 0.0 },
				{ 0.5, 0.5, 3.0 },
				{ 1.5, 0.5, 4.0 } },
				{
					{ -1.5, 1.5, -2.0 },
					{ -0.5, 1.5, -2.0 },
					{ 0.5, 1.5, 0.0 },
					{ 1.5, 1.5, -1.0 } }
};

//atenção: n é o grau do polinomio.
void Casteljau(GLfloat t, point3D_type a[], point3D_type b[], point3D_type c[], int n)
{
	int i, j; GLfloat  t_1 = 1 - t;
	for (i = 0; i <= n; i++)
		c[i] = a[i];
	b[0] = a[0];
	for (j = 1; j <= n; j++)
	{
		for (i = 0; i <= n - j; i++)
		{
			c[i].x = t_1*c[i].x + t*c[i + 1].x;
			c[i].y = t_1*c[i].y + t*c[i + 1].y;
			c[i].z = t_1*c[i].z + t*c[i + 1].z;
		}
		b[j] = c[0];
	}

}

void SaveBezier(char *file_name, point3D_type a[], GLint n, GLint m)
{
	FILE *file;
	fopen_s(&file, file_name, "w");
	fprintf(file, "%d %d\n", n, m);
	for (int j = 0; j<m; j++)
	{
		for (int i = 0; i<n; i++)
			fprintf(file, "%f %f %f , ", a[j*n + i].x, a[j*n + i].y, a[j*n + i].z);
		fprintf(file, "\n");
	}
	fclose(file);
}

void LoadBezier(char *file_name, point3D_type a[], GLint* n, GLint* m)
{
	FILE *file;
	fopen_s(&file, file_name, "r");
	fscanf_s(file, "%d %d\n", n, m);
	for (int j = 0; j<(*m); j++)
	{
		for (int i = 0; i<(*n); i++)
			fscanf_s(file, "%f %f %f", &(a[j*(*n) + i].x), &(a[j*(*n) + i].y), &(a[j*(*n) + i].z));
	}
	fclose(file);
}

void BezierSubdivision(GLfloat u, GLfloat v, point3D_type a[],
	point3D_type b[], point3D_type c[], point3D_type d[], point3D_type e[],
	GLint n, GLint m)
{
	point3D_type *col_a = new point3D_type[m];
	point3D_type *col_b = new point3D_type[m];
	point3D_type *col_c = new point3D_type[m];
	int i, j;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<m; j++)
			col_a[j] = a[j*n + i];
		Casteljau(v, col_a, col_b, col_c, m - 1);
		for (j = 0; j<m; j++)
		{
			b[j*n + i] = col_b[j];
			c[j*n + i] = col_c[j];
		}
	}

	for (j = 0; j<m; j++)
	{
		Casteljau(u, &b[j*n], &b[j*n], &d[j*n], n - 1);
		Casteljau(u, &c[j*n], &c[j*n], &e[j*n], n - 1);
	}
}

void DrawBezier(point3D_type a[], GLint n, GLint m, GLint nn, GLint mm)
{
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, n, 0, 1, n * 3, m, &(GLfloat &)a[0]);
	glMapGrid2f(n, 0.0, 1.0, m, 0.0, 1.0);
	if (fill == 1)
		glEvalMesh2(GL_FILL, 0, nn, 0, mm);
	else glEvalMesh2(GL_LINE, 0, nn, 0, mm);
}

bool Limit(point3D_type a[], GLint n, GLint m)
{
	int i, j; GLdouble x1, y1, z1, x2, y2, z2, max = 0.0, d;
	// analisar os pares (j,i) (j,i+1)
	for (j = 0; j<m; j++)
		for (i = 0; i<n - 1; i++)
		{  // projetar os pontos de controle (j,i) e (j,i+1)
		gluProject((GLdouble)a[j*n + i].x, (GLdouble)a[j*n + i].y, (GLdouble)a[j*n + i].z,
			mvmatrix, projmatrix, viewport, &x1, &y1, &z1);
		gluProject((GLdouble)a[j*n + i + 1].x, (GLdouble)a[j*n + i + 1].y, (GLdouble)a[j*n + i + 1].z,
			mvmatrix, projmatrix, viewport, &x2, &y2, &z2);
		d = sqr(x2 - x2) + sqr(y2 - y1);
		if (d>max)
			max = d;
		}

	// analisar os pares (j, i) (j + 1, i)
	for (j = 0; j<m-1; j++)
		for (i = 0; i<n; i++)
		{  // projetar os pontos de controle (j, i) (j + 1, i)
		gluProject((GLdouble)a[j*n + i].x, (GLdouble)a[j*n + i].y, (GLdouble)a[j*n + i].z,
			mvmatrix, projmatrix, viewport, &x1, &y1, &z1);
		gluProject((GLdouble)a[(j+1)*n + i].x, (GLdouble)a[(j+1)*n + i].y, (GLdouble)a[(j+1)*n + i].z,
			mvmatrix, projmatrix, viewport, &x2, &y2, &z2);
		d = sqr(x2 - x2) + sqr(y2 - y1);
		if (d>max)
			max = d;
		}

	return (max <= tolerance);


}

void BezierRecursiveSubdivision(point3D_type a[], GLint n, GLint m)
{


	if (Limit(a, n, m))
		DrawBezier(a, n, m, n, m);
	else {
		point3D_type *b = new point3D_type[m*n];
		point3D_type *c = new point3D_type[m*n];
		point3D_type *d = new point3D_type[m*n];
		point3D_type *e = new point3D_type[m*n];
		BezierSubdivision(0.5f, 0.5f, a, b, c, d, e, n, m);

		if (Limit(b, n, m))
			DrawBezier(b, n, m, n, m);
		else BezierRecursiveSubdivision(b, n, m);
		if (Limit(c, n, m))
			DrawBezier(c, n, m, n, m);
		else BezierRecursiveSubdivision(c, n, m);
		if (Limit(d, n, m))
			DrawBezier(d, n, m, n, m);
		else BezierRecursiveSubdivision(d, n, m);
		if (Limit(e, n, m))
			DrawBezier(e, n, m, n, m);
		else BezierRecursiveSubdivision(e, n, m);
	}
}

void initlights(void)
{
	GLfloat ambient[] =
	{ 0.1, 0.7, 0.7, 1.0 };
	GLfloat position[] =
	{ 0.0, 0.0, 3.0, 2.0 };
	GLfloat mat_diffuse[] =
	{ 0.1, 0.9, 0.9, 1.0 };
	GLfloat mat_specular[] =
	{ 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] =
	{ 50.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

GLfloat texpts[2][2][2] = { { { 0.0, 0.0 }, { 0.0, 1.0 } },
{ { 1.0, 0.0 }, { 1.0, 1.0 } } };

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMapGrid2f(n, 0.0, 1.0, n, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//glRotatef(85.0, 1.0, 1.0, 1.0);
	//gluPerspective(10, 20, 30, 40);
	cam.Update();

	BezierRecursiveSubdivision((point3D_type *)ctrlpoints, 4, 4);
	glPopMatrix();

	glFlush();
	glutSwapBuffers(); // display the screen just made 

}


void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &(GLfloat &)ctrlpoints);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glMapGrid2f(n, 0.0, 1.0, n, 0.0, 1.0);
	initlights();       /* for lighted version only */
}

void myReshape(int w, int h)
{
	GLfloat f = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w,
		4.0 * (GLfloat)h / (GLfloat)w, -4.0, 4.0);
	else
		glOrtho(-4.0 * (GLfloat)w / (GLfloat)h,
		4.0 * (GLfloat)w / (GLfloat)h, -4.0, 4.0, -4.0, 4.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cam.setAspect(((float)w)/h);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
}

void inputKey(unsigned char c, int x, int y) {
	float ang = speed;
	switch (c) {
	case 'd': cam.MoveCamera(RIGHT, speed); break;
	case 'a': cam.MoveCamera(LEFT, speed); break;
	case 'w': 
		w = w + speed;
		h = h + speed;
		myReshape(w, h);
		break;
	case 's': 
		w = w - speed;
		h = h - speed;
		myReshape(w, h);
		break;
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
	case 'k': SaveBezier("data.dat", (point3D_type *)ctrlpoints, 4, 4); break;
	case 'l': LoadBezier("data.dat", (point3D_type *)ctrlpoints, &n, &n); break;
	default: break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(inputKey);

	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}
