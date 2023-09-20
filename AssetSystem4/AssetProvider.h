#pragma once
#include <unordered_map>
#include "AssetHandle.h"

class IAsset;

struct ProvidedData
{
	/** ���[�h���ꂽ�A�Z�b�g�I�u�W�F�N�g */
	std::shared_ptr<IAsset> m_target;

	/** �񓯊����[�h���̃t���O */
	std::atomic_bool m_asyncLoading;

	/** ���[�h�����҂��̃n���h�����X�g */
	std::vector<std::shared_ptr<AssetHandle>> m_loadingHandles;

	/** ���ݗL�����̃n���h�����X�g�i�A�Z�b�g��������ꂽ�ۂɒʒm����p�j */
	std::vector<std::shared_ptr<AssetHandle>> m_activeHandles;

	void AddLoadingRequest(std::shared_ptr<AssetHandle> handle)
	{
		m_loadingHandles.emplace_back(handle);
		m_activeHandles.emplace_back(handle);
	}
};

/**
* �A�Z�b�g�̃��[�h�A�A�����[�h��������������B
* AssetManager�ɓo�^���邱�ƂŎ��ۂɎg�p����B
*/
class IAssetProvider
{
public:

	/** ���̃v���o�C�_�[���`�����ʂ�ID�A���̏ꍇ�̓A�Z�b�g�^�C�v���w���B*/
	virtual std::string GetProviderId() const = 0;

	/** �h����̃A�Z�b�g�Ǝ��̐�p�񓯊����[�h��������������B*/
	virtual std::shared_ptr<AssetHandle> Provide(const std::string& assetPath, bool IsStartStalled = false);

	std::shared_ptr<IAsset> GetAsset(const std::string& assetPath) const
	{
		if (const auto providedData = GetProvidedData(assetPath))
		{
			return providedData->m_target;
		}

		return std::shared_ptr<IAsset>();
	}

protected:

	std::shared_ptr<ProvidedData> GetProvidedData(const std::string& assetPath) const
	{
		if (m_providedDataMap.contains(assetPath))
		{
			return m_providedDataMap.at(assetPath);
		}

		return std::shared_ptr<ProvidedData>();
	}

	/** �h����̃A�Z�b�g�Ǝ��̐�p���[�h��������������B�i�ʃX���b�h�Ŏ��s�����j*/
	virtual std::shared_ptr<IAsset> AsyncLoad(const std::string& assetPath) const = 0;

	/**
	* ���ۂɔ񓯊����������s������֐��B
	* ���Ƀ��[�h�ς݃f�[�^������ꍇ�A�ǂݍ��݂͍s��ꂸ�����Ɋ����C�x���g�����s�����B
	*/
	virtual std::shared_ptr<ProvidedData> StartAsyncLoad(std::shared_ptr<AssetHandle> handle);

	/**
	* �����f�[�^�̎擾���s���̂���Ȏd���B�i�V�K�̏ꍇ�A���[�h�����Ƌ��ɐV�����o�^����j
	* �߂�l�́u���[�h�ς݁v�܂��́u���[�h���v�K���ǂ��炩�ɂȂ�B
	*/
	virtual std::shared_ptr<ProvidedData> ProvideInternal(const std::string& assetPath);

	/** �ʃX���b�h�ł̃��[�h����������̃R�[���o�b�N�֐� */
	void AsyncLoadCallback(const std::string& assetPath, std::shared_ptr<IAsset> asset) const;

	/** �����f�[�^�i�A�Z�b�g�j�̃��[�h�������̏�����񋟂���B*/
	virtual void CompletedLoadRequests(std::shared_ptr<ProvidedData> providedData) const;

protected:

	/** ���̃v���o�C�_�[�����[�U�[�񋟏o����f�[�^�ւ̃}�b�v */
	std::unordered_map<std::string, std::shared_ptr<ProvidedData>> m_providedDataMap;
};