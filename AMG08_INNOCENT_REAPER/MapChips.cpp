#include "MapChips.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "ResourceServer.h"
#include "Collision.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include <DxLib.h>

#include "picojson/picojson.h"

namespace {
	constexpr auto FIRST = 0;
	constexpr auto SECOND = 1;
	constexpr auto THIRD = 2;

	// ステージ読み込み用のキー
	constexpr auto STAGE_0 = "tutorial";	// チュートリアル
	constexpr auto STAGE_1 = "stage_1";		// ステージ1(廃村)
	constexpr auto STAGE_2_1 = "stage_2_1";	// ステージ2(森林-中央)
	constexpr auto STAGE_2_2 = "stage_2_2";	// ステージ2(森林-上部)
	constexpr auto STAGE_2_3 = "stage_2_3";	// ステージ2(森林-下部)
	constexpr auto STAGE_3 = "boss_stage";	// ステージ3(ボス戦)

	constexpr auto CHIP_RIGHT1 = 35;
	constexpr auto CHIP_RIGHT2 = 40;
	constexpr auto CHIP_LEFT1 = 0;
	constexpr auto CHIP_LEFT2 = 5;

	constexpr auto CHIP_UP1 = 0;
	constexpr auto CHIP_UP2 = 40;
	constexpr auto CHIP_UP3 = 0;
	constexpr auto CHIP_UP4 = 5;

	// 端
	constexpr auto CHIP_TIP1 = 35;
	constexpr auto CHIP_TIP2 = 40;
	constexpr auto CHIP_TIP3 = 0;
	constexpr auto CHIP_TIP4 = 5;

	constexpr auto CHIP_TIP5 = 0;
	constexpr auto CHIP_TIP6 = 5;
}

namespace inr {

	constexpr auto CHIP_KEY = "chips";

	MapChips::MapChips(Game& game, std::string& filePath, std::string& tiledFileName) : _game(game) { //, _debugAABB(Vector2(), Vector2()) {
		TiledJsonLoad(filePath, tiledFileName + ".json");

		auto chipCountW = std::get<SECOND>(_chipCount);
		auto chipCountH = std::get<THIRD>(_chipCount);
		auto chipSizeW = _chipSize.first;
		auto chipSizeH = _chipSize.second;
		auto chipAllNum = chipCountW * chipCountH;

		auto filename = filePath + _fileChips;

		const graph::ResourceServer::DivGraphMap mapchip{
			{CHIP_KEY, {(filename).c_str(), chipCountW, chipCountH, chipAllNum, chipSizeW, chipSizeH}},
		};
		// ResourceServerで画像読み込み
		graph::ResourceServer::LoadGraphList(mapchip);

		_chipCheck = std::make_unique<ChipHitCheck>();

		// 当たり判定を修正するチップ番号を登録
		ChipHitCheck::ChipsMap stagechip1{
			// 左辺:修正するチップ番号、右辺:修正した当たり判定
			// 棘
			{  2, {0, 40, 20 ,40}},
			{  6, {0, 5}},
			{  7, {35, 40}},

			// 左端
			{  9, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 17, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 25, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 33, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 41, {CHIP_RIGHT1, CHIP_RIGHT2}},
			// 右端
			{ 16, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 24, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 32, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 40, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 48, {CHIP_LEFT1, CHIP_LEFT2}},
			// 天井
			{ 49, {CHIP_TIP1, CHIP_TIP2, CHIP_TIP5, CHIP_TIP6}},
			{ 50, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 51, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 52, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 53, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 54, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 55, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 56, {CHIP_TIP3, CHIP_TIP4, CHIP_TIP5, CHIP_TIP6}},

			// 追加のマップチップ
			{ 58, {0, 40, 0, 5}},
			{ 59, {0, 40, 0, 5}},
			// { 61, {0, 10, 0, 10}},
		};
		_chipCheck->LoadChipsMap(STAGE_1, stagechip1);
		_chipCheck->ChangeStageKey(STAGE_1);

		// _mapManager = std::make_unique<MapDataManager>(_game.GetGame());

		// スクリーン座標初期化
		_worldPosition = { WINDOW_W / 2, WINDOW_H / 2 };
		_worldLast = _worldPosition;
	}

	MapChips::~MapChips() {
		// 初期化
		_mapData.clear();

	}

	void MapChips::Init() {

	}

