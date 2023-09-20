#pragma once
#include <memory>
#include "Asset.h"

class IAssetProvider;

/**
* ���݂͉������ʂ͂Ȃ��A
* �����|�C���^�����b�v���Ă��邾���ł����A
* ���ꂪ��X�߂�������ɗ����܂��B
*/
class AssetHandle
{
	friend class IAssetProvider;
public:

	/** Get�֐����Ăяo���\�Ȏ��A�^��Ԃ��܂��B*/
	bool IsVaild() const
	{
		return !!m_asset;
	}

public:

	template<class T>
	std::shared_ptr<T> Get() const
	{
		return std::dynamic_pointer_cast<T>(Get());
	}

	/** ���̃n���h�����w���A�A�Z�b�g��Ԃ��܂��B*/
	std::shared_ptr<IAsset> Get() const
	{
		return m_asset;
	}

private:

	/** ���[�h�\�������}�l�[�W���[�N���X */
	IAssetProvider* m_ownerProvider;

	/** ���[�h�����A�Z�b�g�ւ̃p�X */
	std::shared_ptr<IAsset> m_asset;
};