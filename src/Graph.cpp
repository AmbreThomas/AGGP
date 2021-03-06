//============================ INCLUDES ================================
#include "Graph.h"

using namespace std;

//========================= STATIC ATTRIBUTES ==========================

igraph_real_t	Graph::LAW_EXPONENT = 2.5;

//=========================== CONSTRUCTORS =============================

Graph::Graph(int n, int edges)
{
	graph_	=	new igraph_t;
	Nnodes_	=	n + edges - edges;
	psub_	=	0.1;
	pins_	=	0.1;
	pdel_	=	0.1;
	//igraph_static_power_law_game(graph_, n, edges, LAW_EXPONENT, -1, 0, 0, 1);
	//~ igraph_erdos_renyi_game(graph_, IGRAPH_ERDOS_RENYI_GNM, n, edges, 0, 0);
	igraph_barabasi_game(graph_, n, /*power*/ 1.0/(LAW_EXPONENT-1), /*m*/ 1, /*outseq*/ 0, /*outpref*/ 0, /*A*/ 1, /*directed*/ 0, IGRAPH_BARABASI_PSUMTREE, 0);
	igraph_simplify(graph_, 1, 1, 0);
	cost_	=	this->cost();
}

Graph::Graph(Graph* parent1, Graph* parent2, unsigned int crosspt)
{
	igraph_vit_t		Vset;
	igraph_vector_t		to_delete;
	graph_ 	=			new igraph_t;
	Nnodes_	=			parent1->Nnodes_;
	psub_	=	0.3;
	pins_	=	0.3;
	pdel_	=	0.3;

	igraph_t			temp1;
	igraph_t			temp2;
	igraph_copy(&temp1, parent1->graph_);
	igraph_copy(&temp2, parent2->graph_);
	
	igraph_vit_create(&temp1, igraph_vss_seq(crosspt, Nnodes_-1), &Vset);
	while (!IGRAPH_VIT_END(Vset))
	{
		igraph_es_t		edges;
		igraph_es_incident(&edges, IGRAPH_VIT_GET(Vset), IGRAPH_ALL);
		igraph_delete_edges(&temp1, edges);
		igraph_es_destroy(&edges);
		IGRAPH_VIT_NEXT(Vset);
	}
	igraph_vit_destroy(&Vset);
	igraph_vit_create(&temp2, igraph_vss_seq(0, crosspt-1), &Vset);
	igraph_vector_init(&to_delete, 0);
	while (!IGRAPH_VIT_END(Vset))
	{
		igraph_es_t		edges;
		igraph_eit_t	iterator;
		igraph_integer_t a, from, to;
		
		igraph_es_incident(&edges, IGRAPH_VIT_GET(Vset), IGRAPH_ALL);
		igraph_eit_create(&temp2, edges, &iterator);
		while(!IGRAPH_EIT_END(iterator))
		{
			a = IGRAPH_EIT_GET(iterator);
			igraph_edge(&temp2, a, &from, &to);
			if ((int)from < (int)crosspt and (int)to < (int)crosspt){
				igraph_vector_push_back(&to_delete, (igraph_real_t)a);
			}
			IGRAPH_EIT_NEXT(iterator);
		}
		IGRAPH_VIT_NEXT(Vset);
		igraph_es_destroy(&edges);
		igraph_eit_destroy(&iterator);
	}
	igraph_es_t			selector;
	igraph_es_vector(&selector, &to_delete);
	igraph_delete_edges(&temp2, selector);
	igraph_es_destroy(&selector);
	
	igraph_union(graph_, &temp1, &temp2, 0, 0);
	igraph_simplify(graph_, 1, 1, 0);
	cost_	=	this->cost();
	
	
	igraph_destroy(&temp1);
	igraph_destroy(&temp2);
	igraph_vit_destroy(&Vset);
	igraph_vector_destroy(&to_delete);
}


//=========================== DESTRUCTORS ==============================

Graph::~Graph()
{
	igraph_destroy(graph_);
	delete graph_;
}


//=========================== PUBLIC METHODS ===========================

float	Graph::average_path_length(void)
{
	igraph_real_t		avg_path;
	igraph_average_path_length(graph_, &avg_path, IGRAPH_UNDIRECTED, 1);
	return 				((float)avg_path);
}

