#include "Item.h"
#include "Game.h"
#include "MapChips.h"
#include "Particle_Image.h"
#include "ObjectServer.h"
#include "Player.h"
#include "ObjectValue.h"

namespace inr {

	ImageValue::ImageValue(std::string key, Vector2 pos, int width, int height) {
		_key = key;
		_position = pos;
		_width = width;
		_height = height;
	}

	Item::Item(Game& game, std::string no) : _game(game), _pi(game), _oValue() {
		_gkey = "";
		_position = { 0, 0 };
		_col = { _position, 0, 0, true };
		_stageNo = no;
		_del = false;
	}

	void Item::Init() {

	}

	void Item::Process() {

		if (_isGet == true) return;	// Šù‚É“üŽè‚³‚ê‚Ä‚¢‚éê‡‚Íˆ—‚ðs‚í‚È‚¢
		auto pbox = _game.GetObjectServer()->GetPlayer()->GetMainCollision();	// Ž©‹@‚Ì“–‚½‚è”»’è‚ðŽæ“¾
		if (_col.HitCheck(pbox) == true) {
			_pi.DrawStart();
		} else { 
			_pi.DrawEnd();	// •`‰æˆ—I—¹
		}
		_pi.Process();
	}

	void Item::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_gkey, 0);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
		_pi.Draw();
	}

	void Item::SetParameter(ObjectValue ovalue, ImageValue ives) {
		_oValue = ovalue;
		_gkey = ives.GraphKey();
		_position = ives.Position();
		_col = { _position, ives.Width(), ives.Height() };
		_pi.SetParameter(ives.GraphKey(), ives.Position());
	}
}
