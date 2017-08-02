/**
 * @file	Math.cpp
 * @brief	算術系関数実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Math.h"

#include <Windows.h>
#include <math.h>


namespace Lib
{
	//----------------------------------------------------------------------
	// Utility Functions
	//----------------------------------------------------------------------
	float ToRadian(float _degree)
	{
		return _degree * (LIB_PI / 180.0f);
	}


	//----------------------------------------------------------------------
	// Vector Functions
	//----------------------------------------------------------------------
	float Vector3Dot(VECTOR3* _pIn1, VECTOR3* _pIn2)
	{
		return 
			((_pIn1->x) * (_pIn2->x) + 
			 (_pIn1->y) * (_pIn2->y) + 
			 (_pIn1->z) * (_pIn2->z));
	}

	VECTOR3* Vector3Cross(VECTOR3* _pIn1, VECTOR3* _pIn2, VECTOR3* _pOut)
	{
		_pOut->x = (_pIn1->y * _pIn2->z - _pIn1->z * _pIn2->y);
		_pOut->y = (_pIn1->z * _pIn2->x - _pIn1->x * _pIn2->z);
		_pOut->z = (_pIn1->x * _pIn2->y - _pIn1->y * _pIn2->x);

		return _pOut;
	}

	VECTOR3* Vector3Normalize(VECTOR3* _pOut)
	{
		float Len = 1.f / sqrt(_pOut->x * _pOut->x + _pOut->y * _pOut->y + _pOut->z * _pOut->z);
		_pOut->x *= Len;
		_pOut->y *= Len;
		_pOut->z *= Len;

		return _pOut;
	}
}
