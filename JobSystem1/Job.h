#pragma once
#include <functional>

class Job
{
public:

	void SetFunc(std::function<void(float)> func)
	{
		m_func = func;
	}

	void Execute(float deletaTime)
	{
		m_func(deletaTime);
	}

private:

	std::function<void(float)> m_func;
};