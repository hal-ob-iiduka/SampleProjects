#pragma once

class GameObject;

class Component
{
public:

	Component() = delete;
	Component(GameObject* owner) { m_owner = owner; }

	virtual ~Component() {}

public:

	virtual void Init() {}

	virtual void UnInit() {}

	virtual void Update() {}

	// 消去申請を受付る関数で消去待ち状態に入ります。
	virtual void Destory()
	{
		m_requestDestroy = true;
	}

	// このコンポーネントが現在消去可能なのかを返します。
	virtual bool Erasable()
	{
		return true;
	}

	bool RequestDestroy()
	{
		return m_requestDestroy;
	}

protected:

	// 所属ゲームオブジェクト
	GameObject* m_owner = nullptr;

	// このコンポーネントが消去前であるか
	bool m_requestDestroy = false;
};