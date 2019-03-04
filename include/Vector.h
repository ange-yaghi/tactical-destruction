#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#define EPSILON 0.0001;
inline bool IsZero(double x)
{

	return fabs(x) < EPSILON; 

}

class Vector2
{

public:

	Vector2(double x, double y) {this->x = x; this->y = y; }
	Vector2() { this->x = this->y = 0; }
	~Vector2() {}

	Vector2 operator+(const Vector2 &v)
	{

		return Vector2(v.x + x, v.y + y);

	}

	void operator+=(const Vector2 &v)
	{

		x += v.x;
		y += v.y;

	}

	Vector2 operator-(const Vector2 &v)
	{

		return Vector2(x - v.x, y - v.y);

	}

	void operator-=(const Vector2 &v)
	{

		x -= v.x;
		y -= v.y;

	}

	Vector2 operator*(float s)
	{

		return Vector2(x * s, y *s);

	}

	void operator*=(float s)
	{

		x *= s;
		y *= s;

	}

	double GetMagnitude()
	{

		return sqrt(x*x + y*y);

	}

	double GetSquareMagnitude()
	{

		return x*x + y*y;

	}

	void Normalize()
	{

		double magnitude = GetMagnitude();

		x /= magnitude;
		y /= magnitude;

	}

	void ComponentWise(const Vector2 &v)
	{

		x *= v.x;
		y *= v.y;

	}

	void Clear()
	{

		x = y = 0;

	}

	bool IsZero()
	{

		return ::IsZero(x) && ::IsZero(y);

	}

	double x, y;

};

#endif