#pragma once
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {

	namespace soul {
		constexpr auto RED_SOUL = "r_soul";
		constexpr auto BLUE_SOUL = "b_soul";
		constexpr auto R_FLOAT = "r_float";
		constexpr auto B_FLOAT = "b_float";

		constexpr auto RED = false;
		constexpr auto BLUE = true;

		constexpr auto IMAGE_SIZE = 100;

		// 各モーションの画像数
		constexpr auto SF_BLUE_SOUL = 1;
		constexpr auto SF_B_SOUL = 4;
		constexpr auto SF_RED_SOUL = 1;
		constexpr auto SF_R_SOUL = 5;
	}

	class SoulSkin : public ObjectBase {
	public:
		SoulSkin(Game& game);
		~SoulSkin() override;

		void Init() override;
		void Process() override;
		void Draw() override;
		// 魂のセットアップ
		inline void SoulChange(bool scolor) { (scolor == soul::RED) ? _sType = Type::RED : _sType = Type::BLUE; }
	private:
		// 魂の色（赤、青）
		enum class Type {
			RED, BLUE
		};

		Type _sType;
		Vector2 _moveVector;

		double _speed;
		double _space;	// 間隔
		bool _active;	// 実体化しているか？
		bool _give;	// 所有権を譲渡するか
		bool _isOwner;	// 所有者はいるか？

		void Tracking(); // 追跡処理
		void Move();	// 移動
		void Give();	// 自機に所有権を移すか？（所有者が居ない時限定）
	public:
		inline Type GetType() { return _sType; }
		void SetStatus(Vector2 spawn, std::string soulcolor)override;
		void SetSpeed(double speed) override { _speed = speed; }

		void SetParameter(int soulcolor, double speed);	
		void SetSpwan(Vector2 spwan);

		inline void Inactive() { _active = false; }
		inline void Active() { _active = true; }
		inline void Space(double space) { _space = space; }
		inline void OwnerNull() { _isOwner = false; }
		inline bool IsGive() { return _give; }
		inline bool IsOwner() { return _isOwner; }	// 所有者はいますか？
		bool SoulColor();	// 魂はどちらか？
	};
}

