#ifndef PHYSIC_SYSTEM_HPP
#define PHYSIC_SYSTEM_HPP
#include "AbstractSystem.hpp"

class PhysicSystem : public AbstractSystem
{
public:
	PhysicSystem();
	~PhysicSystem();

	void Update(float dt) override;

private:
	bool HasCorrectComponents(EntityID entity) const override;
};

#endif