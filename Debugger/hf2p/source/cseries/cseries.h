#pragma once

using uchar = unsigned char;
using ushort = unsigned short;
using ulong = unsigned long;
using qword = unsigned long long;

template<size_t k_length>
using t_padding = uchar[k_length];

TODO("find the actual place this is stored")
template<typename t_enum, typename t_storage, t_enum k_count>
class c_flags
{
public:
	c_flags() : m_value()
	{
	}

	inline void set(t_enum bit, bool add)
	{
		if (bit >= k_count)
			return;

		if (add)
			m_value |= (1 << bit);
		else
			m_value &= ~(1 << bit);
	}

	inline const t_storage get_raw_value()
	{
		return m_value;
	}

	inline void set_raw_value(t_storage value)
	{
		m_value = value;
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
			set_raw_value((t_storage)value);
	}

	inline bool operator==(t_enum value)
	{
		return !!(m_value == value);
	}

	inline bool operator!=(t_enum value)
	{
		return !!(m_value != value);
	}

	inline const t_storage get_raw_value()
	{
		return m_value;
	}

	inline void set_raw_value(t_storage value)
	{
		m_value = value;
	}

protected:
	t_storage m_value;
};
