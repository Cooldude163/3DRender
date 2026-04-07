#pragma once
#include <SFML/System.hpp>
#include <cmath>
#include "utils/math.hpp"

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

class Object
{
	public:
		Object() = default;
		
		int addVertex(sf::Vector3f location) 
		{
			vertices.emplace_back(location);
			updateCenter();
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

		void move(sf::Vector3f change) //Changes all Vertexes
		{
			for (auto& vert : vertices) {
				vert.location += change;
			}
			center += change;
		}
	
		void rotateXZ(float radians)
		{
			float c = cos(radians);
			float s = sin(radians);
			for (auto& vert : vertices) {
				float x = vert.location.x - center.x;
				float z = vert.location.z - center.z;
				vert.location.x = x * c - z * s + center.x;
				vert.location.z = x * s + z * c + center.z;
			}
		}
		void rotateXY(float radians)
		{
			float c = cos(radians);
			float s = sin(radians);
			for (auto& vert : vertices) {
				float x = vert.location.x - center.x;
				float y = vert.location.y - center.y;
				vert.location.x = x * c - y * s + center.x;
				vert.location.y = x * s + y * c + center.y;
			}
		}
		void rotateYZ(float radians)
		{
			float c = cos(radians);
			float s = sin(radians);
			for (auto& vert : vertices) {
				float y = vert.location.y - center.y;
				float z = vert.location.z - center.z;
				vert.location.y = y * c - z * s + center.y;
				vert.location.z = y * s + z * c + center.z;
			}
		}

		sf::Vector3f calcNormal(int index)
		{
			Face& face     = faces[index];
			sf::Vector3f a = vertices[face.v2].location - vertices[face.v1].location;
			sf::Vector3f b = vertices[face.v3].location - vertices[face.v1].location;

			sf::Vector3f unNormed = sf::Vector3f{((a.y * b.z) - (a.z * b.y)),((a.x * b.z) - (a.z * b.x)),((a.x * b.y) - (a.y * b.x))};
			float	     length   = Math::hyp(unNormed);
			return unNormed / length;
		}

	protected:
		std::vector<Vertex> vertices;
		std::vector<Face>	faces;
		sf::Vector3f		center;

		void updateCenter()
		{
			float x = 0;
			float y = 0;
			float z = 0;
			for (auto& vert : vertices) {
				x += vert.location.x;
				y += vert.location.y;
				z += vert.location.z;
			}
			int size = vertices.size();
			center = {x / size, y / size, z / size};
		}


};

class rectPrism: public Object
{
public:
	rectPrism(sf::Vector3f topCorner, sf::Vector3f spacing)
	{

		int a = addVertex({topCorner.x 				, topCorner.y 			   , topCorner.z});
		int b = addVertex({topCorner.x 				, topCorner.y - spacing.y  , topCorner.z});
		int c = addVertex({topCorner.x + spacing.x  , topCorner.y - spacing.y  , topCorner.z});
		int d = addVertex({topCorner.x + spacing.x  , topCorner.y			   , topCorner.z});

		int e = addVertex({topCorner.x 				, topCorner.y 			   , topCorner.z + spacing.z});
		int f = addVertex({topCorner.x 				, topCorner.y - spacing.y  , topCorner.z + spacing.z});
		int g = addVertex({topCorner.x + spacing.x  , topCorner.y - spacing.y  , topCorner.z + spacing.z});
		int h = addVertex({topCorner.x + spacing.x  , topCorner.y			   , topCorner.z + spacing.z});

		addQuad(a, d, c, b);  // front  (looking toward -z)
		addQuad(h, e, f, g);  // back   (looking toward +z)

		addQuad(d, h, g, c);  // right  (looking toward +x)
		addQuad(e, a, b, f);  // left   (looking toward -x)

		addQuad(a, e, h, d);  // top    (looking toward +y)
		addQuad(b, c, g, f);  // bottom (looking toward -y)
		}
};

class pyramid: public Object
{
public:
	pyramid(sf::Vector3f frontCorner, sf::Vector3f dimensions)
	{
		int a = addVertex({frontCorner.x 				, frontCorner.y , frontCorner.z});
		int b = addVertex({frontCorner.x + dimensions.x , frontCorner.y , frontCorner.z});
		int c = addVertex({frontCorner.x + dimensions.x , frontCorner.y , frontCorner.z + dimensions.z});
		int d = addVertex({frontCorner.x				, frontCorner.y , frontCorner.z + dimensions.z});

		int e = addVertex({frontCorner.x + dimensions.x / 2 , frontCorner.y + dimensions.y , frontCorner.z + dimensions.z / 2});

		addQuad(a, b, c, d);

		addFace(a, d, e);
		addFace(a, b, e);
		addFace(b, c, e);
		addFace(c, d, e);
	}
};