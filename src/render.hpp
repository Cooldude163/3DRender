#pragma once
#include <SFML/Window.hpp>
#include "objects.hpp"
#include "config.hpp"
#include "utils/math.hpp"


sf::Vector2f ScreenSpaceToPix(sf::Vector2f screenCoords) {
	return sf::Vector2f{(screenCoords.x + 1) / 2 * screenSize.x, (1 - (screenCoords.y + 1) / 2) * screenSize.y};
}

sf::Vector2f pixToScreenSpace(sf::Vector2f pixelCoords) {
	return sf::Vector2f{(pixelCoords.x / screenSize.x) * 2 - 1, ((pixelCoords.y / screenSize.y) * (-1) - 1) * 2 - 1};
}

class Renderer
{
    public:
		Renderer(sf::RenderWindow& window)
			: m_target{window}
			{}
		void render(Object& obj)
		{
			auto& vertices = obj.getVertices();
			auto& faces	   = obj.getFaces();

			//Rendering faces
			sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);
            sf::VertexArray outline(sf::PrimitiveType::LineStrip, 4);
			for (int i = 0; i < faces.size(); i++){
                auto& face = faces[i];
                if ( vertices[face.v1].location.z > 0 || vertices[face.v2].location.z > 0 || vertices[face.v3].location.z > 0) {
                    triangle[0].position = ScreenSpaceToPix(vertices[face.v1].project());
                    triangle[1].position = ScreenSpaceToPix(vertices[face.v2].project());
                    triangle[2].position = ScreenSpaceToPix(vertices[face.v3].project());
                    if ( Math::dot(obj.calcNormal(i), {0, 0, 1}) > 0) {continue;} //Facing away from Camera
                    else {
                        for (int i = 0; i < 3; i++) {triangle[i].color = sf::Color::Red;}
                    }
                    m_target.draw(triangle);
                }
			}

            for (int i = 0; i < 4; i++) {outline[i].color = sf::Color::Black;};

			for (auto& face : faces){
				outline[0].position = ScreenSpaceToPix(vertices[face.v1].project());
				outline[1].position = ScreenSpaceToPix(vertices[face.v2].project());
				outline[2].position = ScreenSpaceToPix(vertices[face.v3].project());
				outline[3].position = outline[0].position;

				m_target.draw(outline);
		    }
        }
    private:
		sf::RenderWindow& m_target;
};