	void MapChips::Process() {
		WorldClanp();



		// プレイヤーからスクリーン座標を取得して代入
		/*if (*scrX < 0) { *scrX = 0; }
		if (*scrX > mapW * chipW - WINDOW_W) *scrX = mapW * chipW + WINDOW_W;
		if (*scrY < 0) { *scrY = 0; }
		if (*scrY > mapH * chipH - WINDOW_H) *scrY = mapH * chipH + WINDOW_H;*/
	}

	void MapChips::Draw() {
#ifdef _DEBUG
		DrawFormatString(0, 300, GetColor(255, 0, 0), "worldPosition.x = %d\n", _worldPosition.IntX());
		DrawFormatString(0, 325, GetColor(255, 0, 0), "worldPosition.y = %d\n", _worldPosition.IntY());
#endif
		// 描画処理
		int x, y, layer;

		int miny = _worldPosition.IntY() - 640;
		int maxy = _worldPosition.IntY() + 640;
		int minx = _worldPosition.IntX() - 1060;
		int maxx = _worldPosition.IntX() + 1060;

		int starty = miny / _chipSize.second;
		int endy = maxy / _chipSize.second;
		int startx = minx / _chipSize.first;
		int endx = maxx / _chipSize.first;

		if (startx < 0) startx = 0;
		if (_mapSize.first < endx) endx = _mapSize.first;
		if (starty < 0) starty = 0;
		if (_mapSize.second < endy) endy = _mapSize.second;

		for (layer = 0; layer < _mapSizeLayer; ++layer) {
			for (y = starty; y < endy; ++y) {
				for (x = startx; x < endx; ++x) {

					int layerStart = _mapSize.first * _mapSize.second * layer;
					//int layerStart = endx * endy * layer;
					// int index = y * endy + x;
					int index = y * _mapSize.first + x;
					// int index = y * endx + x;
					int no = _mapData[layerStart + index];

					// 当たり判定を取得
					auto c = _chipCheck->ChipCollision(no);
					auto minX = c.GetMin().IntX();
					auto maxX = c.GetMax().IntX();
					auto minY = c.GetMin().IntY();
					auto maxY = c.GetMax().IntY();

					int posX = x * _chipSize.first - _worldPosition.IntX() + HALF_WINDOW_W;//(_worldPosition.IntX() - HALF_WINDOW_W);
					int posY = y * _chipSize.second - (_worldPosition.IntY() - HALF_WINDOW_H);
					--no;

					if (0 <= no) {

						auto gh = graph::ResourceServer::GetHandles(CHIP_KEY, no);
						DrawGraph(posX, posY, gh, TRUE);

#ifdef _DEBUG
						// デバッグ用：当たり判定の描画
						if (CheckHit(x, y)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							DrawBox(posX + minX , posY + minY, posX + maxX, posY + maxY, GetColor(255, 0, 0), TRUE);
							// DrawBox(posX, posY, posX + _chipSize.first, posY + _chipSize.second, GetColor(255, 0, 0), TRUE);
							SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
						}
#endif
					}
				}
			}
		}

		/* 検証用 */
		//auto minx = _debugAABB.GetMin().IntX();
		//auto miny = _debugAABB.GetMin().IntY();
		//auto maxx = _debugAABB.GetMax().IntX();
		//auto maxy = _debugAABB.GetMax().IntY();

		//for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
		//	for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
		//		auto chipMinX = x * _chipSize.first;// _worldPosition.GetX();
		//		auto chipMinY = y * _chipSize.second;// _worldPosition.GetY();
		//		auto chipMaxX = x * _chipSize.first + _chipSize.first;// - _worldPosition.GetX();
		//		auto chipMaxY = y * _chipSize.second + _chipSize.second;// _worldPosition.GetY();

		//		AABB mapchip({ static_cast<double>(chipMinX) , static_cast<double>(y) }, { static_cast<double>(chipMaxX), static_cast<double>(chipMaxY) });

		//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		//		mapchip.DrawBox(GetColor(255, 255, 0));
		//		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		//	}
		//}
		/* 検証用 */
	}

