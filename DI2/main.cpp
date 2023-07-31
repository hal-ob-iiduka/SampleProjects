#include <iostream>
#include "DIContainer.h"

class IClassB
{
public:

	virtual ~IClassB() {}
};

class ClassB : public IClassB
{
public:

	ClassB() { printf("ClassB\n"); }
};

class ClassA
{
public:

	ClassA(IClassB* classB)
	{
		m_classB = classB;
	}

	~ClassA()
	{
		delete m_classB;
		m_classB = nullptr;
	}

private:

	IClassB* m_classB;
};

int main()
{
	auto container = DIContainer();

	// ClassB、ClassA の生成時の設定を登録
	container.Register<IClassB, ClassB>();
	container.Register<ClassA>([](auto c) 
	{
		return new ClassA(c.Resolve<IClassB>()); 
	});

	// ClassA を生成（生成時に引数にはClassBが代入される）
	auto classA = container.Resolve<ClassA>();

	if (classA != nullptr)
	{
		delete classA;
		classA = nullptr;
	}
}