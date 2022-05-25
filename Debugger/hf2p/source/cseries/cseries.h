#pragma once

TODO("find the actual place this is stored")
template<typename t_enum, typename t_storage, t_enum k_count>
class c_flags
{
public:
	c_flags() : m_value()
	{
	}

	inline void set(t_enum bit)
	{
		if (bit < k_count)
			m_value |= (1 << bit);
		else
			m_value &= ~(1 << bit);
	}

protected:
	t_storage m_value;
};

TODO("find the actual place this is stored")
template<typename t_enum, typename t_storage, t_enum k_default, t_enum k_count>
class c_enum
{
public:
	c_enum() : m_value()
	{
	}

	inline void operator=(t_enum value)
	{
		if (value < k_count)
			m_value = value;
	}

	inline t_enum get()
	{
		return (t_enum)m_value;
	}

protected:
	t_storage m_value;
};
