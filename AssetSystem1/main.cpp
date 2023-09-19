#include <iostream>
#include "AssetManager.h"

class Model : public IAsset
{
public:

    bool Load(const std::string& assetPath) override
    {
        // 専用ロード処理を実現しているとする。
        std::cout << "モデルデータをロードしました。\n";

        // 成功した
        return true;
    }
};

int main()
{
    AssetManager assetManager;

    // モデルデータをロードする。
    auto model = assetManager.Load<Model>("Test.model");
}