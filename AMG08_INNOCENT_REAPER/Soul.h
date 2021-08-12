#pragma once
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {

	namespace soul {
		constexpr auto RED_SOUL = "r_soul";
		constexpr auto BLUE_SOUL = "b_soul";
		constexpr auto R_FLOAT = "r_float";
		constexpr auto B_FLOAT = "b_float";

		constexpr auto IMAGE_SIZE = 140;

		// 各モーションの画像数
		constexpr auto SF_BLUE_SOUL = 1;
		constexpr auto SF_B_SOUL = 4;
		constexpr auto SF_RED_SOUL = 1;
		constexpr auto SF_R_SOUL = 5;
	}

	class Soul : public ObjectBase {
	public:
		Soul(Game& game);
		~Soul() override;

		void Init() override;
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

