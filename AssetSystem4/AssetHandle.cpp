#include "AssetHandle.h"
#include "AssetProvider.h"

std::shared_ptr<IAsset> AssetHandle::Get() const
{
	if (IsVaild())
	{
		return m_ownerProvider->GetAsset(m_assetPath);
	}

	return std::shared_ptr<IAsset>();
}