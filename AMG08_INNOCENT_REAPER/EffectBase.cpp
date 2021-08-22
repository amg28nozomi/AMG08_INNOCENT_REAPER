#include "EffectBase.h"
#include "MapChips.h"
#include <DxLib.h>

namespace inr {

	EffectBase::EffectBase(Game& game, const Vector2 spawnpos) : _game(game), _position(spawnpos){
		// 
		_count = std::make_pair(effect::NO_MAXFRAME, effect::NO_COUNT);

	}


	void EffectBase::Init() {

	}

	void EffectBase::Process() {

	}

	void EffectBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
	}

	void EffectBase::GraphResearch(int* gh) {
		// 


		// フラグがオンの時、描画するグラフィックを切り替える
	//	if (_changeGraph) {
	//		_changeGraph = false;
	//		_aCount = 0;
	//		*gh = graph::ResourceServer::GetHandles(_divKey.first, 0);	// 最初の要素を取得
	//		return true;
	//	}
	//	auto interval = AnimationInterval();
	//	// 何番目のアニメーションが呼び出されているか
	//	auto no = AnimationNumber();
	//	// グラフィックハンドルを読み込む
	//	*gh = graph::ResourceServer::GetHandles(_divKey.first, no);
	//}
	}
}