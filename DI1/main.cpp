#include <iostream>

// クラスAに依存させるインターフェイスクラス
class IClassB
{
public:

    virtual ~IClassB()
    {

    }
};

// 実際の実装を持つクラスB
class ClassB : public IClassB
{

};

class ClassA
{
public:

    // 外部から与えることで、クラスBの実装に依存させない。
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

    // インターフェイスを継承する全てのクラスに対応させている。
    IClassB* m_classB;
};

int main()
{
    // ここを書換えるだけでクラスAの動作を変更することが出来る。
    auto classA = ClassA(new ClassB);
}