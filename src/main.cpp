#include "Population.h"
#include <cstring>
#include <iostream>

using namespace sf;
using namespace std;

void	printTime_str(const struct tm*);
void	printDiffTime_str(int);
void	overwatch_window(sf::RenderWindow*);

int		main(int argc, char** argv)
{
    unsigned int	iter(0);
    unsigned int	Ngraphs, Nnodes, Nedges, itermax;
	time_t			startTime, endTime;
	
    srand((unsigned int)time(NULL));
    startTime	=	time(NULL);
    
    //===================== POPULATION GENERATION ======================
    if (argc<5){
		printf("./gengraph Ngraphs Nnodes Nedges Niter\n\nusing default parameters...\n\n");
		Ngraphs	= 	100;
		Nnodes	= 	100;
		Nedges 	= 	300;
		itermax	=	10;
	} else {
		Ngraphs	= 	atoi(argv[1]);
		Nnodes	= 	atoi(argv[2]);
		Nedges 	= 	atoi(argv[3]);
		itermax	=	atoi(argv[4]);
	}
	Population*	experiment1 = new Population(Ngraphs, Nnodes, Nedges, itermax);
	printf("Creation of %d graphs completed, with %d nodes and %d edges in each.\n", Ngraphs, Nnodes, Nedges);
	printf("The nodes were built with a power law degree distribution, which power parameter is %f.\n\n", Graph::LAW_EXPONENT);
	printf("\n");
	printTime_str(localtime(&startTime));
	printf(" : Expect end of computation in %d minutes.\n\n", 
			(int)(Nedges/1000000.0*Ngraphs*itermax));
	
	//===================== MAIN LOOP ==================================
	igraph_matrix_t		coords_;
	string				asked_graph = "1";
	RenderWindow 	window(VideoMode(400, 400), "Best Biological Network");
	RenderWindow	w(VideoMode(400, 400), "Average VS Min Cost Evolution");
	RenderWindow	v(VideoMode(400, 400), "Min Cost Evolution");
	while (iter<itermax and w.isOpen() and v.isOpen() and window.isOpen())
	{
		printf("\tAsk for a graph: ");
		getline(cin, asked_graph);
		printf("\n");
		Graph* best_graph = experiment1->getgraph(atoi(asked_graph.c_str()) - 1);
		//~ Graph* best_graph = experiment1->getbestgraph();
		best_graph->compute_layout(&coords_);
		overwatch_window(&window);
		overwatch_window(&v);
		overwatch_window(&w);
		best_graph->draw(&window, &coords_);
		experiment1->study(&v, &w, iter, itermax);
		window.display();
		v.display();
		w.display();
		
		printf("\n  Iteration %d:\n", ++iter);
		experiment1->cross(); 					//population size N ==> 2N
		experiment1->select_by_tournament();	//population size 2N ==> N
		//~ experiment1->select_elite();			//population size 2N ==> N
	}
	endTime	=	time(NULL);
	printf("\nComputed in ");
	printDiffTime_str((int) difftime(endTime, startTime));
	printf(".\n");
	
	//===================== DISPLAY RESULTS ============================
	Graph* best_graph = experiment1->getbestgraph();
	best_graph->compute_layout(&coords_);
	while (window.isOpen() or (w.isOpen() and v.isOpen()))
	{
		if (window.isOpen()){
			overwatch_window(&window);
			best_graph->draw(&window, &coords_);
			window.display();
		}
		if (v.isOpen() and w.isOpen()){
			overwatch_window(&v);
			overwatch_window(&w);
			experiment1->study(&v, &w, itermax, itermax);
			v.display();
			w.display();
		}
	}
	if (v.isOpen()) v.close();
	if (w.isOpen()) w.close();
	if (window.isOpen()) window.close();
	igraph_matrix_destroy(&coords_);
	delete experiment1;
	
	return 0;
}

void	printTime_str(const struct tm *timeptr)
{
	static const char 	wday_name[][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char 	mon_name[][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	printf("%.3s%3d %.3s %d | %.2dh %.2dm %.2ds", 
			wday_name[timeptr->tm_wday],
			timeptr->tm_mday, mon_name[timeptr->tm_mon], 1900 + timeptr->tm_year,
			timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec);
}

void	printDiffTime_str(int diff)
{
	int	days	=	(int) diff/86400;
	int	hours	=	(int) diff/3600;
	int	min		=	(int) diff/60;
	int	sec		=	(int) diff%60;
	if (days) 	printf("%dd ", days);
	if (hours) 	printf("%d° ", hours);
	if (min) 	printf("%d' ", min);
	printf("%d\"", sec);
}

void	overwatch_window(sf::RenderWindow* w)
{
	Event event;
	while (w->pollEvent(event))
	{
		switch (event.type) 
		{
			case Event::Closed:
				w->close();
				break;
			case Event::Resized:
				w->setView(View(FloatRect(0,0, (float)event.size.width, (float)event.size.height)));
				break;
			default:
				break;
		}
	}
	w->clear();
}
