/*****************************************************************//**
 * \file   ResourceServer.cpp
 * \brief  �f�B�u�O���t�N���X
 *		   DxLib::LoadDivGraph�֐��œǂݍ��މ摜�̏��
 *		   ���\�[�X�T�[�o�N���X
 *		   DxLib::LoadDivGraph�Ŏ擾�����O���t�B�b�N�n���h���̊Ǘ����s��
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#include "ResourceServer.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <DxLib.h>

namespace graph {
	// �ÓI�����o�̒�`
	ResourceServer::DivGraphMap ResourceServer::_graphlists;
	// �R���X�g���N�^
	DivGraph::DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize) : 
		_filename(filename), _xNum(xnum), _yNum(ynum), _allNum(allnum), _xSize(xsize), _ySize(ysize), _handles(){
	}
	// �R���X�g���N�^
	DivGraph::DivGraph() {
		// �e�평����
		_filename = "";
		_xNum = 0;
		_yNum = 0;
		_allNum = 0;
		_xSize = 0;
		_ySize = 0;
		_handles.clear();
	}
	// �f�X�g���N�^
	DivGraph::~DivGraph() {
	}
	// �t�@�C�����̎擾
	std::string& DivGraph::GetFile() {
		std::string* fillname = &_filename;
		return *fillname;
	}
	// ���\�[�X�̏�����
	void ResourceServer::Init() {
		ClearGraphLists();	// �R���e�i�̉��
	}
	// ���\�[�X�̉��
	void ResourceServer::Release() {
		ClearGraphLists();	// �R���e�i�̉��
	}
	// �R���e�i�̉��
	void ResourceServer::ClearGraphLists() {
		// �o�^����Ă���O���t�B�b�N�̉��
		for (auto&& grl: _graphlists) {
			// �O���t�B�b�N�n���h���̎��o��
			for (auto gh : grl.second.GetHandls()) {
				DeleteGraph(gh);	// �������ォ��폜����
			}
		}
		_graphlists.clear();	// �R���e�i������
	}
	// ���\�[�X�̓ǂݍ���
	void ResourceServer::LoadGraphList(const DivGraphMap& divGraphMap) {
		// �L�[�ƃo�����[�����o���������s��
		for (auto&& dgm : divGraphMap) {
			auto& key = dgm.first;				// �L�[���
			auto it = _graphlists.find(key);	// ���ɓo�^����Ă��邩�̊m�F
			if (it != _graphlists.end()) {
				continue;	// �o�^����Ă���
			}
			auto dg = dgm.second;			// �ǂݍ��ݗp���̎��o��
			auto allNum = dg.GetAllNum();	// �摜�̑�������		
			dg.GetHandls().resize(allNum);	// �O���t�B�b�N�i�[�p�̃R���e�i�����T�C�Y
			// �摜�̓ǂݍ��݂��s��
			LoadDivGraph(dg.GetFile().c_str(), allNum, dg.GetXnum(), dg.GetYnum(), dg.GetXsize(), dg.GetYsize(),dg.GetHandls().data());
			_graphlists.emplace(key.c_str(), dg);	// �R���e�i�ɓo�^����
		}

	}
	// �O���t�B�b�N�n���h���̎擾
	int ResourceServer::GetHandles(const std::string& key, int no) {
		auto it = _graphlists.find(key);	// ���\�[�X�̌���
		// �Y�����\�[�X�͌����������H
		if (it == _graphlists.end()) {
			return -1;	// �q�b�g���Ȃ�����
		}
		auto graph = it->second.GetHandle().at(no);		// �O���t�B�b�N�n���h���̎擾
		return graph;	// �n���h����Ԃ�
	}
	// �Y������O���t�B�b�N�n���h�����Ǘ�����R���e�i�̎擾
	bool ResourceServer::GetHandles(const std::string& key, std::vector<int>& handls) {
		auto it = _graphlists.find(key);	// ���\�[�X�̌���
		// �Y�����\�[�X�͌����������H
		if (it == _graphlists.end()) {
			return false;	// �q�b�g���Ȃ�����
		}
		// �n���h�����̃R�s�[
		handls.resize(it->second.GetHandls().size());
		handls = it->second.GetHandle();
		return true;		// �q�b�g����
	}
	// �w�肵���O���t�B�b�N�̑����������擾
	int ResourceServer::GetAllNum(const std::string& key) {
		auto it = _graphlists.find(key);	// ���\�[�X�̌���
		// �Y�����\�[�X�͌����������H
		if (it == _graphlists.end()) {
			return -1;	// �q�b�g���Ȃ�����
		}
		return it->second.GetAllNum();	// ����������Ԃ�
	}
	// ���\�[�X���̓ǂݍ���
	void ResourceServer::SetLoadGraph(const DivGraphMap& divGraphMap) {
		// �L�[�ƃo�����[�����o���������s��
		for (auto&& dgm : divGraphMap) {
			auto& key = dgm.first;					// �L�[
			auto it = _graphlists.find(key);		// ����
			if (it != _graphlists.end()) continue;	// ���ɓo�^�ς݂̏ꍇ�̓X�L�b�v

			DivGraph dgraph = dgm.second;			// �ǂݍ��ݏ��̃R�s�[
			auto&& dghandle = dgraph.GetHandls();	// �R���e�i�̎Q��
			dghandle.resize(dgraph.GetAllNum());	// �T�C�X�ύX
			// �����������摜��ǂݍ���
			for (auto i = 0; i < dgraph.GetAllNum(); ++i) {
				std::string number;		// �摜�t�@�C���p
				// i�����������������Ă��邩
				if (i < 10) {
					// �������Ă��Ȃ��ꍇ�͐擪��"0"�𑫂���string�^�ɕϊ�
					number = "0" + std::to_string(i);
				} else {
					// �������Ă���ꍇ�͂��̂܂�string�^�ɕϊ�����
					number = std::to_string(i);
				}
				// �t�@�C�����̐ݒ�
				std::string fn = dgraph.GetFile() + number + ".png";
				// �摜��ǂݍ���
				LoadDivGraph(fn.c_str(), 1, 1, 1, dgraph.GetXsize(), dgraph.GetYsize(), &dghandle.at(i));
			}
			_graphlists.emplace(dgm.first, dgraph);		// �R���e�i�ɓo�^����
		}
	}
}