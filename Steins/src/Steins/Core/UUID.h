#pragma once

#include <xhash>

namespace Steins
{
	class UUID
	{
	public:
		UUID();
		UUID(u64 uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		u64 m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Steins::UUID>
	{
		std::size_t operator()(const Steins::UUID& uuid) const 
		{
			return hash<u64>()((u64)uuid);
		}
	};
}