double 	Graph::cost(void)
{
	double				nk, cost1, cost2, cost3, cost;
	igraph_integer_t 	d, kmax;
	igraph_vector_t 	degrees, n_of_degree, clust_coeffs;
	
	//Evaluate distance to desired clustering coefficient:
	igraph_vector_init(&clust_coeffs, Nnodes_);
	igraph_vector_init(&degrees,0);
	igraph_transitivity_local_undirected(graph_, &clust_coeffs, igraph_vss_all(), IGRAPH_TRANSITIVITY_ZERO);
    igraph_degree(graph_, &degrees, igraph_vss_all(), IGRAPH_ALL, 0);
	cost1 = 0;
	for (size_t i = 0; i < Nnodes_; i++){
		if (VECTOR(degrees)[i])
			cost1 += (double)abs(VECTOR(clust_coeffs)[i] - 1.0 / VECTOR(degrees)[i]);
	}
	cost1 = cost1 / ((double)Nnodes_);
	
	//Evaluate distance to desired degree distribution:
	kmax = (igraph_integer_t)igraph_vector_max(&degrees);
	igraph_vector_init(&n_of_degree, kmax+1);
	for (size_t i = 0; i < Nnodes_; i++){
		VECTOR(n_of_degree)[(int)VECTOR(degrees)[i]]++;
	}
	cost2 = 0;
	for (int k = 1; k < kmax + 1; k++)
	{
		nk = (double)VECTOR(n_of_degree)[k];
		cost2 = cost2 + nk * abs(nk / ((double)Nnodes_) - pow(k, -1 * LAW_EXPONENT));
	}
	cost2 = cost2 / ((double)Nnodes_);
	
	//Evaluate distance to desired diameter:
	igraph_diameter(graph_, &d, 0, 0, 0, IGRAPH_UNDIRECTED, 0);
	cost3 = ((double)abs(d - log((double)Nnodes_))) / (log((double)Nnodes_));
	
	cost = sqrt(cost1 * cost1 + cost2 * cost2 + cost3 * cost3);
	//~ printf("cost = ||%f, %f, %f|| = %f\n",cost1, cost2, cost3, cost);
	
	
	igraph_vector_destroy(&degrees);
	igraph_vector_destroy(&n_of_degree);
	igraph_vector_destroy(&clust_coeffs);
	return (cost);
}

size_t	Graph::results(void)
{
	// degrees
	igraph_vector_t degree;
	igraph_vector_init(&degree, Nnodes_);
	igraph_degree(graph_, &degree, igraph_vss_all(), IGRAPH_ALL, false);
	// transitivity
	igraph_vector_t transitivity;
	igraph_vector_init(&transitivity, Nnodes_);
	igraph_transitivity_local_undirected(graph_, &transitivity, igraph_vss_all(), IGRAPH_TRANSITIVITY_ZERO);
	// file
	fstream fs("results.txt", fstream::trunc | fstream::out);
	if (fs.is_open())
	{
		fs << "degree\ttransitivity" << endl;
		for (size_t n= Nnodes_; n--;)
		{
			fs << (unsigned int)igraph_vector_e(&degree, n) << "\t" << (float)igraph_vector_e(&transitivity, n) << endl;
		}
		fs.close();
	}
	igraph_vector_destroy(&degree);
	igraph_vector_destroy(&transitivity);
	// diameter
	igraph_integer_t diameter;
	igraph_diameter(graph_, &diameter, 0, 0, 0, IGRAPH_UNDIRECTED, 0);
	return (size_t)diameter;
}

void	Graph::compute_layout(igraph_matrix_t* coords_)
{
	igraph_matrix_init(coords_, Nnodes_, 2);
	igraph_layout_lgl(graph_, coords_, 150, (igraph_real_t)Nnodes_, pow(Nnodes_, 2), 1.5, pow(Nnodes_,3), sqrt(Nnodes_), 0);
}

