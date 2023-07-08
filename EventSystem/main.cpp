#include "Action.h"
#include "EventManager.h"

int main()
{
	printf("=== Actionイベント ================================================================= \n");

	// C#ライクなActionクラスを使った方法のイベントシステム
	{
		Action<void> action1;
		action1.AddFunc([]() { printf("call method\n"); });
		action1.AddFunc([]() { printf("call method\n"); });
		action1.Invoke();

		Action<int> action2;
		action2.AddFunc([](int id) { printf("id = %d\n", id); });
		action2.Invoke(10);

		Action<int, int> action3;
		action3.AddFunc([](int id, int hp) { printf("id = %d hp = %d\n", id, hp); });
		action3.Invoke(10, 1000);
	}

	printf("==================================================================================== \n");
	printf("=== マネージャークラス ============================================================= \n");

	// マネージャークラスを使用した方法のイベントシステム
	{
		// タイマーの開始
		EventManager::Get().PreTick();

		EventListener event1;
		event1.SetFunction(0, [](std::any data) 
		{
			printf("call event1!\n");
		});
		event1.RegisterToEventManager();
		
		EventListener event2;
		event2.SetFunction(1, [](std::any data) 
		{
			// event1を呼ぶ
			EventManager::Get().Invoke(0, std::any());;
			printf("call event2!\n");
		});
		event2.RegisterToEventManager();
		
		// 二つイベントを呼ぶが実行されない
		EventManager::Get().Invoke(0, std::any());
		EventManager::Get().Invoke(1, std::any());
		
		// event2で呼んだイベントを処理するために二回読んでいる
		EventManager::Get().Tick();
		EventManager::Get().Tick();
	}

	printf("==================================================================================== \n");
}