	void MapChips::WorldClanp() {
		auto mapW = _mapSize.first;
		auto mapH = _mapSize.second;
		auto chipW = _chipSize.first;
		auto chipH = _chipSize.second;

		// ワールド座標は画面内に収まっているかどうか？
		if (_worldPosition.GetX() < HALF_WINDOW_W) { _worldPosition.GetPX() = HALF_WINDOW_W; }
		if (mapW * chipW - HALF_WINDOW_W < _worldPosition.GetX()) { _worldPosition.GetPX() = mapW * chipW - HALF_WINDOW_W; }
		if (_worldPosition.GetY() < 0) { _worldPosition.GetPY() = HALF_WINDOW_H; }
		if (mapH * chipH - HALF_WINDOW_H < _worldPosition.GetY()) { _worldPosition.GetPY() = mapH * chipH - HALF_WINDOW_H; }

		/*if (_worldPosition.GetX() < HALF_WINDOW_W) { _worldPosition.GetPX() = HALF_WINDOW_W; }
		if (mapW * chipW - HALF_WINDOW_W < _worldPosition.GetX()) { _worldPosition.GetPX() = mapW * chipW - HALF_WINDOW_W; }
		if (_worldPosition.GetY() < HALF_WINDOW_H) { _worldPosition.GetPY() = HALF_WINDOW_H; }
		if (mapH * chipH - HALF_WINDOW_H < _worldPosition.GetY()) { _worldPosition.GetPY() = mapH * chipH - HALF_WINDOW_H; }*/
	}

	bool MapChips::Clamp(Vector2& pos) {
		auto scrPosX = pos.GetX() - _worldPosition.GetX();
		auto scrPosY = pos.GetY() - _worldPosition.GetY();
		// 対象は描画範囲内に収まっているかどうか？
		if (-960 <= scrPosX <= 960) {
			// x座標を更新
			auto fixX = scrPosX + 960;
			pos.GetPX() = fixX;
			// y座標を更新するかどうか
			if (-540 <= scrPosY <= 540) {
				auto fixY = scrPosY + 540;
				// 欲しい値は何？（差分）
				// auto xx = _worldPosition.GetY() - HALF_WINDOW_H;
				pos.GetPY() = fixY;
			}
			return true;
		}

		//// 0より小さい場合は修正
		//if (cwindowW < 0 ) return false;

		//auto x = pos.GetX() - cwindowW;
		//auto y = pos.GetY() - cwindowH;

		//// x座標は収まっているかどうか？
		//if (0 <= x <= WINDOW_W) {
		//	// 余り算で座標を変換
		//	pos.GetPX() = pos.IntX() % (WINDOW_W + 1);
		//}

		// auto y = pos.GetY() - cwindowH;
		/*auto x = pos.GetX() - _worldPosition.GetX();
		auto y = pos.GetY() - _worldPosition.GetY();*/
		// 対象の座標からカメラ座標を引いた値はプラスかどうか？
		//if (x < HALF_WINDOW_W) return false;
		// if (x < HALF_WINDOW_W && y <= HALF_WINDOW_H) return false;

		//if (x < 0 && y < 0) {
		//	// スクリーン座標内に存在しない
		//	return false;
		//}
		// 対象は現在のスクリーン座標上に存在しているか？
		//if (0 <= x <= WINDOW_W) {
		//	// ワールド座標からスクリーン座標に修正
		//	pos.GetPX() = x;//pos.GetX() - (_worldPosition.GetX() / 2);
		//	return true;
		//}

		//if (0 <= x <= WINDOW_W && 0 <= y <= WINDOW_H) {
		//	// ワールド座標からスクリーン座標に修正
		//	pos.GetPX() = x;//pos.GetX() - (_worldPosition.GetX() / 2);
		//	pos.GetPY() = y;//pos.GetY() - (_worldPosition.GetY() / 2);
		//	return true;
		//}

		return false;
	}

	bool MapChips::IsScrollX() {
		auto mapW = _mapSize.first;
		auto chipW = _chipSize.first;

		auto scrX = mapW * chipW;

		if (HALF_WINDOW_W < _worldPosition.GetX()) { return true; }
		else if (scrX - HALF_WINDOW_W < _worldPosition.GetX()) { return true; }
		return false;
		// if (mapH * chipH - HALF_WINDOW_H < _worldPosition.GetY()) { _worldPosition.GetPY() = mapH * chipH - HALF_WINDOW_H; }
	}

