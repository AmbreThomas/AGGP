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
	pmut_	=	0.3;
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
	igraph_simplify(graph_, 1, 1, 0);
	//initialize layout matrix
	igraph_matrix_init(&coords_,Nnodes_,2);
}

Graph::Graph(Graph* parent1, Graph* parent2, unsigned int crosspt)
{
	graph_ 	= 	new igraph_t;
	Nnodes_	= 	parent1->Nnodes_;
	pmut_	=	0.3;
	
	igraph_t	temp1;
	igraph_t	temp2;
	igraph_copy(&temp1, parent1->graph_);
	igraph_copy(&temp2, parent2->graph_);
	
	igraph_vit_t	Vset;
	igraph_vit_create(&temp2, igraph_vss_seq(crosspt, Nnodes_-1), &Vset);
	while (!IGRAPH_VIT_END(Vset))
	{
		igraph_es_t			edges;
		igraph_es_incident(&edges, IGRAPH_VIT_GET(Vset), IGRAPH_ALL);
		igraph_delete_edges(&temp1, edges);
		igraph_es_destroy(&edges);
		IGRAPH_VIT_NEXT(Vset);
	}
	igraph_vit_destroy(&Vset);
	igraph_vit_create(&temp2, igraph_vss_seq(0, crosspt-1), &Vset);
	while (!IGRAPH_VIT_END(Vset))
	{
		igraph_vector_t		to_delete;
		igraph_es_t			edges;
		igraph_eit_t		iterator;
		igraph_integer_t	a, from, to;
		
		igraph_vector_init(&to_delete, 0);
		igraph_es_incident(&edges, IGRAPH_VIT_GET(Vset), IGRAPH_ALL);
		igraph_eit_create(&temp2, edges, &iterator);
		while(!IGRAPH_EIT_END(iterator))
		{
			a = IGRAPH_EIT_GET(iterator);
			igraph_edge(&temp2, a, &from, &to);
			if ((int)from<(int)crosspt and (int)to<(int)crosspt){
				igraph_vector_push_back(&to_delete, (igraph_real_t) a);
			}
			IGRAPH_EIT_NEXT(iterator);
		}
		IGRAPH_VIT_NEXT(Vset);
		igraph_es_destroy(&edges);
		igraph_eit_destroy(&iterator);
		//~ igraph_vector_destroy(&to_delete);
		igraph_es_t	selector;
		igraph_es_vector(&selector, &to_delete);
		igraph_delete_edges(&temp2, selector);
		igraph_es_destroy(&selector);
	}
	igraph_vit_destroy(&Vset);
	
	igraph_union(graph_, &temp1, &temp2, 0, 0);
	igraph_matrix_init(&coords_,Nnodes_,2);
	
	igraph_destroy(&temp1);
	igraph_destroy(&temp2);
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
	double				nk, cost1, cost2, cost3, cost;
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
	
	cost	=	sqrt(cost1*cost1 + cost2*cost2 + cost3*cost3);
	//~ printf("cost = ||%f, %f, %f|| = %f\n",cost1, cost2, cost3, cost);
	
	
	igraph_vector_destroy(&degrees);
	igraph_vector_destroy(&n_of_degree);
	igraph_vector_destroy(&clust_coeffs);
	return 				(cost);
}

void	Graph::compute_layout(void)
{
	igraph_layout_lgl(graph_,&coords_,150,(igraph_real_t)Nnodes_,pow(Nnodes_,2),1.5,pow(Nnodes_,3),sqrt(Nnodes_),0);
}

void	Graph::draw(sf::RenderWindow* w)
{
	igraph_vector_t pos_x;
	igraph_vector_init(&pos_x,Nnodes_);
	igraph_matrix_get_col(&coords_,&pos_x,0);
	igraph_real_t min_x=igraph_vector_min(&pos_x);
	igraph_real_t max_x=igraph_vector_max(&pos_x);
	igraph_vector_t pos_y;
	igraph_vector_init(&pos_y,Nnodes_);
	igraph_matrix_get_col(&coords_,&pos_y,1);
	igraph_real_t min_y=igraph_vector_min(&pos_y);
	igraph_real_t max_y=igraph_vector_max(&pos_y);
	sf::Vector2f w_size = w->getView().getSize();
	float x_factor=(float)w_size.x/(float)1.1/(float)(max_x-min_x);
	float x_base=(float)w_size.x*(float)(0.05/1.1);
	float y_factor=(float)w_size.y/(float)1.1/(float)(max_y-min_y);
	float y_base=(float)w_size.y*(float)(0.05/1.1);
	sf::CircleShape node_shape;
	node_shape.setRadius(min(x_factor,y_factor)/4);
	node_shape.setFillColor(sf::Color::Green);
	float x;
	float y;
	for (size_t n=0; n<Nnodes_; n++)
	{
		x=x_base+(float)(igraph_vector_e(&pos_x,n)-min_x)*x_factor;
		y=y_base+(float)(igraph_vector_e(&pos_y,n)-min_y)*y_factor;
		node_shape.setPosition(x,y);
		w->draw(node_shape);
	}
}

void	Graph::mutate(void)
{
	/* On a une probabilité de changer le point d'accroche d'un edge. */

	igraph_integer_t	edgeid, from, to, newattach;
	bool				side;

	if (rand()/(double)RAND_MAX < pmut_){
		edgeid		=	rand()%(unsigned int)igraph_ecount(graph_);
		side		=	rand()%2;
		newattach	=	rand()%(unsigned int)Nnodes_;
		igraph_edge(graph_, edgeid, &from, &to);
		igraph_delete_edges(graph_, igraph_ess_1(edgeid));
		if (side) {
			igraph_add_edge(graph_, from, newattach);
		} else {
			igraph_add_edge(graph_, newattach, to);
		}
	}
}

size_t	Graph::getN(void) { return (Nnodes_); }

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
