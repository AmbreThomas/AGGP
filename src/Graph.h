#ifndef Graph_H__
#define Graph_H__

#include <igraph.h>
#include <SFML/Graphics.hpp>

class Graph
{
	public:
		//================== CONSTRUCTORS ==============================
		Graph();
		Graph(int, int);
		//================== DESTRUCTORS ===============================
		~Graph();
		//==================== GETTERS =================================
		
		//===================== SETTERS ================================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================
		float	average_path_length(void);
		void draw(sf::RenderWindow w);
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		igraph_t*				graph;
		igraph_vector_t*		weights;
		static igraph_real_t	LAW_EXPONENT;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Graphe_H__
