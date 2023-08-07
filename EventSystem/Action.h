#pragma once
#include <iostream>
#include <functional>
#include <vector>

template<class... Args>
class Action
{
	using Func = std::function<void(Args...)>;
public:

	void AddFunc(Func&& func)
	{
		functions.push_back(std::forward<Func>(func));
	}

	void Invoke(Args&&... args)
	{
		for (auto& func : functions)
		{
			func(std::forward<Args>(args)...);
		}
	}

private:

	std::vector<Func> functions;
};

template<>
class Action<void>
{
	using Func = std::function<void()>;
public:

	void AddFunc(Func&& func)
	{
		functions.push_back(std::forward<Func>(func));
	}

	void Invoke()
	{
		for (auto& func : functions)
		{
			func();
		}
	}

private:

	std::vector<Func> functions;
};