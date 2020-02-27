#ifndef _commonHeader_HG_
#define _commonHeader_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include<stdio.h>

#include<fstream>
#include<sstream>

#include<string>
#include <map>
#include<vector>
#include <algorithm>
#include <iterator>

#define NOMINMAX
#include <Windows.h>

template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

#endif // !_commonHeader_HG_