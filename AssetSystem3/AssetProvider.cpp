#include "AssetProvider.h"
#include "AssetHandle.h"
#include "AssetLoader.h"
#include <assert.h>

std::shared_ptr<AssetHandle> IAssetProvider::Provide(const std::string& assetPath, bool IsStartStalled)
{
	// �V�K�n���h���̐���
	auto handle = std::make_shared<AssetHandle>();
	handle->m_assetPath = assetPath;
	handle->m_ownerProvider = this;
	handle->m_completedEvent = {};

	// �C�ӂ̃^�C�~���O�Ń��[�h���J�n�o���铙�A�g�������₷���悤�ɂ��Ă�B
	if (!IsStartStalled)
	{
		StartAsyncLoad(handle);
	}

	return handle;
}

std::shared_ptr<ProvidedData> IAssetProvider::StartAsyncLoad(std::shared_ptr<AssetHandle> handle)
{
	std::shared_ptr<ProvidedData> providedData;

	if (handle)
	{
		providedData = ProvideInternal(handle->m_assetPath);
		assert(providedData);

		// ���[�h�����҂����X�g�֓o�^
		providedData->AddLoadingRequest(handle);
	}

	// ���Ɋ����̃f�[�^������i���[�h�ς݁j�̏ꍇ�B
	// ���[�h���s���̏������l���������������Ɨǂ��Ȃ�B
	if (providedData && providedData->m_target)
	{
		CompletedLoadRequests(providedData);
	}

	return providedData;
}

std::shared_ptr<ProvidedData> IAssetProvider::ProvideInternal(const std::string& assetPath)
{
	auto providedData = GetProvidedData(assetPath);
	if (providedData)
	{
		// m_target == null => �A�����[�h����Ă�
		// ���̍ہA�ēx���[�h���������s����K�v������B
		if (providedData->m_target)
		{
			return providedData;
		}
	}
	else
	{
		providedData = std::make_shared<ProvidedData>();
		m_providedDataMap.emplace(assetPath, providedData);
	}

	// ���[�h�������o�^����Ă��Ȃ��B
	if (!providedData->m_asyncLoading)
	{
		providedData->m_asyncLoading = true;

		// �񓯊����[�h�̓o�^���s���B
		LoadAssetAsync(assetPath, [this](auto assetPath) 
		{
			auto newAsset = AsyncLoad(assetPath);
			if (newAsset)
			{
				AsyncLoadCallback(assetPath, newAsset);
			}
		});
	}

	return providedData;
}

void IAssetProvider::AsyncLoadCallback(const std::string& assetPath, std::shared_ptr<IAsset> asset)
{
	auto providedData = GetProvidedData(assetPath);
	if (providedData)
	{
		if (!providedData->m_target)
		{
			// ���[�h���ꂽ�I�u�W�F�N�g���L���b�V��
			providedData->m_target = asset;
		}

		// ������ʒm
		CompletedLoadRequests(providedData);
	}
}

void IAssetProvider::CompletedLoadRequests(std::shared_ptr<ProvidedData> providedData) const
{
	if (providedData)
	{
		// �����҂��n���h���֒ʒm
		for (auto& handle : providedData->m_loadingHandles)
		{
			handle->CompleteLoad();
		}

		providedData->m_loadingHandles.clear();
		providedData->m_loadingHandles.shrink_to_fit();
	}
}}