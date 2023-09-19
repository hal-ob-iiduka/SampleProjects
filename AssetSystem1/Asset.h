#pragma once
#include <string>

class IAsset
{
public:

	virtual ~IAsset()
	{

	}

	/** 
	* 派生先のアセット独自の専用ロード処理を実現する。
	* ロード成功時に戻り値として、trueを返すようにする。
	*/
	virtual bool Load(const std::string& assetPath) = 0;
};