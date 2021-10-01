#pragma once
#include "ModeBase.h"
#include <memory>

namespace inr {

	class MoveImage;

	class ModeEnd : public ModeBase {
	public:
		ModeEnd(Game& game);
		~ModeEnd() = default;

		void Init() override; // 初期化処理(フラグがオンの場合はフレーム初期化)
		void Process() override;
		void Draw() override;
	private:
		bool _end;	// 処理を終了するか？
		std::unique_ptr<MoveImage> _stuffRoll;

		bool IsEnd();	// 
	};
}

