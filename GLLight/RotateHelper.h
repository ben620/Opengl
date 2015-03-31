/**********************************************\
*  MathHelper.h                                *
* this is a simple math helper functions for   *
* opengl rotation.                             *
* no rights reserved.                          *
* ben, 2012                                    *
\**********************************************/


#ifndef MATH_HELP_H_2012_6_22
#define MATH_HELP_H_2012_6_22

#include <cmath>

namespace rthlp{

#define M_PI 3.14159f

/**class CVector3
*/
template<typename T>
class CVector3
{
public:
	CVector3(void)
	{
	}
	CVector3(float x, float y, float z)
	{
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}
	CVector3(const CVector3<T> & vec)
	{
		*this = vec;
	}

	CVector3<T> & operator = (const CVector3<T> & vec)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_data[i] = vec.m_data[i];
		}
		return (*this);
	}

	T Norm(void) const
	{
		return sqrt(m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);
	}

	void operator +=(const CVector3<T>& vec)
	{
		m_data[0] += vec.m_data[0];
		m_data[1] += vec.m_data[1];
		m_data[2] += vec.m_data[2];
	}

	void operator -=(const CVector3<T>& vec)
	{
		m_data[0] -= vec.m_data[0];
		m_data[1] -= vec.m_data[1];
		m_data[2] -= vec.m_data[2];
	}

	void operator *=(T a)
	{
		m_data[0] *= a;
		m_data[1] *= a;
		m_data[2] *= a;
	}

	void MapFromPoint(int x, int y, int nWinWidth, int nWinHeight)
	{
		float d, a;

		m_data[0]  = (2.0f*x - nWinWidth) / nWinWidth;
		m_data[1]  = (nWinHeight - 2.0f*y) / nWinHeight;
		d          = std::sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1]);
		m_data[2]  = std::cos((M_PI/2.0f) * ((d < 1.0f) ? d : 1.0f));
		a          = 1.0f / Norm();
		(*this)   *= a;
	}

	T m_data[3];
};

/**CrossProduct:
* function: compute the cross product of two 3D vector;
* input: two vectors of length 3;
* return: the cross product.
* update:
*2012.7.5, temp object is constructed on return, it may be optimized by compiler.
*/
template <typename T>
const CVector3<T> CrossProduct(const CVector3<T>& vec0, const CVector3<T>& vec1)
{
	T x;
	T y;
	T z;
	x = vec0.m_data[1] * vec1.m_data[2] - vec1.m_data[1] * vec0.m_data[2];
	y = vec1.m_data[0] * vec0.m_data[2] - vec0.m_data[0] * vec1.m_data[2];
	z = vec0.m_data[0] * vec1.m_data[1] - vec1.m_data[0] * vec0.m_data[1];
	return CVector3<T>(x, y, z);
}

template <typename T>
const CVector3<T> operator - (const CVector3<T>& vec0, const CVector3<T>& vec1)
{
	CVector3<float> diff(vec0);
	diff -= vec1;
	return diff;
}


template <class T>
T Clamp(T val, T low, T up)
{
	if (val > up)
	{
		return up;
	}
	if (val < low)
	{
		return low;
	}
	return val;
}


}

#endif