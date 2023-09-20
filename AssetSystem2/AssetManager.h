#pragma once
#include <string>
#include <memory>
#include <vector>
#include "AssetProvider.h"
#include "AssetHandle.h"

/** IAssetProviderを継承したクラスのみ有効にするコンセプト */
template<class T>
concept IProvider = std::derived_from<T, IAssetProvider>;

/**
* アセットマネージャークラスは、
* システムやユーザーに共通のインターフェイスを提供するクラスです。
* 実際のロード処理やデータの保管はプロバイダーしか知りません。
*/
class AssetManager
{
public:

	template<IProvider T>
	void RegisterProvider()
	{
		m_assetProviders.emplace_back(std::make_shared<T>());
	}

	std::shared_ptr<IAssetProvider> GetProvider(const std::string& providerId) const
	{
		for (const auto& provider : m_assetProviders)
		{
			if (provider->GetProviderId() == providerId)
			{
				return provider;
			}
		}

		return std::shared_ptr<IAssetProvider>();
	}

public:

	std::shared_ptr<AssetHandle> Load(const std::string& providerId, const std::string& assetPath)
	{
		std::shared_ptr<AssetHandle> newHandle;

		// マッチするプロバイダーを取得
		auto provider = GetProvider(providerId);
		if (provider)
		{
			// プロバイダーから新しいアセットを取得する（ロードしてもらう）
			newHandle = provider->Provide(assetPath);
		}

		return newHandle;
	}

private:

	/** アセットのロード処理等を保持するプロバイダーリスト */
	std::vector<std::shared_ptr<IAssetProvider>> m_assetProviders;
};