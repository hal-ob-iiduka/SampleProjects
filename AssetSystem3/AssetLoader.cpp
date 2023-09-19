#include "AssetLoader.h"
#include "LoadingThread.h"
#include "Task.h"

class AssetLoadRequest : public ITask
{
public:

	/** ロードされるアセットパス */
	std::string m_assetPath;

	/** 一連のロード処理を持つ関数オブジェクト */
	LoadAsyncDelegate m_assetLoadDelegate;

public:

	void Run() override
	{
		// 別スレッドで実行されるコンバート処理の登録など行うと、
		// コンバート処理とI/O処理を別スレッドで実行させるようにしやすいのでは？

		if (m_assetLoadDelegate)
		{
			m_assetLoadDelegate(m_assetPath);
			m_assetLoadDelegate = nullptr;
		}
	}
};

void LoadAssetAsync(const std::string& asseetPath, LoadAsyncDelegate loadAsyncDelegate)
{
	// ロード処理のリクエストオブジェクトを生成
	auto loadRequest = std::make_shared<AssetLoadRequest>();
	loadRequest->m_assetPath = asseetPath;
	loadRequest->m_assetLoadDelegate = loadAsyncDelegate;

	// 実際に別スレッドで読み込みをお願いする。
	LoadingThread::Get().AddTask(loadRequest);
}