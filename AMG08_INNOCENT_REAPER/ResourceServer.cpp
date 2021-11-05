/*****************************************************************//**
 * @file   ResourceServer.cpp
 * @brief  DxLib::LoadDivGraph�Ŏ擾�����O���t�B�b�N�n���h���̊Ǘ����s�����\�[�X�T�[�o
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ResourceServer.h"
#include "DivGraph.h"
#include <unordered_map>
#include <memory>
#include <DxLib.h>

namespace graph {
  // �ÓI�����o�̒�`
  ResourceServer::DivGraphMap ResourceServer::_graphLists;

  void ResourceServer::Init() {
    ClearGraphLists();
  }

  void ResourceServer::Release() {
    ClearGraphLists();
  }

  void ResourceServer::ClearGraphLists() {
    // �o�^����Ă���O���t�B�b�N�̉��
    for (auto&& grl: _graphLists) {
      // �O���t�B�b�N�n���h���̎��o��
      for (auto gh : grl.second.GetHandls()) {
        DeleteGraph(gh); // �������ォ��폜����
      }
    }
    _graphLists.clear(); // �R���e�i������
  }

  void ResourceServer::LoadGraphList(const DivGraphMap& divGraphMap) {
    // �L�[�ƃo�����[�����o���������s��
    for (auto&& dgm : divGraphMap) {
      auto& key = dgm.first;
      auto it = _graphLists.find(key); // ���ɓo�^����Ă��邩�̊m�F
      if (it != _graphLists.end()) {
        continue; // �o�^����Ă���
      }
      auto dg = dgm.second;          // �ǂݍ��ݗp���̎��o��
      auto allNum = dg.GetAllNum();  // �摜�̑�������    
      dg.GetHandls().resize(allNum); // �O���t�B�b�N�i�[�p�̃R���e�i�����T�C�Y
      // �摜�̓ǂݍ��݂��s��
      LoadDivGraph(dg.GetFile().c_str(), allNum, dg.GetXnum(), dg.GetYnum(), dg.GetXsize(), dg.GetYsize(),dg.GetHandls().data());
      _graphLists.emplace(key.c_str(), dg); // �R���e�i�ɓo�^����
    }
  }

  int ResourceServer::GetHandles(const std::string& key, int no) {
    auto it = _graphLists.find(key); // ���\�[�X�̌���
    // �Y�����\�[�X�͌����������H
    if (it == _graphLists.end()) {
      return -1; // �q�b�g���Ȃ�����
    }
    auto graph = it->second.GetHandle().at(no); // �O���t�B�b�N�n���h���̎擾
    return graph; // �n���h����Ԃ�
  }

  bool ResourceServer::GetHandles(const std::string& key, std::vector<int>& handles) {
    auto it = _graphLists.find(key); // ���\�[�X�̌���
    // �Y�����\�[�X�͌����������H
    if (it == _graphLists.end()) {
      return false;                  // �q�b�g���Ȃ�����
    }
    // �n���h�����̃R�s�[
    handles.resize(it->second.GetHandls().size());
    handles = it->second.GetHandle();
    return true; // �q�b�g����
  }

  int ResourceServer::GetAllNum(const std::string& key) {
    auto it = _graphLists.find(key); // ���\�[�X�̌���
    // �Y�����\�[�X�͌����������H
    if (it == _graphLists.end()) {
      return -1; // �q�b�g���Ȃ�����
    }
    return it->second.GetAllNum(); // ����������Ԃ�
  }

  void ResourceServer::SetLoadGraph(const DivGraphMap& divGraphMap) {
    // �L�[�ƃo�����[�����o���������s��
    for (auto&& dgm : divGraphMap) {
      auto& key = dgm.first;           // �L�[
      auto it = _graphLists.find(key); // ����
      if (it != _graphLists.end()) continue; // ���ɓo�^�ς݂̏ꍇ�̓X�L�b�v

      DivGraph dGraph = dgm.second;             // �ǂݍ��ݏ��̃R�s�[
      auto&& dGraphHandle = dGraph.GetHandls(); // �R���e�i�̎Q��
      dGraphHandle.resize(dGraph.GetAllNum());  // �T�C�X�ύX
      // �����������摜��ǂݍ���
      for (auto i = 0; i < dGraph.GetAllNum(); ++i) {
        std::string number; // �摜�t�@�C���p
        // i�����������������Ă��邩
        if (i < 10) {
          // �������Ă��Ȃ��ꍇ�͐擪��"0"�𑫂���string�^�ɕϊ�
          number = "0" + std::to_string(i);
        } else {
          // �������Ă���ꍇ�͂��̂܂�string�^�ɕϊ�����
          number = std::to_string(i);
        }
        // �t�@�C�����̐ݒ�
        std::string fn = dGraph.GetFile() + number + ".png";
        // �摜��ǂݍ���
        LoadDivGraph(fn.c_str(), 1, 1, 1, dGraph.GetXsize(), dGraph.GetYsize(), &dGraphHandle.at(i));
      }
      _graphLists.emplace(dgm.first, dGraph); // �R���e�i�ɓo�^����
    }
  }
}