#include "Population.h"
#include <cstdlib>
#include <SFML/Graphics.hpp>

using namespace sf;

int	main(void)
{
	RenderWindow	window( VideoMode(500, 500), "Resulting biological network");
    CircleShape 	circle(10.f);
    unsigned int	iter(0);
    unsigned int	Ngraphs, Nnodes, Nedges;
    
    
    //===================== POPULATION GENERATION ======================
	Ngraphs = 100;
	Nnodes = 100;
	Nedges = 300;
	Population	experiment1 = Population(Ngraphs, Nnodes, Nedges);
	printf("Creation of %d graphs completed, with %d nodes and %d edges in each.\n", Ngraphs, Nnodes, Nedges);
	printf("The nodes were built with a power law degree distribution, which power parameter is %f.\n", Graph::LAW_EXPONENT);
	
	//===================== MAIN LOOP ==================================
	while (iter<1)
	{
		experiment1.cross(); 				//population size N ==> 2N
		experiment1.mutate_children();		//population size 2N
		experiment1.select_by_tournament();	//population size 2N ==> N
		//~ experiment1.select_elite();			//population size 2N ==> N
		iter++;
	}
	

	//===================== DISPLAY RESULTS ============================
	RenderWindow window(VideoMode(200, 200), "SFML works!");
	CircleShape shape(100.f);
	shape.setFillColor(Color::Green);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}

