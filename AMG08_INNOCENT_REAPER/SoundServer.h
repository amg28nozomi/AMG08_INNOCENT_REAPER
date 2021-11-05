/*****************************************************************//**
 * @file   SoundServer.h
 * @brief  �T�E���h�̊Ǘ����s���T�E���h�T�[�o
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "SoundData.h"
#include <unordered_map>
#include <string>

namespace se {
  /** �T�E���h�T�[�o */
  class SoundServer {
  public:
    /** �T�E���h�����i�[����A�z�z�� */
    using SoundMap = std::unordered_map<std::string, SoundData>;
    /**
     * @brief          ����������
     */
    static void Init();
    /**
     * @brief          �T�E���h�n���h���̉��
     */
    static void Release();
    /**
     * @brief          �R���e�i�̉��
     */
    static void ClearSoundList();
    /**
     * @brief          �T�E���h�̓ǂݍ���
     * @param soundMap �T�E���h���
     */
    static void LoadSoundMap(const SoundMap& soundMap);
    /**
     * @brief          �T�E���h�n���h���̎擾
     * @param key      �L�[
     * @return         �T�E���h�n���h����Ԃ�
     *                 �L�[���o�^����Ă��Ȃ��ꍇ��-1��Ԃ�
     */
    static int GetSound(const std::string& key);
    /**
     * @brief          �Đ��^�C�v�̎擾
     * @param key      �L�[
     * @return         �Đ��^�C�v��Ԃ�
     *                 �L�[���o�^����Ă��Ȃ��ꍇ��-1��Ԃ�
     *                  
     */
    static int GetPlayType(const std::string& key);
    /**
     * @brief          �T�E���h�̎擾
     * @param key      �L�[
     * @param playType �Đ��^�C�v
     * @return         �T�E���h�n���h����Ԃ�
     *                 �L�[���o�^����Ă��Ȃ��ꍇ��-1��Ԃ�
     */
    static int GetSound(const std::string& key, int* playType);
  private:
    static SoundMap _soundList; //!< �T�E���h���
  };
}