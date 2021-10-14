/*****************************************************************//**
 * \file   Door.h
 * \brief  ドアクラス（ギミックベースクラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {

	// ドア(ギミック)
	class Door : public GimmickBase {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		Door(Game& game);
		// デストラクタ
		~Door() = default;
		// 初期化
		void Init() override;
		// 更新
		void Process() override;
		// 描画
		void Draw() override;
		// オブジェクト情報の設定
		void SetParameter(Vector2 spwan, std::string key, int flag);
		// オブジェクト情報の登録
		void SetParameter(ObjectValue objValue) override;
		// 開閉フラグの起動
		void SwitchOn();
		// 開閉フラグ抑制
		void SwitchOff();
		// オブジェクトの押し出し処理(引数1:対象の当たり判定　引数2:対象の座標　引数3:移動ベクトル　引数4:向きフラグ)
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction);
		// 開閉フラグの取得
		inline bool IsSwitch() { return _switch; }
		// 扉の色の取得
		inline int DoorColor() { return _color; }
	private:
		int _color;			// ドアの色
		double _normalY;	// 通常座標
		bool _switch;		// 開閉フラグ
		bool _ismove;		// 移動中か？
		Vector2 _moves;
		// 何色の扉なのかの判定
		void SetColor(std::string key);
		// アニメーション処理
		bool MotionCount();
		// ドアの移動処理
		bool DoorMove();
	};
}

