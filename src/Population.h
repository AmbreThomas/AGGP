#ifndef Population_H__
#define Population_H__

#include <vector>
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
		//===================== SETTERS ================================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================
		void	cross(void);
		void 	mutate_children(void);
		void	select_by_tournament(void);
		void	select_elite(void);
	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		std::vector<Graph*>	pop_;
		size_t				size_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Population_H__

