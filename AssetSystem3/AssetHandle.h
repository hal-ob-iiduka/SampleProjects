#pragma once
#include <string>
#include <memory>
#include <atomic>
#include "Event.h"
#include "Asset.h"

class IAssetProvider;

class AssetHandle
{
	friend class IAssetProvider;
public:

	/** Get関数が呼び出し可能な時、真を返します。*/
	bool IsVaild() const
	{
		return m_isCompleteLoad.load();
	}

	const std::string& GetAssetPath() const
	{
		return m_assetPath;
	}

public:

	template<class T>
	std::shared_ptr<T> Get() const
	{
		return std::dynamic_pointer_cast<T>(Get());
	}

	/** このハンドルが指す、アセットを返します。*/
	std::shared_ptr<IAsset> Get() const;

	/** ロードが既に終了している場合は考慮してません。*/
	template<class Func>
	void BindCompletedEvent(Func&& func)
	{
		if (!IsVaild())
		{
			m_completedEvent.Add(std::move(func));
		}
	}

private:

	/** 登録された全ての完了イベントを処理 */
	void CompleteLoad()
	{
		if (!IsVaild())
		{
			m_isCompleteLoad = true;

			// コンプリートイベントを実行
			m_completedEvent.Execute();
			m_completedEvent.Clear();
		}
	}

private:

	/** ロードが終了し、アクセス可能なら真になる。 */
	std::atomic_bool m_isCompleteLoad;

	/** ロード申請したマネージャークラス */
	IAssetProvider* m_ownerProvider;

	/** ロードしたアセットへのパス */
	std::string m_assetPath;

	/** ロード完了時に呼ばれるイベント */
	DECLARE_EVENT(CompletedEvent)
	CompletedEvent m_completedEvent;
};