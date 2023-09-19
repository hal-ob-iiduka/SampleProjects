#pragma once
#include <string>
#include <memory>
#include <atomic>
#include "Event.h"
#include "Asset.h"

class IAssetProvider;

class AssetHandle
{
	friend class IAssetProvider;
public:

	/** Get�֐����Ăяo���\�Ȏ��A�^��Ԃ��܂��B*/
	bool IsVaild() const
	{
		return m_isCompleteLoad.load();
	}

	const std::string& GetAssetPath() const
	{
		return m_assetPath;
	}

public:

	template<class T>
	std::shared_ptr<T> Get() const
	{
		return std::dynamic_pointer_cast<T>(Get());
	}

	/** ���̃n���h�����w���A�A�Z�b�g��Ԃ��܂��B*/
	std::shared_ptr<IAsset> Get() const;

	/** ���[�h�����ɏI�����Ă���ꍇ�͍l�����Ă܂���B*/
	template<typename Func>
	void AddCompletedEvent(Func&& func)
	{
		if (!IsVaild())
		{
			m_completedEvent.Add(std::move(func));
		}
	}

private:

	/** �o�^���ꂽ�S�Ă̊����C�x���g������ */
	void CompleteLoad()
	{
		if (!IsVaild())
		{
			m_isCompleteLoad = true;

			// �R���v���[�g�C�x���g�����s
			m_completedEvent.Execute();
			m_completedEvent.Clear();
		}
	}

private:

	/** ���[�h���I�����A�A�N�Z�X�\�Ȃ�^�ɂȂ�B */
	std::atomic_bool m_isCompleteLoad;

	/** ���[�h�\�������}�l�[�W���[�N���X */
	IAssetProvider* m_ownerProvider;

	/** ���[�h�����A�Z�b�g�ւ̃p�X */
	std::string m_assetPath;

	/** ���[�h�������ɌĂ΂��C�x���g */
	DECLARE_EVENT(CompletedEvent)
	CompletedEvent m_completedEvent;
};