#pragma once
#include <unordered_map>
#include "AssetHandle.h"

class IAsset;

struct ProvidedData
{
	/** ロードされたアセットオブジェクト */
	std::shared_ptr<IAsset> m_target;

	/** 非同期ロード中のフラグ */
	std::atomic_bool m_asyncLoading;

	/** ロード完了待ちのハンドルリスト */
	std::vector<std::shared_ptr<AssetHandle>> m_loadingHandles;

	/** 現在有効中のハンドルリスト（アセットが解放された際に通知する用） */
	std::vector<std::shared_ptr<AssetHandle>> m_activeHandles;

	void AddLoadingRequest(std::shared_ptr<AssetHandle> handle)
	{
		m_loadingHandles.emplace_back(handle);
		m_activeHandles.emplace_back(handle);
	}
};

/**
* アセットのロード、アンロード処理を実装する。
* AssetManagerに登録することで実際に使用する。
*/
class IAssetProvider
{
public:

	/** このプロバイダーを定義する一位のID、大抵の場合はアセットタイプを指す。*/
	virtual std::string GetProviderId() const = 0;

	/** 派生先のアセット独自の専用非同期ロード処理を実現する。*/
	virtual std::shared_ptr<AssetHandle> Provide(const std::string& assetPath, bool IsStartStalled = false);

	std::shared_ptr<IAsset> GetAsset(const std::string& assetPath) const
	{
		if (const auto providedData = GetProvidedData(assetPath))
		{
			return providedData->m_target;
		}

		return std::shared_ptr<IAsset>();
	}

protected:

	std::shared_ptr<ProvidedData> GetProvidedData(const std::string& assetPath) const
	{
		if (m_providedDataMap.contains(assetPath))
		{
			return m_providedDataMap.at(assetPath);
		}

		return std::shared_ptr<ProvidedData>();
	}

	/** 派生先のアセット独自の専用ロード処理を実現する。（別スレッドで実行される）*/
	virtual std::shared_ptr<IAsset> AsyncLoad(const std::string& assetPath) const = 0;

	/**
	* 実際に非同期処理を実行させる関数。
	* 既にロード済みデータがある場合、読み込みは行われず直ぐに完了イベントが実行される。
	*/
	virtual std::shared_ptr<ProvidedData> StartAsyncLoad(std::shared_ptr<AssetHandle> handle);

	/**
	* 供給データの取得を行うのが主な仕事。（新規の場合、ロード処理と共に新しく登録する）
	* 戻り値は「ロード済み」または「ロード中」必ずどちらかになる。
	*/
	virtual std::shared_ptr<ProvidedData> ProvideInternal(const std::string& assetPath);

	/** 別スレッドでのロード処理完了後のコールバック関数 */
	void AsyncLoadCallback(const std::string& assetPath, std::shared_ptr<IAsset> asset) const;

	/** 供給データ（アセット）のロード完了時の処理を提供する。*/
	virtual void CompletedLoadRequests(std::shared_ptr<ProvidedData> providedData) const;

protected:

	/** このプロバイダーがユーザー提供出来るデータへのマップ */
	std::unordered_map<std::string, std::shared_ptr<ProvidedData>> m_providedDataMap;
};