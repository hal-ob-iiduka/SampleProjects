#include "AssetLoader.h"
#include "LoadingThread.h"
#include "Task.h"

class AssetLoadRequest : public ITask
{
public:

	/** ���[�h�����A�Z�b�g�p�X */
	std::string m_assetPath;

	/** ��A�̃��[�h���������֐��I�u�W�F�N�g */
	LoadAsyncDelegate m_assetLoadDelegate;

public:

	void Run() override
	{
		// �ʃX���b�h�Ŏ��s�����R���o�[�g�����̓o�^�ȂǍs���ƁA
		// �R���o�[�g������I/O������ʃX���b�h�Ŏ��s������悤�ɂ��₷���̂ł́H

		if (m_assetLoadDelegate)
		{
			m_assetLoadDelegate(m_assetPath);
			m_assetLoadDelegate = nullptr;
		}
	}
};

void LoadAssetAsync(const std::string& asseetPath, LoadAsyncDelegate loadAsyncDelegate)
{
	// ���[�h�����̃��N�G�X�g�I�u�W�F�N�g�𐶐�
	auto loadRequest = std::make_shared<AssetLoadRequest>();
	loadRequest->m_assetPath = asseetPath;
	loadRequest->m_assetLoadDelegate = loadAsyncDelegate;

	// ���ۂɕʃX���b�h�œǂݍ��݂����肢����B
	LoadingThread::Get().AddTask(loadRequest);
}