	bool MapChips::IsScrollY() {
		auto mapH = _mapSize.second;
		auto chipH = _chipSize.second;
		auto scrY = mapH * chipH;

		// ワールドY座標はスクロール開始地点を超えているか？
		if (HALF_WINDOW_H < _worldPosition.GetY()) { return true; }
		else if (scrY - HALF_WINDOW_H < _worldPosition.GetY()) { return true; }
		return false;
	}

	void MapChips::WorldUpdate(Vector2 pos) {
		// 前フレームの座標として登録
		_worldLast = _worldPosition;

		_worldPosition.GetPX() = pos.GetX();
		_worldPosition.GetPY() = pos.GetY();
		WorldClanp();
	}

	std::string MapChips::StringFillLoad(std::string FillName) {
		FILE* fp;
		fopen_s(&fp, FillName.c_str(), "rb");

		if (fp == NULL) {
			return "";
		}

		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char* p = new char[size + 1];
		fread(p, 1, size, fp);
		fclose(fp);
		p[size] = '\0';

		std::string s = static_cast<std::string>(p);
		delete[] p;

		return s;
	}

	int MapChips::TiledJsonLoad(std::string fillPath, std::string strFillName) {
		std::string strJson = StringFillLoad(fillPath + strFillName);
		if (strJson == "") return 0;

		picojson::value json;
		picojson::parse(json, strJson);
		picojson::object jsRoot = json.get<picojson::object>();

		// マップのサイズ情報をjsonから取得
		auto mapsizeWidth  = static_cast<int>(jsRoot["width"].get<double>());
		auto mapsizeHeight = static_cast<int>(jsRoot["height"].get<double>());

		picojson::array aTileSets = jsRoot["tilesets"].get<picojson::array>();
		picojson::object jsTile = aTileSets[0].get<picojson::object>();

		auto chipCount = static_cast<int>(jsTile["tilecount"].get<double>());
		auto chipCountW = static_cast<int>(jsTile["columns"].get<double>());
		auto chipCountH = (chipCount / chipCountW);
		auto chipSizeW = static_cast<int>(jsRoot["tilewidth"].get<double>());
		auto chipSizeH = static_cast<int>(jsRoot["tileheight"].get<double>());
		auto fileChips = jsTile["image"].get<std::string>();

		picojson::array aLayers = jsRoot["layers"].get<picojson::array>();
		int layer = 0;

		for (int i = 0; i < aLayers.size(); ++i) {
			picojson::object jsLayer = aLayers[i].get<picojson::object>();
			if (jsLayer["type"].get<std::string>() == "tilelayer") {
				++layer;
			}
		}
		_mapSizeLayer = layer;
		_mapData.resize(static_cast<int>(_mapSizeLayer * _mapSize.first * _mapSize.second));
		layer = 0;


		// std::vector<int> addChipsType;
		// レイヤー内データの取得
		for (int i = 0; i < static_cast<int>(aLayers.size()); ++i) {

			picojson::object jsLayer = aLayers[i].get<picojson::object>();
			if (jsLayer["type"].get<std::string>() == "tilelayer") {
				picojson::array aData = jsLayer["data"].get<picojson::array>();

				for (int y = 0; y < _mapSize.second; ++y) {

					for (int x = 0; x < _mapSize.first; ++x) {
						int layerStart = _mapSize.first * _mapSize.second * layer;
						int index = y * _mapSize.first + x;
						_mapData[layerStart + index] = static_cast<int>(aData[index].get<double>());

						// vector配列に使用されているマップチップを登録するかどうか
						int mapchip_no = static_cast<int>(aData[index].get<double>());
						// 0の場合は処理をスキップ
						if (mapchip_no == 0) continue;
						// _mapChipsTypeに値があるかどうかを検索
						auto it = std::find(_mapChipsType.begin(), _mapChipsType.end(), mapchip_no);
						// ヒットしなかった場合は、末尾に要素を追加
						if (it == _mapChipsType.end()) {
							_mapChipsType.emplace_back(mapchip_no);
						}

						//// 要素が空の場合は、末尾に追加
						//if (addChipsType.empty()) {
						//	addChipsType.emplace_back(mapchip_no);
						//} 
						/*for (int it = 0; it < addChipsType.size(); ++it) {
							if (addChipsType[it] == mapchip_no) break;
							if (it == addChipsType.size() - 1) addChipsType.emplace_back(mapchip_no);
							} */
					}
				}
				++layer;
			}
		}
		// チップ番号を若い順に並び変える
		sort(_mapChipsType.begin(), _mapChipsType.end());
		// 末尾に-1を追加
		_mapChipsType.emplace_back(-1);

		return 1;
	}

