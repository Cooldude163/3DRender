#include <SFML/Graphics.hpp>

const sf::Vector2u screenSize = {960, 600};

sf::Vector2f ScreenSpaceToPix(sf::Vector2f screenCoords) {
	return sf::Vector2f{(screenCoords.x + 1) / 2 * screenSize.x, (1 - (screenCoords.y + 1) / 2) * screenSize.y};
}

sf::Vector2f pixToScreenSpace(sf::Vector2f pixelCoords) {
	return sf::Vector2f{(pixelCoords.x / screenSize.x) * 2 - 1, ((pixelCoords.y / screenSize.y) * (-1) - 1) * 2 - 1};
}

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
		
		void moveVertex(int index, sf::Vector3f change){
			vertices[index].location += change;
		}
	private:
		std::vector<Vertex> vertices;
};

class Renderer
{
	public:
		Renderer(sf::RenderWindow& window)
			: m_target{window}
			{}
		void render(Environment& env)
		{
			
			sf::CircleShape circle(1.0f);
			circle.setOrigin({1.0f, 1.0f});
			circle.setPointCount(8);
			auto& vertices = env.getVertices();
			for (auto& vert : vertices) {
				circle.setPosition(ScreenSpaceToPix(vert.project()));
				circle.setScale({5, 5});
				m_target.draw(circle);
			}
		}
		
		private:
		sf::RenderWindow& m_target;
};


int main()
{
	sf::RenderWindow window( sf::VideoMode(screenSize), "3D Render" );
	window.setFramerateLimit(60);
	
	Environment env;
	Renderer renderer(window);
	

	env.addVertex({0, 0.5, 1});

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		env.moveVertex(0, {0, 0, 0.01});
		window.clear();
		
		renderer.render(env);

		window.display();
	}
}
