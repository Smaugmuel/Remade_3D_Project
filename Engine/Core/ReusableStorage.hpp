#ifndef REUSABLE_STORAGE_HPP
#define REUSABLE_STORAGE_HPP
#include <vector>

template<typename T>
class ReusableStorage
{
public:
	ReusableStorage() noexcept
	{

	}
	virtual ~ReusableStorage() noexcept
	{

	}

	int CreateElementAndGetIndex()
	{
		int id = -1;
		if (m_availableIndices.size() == 0)
		{
			// Add a new element and retrieve its index
			m_elements.push_back(T());
			id = m_elements.size() - 1;
		}
		else
		{
			// Retrieve the oldest available index
			id = m_availableIndices.front();

			// Set the the newest index as the first index
			m_availableIndices[0] = m_availableIndices.back();

			// Remove the last index
			m_availableIndices.pop_back();
		}
		return id;
	}
	void RemoveElement(int id) noexcept
	{
		if (ValidID(id))
		{
			// Don't remove the element, but make its index available
			m_availableIndices.push_back(id);
		}
	}
	void SetElement(int id, const T& element)
	{
		if (ValidID(id))
		{
			m_elements[id] = element;
		}
	}
	T* GetElement(int id)
	{
		return ValidID(id) ? &m_elements[id] : nullptr;
	}
	int Size() const noexcept
	{
		return m_elements.size() - m_availableIndices.size();
	}

private:
	bool ValidID(int id) const
	{
		bool isValid = true;

		// Check if id is inside the element range
		if (id >= 0 && id < m_elements.size())
		{
			unsigned int n = m_availableIndices.size();
			for (unsigned int i = 0; i < n; i++)
			{
				// Check if this index was previously removed
				if (id == m_availableIndices[i])
				{
					isValid = false;
					break;
				}
			}
		}
		else
		{
			isValid = false;
		}
		return isValid;
	}

	std::vector<T> m_elements;
	std::vector<int> m_availableIndices;
};

#endif