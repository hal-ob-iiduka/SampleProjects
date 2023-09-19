﻿#include <iostream>
#include "AssetManager.h"
#include "AssetProvider.h"

class ModelProvider : public IAssetProvider
{
public:

    std::string GetProviderId() const override
    {
        return "Model";
    }

    /** モデルアセットのロード処理を実現する。*/
    std::shared_ptr<IAsset> Provide(const std::string& assetPath) const override
    {
        // 専用ロード処理を実現しているとする。
        std::cout << "モデルデータをロードしました。\n";

        // 成功すれば正常なアセットデータを返す。
        return std::shared_ptr<IAsset>();
    }
};

class Model : public IAsset
{
public:
};

int main()
{
    AssetManager assetManager;

    // プロバイダーの登録（これをしなかったら不正常な申請としてロードされない
    assetManager.RegisterProvider<ModelProvider>();

    // モデルデータをロードする。
    auto model = assetManager.Load<Model>("Model", "Test.model");
}