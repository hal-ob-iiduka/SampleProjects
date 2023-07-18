#include <iostream>
#include "BehaviorTree.h"
#include "SequencerNode.h"
#include "SelectorNode.h"
#include "DecoratorNode.h"
#include "ActionNode.h"

int main()
{
    // ランダム値を使用するのでシード値を初期化
    std::srand(std::time(nullptr));

    // 今回は買い物にいくビヘイビアツリーを実装します。
    int money = 220;
    BehaviorTree behaviorTree;

    // ルートノードを初期化
    auto rootNode = std::make_shared<SequencerNode>();
    behaviorTree.SetRootNode(rootNode);

    // 家を出てコンビニを目指す。
    auto action1 = std::make_shared<ActionNode>([]()
    {
        printf("家を出、近くのコンビニへ行く。\n");
        return NodeState::Success;
    });

    rootNode->AddChild(action1);

    // 近くのコンビニか自販機へ向かいます。
    auto selector = std::make_shared<SelectorNode>();
    rootNode->AddChild(selector);

    // 選択にはランダム値を使用します。
    auto decorator1 = std::make_shared<DecoratorNode>([&]()
    {
        return std::rand() % 2;
    });

    selector->AddChild(decorator1);

    // コンビニに向かった。
    auto action2 = std::make_shared<ActionNode>([&]()
    {
        printf("今日の気分でコンビニに行った。\n");
        return NodeState::Success;
    });

    decorator1->AddChild(action2);

    // 自販機に向かった。
    auto action3 = std::make_shared<ActionNode>([&]()
    {
        printf("今日の気分で自販機に行った。\n");
        return NodeState::Success;
    });

    selector->AddChild(action3);

    // 買い物ができるかを調べる。
    constexpr int PRICE = 220;
    auto decorator2 = std::make_shared<DecoratorNode>([&]()
    {
        return money >= PRICE;
    });

    rootNode->AddChild(decorator2);

    // エナジードリンクを購入する。
    auto action4 = std::make_shared<ActionNode>([&]()
    {
        printf("エナジードリンクを購入。\n");
        money -= PRICE;
        printf("残り金額は%dです。\n", money);
        return NodeState::Success;
    });

    decorator2->AddChild(action4);

    // メインループ
    while (true)
    {
        // ツリーの更新
        behaviorTree.Update();

        // 更新が終了したら終了する。
        if (behaviorTree.IsFinished())
        {
            break;
        }
    }
}