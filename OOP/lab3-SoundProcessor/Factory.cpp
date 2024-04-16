#include "Factory.h"

void Factory::RegisterCreator(std::string Converter, std::shared_ptr<ICreator> Creator)
{
	FactoryMap.insert({ std::move(Converter), std::move(Creator) });
}

std::shared_ptr<IConverter> Factory::Create(const std::string& Converter)
{
	auto it = FactoryMap.find(Converter);
	if (it == FactoryMap.end())
	{
		return nullptr;
	}

	return it->second->Create();
}
