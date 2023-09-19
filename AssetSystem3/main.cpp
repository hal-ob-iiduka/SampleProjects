#include <iostream>
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
    std::shared_ptr<IAsset> AsyncLoad(const std::string& assetPath) override
    {
        // 専用ロード処理を実現しているとする。
        std::cout << "ロード中\n";

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

    // モデルデータをロードする。
    auto handle = assetManager.Load("Model", "Test.model");

    handle->AddCompletedEvent([] 
    {
        std::cout << "完了！\n";
    });

    // ロード終了するまで待機
    while (!handle->IsVaild())
    {

    }

    auto model = handle->Get<Model>();
    assert(model);
}