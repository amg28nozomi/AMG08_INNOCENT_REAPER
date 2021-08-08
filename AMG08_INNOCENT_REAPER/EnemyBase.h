#pragma once
#include "ObjectBase.h"
#include "Collision.h"
// #include "Soul.h"

namespace inr {

	namespace enemy {
		// �L�[���
		constexpr auto SOLDIER_EMPTY = "soldier_empty";	// �����k
		constexpr auto SOLDIER_PATROL = "soldier_patrol";	// ����
		constexpr auto SOLDIER_ATTACK = "soldier_attack";	// �U��
		constexpr auto SOLDIER_ESCAPE = "soldier_escape";	// ����
	}

	class Soul; // �܂�����ĂȂ���

	class EnemyBase : public ObjectBase {
	protected:
		// �\���W���[�h�[���A�r�b�O�h�[���A�N���E�h�[��
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		// �����k�A����A�U���A����
		enum class ActionState {
			EMPTY, PATROL, ATTACK, ESCAPE
		};
		// ��A�ԁA��
		enum class SoulState{
			EMPTY, RED, BLUE
		};

		// Soul _soul;
		EnemyType _eType;
		ActionState _aState;
		SoulState _sState;

		AABB _searchBox; // ���G�p�����蔻��

		Keys _divKey;
		KeyDates _enemyKey;
		boxs _collisions;	// �e�퓖���蔻��i�[�p
		Vector2 _moveVector;	// �ړ��x�N�g��
		int _aFrame;	// �A�N�V�������s�̂��߂̃t���[��
		int _aCount;	// �A�j���[�V�����̓Y����
		int _sounds;
		bool _changeGraph;	// �`��O���t�B�b�N�؂�ւ��p�t���O

		double _actionX;	// �A�N�V�������s����x���W

		int GetSize(const std::string& key);
		int SoundResearch(const std::string& key);
		int GetSoundFrame(const std::string& key);
		bool GraphResearch(int* gh);	// ����
		bool SearchPlayer(); // ���G����(���@�̍��G

		virtual void Move() = 0; // �ړ�����(�h����Œ�`)
		virtual void Action() = 0; // �s��(�h����Œ�`)
		// virtual void AnimationInit() = 0;
		// �v���C���[�̌��ݍ��W����A���E�ǂ���ɋ��邩������o��
		virtual bool SearchPosition();
		virtual void Escape();	// ����
	public:
		EnemyBase(Game& game);
		~EnemyBase() override;

		virtual void Init() override;
		void Draw() override;
		
		inline EnemyType GetEnemyType() { return _eType; }
		inline ActionState GetActionState() { return _aState; }
		inline SoulState GetSoulState() { return _sState; }

		// virtual void Process() override;
		// virtual void Draw() override;
	};
}

