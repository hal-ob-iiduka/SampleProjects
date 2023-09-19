#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "AssetProvider.h"
#include "Asset.h"

/** IAssetProvider���p�������N���X�̂ݗL���ɂ���R���Z�v�g */
template<class T>
concept IProvider = std::derived_from<T, IAssetProvider>;

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

	template<class T>
	std::shared_ptr<T> Load(const std::string& providerId, const std::string& assetPath)
	{
		// ���Ƀ��[�h�ς݃f�[�^�����邩���m�F
		if (auto asset = Get<T>(assetPath))
		{
			return asset;
		}

		std::shared_ptr<T> newAsset;

		// �}�b�`����v���o�C�_�[���擾
		auto provider = GetProvider(providerId);
		if (provider)
		{
			// �v���o�C�_�[����V�����A�Z�b�g���擾����i���[�h���Ă��炤�j
			newAsset = provider->Provide<T>(assetPath);
		}
		
		if (newAsset)
		{
			// �V�����A�Z�b�g�Ƃ��ă}�b�v�֓o�^
			Add(assetPath, newAsset);
		}

		return newAsset;
	}

	template<class T>
	std::shared_ptr<T> Get(const std::string& assetPath) const
	{
		if (m_assetMap.contains(assetPath))
		{
			// shared_ptr�p�̃_�C�i�~�b�N�L���X�g���g�p
			return std::dynamic_pointer_cast<T>(m_assetMap.at(assetPath));
		}

		return std::shared_ptr<T>();
	}

private:

	/** �A�Z�b�g�̓o�^�i���ɓo�^�ς݂̏ꍇ�͍l�����Ă��Ȃ��j */
	void Add(const std::string& assetPath, std::shared_ptr<IAsset> asset)
	{
		m_assetMap.emplace(assetPath, asset);
	}

private:

	/** ���[�h�ς݃A�Z�b�g�}�b�v */
	std::unordered_map<std::string, std::shared_ptr<IAsset>> m_assetMap;

	/** �A�Z�b�g�̃��[�h��������ێ�����v���o�C�_�[���X�g */
	std::vector<std::shared_ptr<IAssetProvider>> m_assetProviders;
};