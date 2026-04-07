#include <SFML/Graphics.hpp>
#include "objects.hpp"
#include "config.hpp"
#include "render.hpp"

int main()
{
	sf::RenderWindow window( sf::VideoMode(screenSize), "3D Render" );
	window.setFramerateLimit(60);


	pyramid pyr({0, 0.5, 1}, {1, 1, 1});

	rectPrism cube({-0.5, 0.5, 1}, {1, 1, 1});
	Renderer renderer(window);
	
	cube.move({1, 0, 3});
	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
			else if ( const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {window.close();}
            }
		}


		cube.rotateXZ(6.28 / (60 * 5));

		window.clear();
		
		renderer.render(cube);

		window.display();
	}
}