void	Graph::draw(sf::RenderWindow* w, igraph_matrix_t* coords_)
{
	igraph_vector_t pos_x;
	igraph_vector_init(&pos_x, Nnodes_);
	igraph_matrix_get_col(coords_, &pos_x,0);
	igraph_real_t min_x=igraph_vector_min(&pos_x);
	igraph_real_t max_x=igraph_vector_max(&pos_x);
	igraph_vector_t pos_y;
	igraph_vector_init(&pos_y, Nnodes_);
	igraph_matrix_get_col(coords_, &pos_y, 1);
	igraph_real_t min_y=igraph_vector_min(&pos_y);
	igraph_real_t max_y=igraph_vector_max(&pos_y);
	sf::Vector2f w_size = w->getView().getSize();
	float x_factor = (float)w_size.x / (float)1.1 / (float)(max_x - min_x);
	float x_base = (float)w_size.x * (float)(0.05 / 1.1);
	float y_factor = (float)w_size.y / (float)1.1 / (float)(max_y - min_y);
	float y_base = (float)w_size.y * (float)(0.05 / 1.1);
	//float radius=min(x_factor,y_factor)*1;
	float radius = 4;
	float screen_x[Nnodes_];
	float screen_y[Nnodes_];
	for (size_t n = 0; n < Nnodes_; n++)
	{
		screen_x[n] = x_base + (float)(igraph_vector_e(&pos_x, n) - min_x) * x_factor;
		screen_y[n] = y_base + (float)(igraph_vector_e(&pos_y, n) - min_y) * y_factor;
	}
	igraph_vector_destroy(&pos_x);
	igraph_vector_destroy(&pos_y);
	igraph_es_t 		es = igraph_ess_all(IGRAPH_EDGEORDER_ID);
	igraph_eit_t		eit;
	sf::Vertex			edge[2];
	igraph_integer_t	eid;
	igraph_integer_t	from;
	igraph_integer_t	to;
	igraph_eit_create(graph_, es, &eit);
	IGRAPH_EIT_RESET(eit);
	
	bool stop(false);
	do
	{
		eid=IGRAPH_EIT_GET(eit);
		igraph_edge(graph_, eid, &from, &to);
		edge[0] = sf::Vertex(sf::Vector2f(screen_x[from] + radius, screen_y[from] + radius));
		edge[1] = sf::Vertex(sf::Vector2f(screen_x[to] + radius, screen_y[to] + radius));
		w->draw(edge, 2, sf::Lines);
		stop=IGRAPH_EIT_END(eit);
		if (!stop)
		{
			IGRAPH_EIT_NEXT(eit);
		}
	}
	while (!stop);
	igraph_eit_destroy(&eit);
	igraph_vs_t vs=igraph_vss_all();
	igraph_vector_t degree;
	igraph_vector_init(&degree, Nnodes_);
	igraph_degree(graph_, &degree, vs, IGRAPH_ALL, false);
	igraph_real_t c_factor = 255 / igraph_vector_max(&degree);
	sf::Color color[Nnodes_];
	sf::Uint8 r;
	sf::Uint8 b;
	for (size_t n = 0; n < Nnodes_; n++)
	{
		r = (sf::Uint8)(igraph_vector_e(&degree, n) * c_factor);
		b = (sf::Uint8)(255 - igraph_vector_e(&degree, n) * c_factor);
		color[n] = sf::Color(r, 0, b);
	}
	igraph_vector_destroy(&degree);
	sf::CircleShape node_shape;
	node_shape.setRadius(radius);
	for (size_t n = 0; n < Nnodes_; n++)
	{
		node_shape.setFillColor(color[n]);
		node_shape.setPosition(screen_x[n], screen_y[n]);
		w->draw(node_shape);
	}
}

void	Graph::substitution(void)
{
	igraph_integer_t from;
	igraph_integer_t to;
	igraph_integer_t alter;
	
	igraph_integer_t eid;
	for (size_t i = Nnodes_/100; i--;)
	{
		eid = (igraph_integer_t)(rand()%(int)igraph_ecount(graph_));
		if ((float)rand() / (float)RAND_MAX < psub_)
		{
			igraph_edge(graph_, eid, &from, &to);
			igraph_delete_edges(graph_, igraph_ess_1(eid));
			if (!is_connected())
				igraph_add_edge(graph_, from, to);
			else
			{
				do
				{
					alter = (igraph_integer_t)(rand() % (int)Nnodes_);
				}
				while (alter == from || alter == to);
				if (rand() % 2)
				{
					if (!are_neighbors(from, alter))
						igraph_add_edge(graph_, from, alter);
				}
				else
				{
					if (!are_neighbors(alter, to))
						igraph_add_edge(graph_, alter, to);
				}
			}
		}
	}
	/*
	for (igraph_integer_t eid = igraph_ecount(graph_); eid--;)
	{
		if ((float)rand() / (float)RAND_MAX < psub_)
		{
			igraph_edge(graph_, eid, &from, &to);
			igraph_delete_edges(graph_, igraph_ess_1(eid));
			if (!is_connected())
				igraph_add_edge(graph_, from, to);
			else
			{
				do
				{
					alter = (igraph_integer_t)(rand() % (int)Nnodes_);
				}
				while (alter == from || alter == to);
				if (rand() % 2)
				{
					if (!are_neighbors(from, alter))
						igraph_add_edge(graph_, from, alter);
				}
				else
				{
					if (!are_neighbors(alter, to))
						igraph_add_edge(graph_, alter, to);
				}
			}
		}
	}
	*/
}

void	Graph::insertion(void)
{
	igraph_integer_t alter;
	
	igraph_integer_t vid;
	for (size_t i = Nnodes_/100; i--;)
	{
		vid=(igraph_integer_t)(rand()%(int)Nnodes_);
		if ((float)rand() / (float)RAND_MAX < pins_)
		{
			do
			{
				alter = (igraph_integer_t)(rand() % (int)Nnodes_);
			}
			while (alter == vid);
			if (!are_neighbors(vid, alter))
				igraph_add_edge(graph_, vid, alter);
		}
	}
	/*
	for (igraph_integer_t vid = (igraph_integer_t)Nnodes_; vid--;)
	{
		if ((float)rand() / (float)RAND_MAX < pins_)
		{
			do
			{
				alter = (igraph_integer_t)(rand() % (int)Nnodes_);
			}
			while (alter == vid);
			if (!are_neighbors(vid, alter))
				igraph_add_edge(graph_, vid, alter);
		}
	}
	*/
}

