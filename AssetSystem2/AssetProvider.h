#pragma once
#include <string>
#include <memory>
#include <concepts>

class IAsset;

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
	virtual std::shared_ptr<IAsset> Provide(const std::string& assetPath) const = 0;

	template<class T>
	std::shared_ptr<T> Provide(const std::string& assetPath) const
	{
		return std::dynamic_pointer_cast<T>(Provide(assetPath));
	}
};