/*****************************************************************//**
 * @file   ResourceServer.h
 * @brief  DxLib::LoadDivGraph�Ŏ擾�����O���t�B�b�N�n���h���̊Ǘ����s�����\�[�X�T�[�o
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace graph {
	/** ��d�C���N���[�h�h�~ */
	class DivGraph;
	// ���\�[�X�T�[�o
	class ResourceServer {
	public:
		// ���\�[�X�ǂݍ��ݗp�����Ǘ�����A�z�z��
		using DivGraphMap = std::unordered_map<std::string, DivGraph>;
		// ���\�[�X�̏�����
		static void Init();
		// ���\�[�X�̉��
		static void Release();
		// �R���e�i�̉��
		static void ClearGraphLists();
		// ���\�[�X���̓ǂݍ���(����:�ǂݍ��ݗp���)
		static void LoadGraphList(const DivGraphMap& divGraphMap);
		// ���\�[�X���̓ǂݍ���(����:�ǂݍ��ݗp���)
		// �摜�t�@�C���̃O���t�B�b�N�n���h�������̃R���e�i�ɂ܂Ƃ߂�
		static void SetLoadGraph(const DivGraphMap& divGraphMap);
		// �w�肵���O���t�B�b�N�n���h���̎擾(����1:�L�[�@����2:�摜�ԍ�)
		static int GetHandles(const std::string& key, int no = 0);
		// �w�肵���O���t�B�b�N���Ǘ�����R���e�i�̎擾(����1:�L�[�@����2:�n���h�����󂯎�邽�߂̃R���e�i)
		static bool GetHandles(const std::string& key, std::vector<int>& handls);
		// �w�肵���O���t�B�b�N�̑����������擾(����;�L�[)
		static int GetAllNum(const std::string& key);
	private:
		// ���\�[�X���Ǘ�����A�z�z��
		static DivGraphMap _graphlists;
	};
}