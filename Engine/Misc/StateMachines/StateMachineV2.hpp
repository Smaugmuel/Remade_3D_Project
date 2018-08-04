#ifndef STATE_MACHINE_V2_HPP
#define STATE_MACHINE_V2_HPP

/* Notes */
// ===================================================================
// | The state category defines what kind of states this machine has |
// -------------------------------------------------------------------
// | I.e. StateMachineV2<GameState> or StateMachineV2<EnemyState>        |
// ====================================================================================================================
// | Using a forward declared state category when declaring the state machine requires it to be dynamically allocated |
// --------------------------------------------------------------------------------------------------------------------
// | I.e. StateMachineV2<GameState>* stateMachine;                                                                      |
// ===============================================================================================================================
// | Only child states of the defined state category can be pushed to the machine                                                |
// -------------------------------------------------------------------------------------------------------------------------------
// | All the child states need a constructor where the first argument is a pointer to the state machine that they were pushed to |
// | This pointer then needs to be sent to the parent constructor                                                                |
// | Any following arguments can be whatever that state needs, due to variadic templating                                        |
// -------------------------------------------------------------------------------------------------------------------------------
// | I.e. PlayState(StateMachineV2<GameState>* stateMachine, Vector3 startPos, Vector3 enemyStartPos) : GameState(stateMachine)    |
// ===============================================================================================================================
// | Whenever a state is pushed, that state needs to be defined / included                   |
// | When a state is popped using PopUntil<State>(), State only needs to be forward declared |
// ===========================================================================================
//
// =======================================================================================================
// | This version of the state machine requires the state category to inherit from StateV2               |
// | The child class of the state category then needs to override the OnEntry and OnExit functions       |
// | Within those functions anything that should happen upon entering or leaving the state can be placed |
// -------------------------------------------------------------------------------------------------------
// | I.e. class GameState : public StateV2                                                               |
// |      class PlayState : public GameState                                                             |
// -------------------------------------------------------------------------------------------------------
// |      void PlayState::OnEntry()                                                                      |
// |      {                                                                                              |
// |          EnableRendering();                                                                         |
// |      }                                                                                              |
// |      void PlayState::OnExit()                                                                       |
// |      {                                                                                              |
// |          DisableRendering();                                                                        |
// |      }                                                                                              |
// ========================================================================================================

class StateV2
{
public:
	void Enter()
	{
		if (m_isCurrentState)
		{
			return;
		}
		m_isCurrentState = true;
		OnEntry();
	}
	void Leave()
	{
		if (!m_isCurrentState)
		{
			return;
		}
		m_isCurrentState = false;
		OnExit();
	}

protected:
	StateV2() : m_isCurrentState(false) {}
	~StateV2() {}

	virtual void OnEntry() = 0;
	virtual void OnExit() = 0;

private:
	bool m_isCurrentState;
};

template<typename StateCategory>
class StateMachineV2 final
{
public:
	StateMachineV2() : m_top(nullptr)
	{
	}
	~StateMachineV2()
	{
		while (m_top)
		{
			Pop();
		}
	}

	template<typename StateType, typename ... StateArgument> void Push(const StateArgument& ... args)
	{
		// Leave the current state if it exists
		if (m_top && m_top->state)
		{
			m_top->state->Leave();
		}

		// Create the new state
		Node* newNode = new Node;
		newNode->state = new StateType(this, args...);
		newNode->next = m_top;
		m_top = newNode;

		// Enter the new state
		m_top->state->Enter();
	}
	void Pop()
	{
		if (Node* current = m_top)
		{
			m_top = m_top->next;

			// Leave the current state
			current->state->Leave();
			delete current->state;
			delete current;

			// Enter the top state if it exists
			if (m_top)
			{
				m_top->state->Enter();
			}
		}
	}
	StateCategory* Peek()
	{
		return m_top ? m_top->state : nullptr;
	}
	bool IsEmpty() const
	{
		return !m_top;
	}

private:
	struct Node
	{
		StateCategory* state;
		Node* next;
	};

	Node* m_top;
};

#endif