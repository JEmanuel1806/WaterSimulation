#pragma once

#include <glad/glad.h>    

#include <array>
#include <vector>
#include <string>
#include <stb_image.h>
#include <iostream>

class SkyBox {

public:
	SkyBox();
	GLuint generateCubeMap(const std::vector<std::string>& faces);
	std::array<float, 108> skyboxVerts;

private:
	
	int m_width;
	int m_height;
	int m_channels;

};