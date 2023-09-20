#pragma once
#include <memory>
#include "Asset.h"

class IAssetProvider;

/**
* 現在は何も効果はなく、
* ただポインタをラップしているだけですが、
* これが後々めっちゃ役に立ちます。
*/
class AssetHandle
{
	friend class IAssetProvider;
public:

	/** Get関数が呼び出し可能な時、真を返します。*/
	bool IsVaild() const
	{
		return !!m_asset;
	}

public:

	template<class T>
	std::shared_ptr<T> Get() const
	{
		return std::dynamic_pointer_cast<T>(Get());
	}

	/** このハンドルが指す、アセットを返します。*/
	std::shared_ptr<IAsset> Get() const
	{
		return m_asset;
	}

private:

	/** ロード申請したマネージャークラス */
	IAssetProvider* m_ownerProvider;

	/** ロードしたアセットへのパス */
	std::shared_ptr<IAsset> m_asset;
};