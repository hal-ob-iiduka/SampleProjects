#include <iostream>
#include <assert.h>
#include "GameObject.h"

class Player : public GameObject
{
public:

};

class Transform : public Component
{
public:

	Transform(GameObject* owner) : Component(owner)
	{
		printf("Init\n");
	}

	~Transform()
	{
		printf("Destroy\n");
	}
};

int main()
{
	auto player = Player();

	// コンポーネントの追加＆取得（正常な値かをチェックしている）
	assert(!player.AddComponent<Transform>().expired());
	assert(!player.GetComponent<Transform>().expired());

	printf("Remove Transform\n");

	// コンポーネントの消去（実際には消去されていない）
	player.RemoveComponent<Transform>();

	// コンポーネントは消去されてないが、取得不可
	assert(player.GetComponent<Transform>().expired());

	printf("Player Update\n");

	// ここで初めてコンポーネントが消去される
	player.Update();

	// 取得不可
	assert(player.GetComponent<Transform>().expired());
}