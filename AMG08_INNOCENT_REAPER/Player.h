#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
#include <queue>
#include <unordered_map>

namespace inr {
	// �����p�L�[
	constexpr auto PKEY_DEATH = "player_death";	// ���S�i���[�V�������Ȃ��̂Ń_���[�W���[�V�������p�j
	constexpr auto PKEY_IDOL = "player_idol";	// �ҋ@
	constexpr auto PKEY_RUN = "player_run";		// �ړ�
	constexpr auto PKEY_DASH = "player_dash";	// �_�b�V��
	constexpr auto PKEY_ROB = "player_rob";		// ����D��
	constexpr auto PKEY_JUMP = "player_jump";	//�@�W�����v
	constexpr auto PKEY_GIVE = "player_give";	// ����^����
	constexpr auto PKEY_FALL = "player_fall";	// ����
	constexpr auto PKEY_HIT = "player_hit";
	constexpr auto PKEY_CLIMB = "player_climb";	// �ӓo��

	constexpr auto PIMAGE_SIZE = 140;	// �v���C���[�̕`��T�C�Y
	constexpr auto PLAYER_DEATH_IMAGE = 400;

	// �v���C���[�̌���
	constexpr auto PL_LEFT = true;
	constexpr auto PL_RIGHT = false;

	constexpr auto SOUL_MAX = 3;	// �ۗL�ł��鍰�̏��

	class SoulSkin;


	class Player : public ObjectBase {
	public:
		Player(Game& game);
		~Player() override;

		void Init() override;
		void Process() override;
		void Draw() override;

		inline Vector2 GetMoveVector() override{ return _moveVector; }
		inline bool HaveSoul() { return !_souls.empty(); }	// �^���鍰��L���Ă��邩�H
		inline bool IsSoulMax() { return _souls.size() == SOUL_MAX; };	// ���͓������ɓ��B���Ă��邩�H
		inline int SoulsSize() { return static_cast<int>(_souls.size()); }
		inline void InputChange(bool input) { _input = input; }
		inline void InputOff() { _input = false; }

		bool Debuf();	// ��f�o�t�U��
		bool Damage(bool mv);
		bool KnockBack(bool mv);	// �m�b�N�o�b�N����
		bool Reset();	// �J�n�n�_�ɖ߂�

		void SetParameter(ObjectValue objValue) override;	// ����1:�������W�@����2:����ۗL���Ă��邩�@����3:
		void SetParameter(std::pair<Vector2, bool> newdata, std::string sKey);
		void SoulCatch(std::shared_ptr<SoulSkin> soul);
		std::shared_ptr<SoulSkin> GiveSoul();	// ���̏��n
		inline std::queue<std::shared_ptr<SoulSkin>> GetSouls() { return _souls; }

	private:
		// �A�N�V�����̏��
		enum class ActionState {
			// ���S�A�ҋ@�A�ړ��A�_�b�V���A�D���A�^����A�m�b�N�o�b�N�A�c�^�o��
			DEATH, IDOL, MOVE, DASH, JUMP, FALL, ROB, GIVE, HIT, GRAB
		};
		enum class State {
			// �����A�_���[�W�A���S
			ALLIBE, DAMEGE, DEAD
		};

		ActionState _aState;
		Vector2 _moveVector;	// �ړ���(�P�ʃx�N�g��)
		std::queue<std::shared_ptr<SoulSkin>> _souls;

		double _dashX;	// �_�b�V���̍ő�ړ�����(���W)
		double _moveD;	// �ړ��{��

		int _pal;
		
		int _judegFrame;	// ����t���[����
		int _aFrame;	// �A�N�V�������s�̂��߂̃t���[��
		int _sounds;
		int _debuffCount;	// �f�o�t�J�E���^

		int _hitCount;	// ����t���[��(�A�N�V����)

		int _dashInterval;	// �_�b�V���̍Ďg�p�Ԋu
		int _knockBack;	// �m�b�N�o�b�N�J�E���^
		int _invincible;	// ���G����
		std::string _moveType;	// �ړ���
		std::string _landingType;	// ���n��
		double _jumpPower;	// �W�����v����
		std::pair<double, double> _ivx;
		bool _input;	// ���͏������󂯕t���邩
		bool _grab;	// �͂ݔ���
		bool _sChange;	// �X�e�[�W���ς�������ǂ���
		bool _ivxInterval;	// ���_���[�W��̃t���O
		// std::string _divKey;	// �ǂݍ��ݗp���ʃL�[
		boxs _collisions;	// �L�[�A�����s���E��BOX(AABB�N���X)

		// std::pair<double, double> _moveVector;	// �ړ�����(����:x, �E��:y)

		void AnimationInit();
		void ChangeState(Player::ActionState nextState, std::string key);	// ��ԑJ��

		void PositionUpdate();
		// Action�����܂Ƃ�
		bool Action(int key);

		// �A�N�V�������͏���
		void InputDash(double x);
		void InputJump();
		
		// �e��A�N�V����
		void Move(int lever); // �ړ�
		void Move();	// �L�[���͑Ή�
		void Climb(int leverUD);	// �㉺�ړ�
		void Dash(); // �_�b�V��
		void Jump(); // �W�����v
		void Rob(double x, double y); // �D��
		void Give(double x, double y); // �^����
		void ChangeSoul(); // ���̐؂�ւ�
		// bool IsDamage(); // �_���[�W������󂯂邩�H
		bool Dead(); // ���S����
		void Death();	// ���S�����i�j
		bool IsStandChip() override;

		// ���̃_���[�W����
		void DamageThorm();

		void IsGrab();	// �����蔻�蓙�̏���
		void Grab();

		void StateUpdate();	// ��ԑJ�ڂ���т��̑�����

		AABB GetAABB();
		AABB NowCollision(std::string key) override;

		double GetFix(double value);	// �����ɉ����Ĕ��]������������
		
		void InputOn();	// ���͂��ĊJ���邩
		
#ifdef _DEBUG
		void DebugInfo();
#endif
	};
}

// �����蔻����s�p�̏�����ʓr�쐬�i�֐����j
