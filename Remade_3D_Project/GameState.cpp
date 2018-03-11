#include "GameState.hpp"

GameState::GameState(StateMachine<GameState>* stateMachine) :
	m_stateMachine(stateMachine)
{
}

GameState::~GameState()
{
}