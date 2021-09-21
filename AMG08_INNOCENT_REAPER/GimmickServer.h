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
		void Del();
		inline std::vector<std::shared_ptr<GimmickBase>> GetGimmicks() { return _gimmicks; }	// ゲッター
		inline void Clear() { _gimmicks.clear(); }	// 全オブジェクト消去
		inline void DelOn() { _isDel = true; }
	private:
		Game& _game;
		bool _isDel;	// オブジェクトを消去するか否か
		std::vector<std::shared_ptr<GimmickBase>> _gimmicks;	// ギミック保持用
	};
}

