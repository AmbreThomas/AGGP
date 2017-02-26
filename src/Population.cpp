//============================ INCLUDES ================================
#include "Population.h"

using namespace std;

//========================= STATIC ATTRIBUTES ==========================


//=========================== CONSTRUCTORS =============================

Population::Population(unsigned int n, unsigned int Nnodes, unsigned int Nedges)
{
	for (unsigned int i = 0; i<n; i++){
		pop_.push_back(new Graph(Nnodes, Nedges));
	}
	size_ = n;
}

//=========================== DESTRUCTORS ==============================

Population::~Population(void)
{
	for ( size_t i = 0; i<pop_.size(); i++ ){
		delete pop_[i];
	}
}


//=========================== PUBLIC METHODS ===========================

size_t	Population::size(void) { return (size_); }

Graph 	Population::getgraph(int i) { return (*pop_[i]); }

void	Population::cross(void)
{
	
}

void	Population::mutate_children(void)
{
	
}

void	Population::select_by_tournament(void)
{
	for (unsigned int i = 0; i<size_; i++){
		printf("Graphe %d: \t%f\n",i+1, pop_[i]->cost());
	}
}

void	Population::select_elite(void)
{
	
}

//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
