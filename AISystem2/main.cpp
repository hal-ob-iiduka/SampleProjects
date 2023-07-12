#include <iostream>
#include "StateMachine.h"

class Player;

class IdleState : public State<Player>
{
public:

    IdleState(Player* context) : State(context) {}

    void OnEnter()  override { printf("OnEnter Idle\n"); }
    void OnUpdate() override { printf("OnUpdate Idle\n"); }
    void OnExit()   override { printf("OnExit Idle\n"); }
};

class JumpState : public State<Player>
{
public:

    JumpState(Player* context) : State(context) {}

    void OnEnter()  override { printf("OnEnter Jump\n"); }
    void OnUpdate() override { printf("OnUpdate Jump\n"); }
    void OnExit()   override { printf("OnExit Jump\n"); }
};

// 状態遷移の発動トリガー
enum EventTrigger
{
    ToJump
};

class RootState : public State<Player>
{
public:

    RootState(Player* context) : State(context)
    {
        // 使用する状態の追加
        AddChildState<IdleState>();
        AddChildState<JumpState>();

        // IdleStateからJumpStateに遷移する条件を登録
        AddChildTransition<IdleState, JumpState>(EventTrigger::ToJump);

        // 最初のステートをセット
        SetCurrentState<IdleState>();
    }

    void OnEnter()  override { printf("OnEnter Root\n"); }
    void OnUpdate() override { printf("OnUpdate Root\n"); }
    void OnExit()   override { printf("OnExit Root\n"); }
};

class Player
{
public:

    Player() : m_stateMachine(this)
    {
        // 使用する状態の追加
        m_stateMachine.AddState<RootState>();

        // 最初のステートをセット
        m_stateMachine.SetStartState<RootState>();
    }

    void Update()
    {
        // IdleStateの更新関数が呼ばれる。
        m_stateMachine.Update();

        // IdleState -> JumpState へ状態遷移させるトリガーを送信。
        m_stateMachine.SendTrigger(EventTrigger::ToJump);

        // 状態が変わったため、JumpStateの更新関数が呼ばれる。
        m_stateMachine.Update();
    }

private:

    // ステート管理クラス
	StateMachine<Player> m_stateMachine;
};

int main()
{
    // テスト用に一回更新関数を呼ぶ
    Player player;
    player.Update();
}