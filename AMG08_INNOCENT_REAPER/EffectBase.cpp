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

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
	}

	void EffectBase::GraphResearch(int* gh) {
		// 


		// �t���O���I���̎��A�`�悷��O���t�B�b�N��؂�ւ���
	//	if (_changeGraph) {
	//		_changeGraph = false;
	//		_aCount = 0;
	//		*gh = graph::ResourceServer::GetHandles(_divKey.first, 0);	// �ŏ��̗v�f���擾
	//		return true;
	//	}
	//	auto interval = AnimationInterval();
	//	// ���Ԗڂ̃A�j���[�V�������Ăяo����Ă��邩
	//	auto no = AnimationNumber();
	//	// �O���t�B�b�N�n���h����ǂݍ���
	//	*gh = graph::ResourceServer::GetHandles(_divKey.first, no);
	//}
	}
}