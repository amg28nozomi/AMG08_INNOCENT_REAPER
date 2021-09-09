#pragma once
#include "Image.h"

namespace inr {

	namespace animation {
		constexpr auto A_SUB = -1;
		constexpr auto A_NORMAL = 0;
		constexpr auto A_ADD = 1;
	}

	class Particle_Image : public Image {
	public:
		Particle_Image(Game& game);
		~Particle_Image() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(Vector2 pos, std::string graph, int pal, double extrate);
		bool Animation();
		bool DrawStart();
		bool DrawEnd();
	private:
		int _animation;	// �A�j���[�V�������s����
		int _pal;	// �����l
		double _extRate;	// �g�嗦
		bool _isDraw;	// �`�揈�����s���Ă��邩

		bool AddPal();
		bool SubPal();
	};
}

