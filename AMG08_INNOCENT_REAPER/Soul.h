#pragma once
#include "ObjectBase.h"

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

		void Tracking(); // 追跡処理
	public:
		inline Type GetType() { return _sType; }
	};
}

