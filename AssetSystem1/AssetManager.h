#pragma once
#include <memory>
#include <unordered_map>
#include "Asset.h"

class AssetManager
{
public:

	template<class T>
	std::shared_ptr<T> Load(const std::string& assetPath)
	{
		// ���Ƀ��[�h�ς݃f�[�^�����邩���m�F
		if (auto asset = Get<T>(assetPath))
		{
			return asset;
		}

		// �f�[�^�����[�h����B
		auto newAsset = std::make_shared<T>();
		if (newAsset->Load(assetPath))
		{
			Add(assetPath, newAsset);
			return newAsset;
		}

		// ���s
		return std::shared_ptr<T>();
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
};