#include "Pause_UI.h"
#include "SoundServer.h"
#include <DxLib.h>

namespace {
	constexpr auto MOVE_NULL = 0;	// 移動量無し
	constexpr auto MOVE_UP = 1;
	constexpr auto MOVE_DOWN = -1;

	constexpr auto MOVE_VECTOR = 5.0;
}

namespace inr {

	Pause_UI::Pause_UI(Game& game) : Particle_Image(game) {
		_imageType = image::particle::UI;
		_no = 0;
		_upDown = MOVE_NULL;
		_nextY = 0;
	}

	void Pause_UI::Process() {
		Particle_Image::Process();
		// 移動処理がある場合は、目標座標方向へ移動する
		if (_move == true) {
			// 目標地点への移動が完了した時
			if (_pos.GetY() == _nextY) {
				_upDown = MOVE_NULL;
				_nextY = MOVE_NULL;
				_move = false;	// 移動処理終了
				return;	// 処理を脱出する
			}
			_pos.GetPY() += (MOVE_VECTOR * _upDown);
		}
	}

	void Pause_UI::ChangePosition(bool type) {
		if (_move == true)	return;	// 移動処理を実行中の場合は処理を行わなわない
		switch (type) {
		case true:
			++_no;
			break;
		case false:
			--_no;
			break;
		}
		auto sound = se::SoundServer::GetSound(system::CURSOR_MOVE);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(system::CURSOR_MOVE));
		_move = true;

		switch (_no) {
		case -1:
			_nextY = 540;
			_no = 2;
			break;
		case 0:
			_nextY = 300;
			break;
		case 1:
			_nextY = 420;
			break;
		case 2:
			_nextY = 540;
			break;
		case 3:
			_no = 0;
			_nextY = 300;
			break;
		default:
			break;
		}
		if (_pos.GetY() - _nextY < 0) _upDown = MOVE_UP;	// 移動方向に応じて、値を切り替える
		else _upDown = MOVE_DOWN;
	}
}