	int MapChips::CheckHit(int x, int y) {

		auto mapSizeW = _mapSize.first;
		auto mapSizeH = _mapSize.second;

		if (0 <= x && x < mapSizeW && 0 <= y && y < mapSizeH) {
			int chip_no = _mapData[y * mapSizeW + x];
			// 当たるIDかどうかの判定
			auto i = 0;
			while (_mapChipsType[i] != -1) {
				if (chip_no == _mapChipsType[i]) {
					// 当たった場合、そのチップ番号を返す
					return chip_no;
				}
				++i;
			}
		}
		// 当たらなかった場合は0を返す
		return 0;
	}

	bool MapChips::IsStand(AABB box, double g) {
		auto gs = static_cast<int>(g);

		auto footMinX = box.GetMin().IntX();
		auto footMaxX = box.GetMax().IntX();	// 足元の座標
		auto footMinY = box.GetMin().IntY() + gs;
		auto footMaxY = box.GetMax().IntY() + gs;
		Vector2 m = { box.GetMin().GetX(), box.GetMin().GetY() + g };
		Vector2 n = { box.GetMax().GetX(), box.GetMax().GetY() + g };
		AABB mn = { m, n, box.GetCollisionFlg() };

		int x, y;

		// 当たり判定と接触する可能性のある範囲とのみ判定を行う
		for (y = footMinY / _chipSize.second; y <= footMaxY / _chipSize.second; ++y) {
			for (x = footMinX / _chipSize.first; x <= footMaxX / _chipSize.first; ++x) {
				int chip_no = CheckHit(x, y);	// この場所には何のチップがあるか？
				if (chip_no != 0) {
					// 衝突判定
					auto c = _chipCheck->ChipCollision(chip_no);

					double minX = x * _chipSize.first + c.GetMin().IntX();
					double maxX = x * _chipSize.first + c.GetMax().IntX();
					double minY = y * _chipSize.second + c.GetMin().IntY();
					double maxY = y * _chipSize.second + c.GetMax().IntY();
					Vector2 cmin = { minX, minY };
					Vector2 cmax = { maxX, maxY };
					AABB cbox = { cmin, cmax, true };

					// 判定範囲内に収まっているか？
					if (mn.GetMin().GetX() < cbox.GetMax().GetX() && cbox.GetMin().GetX() < mn.GetMax().GetX()) {
						// 加速値が正の場合（落下中、床と接触しているか？）
						if (0 < g) {
							// プレイヤーの上部はマップチップ上部より小さいか
							if (mn.GetMin().GetY() < cbox.GetMin().GetY() &&
								// マップチップの上部に対象の下が足元が埋まっているかどうか
								cbox.GetMin().GetY() < mn.GetMax().GetY()) return true;
							/*if (mn.GetMin().GetY() < cbox.GetMax().GetY() && cbox.GetMin().GetY() < mn.GetMax().GetY()) return true;*/
						} if (g < 0) {
							// 加速度が負の場合（）
								// プレイヤーの下部はマップチップの下部より大きいか
							if (cbox.GetMax().GetY() < mn.GetMax().GetY() && mn.GetMin().GetY() < cbox.GetMax().GetY()) return true;
							// if (mn.GetMax().GetY() < cbox.GetMin().GetY() && cbox.GetMax().GetY() < mn.GetMin().GetY()) return true;
						}
					}

					// if (mn.GetMax().GetY() < cbox.GetMin().GetY() && cbox.GetMax().GetY() < mn.GetMin().GetY()) ;



				// if (mn.HitUpDown(cbox)) return true;

				/*auto posmx = x * _chipSize.first;
				auto posnx = x * _chipSize.first;*/

				//// y座標は範囲内に収まっているか？
				//if ((minY <= footMaxY && footMaxY <= maxY) || (minY <= footMinY && footMinY <= maxY)) {
				//	if ((minX <= footMinX && footMinX <= maxX) || (minX <= footMaxX && footMaxX <= maxX)) {
				//		return true;
				//	}
				//}

				}
			}
		}

		return false;
	}

	// 当たり判定の取得
	//bool MapChips::IsHit(AABB box, double g) {
	//	int x, y;

