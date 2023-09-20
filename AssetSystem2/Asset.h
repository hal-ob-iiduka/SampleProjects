#pragma once

/**
* 派生クラスでは、
* 各専用のデータのみを保持する。
*/
class IAsset
{
public:

	virtual ~IAsset()
	{

	}

	// ロード処理がなくなるためスッキリする。
};