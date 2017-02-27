#include "Population.h"

using namespace sf;

int	main(void)
{
    unsigned int	iter(0);
    unsigned int	Ngraphs, Nnodes, Nedges, itermax;
    
    srand((unsigned int)time(NULL));
    
    //===================== PARAMETERS DEFININTION =====================
    itermax	=	10;
    
    //===================== POPULATION GENERATION ======================
	Ngraphs	= 	100;
	Nnodes	= 	100;
	Nedges 	= 	300;
	Population	experiment1 = Population(Ngraphs, Nnodes, Nedges);
	printf("Creation of %d graphs completed, with %d nodes and %d edges in each.\n", Ngraphs, Nnodes, Nedges);
	printf("The nodes were built with a power law degree distribution, which power parameter is %f.\n\n", Graph::LAW_EXPONENT);
	
	//===================== MAIN LOOP ==================================
	while (iter<itermax)
	{
		printf("  Iteration %d: pop of size %d\n", iter++, (int)experiment1.getCurrentSize());
		experiment1.cross(); 				//population size N ==> 2N
		experiment1.select_by_tournament();	//population size 2N ==> N
		//~ experiment1.select_elite();			//population size 2N ==> N
	}

	//===================== DISPLAY RESULTS ============================
	Graph best_graph = experiment1.getgraph(0);
	best_graph.compute_layout();
	RenderWindow window(VideoMode(400, 400), "Best Biological Network");
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) 
			{
				case Event::Closed:
					window.close();
					break;
				//~ case Event::Resized:
					//~ window.setView(View(FloatRect(0,0, (float)event.size.width, (float)event.size.height)));
					//~ break;
				default:
					break;
			}
		}
		window.clear();
		best_graph.draw(&window);
		window.display();
	}

	return 0;
}

