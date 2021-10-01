#pragma once
#include "Image.h"

namespace inr {

	class MoveImage : public Image {
	public:
		MoveImage(Game& game);
		~MoveImage() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(const std::string gkey, Vector2 pos);	// 引数1:画像用キー、初期生成地点

		inline void MoveOn() { if (_isMove == false) _isMove; }
	private:

		bool _isMove;	// 移動処理を行うか？
		Vector2 _start;	// 生成座標

		bool Move();
	};
}

