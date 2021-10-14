#pragma once
#include "Particle_Image.h"
#include <vector>

namespace inr {

	class StageUi : public Particle_Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		StageUi(Game& game);
		~StageUi() = default;

		void Init() override;
		void Process() override;
		void Draw() override;
		void SetParameter(std::string graph, Vector2 pos, int pal = 0);

		bool ChangeNumber(const std::string stageKey);	// 描画するアイコン切り替え
		bool IntervalCount();	// 待ち時間のカウント処理
		bool DrawStart() override;
		inline bool FadeDraw() { return _fadeDraw; }
	private:
		int _gNumber;	// 何番目の画像を描画するか
		int _alive;	// 生存フレーム
		bool _fadeDraw;	// 描画処理を実行したか
	};

}

