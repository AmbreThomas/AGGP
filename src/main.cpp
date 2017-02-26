#include "Population.h"

using namespace sf;

int	main(void)
{
    unsigned int	iter(0);
    unsigned int	Ngraphs, Nnodes, Nedges, itermax;
    
    //===================== PARAMETERS DEFININTION =====================
    itermax	=	1;
    
    //===================== POPULATION GENERATION ======================
	Ngraphs	= 	10;
	Nnodes	= 	100;
	Nedges 	= 	300;
	Population	experiment1 = Population(Ngraphs, Nnodes, Nedges);
	printf("Creation of %d graphs completed, with %d nodes and %d edges in each.\n", Ngraphs, Nnodes, Nedges);
	printf("The nodes were built with a power law degree distribution, which power parameter is %f.\n\n", Graph::LAW_EXPONENT);
	
	//===================== MAIN LOOP ==================================
	while (iter<itermax)
	{
		experiment1.cross(); 				//population size N ==> 2N
		experiment1.mutate_children();		//population size 2N
		experiment1.select_by_tournament();	//population size 2N ==> N
		//~ experiment1.select_elite();			//population size 2N ==> N
		iter++;
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
				case Event::Resized:
					window.setView(View(FloatRect(0,0, (float)event.size.width, (float)event.size.height)));
					break;
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

