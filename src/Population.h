#ifndef Population_H__
#define Population_H__

#include <vector>
#include <ctime>
#include <numeric>
#include <algorithm>
#include "Graph.h"

class Population
{
	public:
		//================== CONSTRUCTORS ==============================
		Population(unsigned int, unsigned int, unsigned int, int);
		//================== DESTRUCTORS ===============================
		~Population(void);
		//==================== GETTERS =================================
		size_t				size(void);
		Graph				getgraph(int);
		size_t				getCurrentSize(void);
		//===================== SETTERS ================================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================
		void				cross(void);
		void				select_by_tournament(void);
		void				select_elite(void);
		void				study(sf::RenderWindow*, sf::RenderWindow*, int, int);
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		std::vector<Graph*>	pop_;
		double				pselect_;
		size_t				size_;
		std::vector<double>	remember_mins_;
		std::vector<double>	remember_means_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================

void	drawline(float, float, float, float, float, float, sf::RenderWindow*, sf::Color);
#endif // Population_H__

