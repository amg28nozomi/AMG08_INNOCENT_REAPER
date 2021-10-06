#pragma once
#include "GimmickBase.h"
#include "SoulSkin.h"
#include <memory>
#include <vector>

namespace inr {

	class Door;

	// 水晶(ギミック)
	class Crystal : public GimmickBase {
	public:
		Crystal(Game& game);
		~Crystal() = default;

		// 描画
		void Draw() override;

		// オブジェクト情報の登録
		void SetParameter(ObjectValue objValue) override;
		// オブジェクト情報の更新
		void ObjValueUpdate() override;
		// 自機アクションとの衝突判定
		void CollisionHit(const std::string ckey, Collision acollision, bool direction);

	private:
		std::shared_ptr<SoulSkin> _soul;	// 保持している魂
		std::vector<std::shared_ptr<Door>> _doors;	// 保持しているドア

		// ドアの設定
		void SetDoors(std::vector<std::string> key);
		// 描画画像の設定
		void GraphKey();
		// 魂の生成
		void AddSoul();
		// ドアの解放
		bool IsOpen(int crystal);

		
	};
}

