#pragma once
#include "Vector2.h"
#include <string>
#include "Particle_Image.h"
#include "Collision.h"
#include"ObjectValue.h"

namespace inr {

	class Game;

	class ImageValue {
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
	};

	class Item {
	public:
		Item(Game& game, std::string no);
		~Item() = default;

		void Init();
		void Process();
		void Draw();

		void SetParameter(ObjectValue ovalue, ImageValue ives);
		inline bool IsGet() { return _isGet; }
		inline bool IsDel() { return _del; }
		inline std::string ThisNnumber() { return _stageNo; }
	private:
		Game& _game;
		ObjectValue _oValue;
		Particle_Image _pi;
		Collision _col;	// 当たり判定

		std::string _gkey;	// 画像のキー
		Vector2 _position;
		std::string _stageNo;	// ステージ番号
		bool _isGet;	// すでに入手されているか？
		bool _del;
	};

}
