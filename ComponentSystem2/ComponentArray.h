#pragma once
#include <map>
#include <memory>
#include <vector>
#include <assert.h>

// ComponentArray���e���v���[�g�N���X�ł��邽�߁A
// �ʏ�́A�ÓI�^�C�~���O�ł����N���X���`�o���Ȃ��B�iComponentArray<T> test; �݂����ȁj
// �������A���I��ComponentArray�̌^��ύX�o����p�ɂ����������̂ŁA���̃N���X���쐬�B
struct IComponentArray
{
	virtual ~IComponentArray() = default;

	virtual std::shared_ptr<IComponentArray> Create() = 0;

	virtual bool Has(size_t id) = 0;

	virtual void Remove(size_t id) = 0;

	virtual void Move(size_t id, IComponentArray* components) = 0;
};

template<class T>
struct ComponentArray : public IComponentArray
{
	std::shared_ptr<IComponentArray> Create() override
	{
		return std::make_shared<ComponentArray<T>>();
	}

	template<typename ...Args>
	T& Add(size_t id, Args&& ...args)
	{
		// �A�N�Z�X�p�ϐ��̏�����
		m_toDense[id] = m_datas.size();
		m_toSparse.emplace_back(id);

		// �R���|�[�l���g�̐��� & ���[�ɒǉ�
		m_datas.emplace_back(std::forward<Args>(args)...);

		return m_datas.back();
	}

	T& Get(size_t id)
	{
		assert(m_toDense.contains(id));
		const auto denseIdx = m_toDense[id];
		return m_datas[denseIdx];
	}

	bool Has(size_t id) override
	{
		return m_toDense.contains(id);
	}

	void Remove(size_t id) override
	{
		// ���X�f�[�^�������ĂȂ�
		if (!m_toDense.contains(id))
		{
			return;
		}

		// ��������f�[�^�ʒu�ɖ��[�f�[�^���R�s�[�i�����������ȒP�ɂ��Ă�j
		{
			const auto denseIdx = m_toDense[id];
			const auto sparseIdx = m_toSparse.back();

			// ���ۂɃR�s�[
			m_toDense[sparseIdx] = denseIdx;
			m_toSparse[denseIdx] = sparseIdx;
			m_datas[denseIdx] = std::move(m_datas.back());
		}

		// ���[�f�[�^������
		m_datas.pop_back();
		m_toSparse.pop_back();
		m_toDense.erase(id);
	}

	void Move(size_t id, IComponentArray* ptr) override
	{
		auto components = dynamic_cast<ComponentArray*>(ptr);
		assert(components);

		// �V�����z��ɒǉ��i�f�[�^�����z���j�����̔z����̃f�[�^������
		const auto denseIdx = m_toDense[id];
		components->Add(id, m_datas[denseIdx]);
		Remove(id);
	}

public:

	// �͈�for���g�p�o����p�ɂ��邽�߂ɒ�`�B
	auto begin() { return m_datas.begin(); }
	auto end() { return m_datas.end(); }

private:

	// �R���|�[�l���g�f�[�^�z��iDenseIndex�ŃA�N�Z�X����j
	std::vector<T> m_datas;

	// DenseIndex -> ID �ɕϊ�
	std::vector<size_t> m_toSparse;

	// ID -> DenseIndex �ɕϊ�
	std::map<size_t, size_t> m_toDense;
};