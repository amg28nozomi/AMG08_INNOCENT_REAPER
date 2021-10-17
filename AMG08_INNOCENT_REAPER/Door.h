/*****************************************************************//**
 * @file   Door.h
 * @brief  ドアクラス（ギミックベースクラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	/** ドアギミック */
	class Door : public GimmickBase {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Door(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~Door() = default;
		/**
		 * @brief	初期化処理
		 */
		void Init() override;
		/**
		 * @brief	更新処理
		 */
		void Process() override;
		/**
		 * @brief	描画処理
		 */
		void Draw() override;
		/**
		 * @brief				オブジェクト情報の登録
		 * @param spwan	生成座標
		 * @param key		画像キー
		 * @param flag	ギミックフラグ
		 */
		void SetParameter(Vector2 spwan, std::string key, int flag);
		/**
		 * @brief						オブジェクト情報の登録
		 * @param objValue	オブジェクト情報
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief	開閉フラグの起動
		 */
		void SwitchOn();
		/**
		 * @brief	開閉フラグの抑制
		 */
		void SwitchOff();
		/**
		 * @brief						オブジェクトの押し出し処理
		 * @param	box				対象の当たり判定ボックス
		 * @param	pos				対象の座標ベクトル(参照)
		 * @param	move			対象の移動ベクトル(参照)
		 * @param	direction	対象の向きフラグ
		 * @return					衝突している場合はtrueを返す
		 *									衝突していない場合はfalseを返す
		 */
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction);
		/**
		 * @brief		扉の色の取得
		 * @return	扉の色を返す
		 */
		inline int DoorColor() { return _color; }
		/**
		 * @brief		開閉フラグの取得
		 * @return	開閉フラグを返す 
		 */
		inline bool IsSwitch() { return _switch; }
	private:
		int _color;					//!< ドアの色
		double _normalY;		//!< 通常座標
		bool _switch;				//!< 開閉フラグ
		bool _ismove;				//!< 移動中か？
		Vector2 _moves;			//!< 移動座標
		/**
		 * @brief			扉の色の設定
		 * @param key	画像キー
		 */
		void SetColor(std::string key);
		/**
		 * @brief		アニメーション処理
		 * @return	アニメーションの再生を行う場合はtrueを返す
		 *					再生を行わない場合はfalseを返す
		 */
		bool MotionCount();
		/**
		 * @brief		移動処理
		 * @return 
		 */
		bool DoorMove();
	};
}

