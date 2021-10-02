#pragma once
#include "ModeBase.h"
#include "MoveImage.h"
#include <memory>

namespace inr {

	class ModeEnd : public ModeBase {
	public:
		ModeEnd(Game& game);
		~ModeEnd() = default;

		void Init() override; // 初期化処理(フラグがオンの場合はフレーム初期化)
		void Process() override;
		void Draw() override;
	private:
		int _count;	// 待ち時間
		bool _end;	// 処理を終了するか？
		bool _input; // 入力を受け付けるか？
		std::unique_ptr<MoveImage> _staffRoll;

		bool IsEnd();	// 
		bool IsInput();
	};
}

