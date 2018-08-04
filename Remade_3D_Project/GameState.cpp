#include "GameState.hpp"

GameState::GameState(StateMachineV2<GameState>* stateMachine) :
	m_stateMachine(stateMachine)
{
}

GameState::~GameState()
{
}