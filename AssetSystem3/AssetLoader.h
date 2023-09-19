#pragma once
#include <string>
#include "Event.h"

/**
* 非同期ロード時の実行中に呼び出されるデリゲート
*/
DECLARE_DELEGATE_PRAM_1(LoadAsyncDelegate, const std::string&)

/** 
* 第一引数には、ロードするアセットパスを、
* 第二引数には、ロード関数を指定します。
* 
* このような実装にするのはいくつか理由があります。
* このサンプルを拡張していくと考えた際、I/O処理とコンバート処理を別スレッドで実装する場合など想像すると
* 今回の実装だと結構作りやすいんではないでしょうか。（もっと良い方法はあると思いますが）
*/
void LoadAssetAsync(const std::string& asseetPath, LoadAsyncDelegate loadAsyncDelegate = LoadAsyncDelegate());