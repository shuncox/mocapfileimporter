////////////////////////////////////////////////////////////////////////////
//
//  iQuaternion
//  Copyright(c) Shun Cox
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IQUATERNION_HPP__
#define __IQUATERNION_HPP__

// We are in namespace "imath" now
// DO NOT including any header file here

template <typename T>
class iQuaternion {
public:
	iVector<T> v;
	T n;

	// initialization
	iQuaternion() {
		clear();
	}

	inline void clear() {
		v.clear();
		n = 0;
	}

	iQuaternion(const iQuaternion<T> &q1) : v(q1.v), n(q1.n) {}

	iQuaternion(const iVector<T> &vv, const T &nn) : v(vv), n(nn) {}

	iQuaternion(const T xx, const T yy, const T zz, const T ww) : v(iVector<T>(xx, yy, zz)), n(ww) {}

	// caculation with quaternions
	inline iQuaternion<T> operator+(const iQuaternion<T> &q1) const {
		return iQuaternion<T>(v + q1.v, n + q1.n);
	}

	inline iQuaternion<T> operator-(const iQuaternion<T> &q1) const {
		return iQuaternion<T>(v - q1.v, n - q1.n);
	}

	inline iQuaternion<T> operator*(const iQuaternion<T> &q1) const {
		iQuaternion<T> result;
//		result.v.x = q1.n * v.x +  q1.v.x * n + q1.v.y * v.z - q1.v.z * v.y;
//		result.v.y = q1.n * v.y +  q1.v.y * n + q1.v.z * v.x - q1.v.x * v.z;
//		result.v.z = q1.n * v.z +  q1.v.z * n + q1.v.x * v.y - q1.v.y * v.x;
//		result.n = q1.n * n - (q1.v.x * v.x + q1.v.y * v.y + q1.v.z * v.z);
		result.n = q1.n * n - q1.v * v;
		result.v = v * q1.n + q1.v * n + q1.v ^ v;
		result.normalize();
		return result;
	}

	inline iQuaternion<T> &operator=(const iQuaternion<T> &q1) {
		v = q1.v; n = q1.n;
		return *this;
	}

	inline iQuaternion<T> &operator+=(const iQuaternion<T> &q1) {
		v += q1.v; n += q1.n;
		return *this;
	}

	inline iQuaternion<T> &operator-=(const iQuaternion<T> &q1) {
		v -= q1.v; n -= q1.n;
		return *this;
	}

	inline iQuaternion<T> &operator*=(const iQuaternion<T> &q1) {
		*this = (*this) * q1;
		return *this;
	}

	inline iQuaternion<T> operator-() const {
		return iQuaternion<T>(-v, -n);
	}

	// caculation with scalars
	inline iQuaternion<T> operator*(const T &s) const {
		return iQuaternion<T>(v * s, n * s);
	}

	inline iQuaternion<T> operator/(const T &s) const {
		return iQuaternion<T>(v / s, n / s);
	}

	inline iQuaternion<T> &operator*=(const T &s) {
		v *= s; n *= s;
		return *this;
	}

	inline iQuaternion<T> &operator/=(const T &s) {
		v /= s; n /= s;
		return *this;
	}

	// with vectors
	inline iQuaternion<T> operator*(const iVector<T> &v1) const {
		return iQuaternion<T>(
			-(v.x * v1.x + v.y * v1.y + v.z * v1.z),
			n * v1.x + v.y * v1.z - v.z * v1.y,
			n * v1.y + v.z * v1.x - v.x * v1.z,
			n * v1.z - v.x * v1.y - v.y * v1.x);
	}

