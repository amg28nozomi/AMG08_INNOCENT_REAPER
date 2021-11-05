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
  /** ���\�[�X�T�[�o */
  class ResourceServer {
  public:
    /** �ǂݍ��ݗp�����Ǘ�����A�z�z�� */
    using DivGraphMap = std::unordered_map<std::string, DivGraph>;
    /**
     * @brief             ���\�[�X�̏�����
     */
    static void Init();
    /**
     * @brief             ���\�[�X�̉��
     */
    static void Release();
    /**
     * @brief             �R���e�i
     */
    static void ClearGraphLists();
    /**
     * @brief             ���\�[�X���̓ǂݍ���
     * @param divGraphMap �ǂݍ��ݗp���
     */
    static void LoadGraphList(const DivGraphMap& divGraphMap);
    /**
     * @brief             ���\�[�X���̓ǂݍ���(��������)
     * @param divGraphMap �ǂݍ��ݗp���
     */
    static void SetLoadGraph(const DivGraphMap& divGraphMap);
    /**
     * @brief             �w�肵���O���t�B�b�N�n���h���̎擾
     * @param key         �L�[
     * @param no          �n���h���ԍ�
     * @return            �O���t�B�b�N�n���h����Ԃ�
     *                    �q�b�g���Ȃ������ꍇ��-1��Ԃ�
     */
    static int GetHandles(const std::string& key, int no = 0);
    /**
     * @brief             �w�肵���O���t�B�b�N���Ǘ�����R���e�i�̎擾
     * @param key         �L�[
     * @param handles     �n���h���i�[�p�R���e�i
     * @return            �擾�ɐ��������ꍇ��true��Ԃ�
     *                    ���s�����ꍇ��false��Ԃ�
     */
    static bool GetHandles(const std::string& key, std::vector<int>& handles);
    /**
     * @brief             �w�肵���O���t�B�b�N�̑����������擾
     * @param key         �L�[
     * @return            �擾�ɐ��������ꍇ��true��Ԃ�
     *                    �擾�Ɏ��s�����ꍇ��false��Ԃ�
     */
    static int GetAllNum(const std::string& key);
  private:
    static DivGraphMap _graphLists; //!< ���\�[�X���Ǘ�����A�z�z��
  };
}