	//	auto gs = static_cast<int>(g);

	//	auto minx = box.GetMin().IntX();
	//	auto miny = box.GetMin().IntY() + gs;
	//	auto maxx = box.GetMax().IntX();
	//	auto maxy = box.GetMax().IntY() + gs;
	//	
	//	// 対象の当たり判定
	//	Vector2 m = { static_cast<double>(minx), static_cast<double>(miny) };
	//	Vector2 n = { static_cast<double>(maxx), static_cast<double>(maxy) };
	//	AABB mn = { m, n, true };

	//	for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
	//		for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
	//			// マップチップと接触しているかどうか？
	//			int chip_no = CheckHit(x, y);

	//			// チップ番号が0かどうか
	//			if (chip_no != 0) {
	//				auto c = _chipCheck->ChipCollision(chip_no);
	//				auto minX = c.GetMin().IntX();
	//				auto maxX = c.GetMax().IntX();
	//				auto minY = c.GetMin().IntY();
	//				auto maxY = c.GetMax().IntY();

	//				// 新規追加
	//				Vector2 chipMin = { static_cast<double>(x * _chipSize.first + minX), static_cast<double>(y * _chipSize.second + minY) };
	//				Vector2 chipMax = { static_cast<double>(x * _chipSize.first + maxX), static_cast<double>(y * _chipSize.second + maxY) };
	//				AABB chipbox = { chipMin, chipMax, true };
	//				if (mn.HitCheck(chipbox)) return true;
	//				
	//			}
	//		}
	//	}
	//	// 当たっていない
	//	return false;
	//}

