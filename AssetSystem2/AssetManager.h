#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "AssetProvider.h"
#include "Asset.h"

/** IAssetProviderを継承したクラスのみ有効にするコンセプト */
template<class T>
concept IProvider = std::derived_from<T, IAssetProvider>;

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

	template<class T>
	std::shared_ptr<T> Load(const std::string& providerId, const std::string& assetPath)
	{
		// 既にロード済みデータがあるかを確認
		if (auto asset = Get<T>(assetPath))
		{
			return asset;
		}

		std::shared_ptr<T> newAsset;

		// マッチするプロバイダーを取得
		auto provider = GetProvider(providerId);
		if (provider)
		{
			// プロバイダーから新しいアセットを取得する（ロードしてもらう）
			newAsset = provider->Provide<T>(assetPath);
		}
		
		if (newAsset)
		{
			// 新しいアセットとしてマップへ登録
			Add(assetPath, newAsset);
		}

		return newAsset;
	}

	template<class T>
	std::shared_ptr<T> Get(const std::string& assetPath) const
	{
		if (m_assetMap.contains(assetPath))
		{
			// shared_ptr用のダイナミックキャストを使用
			return std::dynamic_pointer_cast<T>(m_assetMap.at(assetPath));
		}

		return std::shared_ptr<T>();
	}

private:

	/** アセットの登録（既に登録済みの場合は考慮していない） */
	void Add(const std::string& assetPath, std::shared_ptr<IAsset> asset)
	{
		m_assetMap.emplace(assetPath, asset);
	}

private:

	/** ロード済みアセットマップ */
	std::unordered_map<std::string, std::shared_ptr<IAsset>> m_assetMap;

	/** アセットのロード処理等を保持するプロバイダーリスト */
	std::vector<std::shared_ptr<IAssetProvider>> m_assetProviders;
};