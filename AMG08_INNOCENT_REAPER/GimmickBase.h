#pragma once
#include "ObjectBase.h"

namespace inr {

	class GimmickBase : public ObjectBase {
	protected:
		enum class GimmickType {
			// ���o�[�A��A����
			LEVER, ROCK, CRYSTAL 
		};
		GimmickType _gimmick;
	public:
		GimmickBase(Game& game);
		~GimmickBase();

		virtual void Init() override;
		void Draw() override;
	};
}

