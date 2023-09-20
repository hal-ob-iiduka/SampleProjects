#pragma once
#include <string>
#include <memory>
#include <concepts>
#include <unordered_map>
#include <assert.h>
#include "Asset.h"

class AssetHandle;

/**
* アセットのロード、アンロード処理を実装する。
* AssetManagerに登録することで実際に使用する。
*/
class IAssetProvider
{
public:

	/** このプロバイダーを定義する一位のID、大抵の場合はアセットタイプを指す。*/
	virtual std::string GetProviderId() const = 0;

	/** 派生先のアセット独自の専用ロード処理を実現する。*/
	virtual std::shared_ptr<AssetHandle> Provide(const std::string& assetPath)
	{
		// 既にロード済みデータを取得
		auto asset = GetAsset(assetPath);
		if (!asset)
		{
			// 無かったら新しくロードしてくる
			asset = StartLoad(assetPath);
		}

		// 新規ハンドルの生成
		auto handle = std::make_shared<AssetHandle>();
		handle->m_ownerProvider = this;
		handle->m_asset = asset;

		return handle;
	}

protected:

	/** 派生先のアセット独自の専用ロード処理を実現する。*/
	virtual std::shared_ptr<IAsset> Load(const std::string& assetPath) const = 0;

	/**
	* 実際にロード処理を実行させる関数。
	* ロードが完了すると、新しくキャッシュマップへ登録する。
	*/
	virtual std::shared_ptr<IAsset> StartLoad(const std::string& assetPath)
	{
		auto asset = Load(assetPath);
		if (asset)
		{
			AddAsset(assetPath, asset);
		}
		return asset;
	}

private:

	/** ロードされたアセットの取得 */
	std::shared_ptr<IAsset> GetAsset(const std::string& assetPath) const
	{
		if (m_assetMap.contains(assetPath))
		{
			return m_assetMap.at(assetPath);
		}

		return std::shared_ptr<IAsset>();
	}

	/** アセットの登録（既に登録済みの場合は考慮していない） */
	void AddAsset(const std::string& assetPath, std::shared_ptr<IAsset> asset)
	{
		m_assetMap.emplace(assetPath, asset);
	}

private:

	/** ロード済みアセットマップ */
	std::unordered_map<std::string, std::shared_ptr<IAsset>> m_assetMap;
};