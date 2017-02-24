#ifndef Population_H__
#define Population_H__

#include <vector>
#include "Graph.h"

class Population
{
	public:
		//================== CONSTRUCTORS ==============================
		Population(int, int, int);
		//================== DESTRUCTORS ===============================
		~Population(void);
		//==================== GETTERS =================================
		int		size(void);
		Graph	getgraph(int);
		//===================== SETTERS ================================
		
		//====================== OPERATORS =============================
		
		//==================== PUBLIC METHODS ==========================

	protected:
		//==================== PROTECTED METHODS =======================
		
		//======================= ATTRIBUTES ===========================
		std::vector<Graph*>	pop_;
		int					size_;
};


//===================== GETTERS DEFINITIONS ============================

//===================== SETTERS DEFINITIONS ============================

//===================== OPERATORS DEFINITIONS ==========================

//=================== INLINE FUNCTIONS DEFINITIONS =====================


#endif // Population_H__

