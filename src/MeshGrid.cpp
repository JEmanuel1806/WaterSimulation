#include "MeshGrid.h"

#include <glm/gtc/type_ptr.hpp> 

MeshGrid::MeshGrid(int size)
{
	CreatePlane(size);
}

MeshGrid::~MeshGrid()
{
}

// Initiate the Plane resembling watr
void MeshGrid::CreatePlane(int size)
{
	// 25 x 25 grid
	vertices.reserve(size * size);
    indices.reserve((size - 1) * (size - 1) * 6);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Vertex v;
			v.position = glm::vec3((float)j, (float)i, 0.0f);
			vertices.push_back(v);
		}
	}

    for (int y = 0; y < size - 1; ++y) {
        for (int x = 0; x < size - 1; ++x) {
            unsigned int topLeft = y * size + x;
            unsigned int bottomLeft = (y + 1) * size + x;
            unsigned int topRight = y * size + (x + 1);
            unsigned int bottomRight = (y + 1) * size + (x + 1);

            // Dreieck 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Dreieck 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

	std::cout << "Plane created. Vertices: " << vertices.size() << "\n";



}

