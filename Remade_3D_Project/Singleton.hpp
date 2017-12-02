#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/*


How to inherit correctly from Singleton
---------------------------------------

1) Inherit from Singleton<SubClassName>

2) Type "friend class Singleton<SubClassName>;" at the top of sub class

3) Make the sub class's default constructor and destructor private

4) At the top of the .cpp file of the sub class, type "SubClassName* Singleton<SubClassName>::s_instance = nullptr;"


*/



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