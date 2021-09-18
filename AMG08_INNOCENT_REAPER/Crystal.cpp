#include "Crystal.h"
#include "Door.h"
#include "Game.h"
#include "Player.h"
#include "ObjectServer.h"
#include "SoundServer.h"

namespace inr {

	Crystal::Crystal(Game& game) : GimmickBase(game) {
		_gType = GimmickType::CRYSTAL;
		_divKey = { "", gimmick::door::SE_CLOSE_DOOR };
		_motionKey.clear();
		_soul.reset();
		_doors.clear();
	}

	void Crystal::Init() {

	}

	void Crystal::Process() {
	}

	void Crystal::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph = graph::ResourceServer::GetHandles(_divKey.first, 0);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}

	void Crystal::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];	// 水晶の座標
		AddSoul();
		GraphKey();

		for (auto i = 0; i < _oValue.DoorType().size(); ++i) _doors.emplace_back(std::move(std::make_shared<Door>(_game.GetGame())));
		// ドアの調整
		std::vector<std::string> key;
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) {
			switch (_oValue.DoorType().at(i)) {
			case oscenario::gimmick::crystal::DOOR_RED:
				key.emplace_back(gimmick::door::KEY_DOOR_RED);
				break;
			case oscenario::gimmick::crystal::DOOR_BLUE:
				key.emplace_back(gimmick::door::KEY_DOOR_BLUE);
				break;
			default:
				break;	// そんなものはない
			}
		}
		SetDoors(key);
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _game.GetObjectServer()->Add(_doors.at(i));
		
		_mainCollision = { _position, 50, 50, 30, 70, true };	// 当たり判定
	}

	void Crystal::SetDoors(std::vector<std::string> key) {
		// 魂の色を見て対応したものを開ける
		if (_soul == nullptr) { // 魂が空ではない時
			for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) { 
				_doors[i]->SetParameter(_oValue.Positions()[1 + i], key.at(i), oscenario::gimmick::FLAG_FALSE); 
			}
			return;
		}
		if (_doors.size() == 1) {
			// 色を比較する（アイテム）
			auto flag = oscenario::gimmick::FLAG_FALSE;
			if (_doors[0]->DoorColor() == static_cast<int>(_soul->SoulColor())) flag = oscenario::gimmick::FLAG_TRUE;
			_doors[0]->SetParameter(_oValue.Positions()[1], key.at(0), gimmick::ON);
			return;
		}
		// ドアが1つ以上ある場合
		std::vector<int> flag;
		switch (_soul->SoulColor()) {
		case soul::RED:
			flag = { oscenario::gimmick::FLAG_TRUE, oscenario::gimmick::FLAG_FALSE };
			break;
		case soul::BLUE:
			flag = { oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::FLAG_TRUE };
			break;
		}
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _doors[i]->SetParameter(_oValue.Positions()[1 + i], key.at(i), flag.at(i));
	}

	void Crystal::GraphKey() {
		// 自分の見た目はどちらか
		switch (_oValue.CrystalType()) {
		case oscenario::gimmick::crystal::TYPE_STAGE_0:
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_EMPTY;
				return;
			}
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_RRD;
				return;
			case soul::BLUE:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_BLUE;
				return;
			}
		case oscenario::gimmick::crystal::TYPE_STAGE_2:
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type2::KEY_CRYSTAL_EMPTY;
				return;
			}
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = gimmick::crystal::type2::KEY_CRYSTAL_RED;
				return;
			case soul::BLUE:
				_divKey.first = gimmick::crystal::type2::KEY_CRTSTAL_BLUE;
				return;
			}
			return;
		default:
			break;
		}
	}

	void Crystal::AddSoul() {
		if (_oValue.SoulType() == 0) {
			_soul = nullptr;
			return;
		}
		auto soul_c = std::make_shared<SoulSkin>(_game.GetGame());
		
		switch (_oValue.SoulType()) {
		case 1:
			soul_c->SetParameter(_oValue.SoulType(), 7);
			break;
		case 2:
			soul_c->SetParameter(_oValue.SoulType(), 7);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error：EnemyBase->SetParameter　ObjectValueの_soulTypeの値が不正です\n");
#endif
			break;
		}
		_soul = soul_c;
		_game.GetObjectServer()->Add(std::move(soul_c));
	}

	void Crystal::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		auto player = _game.GetObjectServer()->GetPlayer();
		// 魂は奪われるか？
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_mainCollision.HitCheck(acollision)) {
					auto sound = se::SoundServer::GetSound(gimmick::door::SE_CLOSE_DOOR);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
					// 扉を両方閉じる
					for (auto door_c : _doors) door_c->SwitchOff();
					

					_soul->SetSpwan(_position);	// 自身の中心座標に実体化させる

					// 自機が保有する魂が所持上限に到達している場合は所有権を手放す
					if (player->IsSoulMax()) {
						_soul->OwnerNull();
						_soul.reset();	// 所有権を手放す
						GraphKey();	// 画像切り替え
						return;
					}
					player->SoulCatch(std::move(_soul));	// 魂の所有権をプレイヤーに譲渡
					GraphKey();	// 画像切り替え
					return;
				}
			}
		}
		// 魂を与えられるか？
		if (ckey == PKEY_GIVE) {
			// プレイヤーは魂を所持しているか？
			if (player->HaveSoul()) {
				// 魂が空の場合にボックスが接触したら
				if (_soul == nullptr) {
					// 接触時の判定はAABBで行う（奪うアクションとは違い、向きによる制限なし）
					if (_mainCollision.HitCheck(acollision)) {
						_soul = player->GiveSoul();	// プレイヤ―から対象の魂を受け取る
						_soul->Inactive();	// 魂を非活性状態にする
						GraphKey();	// 画像切り替え
						// 対応したドアを開く
						for (auto door : _doors) {
							if (IsOpen(door->DoorColor()) != true) continue;
							door->SwitchOn();	// 一致した場合は扉を開く
							
						}
					}
				}
			}
		}
	}

	bool Crystal::IsOpen(int type) {
		if (static_cast<int>(_soul->SoulColor()) != type) return false;
		return true;
		// この色と魂の色が一致しているか？
		/*auto color = 0;
		if (_soul->SoulColor() == soul::BLUE) color = 1;
		if (color != type) return false;
		return true;*/

	}


	void Crystal::ObjValueUpdate() {
		// 魂が空の場合は初期状態に戻す
		if (_soul == nullptr) {
			// 
			_oValue.SoulUpdate(0);
			return;
		}
		else {
			// 初期化する
			switch (_soul->SoulColor()) {
			case soul::RED:
				_oValue.SoulUpdate(1);
				break;
			case soul::BLUE:
				_oValue.SoulUpdate(2);
				break;
			}
		}
	}
}