	// magnitude
	inline T magnitude() {
		return static_cast<T>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z + n * n));
	}

	// normalization
	inline iQuaternion<T>& normalize() {
		T factor = 1.0 / magnitude();
		v *= factor; n *= factor;
		return *this;
	}

	// conjugate
	inline iQuaternion<T> operator~() const {
		return iQuaternion<T>(-v , n);
	}

	// with matrices
	inline iQuaternion<T> &operator=(const iMatrix<T> &m) {
		// do it later...
		return *this;
	}

	iMatrix<T> toMatrix() const {
		iMatrix<T> m;
		T nn = n*n, xx = v.x*v.x, yy = v.y*v.y, zz = v.z*v.z;
		T s = 2.0 / (nn + xx + yy + zz);
		T xy = v.x*v.y, xz = v.x*v.z, yz = v.y*v.z, nx = n*v.x, ny = n*v.y, nz = n*v.z;
		m[0]	= 1.0 - s * (yy + zz);
		m[4]	= s * (xy - nz);
		m[8]	= s * (xz + ny);
		m[12]	= 0.0;
		m[1]	= s * (xy + nz);
		m[5]	= 1.0 - s * (xx + zz);
		m[9]	= s * (yz - nx);
		m[13]	= 0.0;
		m[2]	= s * (xz - ny);
		m[6]	= s * (yz + nx);
		m[10]	= 1.0 - s * (xx + yy);
		m[14]	= 0.0;
		m[3]	= 0.0;
		m[7]	= 0.0;
		m[11]	= 0.0;
		m[15]	= 1.0;
		return m;
	}

	// with euler angles
	iVector<T> toEulerAngleXYZ() const {
		T rx, ry, rz;
		rx = - atan2(2 * v.y * v.z - 2 * n * v.x , 1 - 2 * v.x * v.x - 2 * v.y * v.y);
		ry = asin(2 * n * v.y + 2 * v.x * v.z);
		rz = - atan2(2 * v.x * v.y - 2 * n * v.z , 1 - 2 * v.y * v.y - 2 * v.z * v.z);

		return iVector<T>(rx, ry, rz);
	}

	iVector<T> toEulerAngleZXY() const {
		T rx, ry, rz;
		rx = asin( 2 * n * v.x + 2 * v.y * v.z);
		ry = - atan2(2 * v.x * v.z - 2 * n * v.y , 1 - 2 * v.x * v.x - 2 * v.y * v.y);
		rz = - atan2(2 * v.x * v.y - 2 * n * v.z , 1 - 2 * v.x * v.x - 2 * v.z * v.z);
	
		return iVector<T>(rx, ry, rz);
	}

	iQuaternion<T> &fromEulerAngleXYZ(iVector<T> v1) {
		T cyaw, cpitch, croll, syaw, spitch, sroll;
		T cyawcpitch, syawspitch, cyawspitch, syawcpitch;
	
		cyaw = cos(0.5F * v1.z);
		cpitch = cos(0.5F * v1.y);
		croll = cos(0.5F * v1.x);
		syaw = sin(0.5F * v1.z);
		spitch = sin(0.5F * v1.y);
		sroll = sin(0.5F * v1.x);
		cyawcpitch = cyaw * cpitch;
		syawspitch = syaw * spitch;
		cyawspitch = cyaw * spitch;
		syawcpitch = syaw * cpitch;
		n = static_cast<T>(cyawcpitch * croll + syawspitch * sroll);
		v.x = static_cast<T>(cyawcpitch * sroll - syawspitch * croll);
		v.y = static_cast<T>(cyawspitch * croll + syawcpitch * sroll);
		v.z = static_cast<T>(syawcpitch * croll + cyawspitch * sroll);
	
		return (*this);
	}

	// with axis angles
	iQuaternion<T> &setAxisAngle(const iVector<T> &axis, T theta) {
		//theta *= 0.5;
		//n = cos(theta);
		//v = sin(theta) * axis;
		return *this;
	}

	void getAxisAngle(iVector<T> &axis, T &theta) {
		// do it later...
	}

	// rotation
	inline iQuaternion<T> &rotate(const iQuaternion<T> &q1) {
		*this = q1 * (*this) * (~q1);
		return (*this);
	}

	//iVector<T> rotate(const iVector<T> &v1) {
	//	iQuaternion<T> t((*this) * v * (~(*this)));
	//	return t.v;
	//}

	friend std::ostream& operator<<(std::ostream &out, iQuaternion<T> &temp) {
		temp.output(out);
		return out;
	}

private:
	// overriding standard output operator
	void output(std::ostream &out) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << "; " << n << ")";
	}

};

#endif	// __IQUATERNION_HPP__
