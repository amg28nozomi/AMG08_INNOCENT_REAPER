/*****************************************************************//**
 * @file   Crystal.cpp
 * @brief  水晶クラス（ギミックベースクラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Crystal.h"
#include "Door.h"
#include "Game.h"
#include "Player.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "GimmickValue.h"
#include "SoundServer.h"

namespace inr {
	// コンストラクタ
	Crystal::Crystal(Game& game) : GimmickBase(game) {
		_gType = GimmickType::CRYSTAL;	// ギミックタイプの設定
		_divKey = { "", gimmick::door::SE_CLOSE_DOOR };	// 画像キーの設定
		// 各種初期化
		_motionKey.clear();
		_soul.reset();
		_doors.clear();
	}
	// 描画
	void Crystal::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// 座標のクランプ
		// 描画座標の算出
		auto x = xy.IntX();
		auto y = xy.IntY();
		// グラフィックハンドルの取得
		int graph = graph::ResourceServer::GetHandles(_divKey.first, 0);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		// 当たり判定の描画
		DrawDebugBox(_mainCollision);
#endif
	}
	// オブジェクト情報の登録
	void Crystal::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// オブジェクト情報の登録
		_position = _oValue.Positions()[0];	// 水晶の座標
		AddSoul();	// 魂の生成
		GraphKey();	// 画像キーの算出
		// ドアの生成
		for (auto i = 0; i < _oValue.DoorType().size(); ++i) _doors.emplace_back(std::move(std::make_shared<Door>(_game.GetGame())));
		// ドアの調整
		std::vector<std::string> key;
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) {
			switch (_oValue.DoorType().at(i)) {
				// 赤い扉
			case oscenario::gimmick::crystal::DOOR_RED:
				key.emplace_back(gimmick::door::KEY_DOOR_RED);
				break;
				// 青い扉
			case oscenario::gimmick::crystal::DOOR_BLUE:
				key.emplace_back(gimmick::door::KEY_DOOR_BLUE);
				break;
			default:
				break;
			}
		}
		SetDoors(key);	// ドアの設定
		// ギミックサーバーに登録する
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _game.GetGimmickServer()->Add(_doors.at(i));
		
		// 当たり判定
		_mainCollision = { _position, 50, 50, 30, 70, true };
	}
	// ドアの設定
	void Crystal::SetDoors(std::vector<std::string> key) {
		// 魂の色を見て対応したものを開ける
		if (_soul == nullptr) { // 魂が空ではない時
			for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) { 
				_doors[i]->SetParameter(_oValue.Positions()[1 + i], key.at(i), oscenario::gimmick::FLAG_FALSE); 
			}
			return;
		}
		if (_doors.size() == 1) {
			// 色を比較
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
	// 描画画像の設定
	void Crystal::GraphKey() {
		// 水晶の種類に応じて、キーを切り替える
		switch (_oValue.CrystalType()) {
		// 封印の祭壇(ステージ0～チュートリアルステージ)
		case oscenario::gimmick::crystal::TYPE_STAGE_0:
			// 魂は空か？
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_EMPTY;
				return;
			}
			// 魂は何色か？
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_RRD;
				return;
			case soul::BLUE:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_BLUE;
				return;
			}
			// 契りの森(ステージ2)
		case oscenario::gimmick::crystal::TYPE_STAGE_2:
			// 魂は空か？
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type2::KEY_CRYSTAL_EMPTY;
				return;
			}
			// 魂は何色か？
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
	// 魂の生成
	void Crystal::AddSoul() {
		// 魂は空か？
		if (_oValue.SoulType() == 0) {
			_soul = nullptr;
			return;
		}
		// 魂を生成
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());
		// 魂の生成情報に応じて、値を変更
		switch (_oValue.SoulType()) {
		case 1:
			// 赤
			soul->SetParameter(_oValue.SoulType(), 7);
			break;
		case 2:
			// 青
			soul->SetParameter(_oValue.SoulType(), 7);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error：EnemyBase->SetParameter　ObjectValueの_soulTypeの値が不正です\n");
#endif
			break;
		}
		// 各種魂の登録
		_soul = soul;
		_game.GetObjectServer()->Add(std::move(soul));
	}
	// 自機アクションとの衝突判定
	void Crystal::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		auto player = _game.GetObjectServer()->GetPlayer();
		// 魂は奪われるか？
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_mainCollision.HitCheck(acollision)) {
					// 扉を両方閉じる
					for (auto door_c : _doors) door_c->SwitchOff();
					auto sound = se::SoundServer::GetSound(gimmick::crystal::KEY_ROB);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
					_soul->SetSpwan(_position);	// 中心座標に実体化させる

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
						auto sound = se::SoundServer::GetSound(gimmick::crystal::KEY_GIVE);
						PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));

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
	// ドアの解放
	bool Crystal::IsOpen(int type) {
		// ドアの色と保有している魂を比較
		if (static_cast<int>(_soul->SoulColor()) != type) return false;
		// 一致している
		return true;
	}
	// オブジェクト情報の更新
	void Crystal::ObjValueUpdate() {
		// 魂が空の場合は初期状態に戻す
		if (_soul == nullptr) {
			_oValue.SoulUpdate(0);
			return;
		} else {
			// 魂の情報を登録
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