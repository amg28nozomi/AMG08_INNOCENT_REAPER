#pragma once
#include "Vector2.h"
#include <string>
#include "Particle_Image.h"
#include "Collision.h"
#include "ObjectValue.h"

namespace inr {

	namespace item {
		constexpr auto ITEM = "item_light";
		constexpr auto ITEM_IMAGE = 150;
	}

	class Game;

	/*class ImageValue {
	public:
		ImageValue(std::string key, Vector2 pos, int width = 0, int height = 0);
		~ImageValue() = default;

		inline std::string GraphKey() { return _key; }
		inline Vector2 Position() { return _position; }
		inline int Width() { return _width; }
		inline int Height() { return _height; }
	private:
		std::string _key;
		Vector2 _position;

		int _width;
		int _height;
	};*/

	class Item {
	public:
		Item(Game& game);
		~Item() = default;

		void Init();
		void Process();
		void Draw();

		void SetParameter(ObjectValue ovalue);
		void ObjValueUpdate();
		inline bool IsGet() { return _isGet; }
		inline bool IsDel() { return _del; }

		inline ObjectValue GetObjectValue() { return _oValue; }
	private:
		Game& _game;
		ObjectValue _oValue;
		Collision _col;	// �����蔻��

		std::string _gkey;	// �摜�̃L�[
		Vector2 _position;
		int _count;
		int _maxFrame;	// �A�j���[�V�����̕`��t���[����
		int _messageNo;	//�@�Ή����Ă���message�ԍ�
		bool _isGet;	// ���łɓ��肳��Ă��邩�H
		bool _del;
	};

}
