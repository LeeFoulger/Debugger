#pragma once

TODO("find the actual place this is stored")
template<typename t_enum, typename t_storage, t_enum k_count>
class c_flags
{
public:
	c_flags()
	{
	}

	inline void set(t_enum value)
	{
		if (value < k_count)
			m_value |= (1 << (t_storage)value);
	}

protected:
	t_storage m_value;
};

TODO("find the actual place this is stored")
template<typename t_enum, typename t_storage, t_enum k_count>
class c_enum
{
public:
	c_enum()
	{
	}

	inline void set(t_enum value)
	{
		if (value < k_count)
			m_value = (t_storage)value;
	}

protected:
	t_storage m_value;
};