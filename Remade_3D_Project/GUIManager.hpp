#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP
#include "Singleton.hpp"

class GUIManager : public Singleton<GUIManager>
{
	friend class Singleton<GUIManager>;
	GUIManager();
	~GUIManager();

public:


private:

};

#endif