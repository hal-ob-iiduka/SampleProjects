#pragma once
#include <string>
#include <vector>

struct AssetLocation
{
	/** ���̃A�Z�b�g�̃f�[�^ */
	std::string m_assetPath;

	/** ���̃A�Z�b�g�̃f�[�^���g�p����v���o�C�_�[ID */
	std::string m_providerId;

	/** ���̃A�Z�b�g���ˑ�����e�A�Z�b�g���X�g */
	std::vector<std::shared_ptr<AssetLocation>> m_dependencies;
};