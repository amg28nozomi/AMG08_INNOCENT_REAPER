#pragma once
#include "Vector2.h"
#include <DxLib.h>

class AABB {
private:
	Vector2 minV;
	Vector2 maxV;
	int _width;	// ��
	int _height; // ����
	int _fix;
	bool _drawFlg; // �`�悷�邩�ǂ���
	bool _collisionFlg; // ���肷�邩�ǂ����̃t���O
public:
	AABB(const Vector2& vmin, const Vector2& vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, int fix =0 ,bool cflg = false);
	~AABB() = default;

	void Draw(int color = GetColor(255, 255, 255)); // �`��(�F�w��Ȃ��̏ꍇ�͔�)
	void SetVector(const Vector2& lhs, Vector2& rhs);
	inline Vector2 GetMin() { return minV; }
	inline Vector2 GetMax() { return maxV; }
	// �`�悷�邩�ǂ����̔���
	inline bool GetDrawFlg() { return _drawFlg; }
	// �Q�Ɠn��
	inline bool& GetbDrawFlg() { return _drawFlg; }
	// �t���O�擾�p
	inline bool GetCollisionFlg() { return _collisionFlg; }
	// �Q�Ɠn��
	inline bool& GetCollisionFlgB() { return _collisionFlg; }

	// �����蔻��
	bool HitCheck(AABB collision);
	// �X�V�E���]������
	// void Update(Vector2& pos);
	// �X�V�E���]�����L
	void Updata(Vector2& pos, bool inv);
#ifdef _DEBUG
	// �f�o�b�O�p:AABB�̕`��
	void DrawBox();
#endif
};
