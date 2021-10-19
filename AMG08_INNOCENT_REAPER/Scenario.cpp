/*****************************************************************//**
 * @file   Scenario.cpp
 * @brief  シナリオクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Scenario.h"
#include "Game.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include <unordered_set>
#include <string>
#include <algorithm>
#include "ObjectValue.h"
#include "GimmickValue.h"
#include "Loads.h"
#include "Player.h"
#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "BigDoll.h"
#include "CrowDoll.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "Door.h"
#include "Block.h"
#include "Crystal.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "ItemServer.h"
#include "Item.h"
#include "Particle_Image.h"
#include "TutorialImage.h"
#include "TutorialValue.h"
#include "TutorialServer.h"

namespace {
	constexpr auto COUNT_MIN = 0;
}

namespace inr {

	Scenario::Scenario(Game& game) : _game(game) {

		Init();	// 配列初期化
	}

	Scenario::~Scenario() {
		ClearScenario();
	}

	void Scenario::Init() {
		_scenarios.clear();
	}

	void Scenario::LoadObjectData(std::string key , std::vector<ObjectValue> ovalues) {
		auto it = _scenarios.find(key);
		if (it == _scenarios.end()) {
			_scenarios.emplace(key, ovalues);
		}
	}

	void Scenario::LoadImageData(ImageMap imap) {
		for (auto image : imap) {
			// すでに登録されているか確認。登録されていない場合のみ、処理を続ける
			if (_game.GetModeServer()->GetModeMain()->GetItemImages()->IsResister(image.first) != true) continue;
			_game.GetModeServer()->GetModeMain()->GetItemImages()->AddImage(image.first, AddImage(image.second));
		}
	}

	void Scenario::ClearScenario() {
		_scenarios.clear();
	}

	bool Scenario::AddObjects(const std::string key) {
		IsLoad(key);				// オブジェクトの読み込みを行うか？
		AddPlayer();				// プレイヤーを生成するか？
		AddTutorials(key);	// チュートリアルUIを生成する
		if (_game.GetModeServer()->GetModeMain()->GetItemImages()->IsLoad() == true) LoadImageData(Loads::LoadImages());

		auto it = _scenarios.find(key);
		// 登録されているオブジェクトの生成
		for (auto ovalue : it->second) {
			auto classType = ovalue.ClassName();	// 生成するオブジェクトは何なのか？
			switch (classType) {
			// ソルジャードールの生成
			case oscenario::OBJ_SOLDIER_DOLL:
				AddSoldierDoll(ovalue);
				continue;
			// ビッグドールの生成
			case oscenario::OBJ_BIG_DOLL:
				AddBigDoll(ovalue);
				continue;
			// クロウドールの生成
			case oscenario::OBJ_CROW_DOLL:
				AddCrowDoll(ovalue);
				continue;
			// レバーの生成
			case oscenario::OBJ_LEVER:
				AddLever(ovalue);
				continue;
			// 水晶の生成
			case oscenario::OBJ_CRYSTAL:
				AddCrystal(ovalue);
				continue;
			// ブロックの生成
			case oscenario::OBJ_BLOCK:
				AddBlock(ovalue);
				continue;
			// ボス専用ドアの生成
			case oscenario::OBJ_DOOR:
				AddDoor(ovalue);
				continue;
			// アイテムの生成
			case oscenario::OBJ_ITEM:
				AddItem(ovalue);
				continue;
			default:
#ifdef _DEBUG
				OutputDebugString("error：オブジェクトの生成に失敗しました　Scenatio->AddObjectsで存在しないクラスの値が登録されています\n");
#endif
				continue;
			}
		}
		return true;
	}

	bool Scenario::IsLoad(const std::string key) {
		auto it = _scenarios.find(key);
		if (it != _scenarios.end()) return false;	// 登録されている場合はそのまま生成に移る

		// 該当するシナリオの読み込みを行う
		std::vector<ObjectValue> _ovalue;
		if (key == stage::STAGE_0) _ovalue = Loads::LoadScenarioS();
		if (key == stage::STAGE_T) _ovalue = Loads::LoadScenarioS_1();
		if (key == stage::STAGE_1) _ovalue = Loads::LoadScenario1();
		if (key == stage::STAGE_2) _ovalue = Loads::LoadScenario2();
		if (key == stage::STAGE_2_1) _ovalue = Loads::LoadScenario2_1();
		if (key == stage::STAGE_2_2) _ovalue = Loads::LoadScenario2_2();
		if (key == stage::STAGE_3) _ovalue = Loads::LoadScenarioB();
		LoadObjectData(key, _ovalue);	// 登録
		return true;
	}

	void Scenario::AddPlayer() {
		// 既に自機が登録されている場合は処理を終了する
		if (_game.GetObjectServer()->IsPlayer() == true) return;
#ifdef _DEBUG
		 // ObjectValue ovalue(oscenario::OBJ_PLAYER, { 4000, 400 });	// ボスステージデバッグ用
		ObjectValue ovalue(oscenario::OBJ_PLAYER, { 560, 905 });
#endif
#ifndef _DEBUG
		ObjectValue ovalue(oscenario::OBJ_PLAYER, { 560, 905 });	// ステージSに合わせた地点に生成する
#endif
		auto player = std::make_shared<Player>(_game.GetGame());
		player->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(player));
	}

	void Scenario::AddSoldierDoll(ObjectValue ovalue) {
		auto enemy = std::make_shared<SoldierDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
	}

	void Scenario::AddBigDoll(ObjectValue ovalue) {
		auto enemy = std::make_shared<BigDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
	}

	void Scenario::AddCrowDoll(ObjectValue ovalue) {
		auto enemy = std::make_shared<CrowDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
	}

	void Scenario::AddLever(ObjectValue ovalue) {
		auto glever = std::make_shared<Lever>(_game.GetGame());
		glever->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(glever));
	}

	void Scenario::AddCrystal(ObjectValue ovalue) {
		auto gcrystal = std::make_shared<Crystal>(_game.GetGame());
		gcrystal->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(gcrystal));
	}

	void Scenario::AddBlock(ObjectValue ovalue) {
		auto glever = std::make_shared<Block>(_game.GetGame());
		auto posv = ovalue.Positions();
		glever->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(glever));
	}

	void Scenario::AddDoor(ObjectValue ovalue) {
		auto gdoor = std::make_shared<Door>(_game.GetGame());
		gdoor->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(gdoor));
	}

	std::unique_ptr<Particle_Image> Scenario::AddImage(ImageValue pvalue) {
		auto pimage = std::make_unique<Particle_Image>(_game.GetGame());
		pimage->SetImage(pvalue);
		return std::move(pimage);
	}

	void Scenario::AddItem(ObjectValue ovalue) {
		auto item = std::make_shared<Item>(_game.GetGame());
		item->SetParameter(ovalue);
		_game.GetModeServer()->GetModeMain()->GetItemServer()->Add(std::move(item));
	}

	void Scenario::ScenarioUpdate(std::string key) {
		// 現在登録されているギミックの値を取得する
		auto Gimmicks = _game.GetGimmickServer()->GetGimmicks();
		auto Items = _game.GetModeServer()->GetModeMain()->GetItemServer()->GetItems();

		auto scenario = _scenarios.find(key);
#ifdef _DEBUG
		if (scenario == _scenarios.end()) {
			OutputDebugString("Scenario->ScenarioUpdate(std::string key)：登録されていないキーが渡されています。修正してください。\n");
			return;
		}
#endif
		std::vector<std::shared_ptr<GimmickBase>> gs;
		auto gsize = static_cast<int>(Gimmicks.size());
		int fix = 0;
		// ギミックの数値を修正する
		for (auto gimmick = 0; gimmick < gsize; ++gimmick) {
			// if (gsize != static_cast<int>(Gimmicks.size())) fix = (gsize - static_cast<int>(Gimmicks.size()));
			if (Gimmicks[gimmick]->GimmickType() == gimmick::DOOR) continue;	// ドアの場合は処理を飛ばす
			Gimmicks[gimmick]->ObjValueUpdate();
			gs.emplace_back(std::move(Gimmicks[gimmick]));
		}
		Gimmicks.clear();	// 役目を終えたのでメモリ処理

		for (auto ite : Items) ite->ObjValueUpdate();

		for (auto&& ovalue : scenario->second) {
			if (gs.empty() && Items.empty()) break;							// 要素が空になった場合は処理を終了する
			if (ovalue.ObjectType() == oscenario::type::ITEM) {	// アイテムの場合は更新をかける
				ovalue = Items.front()->GetObjectValue();					// 情報を取得
				Items.erase(Items.begin());
				continue;
			}
			if (gs.empty()) continue;
			// ギミックにのみ更新をかける
			if (ovalue.ObjectType() != oscenario::type::GIMMICK) continue;
			ovalue = gs.front()->GetObjectValue();	// 現在の情報に上書きする
			gs.erase(gs.begin());										// 先端の要素を削除する
		}
	}

	void Scenario::BossBlock() {
		ObjectValue b_block = { oscenario::OBJ_BLOCK, {3840, 820}, false,  0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE} };
		AddBlock(b_block);
	}

	bool Scenario::AddTutorials(const std::string key) {
		if (key == stage::STAGE_0) {
			auto ite =  Loads::LoadTutorialF();
			for (auto&& it : ite) {
				auto timage = std::make_unique<TutorialImage>(_game.GetGame());
				timage->SetTutorialImage(it);
				_game.GetModeServer()->GetModeMain()->GetTutorialServer()->Add(std::move(timage));
			}
			return true;
		}
		else if (key == stage::STAGE_T) {
			auto ite = Loads::LoadTutorialS();
			for (auto&& it : ite) {
				auto timage = std::make_unique<TutorialImage>(_game.GetGame());
				timage->SetTutorialImage(it);
				_game.GetModeServer()->GetModeMain()->GetTutorialServer()->Add(std::move(timage));
			}
			return true;
		}
		else return false;
	}
}