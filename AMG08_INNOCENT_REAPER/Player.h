#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
#include <unordered_map>

namespace inr {
	// �����p�L�[
	constexpr auto PKEY_IDOL = "player_idol";	// �ҋ@
	constexpr auto PKEY_RUN = "player_run";		// �ړ�
	constexpr auto PKEY_DASH = "player_dash";	// �_�b�V��
	constexpr auto PKEY_ROB = "player_rob";		// ����D��
	constexpr auto PKEY_JUMP = "player_jump";	//�@�W�����v
	constexpr auto PKEY_GIVE = "player_give";	// ����^����
	constexpr auto PKEY_FALL = "player_fall";	// ����
	constexpr auto PKEY_HIT = "player_hit";

	constexpr auto BACK_GRAUND = "backP";

	constexpr auto PIMAGE_SIZE = 210;	// �v���C���[�̕`��T�C�Y


	class Player : public ObjectBase {
	public:
		Player(Game& game);
		~Player() override;

		void Init() override;
		void Process() override;
		void Draw() override;

	private:
		// �A�N�V�����̏��
		enum class ActionState {
			// �ҋ@�A�ړ��A�_�b�V���A�D���A�^����A�m�b�N�o�b�N
			IDOL, MOVE, DASH, JUMP, FALL, ROB, GIVE, HIT
		};
		enum class State {
			// �����A�_���[�W�A���S
			ALLIBE, DAMEGE, DEAD
		};

		ActionState _aState;
		Vector2 _moveVector;	// �ړ���(�P�ʃx�N�g��)

		double _lastX;	// �_�b�V���̍ő�ړ�����(���W)
		
		int _aFrame;	// �A�N�V�������s�̂��߂̃t���[��
		int _aCount;	// �A�j���[�V�����̓Y����
		int _sounds;

		int _dashInterval;	// �_�b�V���̍Ďg�p�Ԋu
		double _jumpPower;	// �W�����v����
		bool _changeGraph;	// �`��O���t�B�b�N�؂�ւ��p�t���O
		bool _input;	//�_�b�V�����Ă��邩�ǂ����itrue�̏ꍇ�͑��A�N�V���������s�ł��Ȃ��j
		// std::string _divKey;	// �ǂݍ��ݗp���ʃL�[
		Keys _divKey;	// ����:�O���t�B�b�N�p�A�E��:�T�E���h�p
		KeyDates _aMotions;	// first:�L�[, s.first:���t���[����, SE�̍Đ�����
		boxs _collisions;	// �L�[�A�����s���E��BOX(AABB�N���X)

		// std::pair<double, double> _moveVector;	// �ړ�����(����:x, �E��:y)

		void AnimationInit();
		bool GraphResearch(int* gh);	// ����
		int GetSize(const std::string& key);
		int SoundResearch(const std::string& key);
		int GetSoundFrame(const std::string& key);

		void PositionUpdate();
		// Action�����܂Ƃ�
		bool Action(int key);

		// �A�N�V�������͏���
		void InputDash(double x);
		
		// �e��A�N�V����
		void Move(int lever); // �ړ�
		void Dash(); // �_�b�V��
		void Jump(); // �W�����v
		void Rob(double x, double y); // �D��
		void Give(double x, double y); // �^����
		void ChangeSoul(); // ���̐؂�ւ�
		void Damage(); // �_���[�W(�m�b�N�o�b�N)
		bool Dead(); // ���S����

		AABB GetAABB();

#ifdef _DEBUG
		void DebugInfo();
#endif
	};
}

// �����蔻����s�p�̏�����ʓr�쐬�i�֐����j