	bool MapChips::IsHit(AABB box, Vector2& pos, Vector2& move, bool direction) {
		int x, y;

		auto thisbox = box;
		auto movepos = pos + move;
		thisbox.Update(movepos, direction);
		//thisbox.Update()

		auto vectorX = move.GetX();
		auto vectorY = move.GetY();

		auto minx = thisbox.GetMin().IntX() + move.IntX();
		auto miny = thisbox.GetMin().IntY() + move.IntY();
		auto maxx = thisbox.GetMax().IntX() + move.IntX();
		auto maxy = thisbox.GetMax().IntY() + move.IntY();
		Vector2 minp = { static_cast<double>(minx), static_cast<double>(miny) };
		Vector2 maxp = { static_cast<double>(maxx), static_cast<double>(maxy) };

		/*AABB boxcol({ box.GetMin().GetX() + move.GetX(), box.GetMin().GetY() + move.GetY() },
			{ box.GetMax().GetX() + move.GetX(), box.GetMax().GetY() + move.GetY() }, box.GetCollisionFlg());*/


			/* 検証用 */
			// _debugAABB = { {static_cast<double>(minx), static_cast<double>(miny)}, {static_cast<double>(maxx), static_cast<double>(maxy)} };
			/* 検証用 */

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				// マップチップと接触しているかどうか？
				int chip_no = CheckHit(x, y);
				// チップ番号が0かどうか
				if (chip_no != 0) {
					//chip_no;
					// 当たり判定を取得
					auto c = _chipCheck->ChipCollision(chip_no);
					auto minX = c.GetMin().IntX();
					auto maxX = c.GetMax().IntX();
					auto minY = c.GetMin().IntY();
					auto maxY = c.GetMax().IntX();

					// 新規追加
					auto chipMinX = x * _chipSize.first + minX;
					auto chipMinY = y * _chipSize.second + minY;
					auto chipMaxX = x * _chipSize.first + maxX;
					auto chipMaxY = y * _chipSize.second + maxY;

					// 範囲内に収まっているか？
					if (minp.GetX() < chipMaxX && chipMinX < maxp.GetX()) {
						if (vectorY < 0) {
							// ジャンプアクション中に天井にめり込んでいるか？
							// 天井のmaxyよりも対象のminyが小さくてかつ、
							if (minp.GetY() < chipMaxY && chipMinY < maxp.GetY()) {
								auto cave = box.GetHeightMin();
								move.GetPY() = 0;	// 移動量初期化
								pos.GetPY() = chipMaxY + cave;
							}
							else if (0 < vectorY) {
								
							}
						}
					}

					if (minp.GetY() < chipMaxY && chipMinY < maxp.GetY()) {
						if (vectorX < 0) {
							// 
							if (minp.GetX() <= chipMaxX && chipMinX < maxp.GetX()) {
								auto cave = box.GetWidthMin();
								move.GetPX() = 0;
								pos.GetPX() = chipMaxX + cave;
							}
						}
						else if (0 < vectorX) {
							if (chipMinX <= maxp.GetX() && minp.GetX() < chipMaxX) {
								auto cave = box.GetWidthMin();
								move.GetPX() = 0;
								pos.GetPX() = chipMinX - cave;
							}
						}
					}

					/*auto chipMinX = x * _chipSize.first;
					auto chipMinY = y * _chipSize.second;
					auto chipMaxX = x * _chipSize.first + _chipSize.first;
					auto chipMaxY = y * _chipSize.second + _chipSize.second;*/



					//AABB mapchip({ static_cast<double>(chipMinX) , static_cast<double>(chipMinY) }, { static_cast<double>(chipMaxX), static_cast<double>(chipMaxY)}, true);
					// //押し出し処理
					//if (thisbox.HitDirection(mapchip)) {
					//	move.GetPX() = thisbox.HitDirection(mapchip);
					//}

					// x座標のめり込み判定
					// if (vectorX < 0 && boxcol.HitCheck(mapchip) == true) {
					// 左移動状態かつ
					// 埋まっているか？
					//if (vectorX < 0 && (miny < chipMinY && miny < chipMaxY)
					//	//chipMinY <= miny && miny <= chipMaxY) || (chipMinY <= maxy && maxy <= chipMaxY)) {
					//	if (minx < chipMaxX && chipMinX < minx) {
					//		pos.GetPX() = chipMaxX + 1;
					//		move.GetPX() = 0;
					//	}
					//}
					//if (0 < vectorX && (chipMinY < miny && miny < chipMaxY) || (chipMinY < maxy && maxy < chipMaxY)) {
					//	if ( chipMinX < maxx && maxx < chipMinX) {
					//		pos.GetPX() = chipMinX - 1;
					//		move.GetPX() = 0;
					//	}
					//}

					//if (vectorX < 0 && mapchip.SideCheck(boxcol) == true) {
					//	// めり込んでいるピクセルを算出し、座標を置き換える
					//	//auto m = (chipMaxX - minx);
					//	//auto vec1 = _chipSize.first - m;

					//	//pos.GetPX() = pos.GetX() + vec1;
					//	//// pos.GetPX() = chipMaxX + boxcol.GetCenter().GetX();//pos.GetX() + vec1;
					//	//move.GetPX() = 0;
					//	
					//	// move.GetPX() = (_chipSize.first - m) / vectorX;
					//} if (0 < vectorX && mapchip.SideCheck(boxcol) == true) {
					//	// auto n = chipMaxX - maxx;
					//	/*auto n = maxx - chipMaxX;
					//	auto vec2 = _chipSize.first - n;
					//	move.GetPX() = pos.GetPX() - vec2;
					//	move.GetPX() = 0;*/
					//}
					// y座標のめり込み判定
					// 横は収まっているか？
					// ジャンプ

					//if (vectorY < 0 && (chipMinX <= minx && minx <= chipMaxX) || (chipMinX <= maxx && maxx <= chipMaxX)) {
					//	if (miny < chipMaxY) {
					//		pos.GetPY() = chipMaxY - 1;
					//		move.GetPY() = 0;
					//	}
					//}
					//// 落下処理
					//if (0 < vectorY && (chipMinX <= minx && minx <= chipMaxX) || (chipMinX <= maxx && maxx <= chipMaxX)) {
					//	// 対象のボックス下はマップチップ上にめり込んでいるか
					//	if (chipMinY < maxy) { 
					//		pos.GetPY() = chipMinY + 1;
					//		move.GetPY() = 0;
					//	}

					//}

					/*if (minY <= footMaxY && footMaxY <= maxY) {
						if ((minX <= footMinX && footMinX <= maxX) || (minX <= footMaxX && footMaxX <= maxX)) {
							return true;
						}
					}*/


					/*if (vectorY < 0 && mapchip.HitCheck(mapchip) == true) {
						move.GetPY() = 0;
					} else if (0 < vectorY && mapchip.HitCheck(mapchip) == true) {
						move.GetPY() = 0;
					}*/
					return true;
				}
			}
		}
		return false;
	}
}