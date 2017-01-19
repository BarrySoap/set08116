#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\projection.hpp>
#include <iostream>

using namespace std;
using namespace glm;

int main() {

	/*********************		VECTORS		*************************/

	vec2 a(1.0f, 0.0f);
	vec2 b(0.0f, 1.0f);

	vec3 c(2.0f, 0.0f, 0.0f);
	vec3 d(0.0f, 2.0f, 0.0f);

	vec4 e(3.0f, 0.0f, 0.0f, 0.0f);
	vec4 f(0.0f, 3.0f, 0.0f, 0.0f);

	/**2D Vectors to 3D/4D**/
	vec3(a);
	vec4(b);
	/***********************/

	/**3D Vectors to 2D/4D**/
	vec2(c);
	vec4(d);
	/***********************/

	/**4d Vectors to 2D/3D**/
	vec2(e);
	vec3(f);
	/***********************/

	/**Addition/Subtraction of two 2D Vectors**/
	vec2 ab = a + b;
	vec2 _ab = a - b;
	/******************************************/

	/**Addition/Subtraction of two 3D Vectors**/
	vec3 cd = c + d;
	vec3 _cd = c - d;
	/******************************************/

	/**Addition/Subtraction of two 4D Vectors**/
	vec4 ef = e + f;
	vec4 _ef = e - f;
	/******************************************/

	/**Vector Scaling**/
	vec2 A = a * 2.0f;
	vec3 C = c * 3.0f;
	vec4 E = e * 4.0f;

	vec2 _A = a / 2.0f;
	vec3 _C = c / 3.0f;
	vec4 _E = e / 4.0f;
	/******************/

	/**Length of 2D/3D/4D Vectors**/
	float lengthA = length(a);
	float lengthC = length(c);
	float lengthE = length(e);
	/******************************/

	/**Normalisation of 2D/3D/4D Vectors**/
	vec2 normalA = normalize(a);
	vec3 normalC = normalize(c);
	vec4 normalE = normalize(e);
	/*************************************/

	/**Dot Products of 2D/3D/4D Vectors**/
	float dotAB = dot(a, b);
	float dotCD = dot(c, d);
	float dotEF = dot(e, f);
	/************************************/

	/**Projection of 2D/3D/4D Vectors**/
	vec2 projectionAB = proj(a, b);
	vec3 projectionCD = proj(c, d);
	vec4 projectionEF = proj(e, f);
	/**********************************/

	/**Cross-Product of a 3D Vector**/
	vec3 crossCD = cross(c, d);
	/********************************/

	/*********************		MATRICES		*************************/

	mat2 g(1.0f);
	mat2 h(2.0f);

	mat3 i(1.0f);
	mat3 j(2.0f);
	
	mat4 k(1.0f);
	mat4 l(2.0f);

	/**Addition/Subtraction of Matrices**/
	mat4 kl = k + l;
	mat4 _kl = k - l;
	/************************************/

	/**Scaling Matrices**/
	mat4 K = k * 2.0f;
	mat4 L = l * 3.0f;
	mat4 _K = k / 2.0f;
	mat4 _L = l / 3.0f;
	/********************/

	/**Multiplying matrix by matrix**/
	mat2 gh = g * h;
	mat3 ij = i * j;
	mat4 kl = k * l;
	/********************************/

	/**Multiplying vectors by matrices**/
	vec2 ga = g * a;
	vec3 ic = i * c;
	vec4 ke = k * e;
	vec4 ek = e * k;
	/***********************************/

	/**Translating Matrices**/
	mat4 m = translate(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
	/************************/

	/**Rotation Matrices**/
	float angle = 90.0;
	mat4 rotateX = rotate(mat4(1.0f), angle, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotateY = rotate(mat4(1.0f), angle, vec3(0.0f, 1.0f, 0.0f));
	mat4 rotateZ = rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));

	mat4 XYZ = rotateX * rotateY * rotateZ;
	vec4 _XYZ = XYZ * e;
	/*********************/

	/**Euler Angles**/
	mat4 n = eulerAngleXYZ(1.0f, 2.0f, 3.0f);
	/****************/

	/**Scale Matrices**/
	mat4 o = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
	vec3 oe = o * e;
	/******************/

	/**Matrix Combinations**/
	vec3 p = ke * ek;
	/***********************/

	/*********************		QUATERNIONS		*************************/

	/**Quaternion Rotations**/
	quat qx = rotate(quat(), angle, vec3(1.0f, 0.0f, 0.0f));
	quat qy = rotate(quat(), angle, vec3(0.0f, 1.0f, 0.0f));
	quat qz = rotate(quat(), angle, vec3(0.0f, 0.0f, 1.0f));

	quat qxyz = qx * qy* qz;
	/************************/
	
	/**Quaternion-Vec3 Cast Conversion Transformation???**/
	quat q;
	mat4 wtf = mat4_cast(q);
	vec3 wtff = wtf * e;
	/*****************************************************/
}