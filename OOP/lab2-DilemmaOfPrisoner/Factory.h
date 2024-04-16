#pragma once

#include "IStrategy.h"
#include <map>
#include <memory>

class ICreator
{
public:
	virtual ~ICreator() = default;

	virtual std::shared_ptr<IStrategy> Create() = 0;
};

template<typename T>
class Creator : public ICreator
{
public:
	std::shared_ptr<IStrategy> Create()
	{
		return std::make_shared<T>();
	}
};

class Factory
{
public:
	void RegisterCreator(std::string StrategyName, std::shared_ptr<ICreator> creator);
	std::shared_ptr<IStrategy> Create(const std::string& StrategyName);

	bool CanCreate(std::string StrategyName);
private:
	std::map<std::string, std::shared_ptr<ICreator>> FactoryMap;
};