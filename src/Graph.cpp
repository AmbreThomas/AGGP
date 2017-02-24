//============================ INCLUDES ================================
#include "Graph.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

//========================= STATIC ATTRIBUTES ==========================

igraph_real_t	Graph::LAW_EXPONENT = 2.5;

//=========================== CONSTRUCTORS =============================

Graph::Graph(void)
{
	igraph_vector_init(&dim, 2);
	VECTOR(dim)[0] = 0;
	VECTOR(dim)[1] = 0;
}


Graph::Graph(int n, int edges)
{
	//Create the dim vector as follows: ( |V|, |E| ) 
	igraph_vector_init(&dim, 2);
	VECTOR(dim)[0] = n;			//default nodes number
	VECTOR(dim)[1] = edges;		//default edges number
	
	//Create the random graph with degree law:
	igraph_static_power_law_game(&graph, n, edges, LAW_EXPONENT, -1, 1, 0, 1);
	
	//Create the weights on edges, all fixed to 1:
	igraph_vector_init(&weights, edges);
	for (int i=0; i<igraph_vector_size(&weights); i++ ){
		VECTOR(weights)[i] = 1;
	}
	igraph_add_edges(&graph, &weights, 0);
}

//=========================== DESTRUCTORS ==============================
Graph::~Graph()
{
	igraph_vector_destroy(&dim);
	igraph_vector_destroy(&weights);
	igraph_destroy(&graph);
}


//=========================== PUBLIC METHODS ===========================

void	Graph::average_path_length(void)
{
	igraph_real_t	avg_path;
	igraph_average_path_length(&graph, &avg_path, IGRAPH_UNDIRECTED, 1);
	printf("Average path length:	%f\n", (double) avg_path);
}

//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
