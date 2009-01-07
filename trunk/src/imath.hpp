////////////////////////////////////////////////////////////////////////////
//
//  iMath
//  Copyright(c) Shun Cox
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IMATH_HPP__
#define __IMATH_HPP__

#include <iostream>
#include <string>
#include <cmath>

namespace imath {

const double PI = 3.1415926535897932384626433832795;

enum iRotationOrder {
	IRO_XYZ, IRO_YZX, IRO_ZXY,
	IRO_ZYX, IRO_YXZ, IRO_XZY,
	IRO_NONE = 0x80000000
};

inline float toDegrees(float rad) { return static_cast<float>(rad * 180.0 / PI); }
inline double toDegrees(double rad) { return (rad * 180.0 / PI); }
inline float toRadians(float deg) { return static_cast<float>(deg * PI / 180.0); }
inline double toRadians(double deg) { return (deg * PI / 180.0); }

#include "ivector.hpp"
#include "imatrix.hpp"
#include "iquaternion.hpp"

typedef iVector<float> iVecF;
typedef iVector4<float> iVec4F;
typedef iMatrix<float> iMatF;
typedef iQuaternion<float> iQuaF;

typedef iVector<double> iVec;
typedef iVector4<double> iVec4;
typedef iMatrix<double> iMat;
typedef iQuaternion<double> iQua;

// class for exception
/*
class badIndex {
	string msg;		// message of exception
public:
	badIndex() {};
	badIndex(string str) : msg(str) {}
};
*/

} // namespace imath

#endif	// __IMATH_HPP__
