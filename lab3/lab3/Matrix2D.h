#pragma once
class Matrix2D
{
	float mat[3][2];
	float Ang, Sx, Sy, Cos_a, Sin_a, Tx, Ty;
public:
	Matrix2D();
	~Matrix2D();
	void Identity();
	void SetValue(int i, int j, float value);
	void Translation(float tx, float ty);
	void Scale(float sx, float sy);
	void Rotation(float ang);
	void Rotation(float cos_a, float sin_a);
	void DoTransformation(float &x, float &y);
	void BuildTransformation(float sx, float sy, float ang, float tx, float ty);
};

