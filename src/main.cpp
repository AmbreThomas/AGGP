#include "Population.h"
#include <cstdlib>
#include <SFML/Graphics.hpp>

int	main(void)
{
	float	avg_path_len;

	//100 graphes of 30 nodes and 100 edges:
	Population	experiment1 = Population(100, 100, 300);
	printf("Creation terminée.\n\n");
	for( int i = 0; i<experiment1.size(); i++ ){
		avg_path_len = experiment1.getgraph(i).average_path_length();
		printf("Average path length of graph %d : %f\n", i+1, avg_path_len);
	}
	printf("ça, c'est fait...\n\n");
	
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

	return (0);
}

