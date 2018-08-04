#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP
#include <typeindex>

/* Notes */
// ===================================================================
// | The state category defines what kind of states this machine has |
// -------------------------------------------------------------------
// | I.e. StateMachine<GameState> or StateMachine<EnemyState>        |
// ====================================================================================================================
// | Using a forward declared state category when declaring the state machine requires it to be dynamically allocated |
// --------------------------------------------------------------------------------------------------------------------
// | I.e. StateMachine<GameState>* stateMachine;                                                                      |
// ===============================================================================================================================
// | Only child states of the defined state category can be pushed to the machine                                                |
// -------------------------------------------------------------------------------------------------------------------------------
// | All the child states need a constructor where the first argument is a pointer to the state machine that they were pushed to |
// | This pointer then needs to be sent to the parent constructor                                                                |
// | Any following arguments can be whatever that state needs, due to variadic templating                                        |
// -------------------------------------------------------------------------------------------------------------------------------
// | I.e. PlayState(StateMachine<GameState>* stateMachine, Vector3 startPos, Vector3 enemyStartPos) : GameState(stateMachine)    |
// ===============================================================================================================================
// | Whenever a state is pushed, that state needs to be defined / included                   |
// | When a state is popped using PopUntil<State>(), State only needs to be forward declared |
// ==========================================================================================================================
// | Since StateMachine::Replace() receives reference arguments, it can't accept member variables of the to-be-popped state |
// | Create copies of the necessary variables, and send them instead to StateMachine::Replace                               |
// ==========================================================================================================================

template<class StateCategory>
class StateMachine final
{
public:
	StateMachine() : m_top(nullptr)
	{
	}
	~StateMachine()
	{
		while (m_top)
		{
			Pop();
		}
	}

	template<typename StateType, typename ... StateArgument> void Push(const StateArgument& ... args)
	{
		Node* newNode = new Node;
		newNode->state = new StateType(this, args...);
		newNode->next = m_top;
		m_top = newNode;
	}
	template<typename StateType, typename ... StateArgument> void Replace(const StateArgument& ... args)
	{
		Pop();
		Push<StateType>(args...);
	}

	void Pop()
	{
		if (m_top)
		{
			Node* active = m_top;
			m_top = m_top->next;

			delete active->state;
			delete active;
		}
	}
	template<typename StateType> void PopUntil()
	{
		while (typeid(*m_top->state) != typeid(StateType))
		{
			Pop();
		}
	}
	StateCategory* Peek()
	{
		return m_top ? m_top->state : nullptr;
	}
	bool Empty()
	{
		return !m_top;
	}

private:
	struct Node
	{
		StateCategory* state = nullptr;
		Node* next = nullptr;
	};

	Node * m_top;
};

#endif