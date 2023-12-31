﻿#include <iostream>
#include <assert.h>
#include "AssetManager.h"

class Model : public IAsset
{
public:
};

class ModelProvider : public IAssetProvider
{
public:

    std::string GetProviderId() const override
    {
        return "Model";
    }

    /** モデルアセットのロード処理を実現する。*/
    std::shared_ptr<IAsset> AsyncLoad(const std::string& assetPath) const override
    {
        // 専用ロード処理を実現しているとする。
        std::cout << "モデルデータをロードしました。\n";

        // 成功すれば正常なアセットデータを返す。
        auto model = std::make_shared<Model>();
        return std::static_pointer_cast<IAsset>(model);
    }
};

int main()
{
    AssetManager assetManager;

    // プロバイダーの登録（これをしなかったら不正常な申請としてロードされない
    assetManager.RegisterProvider<ModelProvider>();

    // 下記でロード処理を実行するため、簡単に新しくロケーションの追加する。
    {
        auto location = std::make_shared<AssetLocation>();
        location->m_assetPath = "Test.model";
        location->m_providerId = "Model";
        location->m_dependencies = {}; // 今回はなし

        assetManager.AddAssetLocation("Test.model", location);
    }

    // モデルデータをロードする。
    auto handle = assetManager.Load("Test.model");

    // ロード終了するまで待機
    while (!handle->IsVaild())
    {

    }

    auto model = handle->Get<Model>();
    assert(model);
}