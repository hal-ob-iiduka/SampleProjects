#include "AssetLoader.h"
#include "LoadingThread.h"
#include "Task.h"

class AssetLoadRequest : public ITask
{
public:

	/** ���[�h�����A�Z�b�g�p�X */
	std::string m_assetPath;

	/** ��A�̃��[�h���������֐��I�u�W�F�N�g */
	std::function<void(const std::string&)> m_loadFunc;

public:

	void Run() override
	{
		// �ʃX���b�h�Ŏ��s�����R���o�[�g�����̓o�^�ȂǍs���ƁA
		// �R���o�[�g������I/O������ʃX���b�h�Ŏ��s������悤�ɂ��₷���̂ł́H

		if (m_loadFunc)
		{
			m_loadFunc(m_assetPath);
			m_loadFunc = nullptr;
		}
	}
};

void LoadAssetAsync(const std::string& asseetPath, std::function<void(const std::string&)> loadFunc)
{
	// ���[�h�����̃��N�G�X�g�I�u�W�F�N�g�𐶐�
	auto loadRequest = std::make_shared<AssetLoadRequest>();
	loadRequest->m_assetPath = asseetPath;
	loadRequest->m_loadFunc = loadFunc;

	// ���ۂɕʃX���b�h�œǂݍ��݂����肢����B
	LoadingThread::Get().AddTask(loadRequest);
}