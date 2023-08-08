#pragma once

// ただのIDだけを保持（Unityでいうゲームオブジェクトに相当）
using Entity = size_t;

constexpr Entity null_entity{ std::numeric_limits<Entity>::max() };