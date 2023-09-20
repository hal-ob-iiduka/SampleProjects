#include "AssetProvider.h"
#include "AssetHandle.h"
#include "AssetLoader.h"
#include <assert.h>

std::shared_ptr<AssetHandle> IAssetProvider::Provide(const std::string& assetPath, bool IsStartStalled /* = false */)
{
	// 新規ハンドルの生成
	auto handle = std::make_shared<AssetHandle>();
	handle->m_assetPath = assetPath;
	handle->m_ownerProvider = this;
	handle->m_completedEvent = {};

	// 任意のタイミングでロードを開始出来る等、拡張がしやすいようにしてる。
	if (!IsStartStalled)
	{
		StartAsyncLoad(handle);
	}

	return handle;
}

std::shared_ptr<ProvidedData> IAssetProvider::StartAsyncLoad(std::shared_ptr<AssetHandle> handle)
{
	std::shared_ptr<ProvidedData> providedData;

	if (handle)
	{
		providedData = ProvideInternal(handle->m_assetPath);
		assert(providedData);

		// ロード完了待ちリストへ登録
		providedData->AddLoadingRequest(handle);
	}

	// 既に既存のデータがある（ロード済み）の場合。
	// ロード失敗等の処理を考慮した方がもっと良くなる。
	if (providedData && providedData->m_target)
	{
		CompletedLoadRequests(providedData);
	}

	return providedData;
}

std::shared_ptr<ProvidedData> IAssetProvider::ProvideInternal(const std::string& assetPath)
{
	auto providedData = GetProvidedData(assetPath);
	if (providedData)
	{
		// m_target == null => アンロードされてる
		// ↑の際、再度ロード処理を実行する必要がある。
		if (providedData->m_target)
		{
			return providedData;
		}
	}
	else
	{
		providedData = std::make_shared<ProvidedData>();
		m_providedDataMap.emplace(assetPath, providedData);
	}

	// ロード処理が登録されていない。
	if (!providedData->m_asyncLoading)
	{
		providedData->m_asyncLoading = true;

		// 非同期ロードの登録を行う。
		LoadAssetAsync(assetPath, [this](auto assetPath) 
		{
			auto newAsset = AsyncLoad(assetPath);
			if (newAsset)
			{
				AsyncLoadCallback(assetPath, newAsset);
			}
		});
	}

	return providedData;
}

void IAssetProvider::AsyncLoadCallback(const std::string& assetPath, std::shared_ptr<IAsset> asset)
{
	auto providedData = GetProvidedData(assetPath);
	if (providedData)
	{
		// ロードが終了したことを通知
		providedData->m_asyncLoading = false;

		if (!providedData->m_target)
		{
			// ロードされたオブジェクトをキャッシュ
			providedData->m_target = asset;
		}

		// 完了を通知
		CompletedLoadRequests(providedData);
	}
}

void IAssetProvider::CompletedLoadRequests(std::shared_ptr<ProvidedData> providedData) const
{
	if (providedData)
	{
		// 完了待ちハンドルへ通知
		for (auto& handle : providedData->m_loadingHandles)
		{
			handle->CompleteLoad();
		}

		providedData->m_loadingHandles.clear();
		providedData->m_loadingHandles.shrink_to_fit();
	}
}