#include <SFML/Graphics.hpp>
#include "objects.hpp"
#include "config.hpp"
#include "render.hpp"

int main()
{
	sf::RenderWindow window( sf::VideoMode(screenSize), "3D Render" );
	window.setFramerateLimit(60);

	Environment env;
	Renderer renderer(window);
	
	int a = env.addVertex({-0.5 , -0.5 , 1});
	int b = env.addVertex({-0.5 , 0.5  , 1});
	int c = env.addVertex({0.5  , -0.5 , 1});
	int d = env.addVertex({0.5  , 0.5  , 1});

	int e = env.addVertex({-0.5 , -0.5 , 2});
	int f = env.addVertex({-0.5 , 0.5  , 2});
	int g = env.addVertex({0.5  , -0.5 , 2});
	int h = env.addVertex({0.5  , 0.5  , 2});

	env.addQuad(a, b, d, c);
	env.addQuad(e, f, h, g);
	env.addQuad(a, b, f, e);
	env.addQuad(a, c, g, e);
	env.addQuad(b, d, h, f);
	env.addQuad(c, d, h, g);

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		env.moveVertex({0, 0, 0.01});
		window.clear();
		
		renderer.render(env);

		window.display();
	}
}
