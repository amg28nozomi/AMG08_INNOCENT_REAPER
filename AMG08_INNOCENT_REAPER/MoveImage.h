/*****************************************************************//**
 * @file   MoveImage.h
 * @brief  移動処理を行うモーブ画像クラス（画像クラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"

namespace inr {
	/** 移動処理のある画像 */
	class MoveImage : public Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		MoveImage(Game& game);
		/**
		 * @brief				デストラクタ
		 */
		~MoveImage() = default;
		/**
		 * @brief				初期化処理
		 */
		void Init() override;
		/**
		 * @brief				更新処理
		 */
		void Process() override;
		/**
		 * @brief				
		 */
		void Draw() override;
		/**
		 * @brief				パラメータの設定
		 * @param gkey	画像用キー
		 * @param pos		初期生成地点
		 */
		void SetParameter(const std::string gkey, Vector2 pos);
		/**
		 * @brief				移動処理フラグの取得
		 * @return			移動処理フラグを返す
		 */
		inline bool IsActive() { return _isMove; }
	private:
		bool _isMove;		//!< 移動処理を行うか？
		Vector2 _start;	//!< 生成座標
		/**
		 * @brief				移動処理
		 * @return			移動を終了する場合はtrueを返す
		 *							移動を継続する場合はfalseを返す
		 */
		bool Move();
	};
}

