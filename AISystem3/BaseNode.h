#pragma once
#include <vector>
#include <memory>

enum class NodeState
{
    Waiting, // �ҋ@��
    Running, // ������
    Success, // ����
    Failure, // ���s
};

class Node
{
public:

    virtual ~Node() {};

    // �m�[�h�J�n�O�ɌĂяo�����֐�
    virtual void OnEnter() = 0;

    // �m�[�h���s���ɌĂяo�����֐�
    virtual NodeState OnUpdate() = 0;

    // �m�[�h�I�����ɌĂяo�����֐�
    virtual void OnExit() = 0;

    // �m�[�h�̎��s�p�֐�
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

    // �m�[�h�̍ď������̂��߂̊֐�
    void Reset()
    {
        m_state = NodeState::Waiting;
    }

    bool IsSuccess() { return m_state == NodeState::Success; }

    bool IsFailure() { return m_state == NodeState::Failure; }

    bool IsFinished() { return IsSuccess() || IsFailure(); }

protected:

    // ���݂̃m�[�h��Ԃ�ێ����܂��B
    NodeState m_state;
};

class InternalNode : public Node
{
public:

    virtual ~InternalNode() {};

    // �q�m�[�h��ǉ�����֐�
    void AddChild(std::shared_ptr<Node> node)
    {
        m_children.push_back(node);
    }

protected:

    // �I�u�W�F�N�g�̎q�m�[�h
    std::vector<std::shared_ptr<Node>> m_children;
};

class LeafNode : public Node
{
public:

    virtual ~LeafNode() {};
};