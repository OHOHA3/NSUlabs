#pragma once

#include "IConverter.h"
#include <map>
#include <memory>

class ICreator
{
public:
	virtual ~ICreator() = default;

	virtual std::shared_ptr<IConverter> Create() = 0;
};

template<typename T>
class Creator : public ICreator
{
public:
	std::shared_ptr<IConverter> Create()
	{
		return std::make_shared<T>();
	}
};

class Factory
{
public:
	void RegisterCreator(std::string Converter, std::shared_ptr<ICreator> Creator);
	std::shared_ptr<IConverter> Create(const std::string& Converter);

private:
	std::map<std::string, std::shared_ptr<ICreator>> FactoryMap;
};