#pragma once
#include <SFML/System.hpp>

class Vertex
{
	public: 
		sf::Vector3f location;
		Vertex(sf::Vector3f location_)
		{
			location = location_;
		}
		
		sf::Vector2f project()
		{
			return sf::Vector2f{location.x / location.z, location.y / location.z};
		}
};

class Face // Face is defined here as 3, and always 3, vertices
{
	public:
	int v1;
	int v2;
	int v3;

	Face(int v1_, int v2_, int v3_)
		: v1{v1_}
		, v2{v2_}
		, v3{v3_}
	{}
};

class Environment
{
	public:
		Environment() = default;
		
		int addVertex(sf::Vector3f location) 
		{
			vertices.emplace_back(location);
			return vertices.size() - 1;
		}
		std::vector<Vertex>& getVertices()
		{
			return vertices;
		}
		
		int addFace(sf::Vector3f v1, sf::Vector3f v2, sf::Vector3f v3) //Adds all new vertices
		{
			int vert1 = addVertex(v1);
			int vert2 = addVertex(v2);
			int vert3 = addVertex(v3);

			faces.emplace_back(vert1, vert2, vert3);
			return faces.size() - 1;
		}
		int addFace(int v1, int v2, int v3) //Reuses existing verts
		{
			faces.emplace_back(v1, v2, v3);
			return faces.size() - 1;
		}
		std::vector<Face>& getFaces()
		{
			return faces;
		}
		
		void addQuad(int v1, int v2, int v3, int v4) //Adds the 2 faces in a Quad must be clockwise/counterclockwise
		{
			addFace(v1, v2, v4);
			addFace(v2, v3, v4);
		}

		void moveVertex(sf::Vector3f change) //Changes all Vertexes
		{
			for (auto& vert : vertices) {
				vert.location += change;
			}
		}
		void moveVertex(int index, sf::Vector3f change) // Only changes one index
		{
			vertices[index].location += change;
		}
	private:
		std::vector<Vertex> vertices;
		std::vector<Face>	faces;
};