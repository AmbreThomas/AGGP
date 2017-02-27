#ifndef Population_H__
#define Population_H__

#include <vector>
#include <ctime>
#include "Graph.h"

class Population
{
	public:
		//================== CONSTRUCTORS ==============================
		Population(unsigned int, unsigned int, unsigned int);
		//================== DESTRUCTORS ===============================
		~Population(void);
		//==================== GETTERS =================================
		size_t	size(void);
		Graph	getgraph(int);
		size_t	getCurrentSize(void);
		//===================== SETTERS ================================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================
		void	cross(void);
		void	select_by_tournament(void);
		void	select_elite(void);
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		std::vector<Graph*>	pop_;
		double				pcross_;
		double				pmut_;
		size_t				size_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Population_H__

