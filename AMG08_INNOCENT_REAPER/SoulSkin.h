/*****************************************************************//**
 * @file   SoulSkin.h
 * @brief	 魂クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {
	/** 魂のコンパイル時定数 */
	namespace soul {
		constexpr auto RED_SOUL = "r_soul";		//!< 赤
		constexpr auto BLUE_SOUL = "b_soul";	//!< 青
		constexpr auto R_FLOAT = "r_float";		//!< 浮遊(赤)
		constexpr auto B_FLOAT = "b_float";		//!< 浮遊(青)
		// タイプ
		constexpr auto RED = false;						//!< 赤
		constexpr auto BLUE = true;						//!< 青
		constexpr auto IMAGE_SIZE = 100;			//!< 魂のサイズ
		// 各モーションの総分割数
		constexpr auto SF_BLUE_SOUL = 1;			//!< 青い魂のモーション数
		constexpr auto SF_B_SOUL = 4;					//!< 浮遊する青い魂のモーション数
		constexpr auto SF_RED_SOUL = 1;				//!< 赤い魂のモーション数
		constexpr auto SF_R_SOUL = 5;					//!< 浮遊する赤い魂のモーション数
	}
	/** 魂クラス */
	class SoulSkin : public ObjectBase {
	public:
		/**
		 * @brief							コンストラクタ
		 * @param game				ゲームクラスの参照
		 */
		SoulSkin(Game& game);
		/**
		 * @brief							デストラクタ
		 */
		~SoulSkin() override;
		/**
		 * @brief							初期化処理
		 */
		void Init() override;
		/**
		 * @brief							更新処理
		 */
		void Process() override;
		/**
		 * @brief							描画処理
		 */
		void Draw() override;
		/**
		 * @brief							魂のセットアップ
		 * @param scolor			魂の色
		 */
		inline void SoulChange(bool scolor) { (scolor == soul::RED) ? _sType = Type::RED : _sType = Type::BLUE; }
	private:
		/** 魂の色を表す列挙型 */
		enum class Type {
			RED, BLUE
		};
		Type _sType;					//!< タイプ
		Vector2 _moveVector;	//!< 移動量
		double _speed;				//!< 移動速度
		double _space;				//!< 間隔
		bool _active;					//!< 実体化しているか？
		bool _give;						//!< 所有権を譲渡するか
		bool _isOwner;				//!< 所有者はいるか？
		/**
		 * @brief							追跡処理
		 */
		void Tracking();
		/**
		 * @brief							移動処理
		 */
		void Move();
		/**
		 * @brief							所有権の付与(所有者が居ない時限定）
		 */
		void Give();
	public:
		/**
		 * @brief							タイプの取得
		 * @return						タイプを返す
		 */
		inline Type GetType() { return _sType; }
		/**
		 * @brief							魂の設定
		 * @param spawn				生成座標
		 * @param soulcolor		色
		 */
		void SetStatus(Vector2 spawn, std::string soulcolor)override;
		/**
		 * @brief							移動速度の登録
		 * @param speed				移動速度
		 */
		void SetSpeed(double speed) override { _speed = speed; }
		/**
		 * @brief							情報の設定
		 * @param soulcolor		色
		 * @param speed				移動速度
		 */
		void SetParameter(int soulcolor, double speed);	
		/**
		 * @brief							実体化処理
		 * @param spawn				生成地点
		 */
		void SetSpwan(Vector2 spawn);
		/**
		 * @brief							非活動状態に移行
		 */
		inline void Inactive() { _active = false; }
		/**
		 * @brief							活動状態に移行
		 */
		inline void Active() { _active = true; }
		/**
		 * @brief							追従間隔の登録
		 */
		inline void Space(double space) { _space = space; }
		/**
		 * @brief							所有者フラグを切る
		 */
		inline void OwnerNull() { _isOwner = false; }
		/**
		 * @brief							所有権譲渡フラグの取得
		 * @return						所有権譲渡フラグを返す
		 */
		inline bool IsGive() { return _give; }
		/**
		 * @brief							所有者フラグの取得
		 * @return						所有者フラグを返す
		 */
		inline bool IsOwner() { return _isOwner; }
		/**
		 * @brief							魂の色の取得
		 * @return						魂の色を返す
		 */
		bool SoulColor();
	};
}

