#pragma once
#include <memory>
#include <vector>

namespace inr {

	class Game;
	class GimmickBase;

	class GimmickServer {
	public:
		GimmickServer(Game& game);
		~GimmickServer();

		void Process();
		void Draw();	

		void Add(std::shared_ptr<GimmickBase> gimmick);
		inline std::vector<std::shared_ptr<GimmickBase>> GetGimmicks() { return _gimmicks; }	// �Q�b�^�[
		inline void Clear() { _gimmicks.clear(); }	// �S�I�u�W�F�N�g����
	private:
		Game& _game;
		std::vector<std::shared_ptr<GimmickBase>> _gimmicks;	// �M�~�b�N�ێ��p
	};
}

