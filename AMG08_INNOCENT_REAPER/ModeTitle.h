#pragma once
#include "ModeBase.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {

	class Logo;

	class ModeTitle : public ModeBase{
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		ModeTitle(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~ModeTitle();

		virtual void Init();	// 初期化
		virtual void Process();	// 更新
		virtual void Draw();	// 描画

	private:

		std::unique_ptr<Logo> _titleGh;
	};
}