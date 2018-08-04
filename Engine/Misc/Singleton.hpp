#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/* Notes and instructions */
// ============================================================================================================================
// | How to correctly inherit from Singleton                                                                                  |
// ----------------------------------------------------------------------------------------------------------------------------
// | 1)     Inherit from "public Singleton<SubClassName>"                                                                     |
// |                                                                                                                          |
// | 2)     Make the sub class's default constructor and destructor private                                                   |
// | 2.1)   Type "friend class Singleton<SubClassName>;" under "private:"                                                     |
// |                                                                                                                          |
// | 3)     At the top of the .cpp file of the sub class, type "SubClassName* Singleton<SubClassName>::s_instance = nullptr;" |
// ===================================================================================================================================
// | How to correctly use Singleton                                                                                                  |
// -----------------------------------------------------------------------------------------------------------------------------------
// | 4)     Include the sub class only in .cpp-files (preferrably)                                                                   |
// |                                                                                                                                 |
// | 5)     Call "SubClassName::Get()" to retrieve a pointer to SubClassName, when wanting to access it                              |
// | 5.1)   This pointer can be stored as a member or temporarily in a function instead of calling "SubClassName::Get()" every time  |
// |                                                                                                                                 |
// | 6)     Call "SubClassName::Delete()" whenever SubClassName is to be no longer used or if wanting to reset it completely         |
// | 6.1)   This can be done wherever, and doesn't have to be done in a file where "Singleton::Get()" is called                      |
// | 6.1.1) Make sure that no file calls "SubClassName::Get()" after the last call to "SubClassName::Delete()"                       |
// ================================================================================================================================================
// | Known problems if forgetting any of the steps above                                                                                          |
// ------------------------------------------------------------------------------------------------------------------------------------------------
// | 1)     "Singleton::Get()" becomes inaccessible                                                                                               |
// |                                                                                                                                              |
// | 2)     SubClassName can be created multiple times, outside of "Singleton::Get()", which defeates the purpose of it inheriting from Singleton |
// | 2.1)   Singleton<SubClassName> can't access SubClassName's private constructor or destructor                                                 |
// |                                                                                                                                              |
// | 3)     An "Unresolved external" error appears                                                                                                |
// |                                                                                                                                              |
// | 4)     Compile times are increased slightly, and some files receive unnecessary access to Singleton. No actual errors occur                  |
// |                                                                                                                                              |
// | 5)     SubClassName can't be accessed                                                                                                        |
// | 5.1)   Performance is decreased very slightly, rows become longer and harder to read, and aestetics are lowered (subjective opinion)         |
// |                                                                                                                                              |
// | 6)     Memory leaks appear                                                                                                                   |
// | 6.1)   Some things that should or could survive the destruction of their creator might need to be unnecessarily re-created                   |
// | 6.1.1) Memory leaks appear, which may be hard to find, since "SubClassName::Delete()" actually was called, but too early                     |
// ================================================================================================================================================


template<typename ClassType>
class Singleton
{
protected:
	Singleton()
	{
	}
	virtual ~Singleton()
	{
	}

public:
	static ClassType* Get()
	{
		return s_instance ? s_instance : s_instance = new ClassType;
	}
	static void Delete()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = nullptr;
		}
	}

private:
	static ClassType* s_instance;
};

#endif