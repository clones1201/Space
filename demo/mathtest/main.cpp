// just for math function test

#include <iostream>
using namespace std;

#include <Space.h>
using namespace space::math;

ostream& operator<<(ostream &out, const Matrix &mat){
	for (int i = 0; i < 16; i++){
		out << mat.m[i] << '\t';
		if (i % 4 == 3) out << endl;
	}
	return out;
}

ostream& operator<<(ostream &out, const Vector3 &vec){
	out << vec.x << ' ' << vec.y << ' ' << vec.z;
	return out;
}

ostream& operator<<(ostream &out, const Vector4 &vec){
	out << vec.x << ' ' << vec.y << ' ' << vec.z << ' ' << vec.w;
	return out;
}

int main(){

	Matrix E;

	cout << E;

	Matrix mat(1, 3, 4, 2,
		2, 2, 5, 1,
		3, 4, 3, 9,
		6, 7, 3, 1);

	Matrix inv = MatrixInverse(mat);

	cout << "=================\n" << mat << endl;
	cout << inv;

	Vector4 a(1, 2, 3,4);

	cout << "a " << a << endl;

	Vector4 b = Vec4Transform( a, mat);
	cout << "b " << b << endl;
	Vector4 c = Vec4Transform( b, inv);
	cout << "c " << c << endl;

	cout << MatrixMultiply(mat, inv) << endl;
	cout << "press any key to continue..." << endl;
	cin.get();

}