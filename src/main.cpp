#include "Population.h"

using namespace sf;

void	printTime_str(const struct tm*);
void	printDiffTime_str(int);

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
	Population	experiment1 = Population(Ngraphs, Nnodes, Nedges);
	printf("Creation of %d graphs completed, with %d nodes and %d edges in each.\n", Ngraphs, Nnodes, Nedges);
	printf("The nodes were built with a power law degree distribution, which power parameter is %f.\n\n", Graph::LAW_EXPONENT);
	printf("\n");
	printTime_str(localtime(&startTime));
	printf(" : Expect end of computation in %d minutes.\n\n", 
			(int)(Nedges/1000000.0*Ngraphs*itermax));
	
	//===================== MAIN LOOP ==================================
	while (iter<itermax)
	{
		printf("  Iteration %d: pop of size %d\n",
				++iter, (int)experiment1.getCurrentSize());
		experiment1.cross(); 				//population size N ==> 2N
		experiment1.select_by_tournament();	//population size 2N ==> N
		//~ experiment1.select_elite();			//population size 2N ==> N
	}
	
	endTime	=	time(NULL);
	printf("\nComputed in ");
	printDiffTime_str((int) difftime(endTime, startTime));
	printf(", displaying best solution...\n");
	
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
	int	sec		=	(int) (diff - diff/60);
	if (days) 	printf("%dd ", days);
	if (hours) 	printf("%d° ", hours);
	if (min) 	printf("%d' ", min);
	printf("%d\"", sec);
}
