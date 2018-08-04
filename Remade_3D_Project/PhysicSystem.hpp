#ifndef PHYSIC_SYSTEM_HPP
#define PHYSIC_SYSTEM_HPP
#include "AbstractSystem.hpp"

class PhysicSystem final : public AbstractSystem
{
public:
	PhysicSystem();
	~PhysicSystem();

	void Update(float dt) final override;

private:
	bool HasCorrectComponents(EntityID entity) const final override;
};

#endif