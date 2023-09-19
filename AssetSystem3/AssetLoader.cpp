#include "AssetLoader.h"
#include "LoadingThread.h"
#include "Task.h"

class AssetLoadRequest : public ITask
{
public:

	/** ロードされるアセットパス */
	std::string m_assetPath;

	/** 一連のロード処理を持つ関数オブジェクト */
	std::function<void(const std::string&)> m_loadFunc;

public:

	void Run() override
	{
		// 別スレッドで実行されるコンバート処理の登録など行うと、
		// コンバート処理とI/O処理を別スレッドで実行させるようにしやすいのでは？

		if (m_loadFunc)
		{
			m_loadFunc(m_assetPath);
			m_loadFunc = nullptr;
		}
	}
};

void LoadAssetAsync(const std::string& asseetPath, std::function<void(const std::string&)> loadFunc)
{
	// ロード処理のリクエストオブジェクトを生成
	auto loadRequest = std::make_shared<AssetLoadRequest>();
	loadRequest->m_assetPath = asseetPath;
	loadRequest->m_loadFunc = loadFunc;

	// 実際に別スレッドで読み込みをお願いする。
	LoadingThread::Get().AddTask(loadRequest);
}