#include "ModeBase.h"
#include "Game.h"
#include "ObjectServer.h"

namespace inr {

	int ModeBase::_modeFrame = 0;

	ModeBase::ModeBase(Game& game) : _game(game), _resetFlg(true){
		Init();
	}

	ModeBase::~ModeBase() {
		
	}

	void ModeBase::Init() {
		// フラグがオンの時だけカウント初期化処理を実行
		if (_resetFlg) {
			TimeClear();
		}
	}

	void ModeBase::Process() {
		++_modeFrame;
	}

	void ModeBase::Draw() {

	}


}