#pragma once
#include <string>
#include <memory>
#include <concepts>

class IAsset;

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
	virtual std::shared_ptr<IAsset> Provide(const std::string& assetPath) const = 0;

	template<class T>
	std::shared_ptr<T> Provide(const std::string& assetPath) const
	{
		return std::dynamic_pointer_cast<T>(Provide(assetPath));
	}
};