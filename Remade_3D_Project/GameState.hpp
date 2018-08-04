#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP
#include "../Engine/Misc/StateMachines/StateMachineV2.hpp"

class GameState : public StateV2
{
public:
	GameState(StateMachineV2<GameState>* stateMachine);
	virtual ~GameState();

	virtual bool Initialize() = 0;
	virtual void ProcessInput() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	StateMachineV2<GameState>* m_stateMachine;
};

#endif