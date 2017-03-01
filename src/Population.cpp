//============================ INCLUDES ================================
#include "Population.h"

using namespace std;

//========================= STATIC ATTRIBUTES ==========================


//=========================== CONSTRUCTORS =============================

Population::Population(unsigned int n, unsigned int Nnodes, unsigned int Nedges)
{
	pcross_	=	0.1;
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

size_t 	Population::getCurrentSize(void) { return (pop_.size()); }

void	Population::cross(void)
{
	/* On tire deux individus, 
	 * puis on a une probabilité de les croiser.
	 * 
	 * Les deux graphes sont ensuite mutés,
	 * puis ajoutés à la population jusqu'à atteinte de la taille 2N */
	 
	unsigned int	i,j, crosspt;
	
	printf("\tCrossing graphs...\n");
	while (pop_.size()<2*size_)
	{
		i 	= 	rand()%(int)size_;
		j 	= 	i;
		while (i==j) j = rand()%(int)size_;
		crosspt	=	(unsigned int) rand()%(unsigned int)pop_[i]->getN();
		Graph*	parent1	=	pop_[i];
		Graph*	parent2	=	pop_[j];
		Graph*	child1	=	new Graph(parent1, parent2, crosspt);
		Graph*	child2	=	new Graph(parent1, parent2, crosspt);
		child1->mutate();
		child2->mutate();
		pop_.push_back(child1);
		pop_.push_back(child2);
	}
	//Attention ici le même graphe peut être tiré deux fois.
}



void	Population::select_by_tournament(void)
{
	/* A répéter N fois: 
	 * choisir deux individus, 
	 * garder le meilleur avec une probabilité 0.75 < p < 1
	 * remettre l'autre dans la population de base
	 * 
	 * Retourner les N sélectionnés, supprimer les N autres.
	 * 
	 * Attention, possibilité de ne jamais sélectionner un très bon graphe,
	 * qui finit par être supprimé ! */
	 
	vector<double>	costs(size_, 100);
	
	printf("\tSelecting graphs by tournament...\n");
	for (unsigned int i = 0; i<size_; i++){
		costs[i] = pop_[i]->cost();
	}
}

void	Population::select_elite(void)
{
	/* Classer la population de taille 2N par score de fitness croisant,
	 * Retourner les N meilleurs, supprimer les N plus mauvais 
	 * 
	 * Attention, pas de sortie des minima locaux ! */
	 
	vector<double>	costs(size_, 100);
	
	printf("\tSelecting th graphs' elite...\n");
	for (unsigned int i = 0; i<size_; i++){
		costs[i] = pop_[i]->cost();
	}
}

//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
