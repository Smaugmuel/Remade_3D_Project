#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//#include "StateMachine.hpp"

template<typename StateCategory> class StateMachine;

class GameState
{
public:
	GameState(StateMachine<GameState>* stateMachine);
	virtual ~GameState();

	virtual bool Initialize() = 0;
	virtual void ProcessInput() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	StateMachine<GameState>* m_stateMachine;
};

#endif