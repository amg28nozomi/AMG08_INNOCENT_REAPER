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

		// �e���[�V�����̉摜��
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
		// ���̃Z�b�g�A�b�v
		inline void SoulChange(bool scolor) { (scolor == soul::RED) ? _sType = Type::RED : _sType = Type::BLUE; }
	private:
		// ���̐F�i�ԁA�j
		enum class Type {
			RED, BLUE
		};

		Type _sType;
		Vector2 _moveVector;

		double _speed;

		void Tracking(); // �ǐՏ���
		void Move();	// �ړ�
	public:
		inline Type GetType() { return _sType; }
		void SetStatus(Vector2 spawn, std::string soulcolor)override;
		void SetSpeed(double speed) override { _speed = speed; }
	};
}
