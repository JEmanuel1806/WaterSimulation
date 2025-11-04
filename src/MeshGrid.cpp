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
			v.position = glm::vec3((float)j, 0.0f, (float)i );
			vertices.push_back(v);
		}
	}

    for (int z = 0; z < size - 1; ++z) {
        for (int x = 0; x < size - 1; ++x) {
            unsigned int topLeft = z * size + x;
            unsigned int bottomLeft = (z + 1) * size + x;
            unsigned int topRight = z * size + (x + 1);
            unsigned int bottomRight = (z + 1) * size + (x + 1);

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

