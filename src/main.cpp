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
	bool			display;
	//~ Font			font;
	
	//~ font.loadFromFile("src/font.TTF");
    srand((unsigned int)time(NULL));
    startTime	=	time(NULL);
    
    //===================== POPULATION GENERATION ======================
    if (argc<6){
		printf("./gengraph Ngraphs Nnodes Nedges Niter display?\nusing default parameters...\n\n");
		Ngraphs	= 	100;
		Nnodes	= 	100;
		Nedges 	= 	300;
		itermax	=	200;
		display	=	true;
	} else {
		Ngraphs	= 	atoi(argv[1]);
		Nnodes	= 	atoi(argv[2]);
		Nedges 	= 	atoi(argv[3]);
		itermax	=	atoi(argv[4]);
		display	=	atoi(argv[5]);
	}
	Population*	experiment1 = new Population(Ngraphs, Nnodes, Nedges, itermax);
	printf("Creation of %d graphs completed, with %d nodes and %d edges in each.\n", Ngraphs, Nnodes, Nedges);
	printTime_str(localtime(&startTime));
	printf(" \n");
	
	//===================== MAIN LOOP ==================================
	igraph_matrix_t		coords_;
	string				asked_graph = "1";
	RenderWindow 		window(VideoMode(800, 800), "Best Biological Network");
	RenderWindow		w(VideoMode(400, 400), "Average VS Min Cost Evolution");
	RenderWindow		v(VideoMode(400, 400), "Min Cost Evolution");
	while ((iter<itermax and experiment1->stillEvolves(iter)) and w.isOpen() and v.isOpen() and window.isOpen())
	{
		overwatch_window(&window);
		overwatch_window(&v);
		overwatch_window(&w);
		if (display)
		{
			Graph* best_graph = experiment1->getbestgraph();
			best_graph->compute_layout(&coords_);
			best_graph->draw(&window, &coords_);
		}
		experiment1->study(&v, &w, &window, iter, itermax);
		window.display();
		v.display();
		w.display();
		
		++iter;
		experiment1->cross(); 					//population size N ==> 2N
		experiment1->select_by_tournament();	//population size 2N ==> N
	}
	endTime	=	time(NULL);
	printf(" displaying best graph. Computed in ");
	printDiffTime_str((int) difftime(endTime, startTime));
	printf(".\n\n");
	cout << "Graph diameter:\t" << experiment1->getbestgraph()->results() << endl;
	cout << "Total time:\t" << (int) difftime(endTime, startTime) << endl;
	
	//===================== DISPLAY RESULTS ============================
	Graph* best_graph = experiment1->getbestgraph();
	best_graph->compute_layout(&coords_);
	while (window.isOpen() and w.isOpen() and v.isOpen())
	{
		overwatch_window(&v);
		overwatch_window(&w);
		overwatch_window(&window);
		best_graph->draw(&window, &coords_);
		experiment1->study(&v, &w, &window, iter, itermax);
		window.display();
		v.display();
		w.display();
	}
	if (v.isOpen()) v.close();
	if (w.isOpen()) w.close();
	if (window.isOpen()) window.close();
	igraph_matrix_destroy(&coords_);
	delete experiment1;
	
	return (int) difftime(endTime, startTime);
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
	int	hours	=	(int) (diff-days*86400)/3600;
	int	min		=	(int) (diff-days*86400-hours*3600)/60;
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
