#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Asset.h"

class AssetManager
{
public:

	template<class T>
	std::shared_ptr<T> Load(const std::string& assetPath)
	{
		// 既にロード済みデータがあるかを確認
		if (auto asset = Get<T>(assetPath))
		{
			return asset;
		}

		// データをロードする。
		auto newAsset = std::make_shared<T>();
		if (newAsset->Load(assetPath))
		{
			Add(assetPath, newAsset);
			return newAsset;
		}

		// 失敗
		return std::shared_ptr<T>();
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
};