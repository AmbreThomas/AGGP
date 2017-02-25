#ifndef Graph_H__
#define Graph_H__

#include <igraph.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

//============================ FUNCTIONS ===============================
void	print_vector(igraph_vector_t *);

class Graph
{
	public:
		//====================== CONSTRUCTORS ==========================
		Graph();
		Graph(int, int);
		//====================== DESTRUCTORS ===========================
		~Graph();
		//====================== GETTERS ===============================
		
		//====================== SETTERS ===============================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================
		float					average_path_length(void);
		double					cost(void);
		//==================== PUBLIC ATTRIBUTES =======================
		static igraph_real_t	LAW_EXPONENT;
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		igraph_t*				graph_;
		igraph_vector_t*		weights_;
		size_t		 			Nnodes_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Graphe_H__
