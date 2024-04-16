#include "Factory.h"

void Factory::RegisterCreator(std::string StrategyName, std::shared_ptr<ICreator> creator)
{
	FactoryMap.insert({ std::move(StrategyName), std::move(creator) });
}

std::shared_ptr<IStrategy> Factory::Create(const std::string& StrategyName)
{
	auto it = FactoryMap.find(StrategyName);
	if (it == FactoryMap.end())
	{
		return nullptr;
	}

	return it->second->Create();
}

bool Factory::CanCreate(std::string StrategyName)
{
	auto it = FactoryMap.find(StrategyName);
	if (it == FactoryMap.end())
	{
		return false;
	}
	return true;
}
