#pragma once
#include <concepts>
#include <unordered_map>
#include "AssetLocation.h"
#include "AssetProvider.h"

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

	std::shared_ptr<AssetHandle> Load(const std::string& assetPath) const
	{
		if (auto location = GetAssetLocation(assetPath))
		{
			return Load(location);
		}

		std::shared_ptr<AssetHandle>();
	}

	std::shared_ptr<AssetHandle> Load(std::shared_ptr<AssetLocation> assetLocation) const
	{
		// 依存関係をロードリストに登録する
		for (auto& dependence : assetLocation->m_dependencies)
		{
			Load(dependence);
		}

		std::shared_ptr<AssetHandle> newHandle;

		// マッチするプロバイダーを取得
		auto provider = GetProvider(assetLocation->m_providerId);
		if (provider)
		{
			// プロバイダーから新しいアセットを取得する（ロードしてもらう）
			newHandle = provider->Provide(assetLocation->m_assetPath);
		}

		return newHandle;
	}

public:

	void AddAssetLocation(const std::string& assetPath, std::shared_ptr<AssetLocation> assetLocation)
	{
		if (m_assetLocationMap.contains(assetPath))
		{
			return;
		}

		m_assetLocationMap.emplace(assetPath, assetLocation);
	}

	std::shared_ptr<AssetLocation> GetAssetLocation(const std::string& assetPath) const
	{
		if (m_assetLocationMap.contains(assetPath))
		{
			return m_assetLocationMap.at(assetPath);
		}

		return std::shared_ptr<AssetLocation>();
	}

private:

	/** アセットのロード処理等を保持するプロバイダーリスト */
	std::vector<std::shared_ptr<IAssetProvider>> m_assetProviders;

	/** アセットパスから依存関係などを持つ、ロケーションへのマップ */
	std::unordered_map<std::string, std::shared_ptr<AssetLocation>> m_assetLocationMap;
};