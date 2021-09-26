#include "Item.h"
#include "Game.h"
#include "MapChips.h"
#include "Particle_Image.h"
#include "ObjectServer.h"
#include "Player.h"
#include "ObjectValue.h"

#include "ModeServer.h"
#include "ModeMain.h"
#include "ImageServer.h"
#include "SoundServer.h"

namespace {
	constexpr auto ITEM_MAX = 40;
}

namespace inr {

	/*ImageValue::ImageValue(std::string key, Vector2 pos, int width, int height) {
		_key = key;
		_position = pos;
		_width = width;
		_height = height;
	}*/

	Item::Item(Game& game, std::string no) : _game(game), _oValue() {
		_gkey = "";
		_position = { 0, 0 };
		_col = { _position, 0, 0, true };
		_count = 0;
		_maxFrame = ITEM_MAX;
		_stageNo = no;
		_del = false;
		_isGet = false;
	}

	void Item::Init() {

	}

	void Item::Process() {
		if (_isGet == true) return;	// ���ɓ��肳��Ă���ꍇ�͏������s��Ȃ�

		if (_count < _maxFrame) ++_count;
		else _count = 0;

		auto pbox = _game.GetObjectServer()->GetPlayer()->GetMainCollision();	// ���@�̓����蔻����擾
		if (_col.HitCheck(pbox) == true) {
			// �ڐG��Ԃ��A�{�^�����͂��������ꍇ��message��\������
			if (_game.GetTrgKey() == PAD_INPUT_4 == true) {
				_game.GetModeServer()->GetModeMain()->GetItemImages()->ImageChange(_messageNo);
				_isGet = true;	// ����t���O���I���ɂ���
				// ������SE��炷
			}
		}
		//if (_col.HitCheck(pbox) == true) {
		//	_pi.DrawStart();
		//} else { 
		//	_pi.DrawEnd();	// �`�揈���I��
		//}
		//_pi.Process();
	}

	void Item::Draw() {
		if (_isGet == true) return;
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();



		auto allnum = graph::ResourceServer::GetAllNum(_gkey);
		auto interval = _maxFrame / allnum;	// �P�\���Ԃ̊���o��
		auto no = _count / interval % allnum;	// �`�悷��摜�̎Z�o

		auto gh = graph::ResourceServer::GetHandles(_gkey, no);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}

	void Item::SetParameter(ObjectValue ovalue) {
		_oValue = ovalue;
		_gkey = item::ITEM;
		_isGet = ovalue.GimmickFlag();
		_messageNo = ovalue.GimmickType();
		_position = ovalue.Positions().front();
		_col = { _position, 50, 50, true };
	}

	void Item::ObjValueUpdate() {
		if (_oValue.GimmickFlag() == TRUE) return;	// ���Ƀt���O�I���̏ꍇ�͍X�V���s��Ȃ�
		if (_isGet == true) _oValue.FlagUpdate(TRUE);	// �t���O��؂�ւ���
	}
}
