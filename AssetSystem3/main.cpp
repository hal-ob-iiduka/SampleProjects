#include <iostream>
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

    // モデルデータをロードする。
    auto model = assetManager.Load("Model", "Test.model");

    // ロード終了するまで待機
    while (!model->IsVaild())
    {

    }
}