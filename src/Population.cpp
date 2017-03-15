//============================ INCLUDES ================================
#include "Population.h"

using namespace std;

//========================= STATIC ATTRIBUTES ==========================

static size_t indice_min(vector<double>);

//=========================== CONSTRUCTORS =============================

Population::Population(unsigned int n, unsigned int Nnodes, unsigned int Nedges, int itermax)
{
	pselect_ = 0.01;
	for (unsigned int i = 0; i < n; i++)
		pop_.push_back(new Graph(Nnodes, Nedges));
	remember_means_ = vector<double>(itermax, 0);
	remember_mins_ = vector<double>(itermax, 0);
	size_ = n;
}

//=========================== DESTRUCTORS ==============================

Population::~Population(void)
{
	for ( size_t i = 0; i < pop_.size(); i++)
		delete pop_[i];
}

//=========================== PUBLIC METHODS ===========================

size_t	Population::size(void)
{
	return (size_);
}

Graph*	Population::getgraph(int i)
{
	return (pop_[i]);
}

size_t	Population::getCurrentSize(void)
{
	return (pop_.size());
}

void	Population::cross(void)
{
	/* On tire deux individus, 
	 * puis on a une probabilité de les croiser.
	 * 
	 * Les deux graphes sont ensuite mutés,
	 * puis ajoutés à la population jusqu'à atteinte de la taille 2N */
	
	unsigned int	i, j, crosspt;
	
	printf("\tCrossing graphs...\n");
	while (pop_.size() < 2 * size_)
	{
		i = rand() % (int)size_;
		j = i;
		while (i == j) 
			j = rand() % (int)size_;
		crosspt = (unsigned int)rand() % (unsigned int)pop_[i]->getN();
		Graph*	parent1 = pop_[i];
		Graph*	parent2 = pop_[j];
		Graph*	child1 = new Graph(parent1, parent2, crosspt);
		Graph*	child2 = new Graph(parent1, parent2, crosspt);
		child1->mutate();
		child2->mutate();
		pop_.push_back(child1);
		pop_.push_back(child2);
	}
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
	
	size_t					currentSize = pop_.size();
	vector<unsigned int>	tokeep(currentSize, 0);
	
	printf("\tSelecting graphs by tournament...\n");
	for (size_t i = 0; i < size_; i++)
	{
		unsigned int			a, b, p;
		
		a = rand() % currentSize;
		while (tokeep[a]) a = rand() % currentSize;
		b = a;
		while (a == b or tokeep[b]) b = rand() % currentSize;
		p = ((double)rand()) / RAND_MAX;
		if (p > pselect_)
		{
			/* Garder le pire des deux */
			if (pop_[a]->getCost() >= pop_[b]->getCost()){
				tokeep[a] = 1;
			} else {
				tokeep[b] = 1;
			}
		} else {
			/* Garder le meilleur des deux */
			if (pop_[a]->getCost() >= pop_[b]->getCost()){
				tokeep[b] = 1;
			} else {
				tokeep[a] = 1;
			}
		}
	}
	for (int i = currentSize - 1; i >= 0; i--)
	{
		if (!tokeep[i])
		{
			delete pop_[i];
			pop_.erase(pop_.begin() + i);
		}
	}
}


void	Population::select_elite(void)
{
	/* Classer la population de taille 2N par score de fitness croisant,
	 * Retourner les N meilleurs, supprimer les N plus mauvais 
	 * 
	 * Attention, pas de sortie des minima locaux ! */
	 
	vector<double>		costs(size_, 100);
	size_t				i;
	vector<Graph*>		new_pop;
	size_t				temp;

	i = 0;
	printf("\tSelecting the graphs' elite...\n");
	for (unsigned int i = 0; i < size_; i++){
		costs[i] = pop_[i]->getCost();
	}
	while (i < pop_.size() / 2)
	{
		temp = indice_min(costs);
		new_pop.push_back(pop_[temp]);
		costs.erase(costs.begin() + temp);
		i++;
	}
	pop_ = new_pop;
}


Graph*	Population::getbestgraph(void)
{
	vector<double>	costs(size_, 100);
	
	for (unsigned int i = 0; i < size_; i++){
		costs[i] = pop_[i]->getCost();
	}
	return pop_[distance(costs.begin(), min_element(costs.begin(), costs.end()))];
}


void	Population::study(sf::RenderWindow* v, sf::RenderWindow* w, int iter, int itermax)
{
	vector<double>	costs(pop_.size());
	
	for (unsigned int i = 0; i < pop_.size(); i++)
	{
		costs[i] = pop_[i]->getCost();
		if (iter < itermax)
			pop_[i]->checkConnection();
	}
	double	minmax = *max_element(remember_mins_.begin(), remember_mins_.end());
	double	meanmax = *max_element(remember_means_.begin(), remember_means_.end());
	remember_means_[iter] = accumulate( costs.begin(), costs.end(), 0.0) / (float)costs.size();
	remember_mins_[iter] = *min_element(costs.begin(), costs.end());
	if (iter < itermax) printf("\tThe best graph is graph %d with cost %f.\n",
	 1 + (int)distance(costs.begin(), min_element(costs.begin(), costs.end())),
	 costs[(int)distance(costs.begin(), min_element(costs.begin(), costs.end()))]);
	drawline(0, 0, itermax, 0, (float)itermax, minmax, v, sf::Color::White);
	drawline(0, 0, 0, 50, (float)itermax, minmax, v, sf::Color::White);
	drawline(0, 0, itermax, 0, (float)itermax, meanmax, w, sf::Color::White);
	drawline(0, 0, 0, 50, (float)itermax, meanmax, w, sf::Color::White);
	for( int i = 0; i < iter - 1; i++){
		drawline(i, remember_means_[i], i + 1, remember_means_[i + 1], (float)itermax, meanmax, w, sf::Color::Red);
		drawline(i, remember_mins_[i], i + 1, remember_mins_[i + 1], (float)itermax, meanmax, w, sf::Color::Green);
		drawline(i, remember_mins_[i], i + 1, remember_mins_[i + 1], (float)itermax, minmax, v, sf::Color::Green);
	}
}
//========================== PROTECTED METHODS =========================

//============================ FUNCTIONS ===============================
void	drawline(float x1, float y1, float x2, float y2, float xmax, float ymax, sf::RenderWindow* w, sf::Color col)
{
	float	x_factor, y_factor;
	
	sf::Vector2f w_size		=	w->getView().getSize();
	x_factor				=	(float)(w_size.x - 20) / (xmax);
	y_factor				=	(float)(w_size.y - 20) / (ymax);

	x1 =  10 + x1 * x_factor;
	y1 =  (float)w_size.y - (10 + y1 * y_factor);
	x2 =  10 + x2 * x_factor;
	y2 =  (float)w_size.y - (10 + y2 * y_factor);
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(x1, y1)),
		sf::Vertex(sf::Vector2f(x2, y2))
	};
	line[0].color = col;
	line[1].color = col;
	w->draw(line, 2, sf::Lines);
}


static size_t indice_min(vector<double> tab)
{
	size_t		indice;
	double		cout;
	size_t		i;

	i = 0;
	indice = 0;
	cout = tab[indice];
	while (i < tab.size())
	{
		if (tab[i] < cout)
		{
			cout = tab[i];
			indice = i;
		}
		i++;
	}
	return (indice);
}
