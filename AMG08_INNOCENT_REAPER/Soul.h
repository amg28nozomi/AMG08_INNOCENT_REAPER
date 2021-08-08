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
		// ���̐F�i�ԁA�j
		enum class Type {
			RED, BLUE
		};

		Type _sType;

		void Tracking(); // �ǐՏ���
	public:
		inline Type GetType() { return _sType; }
	};
}

