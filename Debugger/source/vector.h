#pragma once

template<typename t_type>
class c_vector
{
	const size_t k_element_size = sizeof(t_type);

public:
	c_vector() :
		m_count(0),
		m_elements(nullptr)
	{
	}

	c_vector(size_t count) :
		m_count(0),
		m_elements(nullptr)
	{
		create(count);
	}

	~c_vector()
	{
		destroy();
	}

	void create(size_t count)
	{
		destroy();

		m_count += count;
		m_elements = new t_type[m_count];
		memset(m_elements, 0, m_count * k_element_size);
	}

	void destroy()
	{
		if (m_elements)
			delete[] m_elements;

		m_count = 0;
		m_elements = nullptr;
	}

	void expand(size_t count)
	{
		t_type* elements = new t_type[m_count + count];
		memset(elements, 0, (m_count + count) * k_element_size);

		if (m_elements)
		{
			memcpy(elements, m_elements, m_count * k_element_size);
			delete[] m_elements;
		}

		m_count += count;
		m_elements = elements;
	}

	void compact(size_t count)
	{
		if (count > m_count)
			return;

		t_type* elements = new t_type[m_count - count];
		memset(elements, 0, (m_count - count) * k_element_size);

		if (m_elements)
		{
			memcpy(elements, m_elements, (m_count - count) * k_element_size);
			delete[] m_elements;
		}

		m_count -= count;
		m_elements = elements;
	}

	void push(t_type value)
	{
		expand(1);
		::memcpy(&m_elements[m_count - 1], &value, k_element_size);
	}

	void pop()
	{
		compact(1);
	}

	const size_t count()
	{
		return m_count;
	}

	const size_t size()
	{
		return m_count * k_element_size;
	}

	const t_type* data()
	{
		return m_elements;
	}

	t_type& operator[](size_t index)
	{
		if (!m_elements)
			throw;

		return m_elements[index];
	}

	void operator=(c_vector<t_type>& from)
	{
		if (!from.data() || !from.size())
			return;

		if (!m_elements)
			create(from.size());

		::memcpy(m_elements, from.data(), from.size());
	}

private:
	size_t m_count;
	t_type* m_elements;
};
