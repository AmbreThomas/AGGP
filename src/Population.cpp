//============================ INCLUDES ================================
#include "Population.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

//========================= STATIC ATTRIBUTES ==========================


//=========================== CONSTRUCTORS =============================

Population::Population(int n, int Nnodes, int Nedges)
{
	for (int i = 0; i<n; i++){
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

int		Population::size(void)
{ return (size_); }

Graph 	Population::getgraph(int i)
{ return (*pop_[i]); }

//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
