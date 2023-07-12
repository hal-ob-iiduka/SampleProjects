#pragma once
#include <vector>
#include <memory>

enum class NodeState
{
    Waiting, // 待機中
    Running, // 処理中
    Success, // 成功
    Failure, // 失敗
};

class Node
{
public:

    virtual ~Node() {};

    // ノード開始前に呼び出される関数
    virtual void OnEnter() = 0;

    // ノード実行中に呼び出される関数
    virtual NodeState OnUpdate() = 0;

    // ノード終了時に呼び出される関数
    virtual void OnExit() = 0;

    // ノードの実行用関数
    NodeState Execute()
    {
        if (m_state != NodeState::Running)
        {
            OnEnter();
        }

        m_state = OnUpdate();

        if (m_state != NodeState::Running)
        {
            OnExit();
        }

        return m_state;
    }

    // ノードの再初期化のための関数
    void Reset()
    {
        m_state = NodeState::Waiting;
    }

    bool IsSuccess() { return m_state == NodeState::Success; }

    bool IsFailure() { return m_state == NodeState::Failure; }

    bool IsFinished() { return IsSuccess() || IsFailure(); }

protected:

    // 現在のノード状態を保持します。
    NodeState m_state;
};

class BranchNode : public Node
{
public:

    virtual ~BranchNode() {};

    // 子ノードを追加する関数
    void AddChild(std::shared_ptr<Node> node)
    {
        m_children.push_back(node);
    }

protected:

    // オブジェクトの子ノード
    std::vector<std::shared_ptr<Node>> m_children;
};

class LeafNode : public Node
{
public:

    virtual ~LeafNode() {};
};