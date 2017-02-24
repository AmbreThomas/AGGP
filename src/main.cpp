#include <Graph.h>

int	main(void)
{
	Graph	graphe1 = Graph(30, 100); //30 nodes, 100 edges
	graphe1.average_path_length();
	
	Graph	graphe2 = Graph(300, 1000);
	graphe2.average_path_length();

	return(0);
}

