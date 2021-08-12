#pragma once
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {

	class Soul : public ObjectBase {
	public:
		Soul(Game& game);
		~Soul() override;

		void Process() override;
		void Draw() override;
	private:
		// 魂の色（赤、青）
		enum class Type {
			RED, BLUE
		};

		Type _sType;
		Vector2 _moveVector;

		void Tracking(); // 追跡処理
		void Move();	// 移動
	public:
		inline Type GetType() { return _sType; }
	};
}

