#include "GameObjectIDGenerator.h"

#include <sstream>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace S2DE
{
	GameObjectIDGenerator::GameObjectIDGenerator()
		:	m_uuid(boost::uuids::random_generator()())
	{

	}

	GameObjectIDGenerator::~GameObjectIDGenerator()
	{
		ZeroMemory(&m_uuid, sizeof(m_uuid));
	}
	
	void GameObjectIDGenerator::SetID(const std::string id_str)
	{
		ZeroMemory(&m_uuid, sizeof(m_uuid));
		std::istringstream strm(id_str);
		strm >> m_uuid;
	}

	std::string GameObjectIDGenerator::GetIDString() const
	{
		return boost::uuids::to_string(m_uuid);
	}
}