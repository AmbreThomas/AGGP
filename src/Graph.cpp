//============================ INCLUDES ================================
#include "Graph.h"

using namespace std;

//========================= STATIC ATTRIBUTES ==========================

igraph_real_t	Graph::LAW_EXPONENT = 2.5;

//=========================== CONSTRUCTORS =============================

Graph::Graph(int n, int edges)
{
	//Create the random graph with degree law:
	graph_ 	= 	new igraph_t;
	Nnodes_	= 	n;
	//~ igraph_static_power_law_game(graph_, n, edges, LAW_EXPONENT, -1, 0, 0, 1);
	//~ igraph_erdos_renyi_game(graph_, IGRAPH_ERDOS_RENYI_GNM, n, edges, 0, 0);
	igraph_barabasi_game(graph_, n, /*power*/ 1.0/(LAW_EXPONENT-1), /*m*/ 1, 
						 /*outseq*/ 0, /*outpref*/ 0, /*A*/ 1, 
						 /*directed*/ 0, IGRAPH_BARABASI_PSUMTREE, 0);
	
	//Create the weights on edges, all fixed to 1:
	weights_ = new igraph_vector_t;
	igraph_vector_init(weights_, edges);
	for (int i=0; i<igraph_vector_size(weights_); i++ ){
		VECTOR(*weights_)[i] = 1;
	}
	igraph_add_edges(graph_, weights_, 0);
}

//=========================== DESTRUCTORS ==============================

Graph::~Graph()
{
	igraph_vector_destroy(weights_);
	igraph_destroy(graph_);
}


//=========================== PUBLIC METHODS ===========================

float	Graph::average_path_length(void)
{
	igraph_real_t		avg_path;
	igraph_average_path_length(graph_, &avg_path, IGRAPH_UNDIRECTED, 1);
	return 				((float) avg_path);
}

double 	Graph::cost(void)
{
	double				nk, cost1, cost2, cost3;
	igraph_integer_t 	d, kmax;
	igraph_vector_t 	degrees, n_of_degree, clust_coeffs;
	
	//Evaluate distance to desired clustering coefficient:
	igraph_vector_init(&clust_coeffs,Nnodes_);
	igraph_vector_init(&degrees,0);
	igraph_transitivity_local_undirected(graph_, &clust_coeffs, igraph_vss_all(), IGRAPH_TRANSITIVITY_ZERO);
	//J'AI DES DOUTES SUR LE IGRAPH_TRANSITIVITY_ZERO...
    igraph_degree(graph_, &degrees, igraph_vss_all(), IGRAPH_ALL, 0);
	cost1 = 0;
	for (size_t i = 0; i < Nnodes_; i++){
		cost1 	+=	(double) abs(VECTOR(clust_coeffs)[i] - 1.0/VECTOR(degrees)[i]);
	}
	cost1 = cost1/((double) Nnodes_);
	
	//Evaluate distance to desired degree distribution:
	kmax = (igraph_integer_t) igraph_vector_max(&degrees);
	igraph_vector_init(&n_of_degree, kmax+1);
	for (size_t i = 0; i < Nnodes_; i++){
		VECTOR(n_of_degree)[(int)VECTOR(degrees)[i]]++;
	}
	cost2 = 0;
	for (int k 	= 1; k<kmax+1; k++)
	{
		nk 		= 	(double) VECTOR(n_of_degree)[k];
		cost2 	= 	cost2 + nk * abs(nk/((double)Nnodes_) - pow(k, -1 * LAW_EXPONENT));
	}
	cost2 	=	cost2/((double)Nnodes_);
	
	//Evaluate distance to desired diameter:
	igraph_diameter(graph_, &d, 0, 0, 0, IGRAPH_UNDIRECTED, 0);
	cost3 	= 	((double) abs(d - log((double)Nnodes_)))/(log((double)Nnodes_));
	
	double	cost(sqrt(cost1*cost1 + cost2*cost2 + cost3*cost3));
	printf("cost = ||%f, %f, %f|| = %f\n",cost1, cost2, cost3, cost);
	
	
	igraph_vector_destroy(&degrees);
	igraph_vector_destroy(&n_of_degree);
	igraph_vector_destroy(&clust_coeffs);
	return 				(cost);
}

//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
void	print_vector(igraph_vector_t *v)
{
	long int	i;
	printf("\n vector:\n    ");
	for (i = 0; i<igraph_vector_size(v); i++){
		printf(" %li", (long int) VECTOR(*v)[i]);
	}
	printf("\n");
}
