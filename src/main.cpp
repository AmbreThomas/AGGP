#include "Population.h"
#include <cstdlib>

int	main(void)
{
	float	avg_path_len;

	//100 graphes of 30 nodes and 100 edges:
	Population	experiment1 = Population(100, 30, 100);
	printf("Creation terminée.\n\n");
	for( int i = 0; i<experiment1.size(); i++ ){
		avg_path_len = experiment1.getgraph(i).average_path_length();
		printf("Average path length of graph %d : %f\n", i+1, avg_path_len);
	}
	printf("ça, c'est fait...\n\n");
	return (0);
}

