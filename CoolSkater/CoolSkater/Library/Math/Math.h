/**
 * @file	Math.cpp
 * @brief	算術系関数定義
 * @author	morimoto
 */
#ifndef LIB_MATH_H
#define LIB_MATH_H

//----------------------------------------------------------------------
// Define
//----------------------------------------------------------------------
#define LIB_PI 3.14159265358979323846f


//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <Windows.h>
#include "Vector2\Vector2.h"
#include "Vector3\Vector3.h"
#include "Vector4\Vector4.h"


namespace Lib
{
	//----------------------------------------------------------------------
	// Utility Functions
	//----------------------------------------------------------------------
	float ToRadian(float _degree);


	//----------------------------------------------------------------------
	// Vector Functions
	//----------------------------------------------------------------------
	float Vector3Dot(VECTOR3* _pIn1, VECTOR3* _pIn2);
	VECTOR3* Vector3Cross(VECTOR3* _pIn1, VECTOR3* _pIn2, VECTOR3* _pOut);
	VECTOR3* Vector3Normalize(VECTOR3* _pOut);

}



#endif // !LIB_MATH_H
