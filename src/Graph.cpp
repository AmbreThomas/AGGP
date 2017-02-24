//============================ INCLUDES ================================
#include "Graph.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

//========================= STATIC ATTRIBUTES ==========================

igraph_real_t	Graph::LAW_EXPONENT = 2.5;

//=========================== CONSTRUCTORS =============================

Graph::Graph(int n, int edges)
{
	//Create the random graph with degree law:
	graph = new igraph_t;
	igraph_static_power_law_game(graph, n, edges, LAW_EXPONENT, -1, 1, 0, 1);
	
	//Create the weights on edges, all fixed to 1:
	weights = new igraph_vector_t;
	igraph_vector_init(weights, edges);
	for (int i=0; i<igraph_vector_size(weights); i++ ){
		VECTOR(*weights)[i] = 1;
	}
	igraph_add_edges(graph, weights, 0);
}

//=========================== DESTRUCTORS ==============================

Graph::~Graph()
{
	//~ free(VECTOR(*weights));
	//~ free(weights);
	igraph_vector_destroy(weights);
	igraph_destroy(graph);
}


//=========================== PUBLIC METHODS ===========================

float	Graph::average_path_length(void)
{
	igraph_real_t	avg_path;
	igraph_average_path_length(graph, &avg_path, IGRAPH_UNDIRECTED, 1);
	return ((float) avg_path);
}

//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
