#pragma once
#include <unordered_map>
#include <assert.h>
#include "AssetHandle.h"

/**
* �A�Z�b�g�̃��[�h�A�A�����[�h��������������B
* AssetManager�ɓo�^���邱�ƂŎ��ۂɎg�p����B
*/
class IAssetProvider
{
public:

	/** ���̃v���o�C�_�[���`�����ʂ�ID�A���̏ꍇ�̓A�Z�b�g�^�C�v���w���B*/
	virtual std::string GetProviderId() const = 0;

	/** �h����̃A�Z�b�g�Ǝ��̐�p���[�h��������������B*/
	virtual std::shared_ptr<AssetHandle> Provide(const std::string& assetPath)
	{
		// ���Ƀ��[�h�ς݃f�[�^���擾
		auto asset = GetAsset(assetPath);
		if (!asset)
		{
			// ����������V�������[�h���Ă���
			asset = StartLoad(assetPath);
		}

		// �V�K�n���h���̐���
		auto handle = std::make_shared<AssetHandle>();
		handle->m_ownerProvider = this;
		handle->m_asset = asset;

		return handle;
	}

protected:

	/** �h����̃A�Z�b�g�Ǝ��̐�p���[�h��������������B*/
	virtual std::shared_ptr<IAsset> Load(const std::string& assetPath) const = 0;

	/**
	* ���ۂɃ��[�h���������s������֐��B
	* ���[�h����������ƁA�V�����L���b�V���}�b�v�֓o�^����B
	*/
	virtual std::shared_ptr<IAsset> StartLoad(const std::string& assetPath)
	{
		auto asset = Load(assetPath);
		if (asset)
		{
			AddAsset(assetPath, asset);
		}
		return asset;
	}

private:

	/** ���[�h���ꂽ�A�Z�b�g�̎擾 */
	std::shared_ptr<IAsset> GetAsset(const std::string& assetPath) const
	{
		if (m_assetMap.contains(assetPath))
		{
			return m_assetMap.at(assetPath);
		}

		return std::shared_ptr<IAsset>();
	}

	/** �A�Z�b�g�̓o�^�i���ɓo�^�ς݂̏ꍇ�͍l�����Ă��Ȃ��j */
	void AddAsset(const std::string& assetPath, std::shared_ptr<IAsset> asset)
	{
		m_assetMap.emplace(assetPath, asset);
	}

private:

	/** ���[�h�ς݃A�Z�b�g�}�b�v */
	std::unordered_map<std::string, std::shared_ptr<IAsset>> m_assetMap;
};