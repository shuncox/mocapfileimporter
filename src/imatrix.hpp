////////////////////////////////////////////////////////////////////////////
//
//  iMatrix
//  Copyright(c) Erik Chang
//
////////////////////////////////////////////////////////////////////////////
//
// $Id$
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IMATRIX_HPP__
#define __IMATRIX_HPP__

// We are in namespace "imath" now
// DO NOT including any header file here

template <typename T>
class iVector;

////////////////////////////////////////////////////////////////////////////
// class for 4x4 matrices
template <typename T>
class iMatrix {
	T m[16];
public:

	// initialization
	iMatrix() {
		clear();
	}

	inline void clear() {
		memset(m, 0, sizeof(m));
	}

	iMatrix(const iMatrix<T> &m1) {
		for (unsigned int i = 0; i < 16; i ++) m[i] = m1[i];
	}

	iMatrix(const T m1[]) {
		for (unsigned int i = 0; i < 16; i ++) m[i] = m1[i];
	}

	// access elements
	T& operator()(unsigned int row, unsigned col) {
		if ((row > 3) && (col > 3)) throw badIndex("Matrix subscript out of bounds");
		return m[row * 4 + col];
	}

	T operator()(unsigned int row, unsigned col) const {
		if ((row > 3) && (col > 3)) throw badIndex("const Matrix subscript out of bounds");
		return m[row * 4 + col];
	}

	T& operator[](unsigned int idx) {
		if (idx > 15)	throw badIndex("Matrix subscript out of bounds");
		return m[idx];
	}

	T operator[](unsigned int idx) const {
		if (idx > 15)	throw badIndex("const Matrix subscript out of bounds");
		return m[idx];
	}

	// addition
	iMatrix<T> operator+(const iMatrix<T> &m1) {
		iMatrix<T> m2;
		for (unsigned int i = 0; i < 16; i ++) m2[i] = m[i] + m1[i];
		return m2;
	}

	// multiplication with another 4x4 matrix
	iMatrix<T> operator*(const iMatrix<T> &m1) {
		iMatrix<T> m2;
		unsigned baseIdx;
		for (unsigned int row = 0; row < 4; row ++) {
			baseIdx = row * 4;
			for (unsigned int col = 0; col < 4; col ++) {
				m2(row, col) =
					m[baseIdx + 0] * m1(0, col) +
					m[baseIdx + 1] * m1(1, col) +
					m[baseIdx + 2] * m1(2, col) +
					m[baseIdx + 3] * m1(3, col);
			}
		}
		return m2;
	}

	// multiplication with a vector
	iVector<T> operator*(const iVector<T> &v1) {
		iVector<T> v2;
		v2.x = v1.x * m[0] + v1.y * m[1] + v1.z * m[2] + m[3];
		v2.y = v1.x * m[4] + v1.y * m[5] + v1.z * m[6] + m[7];
		v2.z = v1.x * m[8] + v1.y * m[9] + v1.z * m[10] + m[11];
		return v2;
	}

	// assign a value
	iMatrix<T>& operator=(const iMatrix<T> &m1) {
		for (unsigned int i = 0; i < 16; i ++) m[i] = m1[i];
		return *this;
	}


	friend std::ostream& operator<<(std::ostream &out, iMatrix<T> &temp) {

		temp.output(out);
		return out;
	}

private:
	// overriding standard output operator
	void output(std::ostream &out) {
		unsigned int idx = 0;
		out << "[";
		for (unsigned int row = 0; row < 4; row ++) {
			for (unsigned int col = 0; col < 4; col ++) {
				out << "\t" << m[idx ++];
			}
			if (3 == row) out << "\t]";
			out << endl;
		}
	}
};


////////////////////////////////////////////////////////////////////////////
// class for exception
class badIndex {
	std::string msg;		// message of exception
public:
	badIndex() {};
	badIndex(std::string str) : msg(str) {}
};

#endif	// __IMATRIX_HPP__
