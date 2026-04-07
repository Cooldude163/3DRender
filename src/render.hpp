#pragma once
#include <SFML/Window.hpp>
#include "objects.hpp"
#include "config.hpp"


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
		void render(Environment& env)
		{
			auto& vertices = env.getVertices();
			auto& faces	   = env.getFaces();

			//Rendering Lines
			sf::VertexArray triangle(sf::PrimitiveType::LineStrip, 4);
			for (int i = 0; i < 4; i++) {triangle[i].color = sf::Color::Green;}

			for (auto& face : faces){
				triangle[0].position = ScreenSpaceToPix(vertices[face.v1].project());
				triangle[1].position = ScreenSpaceToPix(vertices[face.v2].project());
				triangle[2].position = ScreenSpaceToPix(vertices[face.v3].project());
				triangle[3].position = triangle[0].position;

				m_target.draw(triangle);
			}
		}
		
		private:
		sf::RenderWindow& m_target;
};