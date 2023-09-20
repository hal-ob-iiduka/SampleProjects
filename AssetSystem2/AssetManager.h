#pragma once
#include <vector>
#include <concepts>
#include "AssetProvider.h"

/** IAssetProvider���p�������N���X�̂ݗL���ɂ���R���Z�v�g */
template<class T>
concept IProvider = std::derived_from<T, IAssetProvider>;

/**
* �A�Z�b�g�}�l�[�W���[�N���X�́A
* �V�X�e���⃆�[�U�[�ɋ��ʂ̃C���^�[�t�F�C�X��񋟂���N���X�ł��B
* ���ۂ̃��[�h������f�[�^�̕ۊǂ̓v���o�C�_�[�����m��܂���B
*/
class AssetManager
{
public:

	template<IProvider T>
	void RegisterProvider()
	{
		m_assetProviders.emplace_back(std::make_shared<T>());
	}

	std::shared_ptr<IAssetProvider> GetProvider(const std::string& providerId) const
	{
		for (const auto& provider : m_assetProviders)
		{
			if (provider->GetProviderId() == providerId)
			{
				return provider;
			}
		}

		return std::shared_ptr<IAssetProvider>();
	}

public:

	std::shared_ptr<AssetHandle> Load(const std::string& providerId, const std::string& assetPath) const
	{
		std::shared_ptr<AssetHandle> newHandle;

		// �}�b�`����v���o�C�_�[���擾
		auto provider = GetProvider(providerId);
		if (provider)
		{
			// �v���o�C�_�[����V�����A�Z�b�g���擾����i���[�h���Ă��炤�j
			newHandle = provider->Provide(assetPath);
		}

		return newHandle;
	}

private:

	/** �A�Z�b�g�̃��[�h��������ێ�����v���o�C�_�[���X�g */
	std::vector<std::shared_ptr<IAssetProvider>> m_assetProviders;
};