void	Graph::deletion(void)
{
	igraph_integer_t from;
	igraph_integer_t to;
	
	igraph_integer_t eid;
	for (size_t i = Nnodes_/100; i--;)
	{
		eid=(igraph_integer_t)(rand()%(int)igraph_ecount(graph_));
		if ((float)rand() / (float)RAND_MAX < pdel_)
		{
			igraph_edge(graph_, eid, &from, &to);
			igraph_delete_edges(graph_, igraph_ess_1(eid));
			if (!is_connected())
				igraph_add_edge(graph_, from, to);
		}
	}
	/*
	for (igraph_integer_t eid = igraph_ecount(graph_); eid--;)
	{
		if ((float)rand() / (float)RAND_MAX < pdel_)
		{
			igraph_edge(graph_, eid, &from, &to);
			igraph_delete_edges(graph_, igraph_ess_1(eid));
			if (!is_connected())
				igraph_add_edge(graph_, from, to);
		}
	}
	*/
}

void	Graph::mutate(void)
{
	deletion();
	substitution();
	insertion();
}

size_t	Graph::getN(void)
{
	return (Nnodes_);
}

double	Graph::getCost(void)
{
	return (cost_);
}

bool	Graph::are_neighbors(igraph_integer_t v1, igraph_integer_t v2)
{
	bool neighbors(true);
	igraph_vector_t neis;
	igraph_vector_init(&neis, 0);
	igraph_neighbors(graph_, &neis, v1, IGRAPH_ALL);
	neighbors = (bool)igraph_vector_contains(&neis, (igraph_real_t)v2);
	igraph_vector_destroy(&neis);
	return (neighbors);
}

bool	Graph::is_connected(void)
{
	bool connectivity(true);
	igraph_vector_ptr_t components;
	igraph_vector_ptr_init(&components,0);
	igraph_decompose(graph_, &components, IGRAPH_WEAK, -1, 1);
	connectivity=(int)igraph_vector_ptr_size(&components) < 2;
	igraph_decompose_destroy(&components);
	igraph_vector_ptr_destroy(&components);
	return (connectivity);
}

void	Graph::checkConnection(void)
{
	igraph_vector_ptr_t complist;
	igraph_vector_ptr_init(&complist, 0);
	igraph_decompose(graph_, &complist, IGRAPH_WEAK, -1, 1);
	int a = (int)igraph_vector_ptr_size(&complist);
	if (a > 1)
	{
		igraph_t*			new_graph = new igraph_t;
		igraph_empty(new_graph, 0, IGRAPH_UNDIRECTED);
		igraph_integer_t	nodes_in = 0;
		igraph_integer_t	n_new_nodes, n_new_edges, id1, id2;
		igraph_vector_t		new_edges;
		igraph_vector_init(&new_edges, 0);
		for (int i = 0; i < a; i++) // Pour chaque composante non connexe
		{
			//Ajouter les noeuds au new_graph
			n_new_nodes = igraph_vcount((igraph_t*)VECTOR(complist)[i]);
			igraph_add_vertices(new_graph, n_new_nodes, 0);

			//Ajouter les edges au new_graph
			igraph_get_edgelist((igraph_t*)VECTOR(complist)[i], &new_edges, 0);
			n_new_edges = igraph_vector_size(&new_edges);
			for (int j = 0; j < n_new_edges-1;  j += 2)
			{
				id1 = nodes_in + VECTOR(new_edges)[j];
				id2 = nodes_in + VECTOR(new_edges)[j + 1];
				igraph_add_edge(new_graph, id1, id2);
			}

			if (i) // reconnecter la composante avec la précédente
			{
				id1 = rand() % nodes_in;
				id2 = nodes_in + rand() % n_new_nodes;
				igraph_add_edge(new_graph, id1, id2);
			}
			nodes_in += n_new_nodes;
		}
		igraph_destroy(graph_);
		delete graph_;
		graph_ = new_graph;
		//~ igraph_simplify(graph_,1,1,0);
		cost_ = this->cost();
	}
	igraph_decompose_destroy(&complist);
}
//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
void	print_vector(igraph_vector_t *v)
{
	long int	i;
	printf("\n vector:\n    ");
	for (i = 0; i < igraph_vector_size(v); i++){
		printf(" %li", (long int)VECTOR(*v)[i]);
	}
	printf("\n");
}
