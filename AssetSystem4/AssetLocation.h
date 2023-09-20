#pragma once
#include <string>
#include <vector>

struct AssetLocation
{
	/** このアセットのデータ */
	std::string m_assetPath;

	/** このアセットのデータを使用するプロバイダーID */
	std::string m_providerId;

	/** このアセットが依存する各アセットリスト */
	std::vector<std::shared_ptr<AssetLocation>> m_dependencies;
};