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
    std::shared_ptr<IAsset> Load(const std::string& assetPath) const override
    {
        // 専用ロード処理を実現しているとする。
        std::cout << "モデルデータをロードしました。\n";

        // 成功すれば正常なアセットデータを返す。
        return std::shared_ptr<IAsset>();
    }
};

int main()
{
    AssetManager assetManager;

    // プロバイダーの登録（これをしなかったら不正常な申請としてロードされない
    assetManager.RegisterProvider<ModelProvider>();

    // モデルデータをロードする。
    auto handle = assetManager.Load("Model", "Test.model");
    auto model = handle->Get<Model>();
}