#include "stdafx.h"
#include "Matrix2D.h"
#include <math.h>


Matrix2D::Matrix2D()
{
	Identity();
	Ang = 0.0f;
	Sx = Sy = 1.0f;
	Cos_a = 1.0f; Sin_a = 0.0f;
	Tx = Tx = 0.0f;
}


Matrix2D::~Matrix2D()
{
}

void Matrix2D::Identity()
{
	mat[0][0] = 1.0f; mat[0][1] = 0.0f;
	mat[1][0] = 0.0f; mat[1][1] = 1.0f;
	mat[2][0] = 0.0f; mat[2][0] = 0.0f;
}
void Matrix2D::SetValue(int i, int j, float value)
{
	if ((i >= 0 && i<3) && (j >= 0 && (j<2)))
		mat[i][j] = value;
}

void Matrix2D::Translation(float tx, float ty)
{
	Tx = tx;
	Ty = ty;
	mat[2][0] += tx;
	mat[2][1] += ty;
}
void Matrix2D::Scale(float sx, float sy)
{
	Sx = sx;
	Sy = sy;
	for (int i = 0; i<3; i++)
	{
		mat[i][0] *= sx;
		mat[i][1] *= sy;
	}
}
void Matrix2D::Rotation(float ang)
{
	Rotation((float)cos(ang), (float)sin(ang));
}
void Matrix2D::Rotation(float cos_a, float sin_a)
{
	float temp;
	Cos_a = cos_a;
	Sin_a = sin_a;
	for (int i = 0; i<3; i++)
	{
		temp = mat[i][0] * Cos_a - mat[i][1] * Sin_a;
		mat[i][1] = mat[i][0] * Sin_a + mat[i][1] * Cos_a;
		mat[i][0] = temp;
	}
}
void Matrix2D::DoTransformation(float &x, float &y)
{
	float temp;
	temp = x*mat[0][0] + y*mat[1][0] + mat[2][0];
	y = x*mat[0][1] + y*mat[1][1] + mat[2][1];
	x = temp;
}
void Matrix2D::BuildTransformation(float sx, float sy,
	float ang, float tx, float ty)
{
	Identity();
	Scale(sx, sy);
	Rotation(ang);
	Translation(tx, ty);
}