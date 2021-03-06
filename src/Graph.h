#ifndef Graph_H__
#define Graph_H__

#include <igraph.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <fstream>

//============================ FUNCTIONS ===============================

void	print_vector(igraph_vector_t *);

class Graph
{
	public:
		//====================== CONSTRUCTORS ==========================
		Graph();
		Graph(int, int);
		Graph(Graph*, Graph*, unsigned int);
		//====================== DESTRUCTORS ===========================
		~Graph();
		//====================== GETTERS ===============================
		size_t					getN(void);
		double					getCost(void);
		//====================== SETTERS ===============================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================
		float					average_path_length(void);
		double					cost(void);
		size_t					results(void);
		void					compute_layout(igraph_matrix_t*);
		void 					draw(sf::RenderWindow* w, igraph_matrix_t*);
		void 					mutate(void);
		void 					substitution(void);
		void 					deletion(void);
		void 					insertion(void);
		void					checkConnection(void);
		bool					are_neighbors(igraph_integer_t, igraph_integer_t);
		bool					is_connected(void);

		//==================== PUBLIC ATTRIBUTES =======================
		static igraph_real_t	LAW_EXPONENT;
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		igraph_t*				graph_;
		size_t		 			Nnodes_;
		double					psub_;
		double					pins_;
		double					pdel_;
		double					cost_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Graphe_H__
