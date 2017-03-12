#ifndef Graph_H__
#define Graph_H__

#include <igraph.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <SFML/Graphics.hpp>

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
		void					compute_layout(igraph_matrix_t*);
		void 					draw(sf::RenderWindow* w, igraph_matrix_t*);
		void					substitution(void);
		void					insertion(void);
		void					deletion(void);
		void 					mutate(void);
		void					checkConnection(void);
		//==================== PUBLIC ATTRIBUTES =======================
		static igraph_real_t	LAW_EXPONENT;
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		igraph_t*				graph_;
		size_t		 			Nnodes_;
		float					psub_;
		float					pins_;
		float					pdel_;
		double					cost_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Graphe_H__
