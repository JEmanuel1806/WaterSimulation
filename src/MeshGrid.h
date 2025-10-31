#pragma once

#include <glm/glm.hpp>          
#include <vector>
#include <iostream>

struct Vertex {
	glm::vec3 position;
};

class MeshGrid {
public:
	MeshGrid(int size);
	~MeshGrid();

	std::vector<int> indices;
	std::vector<Vertex> vertices;

	void CreatePlane(int size);

private:
	
};