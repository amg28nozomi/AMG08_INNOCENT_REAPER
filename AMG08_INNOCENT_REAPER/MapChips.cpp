#include "MapChips.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "ResourceServer.h"
#include "Collision.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include "Scenario.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include <DxLib.h>

#include "picojson/picojson.h"

namespace {
	constexpr auto FIRST = 0;
	constexpr auto SECOND = 1;
	constexpr auto THIRD = 2;

	constexpr auto CHIP_RIGHT1 = 35;
	constexpr auto CHIP_RIGHT2 = 40;
	constexpr auto CHIP_LEFT1 = 0;
	constexpr auto CHIP_LEFT2 = 6;

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

	constexpr auto CHIP_THORM1 = 0;
	constexpr auto CHIP_THORM2 = 40;
	constexpr auto CHIP_THORM3 = 20;
	constexpr auto CHIP_THORM4 = 40;

	constexpr auto CHIP_THORM_LEFT1 = 20;	// 棘左端(xmin)
	constexpr auto CHIP_THORM_LEFT2 = 40;	// 棘左端(xmax)
	constexpr auto CHIP_THORM_LEFT3 = 0;
	constexpr auto CHIP_THORM_LEFT4 = 40;

	constexpr auto CHIP_THORM_TOP1 = 0;
	constexpr auto CHIP_THORM_TOP2 = 40;
	constexpr auto CHIP_THORM_TOP3 = 0;
	constexpr auto CHIP_THORM_TOP4 = 20;


	constexpr auto CHIP_THORM_RIGHT1 = 0;
	constexpr auto CHIP_THORM_RIGHT2 = 20;

	// 蔦
	constexpr auto CHIP_IVY1 = 10;
	constexpr auto CHIP_IVY2 = 30;

	constexpr auto DEFAULT_W = 960;
	constexpr auto DEFAULT_H = 540;

	constexpr auto DEFAULT_PATH = "Resource/";
	constexpr auto JSON_FORMAT = ".json";

	constexpr auto FILE_PATH = 0;
}

namespace inr {

	constexpr auto CHIP_KEY = "chips";

	MapChips::MapChips(Game& game) : _game(game), _nowMap() {
		_chipCheck = std::make_unique<ChipHitCheck>();
		_mapManager = std::make_unique<MapDataManager>(_game.GetGame());
		SetChipsMap();

		_worldPosition = { DEFAULT_W, DEFAULT_H };
		_worldLast = _worldPosition;

		_sFiles = { { DEFAULT_PATH, "stage1", JSON_FORMAT},
		};
	}

	MapChips::MapChips(Game& game, std::string& filePath, std::string& tiledFileName) : _game(game), _nowMap() { //, _debugAABB(Vector2(), Vector2()) {
		_chipCheck = std::make_unique<ChipHitCheck>();
		SetChipsMap();

		_mapManager = std::make_unique<MapDataManager>(_game.GetGame());
		 TiledJsonLoad(stage::STAGE_1, filePath, tiledFileName + ".json");
		//TiledJsonLoad(stage::STAGE_2_1, filePath, tiledFileName + ".json");
		_mapManager->GetStageMap(stage::STAGE_1, _nowMap);
		//_mapManager->GetStageMap(stage::STAGE_2_1, _nowMap);

		// スクリーン座標初期化
		_worldPosition = { WINDOW_W / 2, WINDOW_H / 2 };
		_worldLast = _worldPosition;
	}

	MapChips::~MapChips() {
		// 初期化
		_mapManager->StageMapClear();

	}

	void MapChips::Init() {

	}

	void MapChips::Process() {
		// キーが更新された場合はマップ情報を切り替える
		if (_nextStage != stage::CHANGE_NULL) {
			ChangeMap();
		}
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
		auto mswidth = _nowMap.MapSizeWidth();
		auto msheight = _nowMap.MapSizeHeight();
		auto cswidth = _nowMap.ChipSizeWidth();
		auto csheight = _nowMap.ChipSizeHeight();

		// 描画処理
		int x, y, layer;

		int miny = _worldPosition.IntY() - 640;
		int maxy = _worldPosition.IntY() + 640;
		int minx = _worldPosition.IntX() - 1060;
		int maxx = _worldPosition.IntX() + 1060;

		int starty = miny / csheight;//_chipSize.second;
		int endy = maxy / csheight;//_chipSize.second;
		int startx = minx / cswidth;//_chipSize.first;
		int endx = maxx / cswidth;//_chipSize.first;

		if (startx < 0) startx = 0;
		if (mswidth < endx) endx = mswidth;
		if (starty < 0) starty = 0;
		if (msheight < endy) endy = msheight;

		for (layer = 0; layer < _nowMap.MapSizeLayer(); ++layer) {
			for (y = starty; y < endy; ++y) {
				for (x = startx; x < endx; ++x) {

					int layerStart = mswidth * msheight * layer;
					//int layerStart = endx * endy * layer;
					// int index = y * endy + x;
					int index = y * mswidth + x;
					// int index = y * endx + x;
					int no = _nowMap.MapDatas(layerStart + index);

					// 当たり判定を取得
					auto c = _chipCheck->ChipCollision(no);
					auto minX = c.GetMin().IntX();
					auto maxX = c.GetMax().IntX();
					auto minY = c.GetMin().IntY();
					auto maxY = c.GetMax().IntY();

					int posX = x * cswidth - _worldPosition.IntX() + HALF_WINDOW_W;//(_worldPosition.IntX() - HALF_WINDOW_W);
					int posY = y * csheight - (_worldPosition.IntY() - HALF_WINDOW_H);
					--no;

					if (0 <= no) {

						auto gh = graph::ResourceServer::GetHandles(stage::KEY_NORMAL, no);
						DrawGraph(posX, posY, gh, TRUE);

#ifdef _DEBUG
						auto chiptype = _chipCheck->IsChipType(no + 1);
						std::tuple<int, int, int> colortype = std::make_tuple(255, 0, 0);
						if (chiptype == mapchip::IVY) colortype = std::make_tuple(255, 255, 0);
						if (chiptype == mapchip::THORM) colortype = std::make_tuple(255, 255, 255);
						if (chiptype == mapchip::TRANSITION) colortype = std::make_tuple(0, 0, 255);

						// デバッグ用：当たり判定の描画
						if (CheckHit(x, y)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							DrawBox(posX + minX , posY + minY, posX + maxX, posY + maxY, GetColor(std::get<0>(colortype), std::get<1>(colortype), std::get<2>(colortype)), TRUE);
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
		auto mapWidth = _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth();
		auto mapHeight = _nowMap.MapSizeHeight() * _nowMap.ChipSizeHeight();

		// ワールド座標は画面内に収まっているかどうか？
		if (_worldPosition.GetX() < HALF_WINDOW_W) { _worldPosition.GetPX() = HALF_WINDOW_W; }
		if (mapWidth - HALF_WINDOW_W < _worldPosition.GetX()) { _worldPosition.GetPX() = mapWidth - HALF_WINDOW_W; }
		if (_worldPosition.GetY() < HALF_WINDOW_H) { _worldPosition.GetPY() = HALF_WINDOW_H; }
		if (mapHeight - HALF_WINDOW_H < _worldPosition.GetY()) { _worldPosition.GetPY() = mapHeight - HALF_WINDOW_H; }

		/*if (_worldPosition.GetX() < HALF_WINDOW_W) { _worldPosition.GetPX() = HALF_WINDOW_W; }
		if (mapW * chipW - HALF_WINDOW_W < _worldPosition.GetX()) { _worldPosition.GetPX() = mapW * chipW - HALF_WINDOW_W; }
		if (_worldPosition.GetY() < HALF_WINDOW_H) { _worldPosition.GetPY() = HALF_WINDOW_H; }
		if (mapH * chipH - HALF_WINDOW_H < _worldPosition.GetY()) { _worldPosition.GetPY() = mapH * chipH - HALF_WINDOW_H; }*/
	}

	bool MapChips::Clamp(Vector2& pos) {
		// 座標はワールド座標の上限に収まっているか
		auto mapWidth = _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth();
		auto mapHeight = _nowMap.MapSizeHeight() * _nowMap.ChipSizeHeight();

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
		auto mapW = _nowMap.MapSizeWidth();//_mapSize.first;
		auto chipW = _nowMap.ChipSizeWidth();//_chipSize.first;

		auto scrX = mapW * chipW;

		if (HALF_WINDOW_W < _worldPosition.GetX()) { return true; }
		else if (scrX - HALF_WINDOW_W < _worldPosition.GetX()) { return true; }
		return false;
		// if (mapH * chipH - HALF_WINDOW_H < _worldPosition.GetY()) { _worldPosition.GetPY() = mapH * chipH - HALF_WINDOW_H; }
	}

	bool MapChips::IsScrollY() {
		auto mapH = _nowMap.MapSizeHeight();//_mapSize.second;
		auto chipH = _nowMap.ChipSizeHeight();// _chipSize.second;
		auto scrY = mapH * chipH;

		// ワールドY座標はスクロール開始地点を超えているか？
		if (HALF_WINDOW_H < _worldPosition.GetY() && _worldPosition.GetY() < scrY - HALF_WINDOW_H) { return true; }
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

	int MapChips::TiledJsonLoad(std::string skey, std::string filePath, std::string strFileName) {
		std::string strJson = StringFillLoad(filePath + strFileName);
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
		auto mapSizeLayer = layer;
		std::vector<int> mapdata;
		mapdata.resize(static_cast<int>(mapSizeLayer * mapsizeWidth * mapsizeHeight));
		layer = 0;

		std::vector<int> chiptype;

		// std::vector<int> addChipsType;
		// レイヤー内データの取得
		for (int i = 0; i < static_cast<int>(aLayers.size()); ++i) {

			picojson::object jsLayer = aLayers[i].get<picojson::object>();
			if (jsLayer["type"].get<std::string>() == "tilelayer") {
				picojson::array aData = jsLayer["data"].get<picojson::array>();

				for (int y = 0; y < mapsizeHeight; ++y) {

					for (int x = 0; x < mapsizeWidth; ++x) {
						int layerStart = mapsizeWidth * mapsizeHeight * layer;
						int index = y * mapsizeWidth + x;
						mapdata[layerStart + index] = static_cast<int>(aData[index].get<double>());

						// vector配列に使用されているマップチップを登録するかどうか
						int mapchip_no = static_cast<int>(aData[index].get<double>());
						// 0の場合は処理をスキップ
						if (mapchip_no == 0) continue;
						// _mapChipsTypeに値があるかどうかを検索
						auto it = std::find(chiptype.begin(), chiptype.end(), mapchip_no);
						// ヒットしなかった場合は、末尾に要素を追加
						if (it == chiptype.end()) {
							chiptype.emplace_back(mapchip_no);
						}
					}
				}
				++layer;
			}
		}
		// チップ番号を若い順に並び変える
		sort(chiptype.begin(), chiptype.end());
		// 末尾に-1を追加
		chiptype.emplace_back(-1);

		// 配置情報の登録
		MapDataManager::JsonMapData jmd{
			{ skey, { std::make_pair(mapsizeWidth, mapsizeHeight), std::make_tuple(chipCount, chipCountW, chipCountH),
					  std::make_pair(chipSizeW, chipSizeH), mapSizeLayer, fileChips, mapdata, chiptype }},
		};
		_mapManager->LoadStageMap(jmd);	// マップマネージャーに登録

		// ResourceServerでマップチップ画像の読み込み
		auto filename = filePath + fileChips;
		auto chipAllNum = chipCountW * chipCountH;
		const graph::ResourceServer::DivGraphMap mapchip{
			{ stage::KEY_NORMAL, {(filename).c_str(), chipCountW, chipCountH, chipAllNum, chipSizeW, chipSizeH}},
		};
		graph::ResourceServer::LoadGraphList(mapchip);

		return 1;
	}

	int MapChips::CheckHit(int x, int y) {

		auto mapSizeW = _nowMap.MapSizeWidth(); // _mapSize.first;
		auto mapSizeH = _nowMap.MapSizeHeight(); // _mapSize.second;

		auto chips = _nowMap.MapDatas();
		auto chiptype = _nowMap.ChipType();
		

		if (0 <= x && x < mapSizeW && 0 <= y && y < mapSizeH) {
			int chip_no = chips[y * mapSizeW + x];
			// 当たるIDかどうかの判定
			auto i = 0;
			while (chiptype[i] != -1) {
				if (chip_no == chiptype[i]) {
					// 当たった場合、そのチップ番号を返す
					return chip_no;
				}
				++i;
			}
		}
		// 当たらなかった場合は0を返す
		return 0;
	}

	int MapChips::IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos) {
		auto gs = static_cast<int>(g);

		auto footMinX = box.GetMin().IntX();
		auto footMaxX = box.GetMax().IntX();	// 足元の座標
		auto footMinY = box.GetMin().IntY() + gs;
		auto footMaxY = box.GetMax().IntY() + gs;
		Vector2 m = { box.GetMin().GetX(), box.GetMin().GetY() + g };
		Vector2 n = { box.GetMax().GetX(), box.GetMax().GetY() + g };
		AABB mn = { m, n, box.GetCollisionFlg() };


		auto csh = _nowMap.ChipSizeHeight();
		auto csw = _nowMap.ChipSizeWidth();

		int x, y;

		// 当たり判定と接触する可能性のある範囲とのみ判定を行う
		for (y = footMinY / csh; y <= footMaxY / csh; ++y) {
			for (x = footMinX / csw; x <= footMaxX / csw; ++x) {
				int chip_no = CheckHit(x, y);	// この場所には何のチップがあるか？
				if (chip_no != 0) {
					if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON) continue;	// 当たり判定がない場合は抜ける
					// 衝突判定
					auto c = _chipCheck->ChipCollision(chip_no);

					double minX = x * csw + c.GetMin().IntX();
					double maxX = x * csw + c.GetMax().IntX();
					double minY = y * csh + c.GetMin().IntY();
					double maxY = y * csh + c.GetMax().IntY();
					Vector2 cmin = { minX, minY };
					Vector2 cmax = { maxX, maxY };
					AABB cbox = { cmin, cmax, true };

					// 判定範囲内に収まっているか？
					if (mn.GetMin().GetX() < cbox.GetMax().GetX() && cbox.GetMin().GetX() < mn.GetMax().GetX()) {
						// 加速値が正の場合（落下中、床と接触しているか？）
						if (0 < g) {
							// 足元は埋まっていないか？
							if (mn.GetMin().GetY() < cbox.GetMin().GetY() &&
								// マップチップの上部に対象の下が足元が埋まっているかどうか
								cbox.GetMin().GetY() < mn.GetMax().GetY()) {
								auto cavep = box.GetHeightMax();
								pos.GetPY() = minY - cavep;
								// 通常判定チップの場合、座標を更新する
								if (_chipCheck->IsChipType(chip_no) == mapchip::NORMAL) *lastpos = { maxX , pos.GetY() };
								return _chipCheck->IsChipType(chip_no);
							}
							/*if (mn.GetMin().GetY() < cbox.GetMax().GetY() && cbox.GetMin().GetY() < mn.GetMax().GetY()) return true;*/
						} if (g < 0) {
							// 加速度が負の場合（）
								// プレイヤーの下部はマップチップの下部より大きいか
							if (cbox.GetMax().GetY() < mn.GetMax().GetY() && mn.GetMin().GetY() < cbox.GetMax().GetY()) { 
								auto cavep = box.GetHeightMin();
								pos.GetPY() = maxY + cavep;
								return _chipCheck->IsChipType(chip_no);
							}
							
						}
					}
				}
			}
		}

		return mapchip::TYPE_NULL;
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

	int MapChips::IsHit(AABB box, Vector2& pos, Vector2& move, bool direction, bool isUpdate) {
		int x, y;

		auto thisbox = box;
		auto movepos = pos + move;
		thisbox.Update(movepos, direction);
		//thisbox.Update()

		auto vectorX = move.GetX();
		auto vectorY = move.GetY();

		auto minx = thisbox.GetMin().IntX();
		auto miny = thisbox.GetMin().IntY();
		auto maxx = thisbox.GetMax().IntX();
		auto maxy = thisbox.GetMax().IntY();
		/*Vector2 minp = { static_cast<double>(minx), static_cast<double>(miny) };
		Vector2 maxp = { static_cast<double>(maxx), static_cast<double>(maxy) };*/

		/*AABB boxcol({ box.GetMin().GetX() + move.GetX(), box.GetMin().GetY() + move.GetY() },
			{ box.GetMax().GetX() + move.GetX(), box.GetMax().GetY() + move.GetY() }, box.GetCollisionFlg());*/


			/* 検証用 */
			// _debugAABB = { {static_cast<double>(minx), static_cast<double>(miny)}, {static_cast<double>(maxx), static_cast<double>(maxy)} };
			/* 検証用 */

		for (y = miny / _nowMap.ChipSizeHeight(); y <= maxy / _nowMap.ChipSizeHeight(); ++y) {
			for (x = minx / _nowMap.ChipSizeWidth(); x <= maxx / _nowMap.ChipSizeWidth(); ++x) {
				// マップチップと接触しているかどうか？
				int chip_no = CheckHit(x, y);
				// チップ番号が0かどうか
				if (chip_no != 0) {
					if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON ) continue;	// 当たり判定がない場合は抜ける
					// 当たり判定を取得
					auto c = _chipCheck->ChipCollision(chip_no);
					auto minX = c.GetMin().IntX();
					auto maxX = c.GetMax().IntX();
					auto minY = c.GetMin().IntY();
					auto maxY = c.GetMax().IntY();

					// 新規追加
					auto chipMinX = x * _nowMap.ChipSizeWidth() + minX;
					auto chipMinY = y * _nowMap.ChipSizeHeight() + minY;
					auto chipMaxX = x * _nowMap.ChipSizeWidth() + maxX;
					auto chipMaxY = y * _nowMap.ChipSizeHeight() + maxY;

					if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
						if (vectorY < 0) {
							if (miny < chipMinY && chipMinY < maxy) {
								auto cave = box.GetHeightMin();
								move.GetPY() = 0;	// 移動量初期化
								pos.GetPY() = chipMaxY + cave;
							}
						}
						else if (0 < vectorY) {
							if (chipMaxY < maxy && miny < chipMaxY) {
								auto cave = box.GetHeightMin();
								move.GetPY() = 0;	// 移動量初期化
								pos.GetPY() = chipMinY + cave;
							}
						}
					}

					if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
						if (vectorX < 0) {
							if (minx < chipMaxX && chipMinX < maxx) {
										// if (minx < chipMinX && chipMinX < maxx) {
								auto cave = box.GetWidthMin();
								move.GetPX() = 0;
								pos.GetPX() = chipMaxX + cave;
								return _chipCheck->IsChipType(chip_no);
								}
							}
							if (0 < vectorX) {
								if (chipMinX < maxx && minx < chipMaxX) {
										// if (chipMaxX < maxx && minx < chipMaxX) {
										// if (chipMinX < maxx && minx < chipMaxX) {
									auto cave = box.GetWidthMin();
									move.GetPX() = 0;
									pos.GetPX() = chipMinX - cave;
									return _chipCheck->IsChipType(chip_no);
								}
							}
						}
					// 向きに更新がかかっているかどうか？
					//if (isUpdate == false) {
					//	// 横のみ判定（移動量はxのみ加算）
					//	if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
					//		if (vectorX < 0) {
					//			if (minx < chipMaxX && chipMinX < maxx) {
					//				// if (minx < chipMinX && chipMinX < maxx) {
					//				auto cave = box.GetWidthMin();
					//				move.GetPX() = 0;
					//				pos.GetPX() = chipMaxX + cave;
					//				return true;
					//			}
					//		}
					//		if (0 < vectorX) {
					//			if (chipMinX < maxx && minx < chipMaxX) {
					//				// if (chipMaxX < maxx && minx < chipMaxX) {
					//				// if (chipMinX < maxx && minx < chipMaxX) {
					//				auto cave = box.GetWidthMin();
					//				move.GetPX() = 0;
					//				pos.GetPX() = chipMinX - cave;
					//				return true;
					//			}
					//		}
					//	}
					//} else {
					//	// 更新がかかっている場合は処理を反転させる
					//	if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
					//		if (vectorX < 0) {
					//			if (minx < chipMaxX && chipMinX < maxx) {
					//				// if (minx < chipMinX && chipMinX < maxx) {
					//				auto cave = box.GetWidthMax();
					//				move.GetPX() = 0;
					//				pos.GetPX() = chipMinX + cave;
					//				return true;
					//			}
					//		}
					//		if (0 < vectorX) {
					//			if (chipMinX < maxx && minx < chipMaxX) {
					//				// if (chipMaxX < maxx && minx < chipMaxX) {
					//				// if (chipMinX < maxx && minx < chipMaxX) {
					//				auto cave = box.GetWidthMax();
					//				move.GetPX() = 0;
					//				pos.GetPX() = chipMaxX - cave;
					//				return true;
					//			}
					//		}
					//	}
					//}

					// 移動ベクトル分、判定を行うバージョン
					//if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
					//	auto loopX = static_cast<int>(vectorX);
					//	if (vectorX < 0) {
					//		for (auto i = 0; i <= loopX; ++i) {
					//			auto mix = minx - i;
					//			auto max = maxx - i;
					//			if (mix < chipMaxX && chipMinX < max) {
					//				// if (minx < chipMinX && chipMinX < maxx) {
					//				auto cave = box.GetWidthMin();
					//				move.GetPX() = 0;
					//				pos.GetPX() = chipMaxX + cave;
					//				return true;
					//			}
					//		}
					//	}
					//	if (0 < vectorX) {
					//		for (auto i = 0; i <= loopX; ++i) {
					//			auto mix = minx + i;
					//			auto max = maxx + i;
					//			if (chipMinX < max && mix < chipMaxX) {
					//				// if (chipMaxX < maxx && minx < chipMaxX) {
					//				// if (chipMinX < maxx && minx < chipMaxX) {
					//				auto cave = box.GetWidthMin();
					//				move.GetPX() = 0;
					//				pos.GetPX() = chipMinX - cave;
					//				return true;
					//			}
					//		}
					//	}
					//}
					
					//if (minx < chipMaxX && chipMinX < maxx) {
					//	if (vectorY < 0) {
					//		if (box.GetMin().IntY() < chipMaxY && chipMinY < box.GetMax().IntY()) {
					//			auto cave = box.GetHeightMin();
					//			move.GetPY() = 0;	// 移動量初期化
					//			pos.GetPY() = chipMaxY + cave;
					//		}
					//		else if (chipMaxY < box.GetMax().IntY() && box.GetMin().IntY() < chipMaxY) {
					//			auto cave = box.GetHeightMin();
					//			move.GetPY() = 0;	// 移動量初期化
					//			pos.GetPY() = chipMinY + cave;
					//		}
					//	}
					//}

					//// 縦のみ修正有りver
					//if (miny < chipMaxY && chipMinY < maxy) {
					//	if (vectorX < 0) {
					//		if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
					//			auto cave = box.GetWidthMin();
					//			move.GetPX() = 0;
					//			pos.GetPX() = chipMaxX + cave;
					//		}
					//	}
					//	else if (0 < vectorX) {
					//		if (chipMinX < box.GetMax().IntX() && box.GetMin().IntX() < chipMaxX) {
					//			auto cave = box.GetWidthMin();
					//			move.GetPX() = 0;
					//			pos.GetPX() = chipMinX + cave;
					//		}
					//	}
					//}
	


					// 範囲内に収まっているか？
					//if (minp.GetX() < chipMaxX && chipMinX < maxp.GetX()) {
					//	if (vectorY < 0) {
					//		// ジャンプアクション中に天井にめり込んでいるか？
					//		// 天井のmaxyよりも対象のminyが小さくてかつ、
					//		if (minp.GetY() < chipMaxY && chipMinY < maxp.GetY()) {
					//			auto cave = box.GetHeightMin();
					//			move.GetPY() = 0;	// 移動量初期化
					//			pos.GetPY() = chipMaxY + cave;
					//		}
					//		else if (0 < vectorY) {
					//			
					//		}
					//	}
					//}

					//if (minp.GetY() < chipMaxY && chipMinY < maxp.GetY()) {
					//	if (vectorX < 0) {
					//		// 
					//		if (minp.GetX() <= chipMaxX && chipMinX < maxp.GetX()) {
					//			auto cave = box.GetWidthMin();
					//			move.GetPX() = 0;
					//			pos.GetPX() = chipMaxX + cave;
					//		}
					//	}
					//	else if (0 < vectorX) {
					//		if (chipMinX <= maxp.GetX() && minp.GetX() < chipMaxX) {
					//			auto cave = box.GetWidthMin();
					//			move.GetPX() = 0;
					//			pos.GetPX() = chipMinX - cave;
					//		}
					//	}
					//}

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
					// return true;
				}
			}
		}
		return mapchip::NONE;
	}

	void MapChips::SetChipsMap() {
		// 各種当たり判定を登録する
		// 当たり判定を修正するチップ番号を登録
		ChipHitCheck::ChipsMap stagechip {
			// 左辺:修正するチップ番号、右辺:修正した当たり判定+チップに効果を持たせるかどうか
			// 棘
			{  2, {0, 40, 20 ,40, mapchip::THORM}},
			{  6, {0, 5}},	// 端のチップには判定を持たせない
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

			// 追加のマップチップ（端）
			{ 58, {0, 40, 0, 5}},
			{ 59, {0, 40, 0, 5}},
			// { 61, {0, 10, 0, 10}},

			// 2面専用マップチップ
			// 左端
			{ 65, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 73, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 97, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 105, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 113, {CHIP_RIGHT1, CHIP_RIGHT2}},
			// 右端
			{ 72, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 80, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 100, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 108, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 116, {CHIP_LEFT1, CHIP_LEFT2}},


			// 天井
			{ 81, {CHIP_TIP1, CHIP_TIP2, CHIP_TIP5, CHIP_TIP6}},
			{ 82, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 83, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 84, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 85, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 86, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 87, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 88, {CHIP_TIP3, CHIP_TIP4, CHIP_TIP5, CHIP_TIP6}},

			// その他
			{ 90, {0, 40, 0, 5}},
			{ 91, {0, 40, 0, 5}},

			// 棘(床)
			{ 95, {0, 6}},
			{ 96, {35, 40}},
			{ 103, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			{ 104, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			{ 111, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			{ 112, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			// 棘左端
			{ 101, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 109, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 117, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 125, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			// 棘右端
			{ 102, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 110, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 118, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 126, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			// 棘(天井)
			{ 121, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			{ 122, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			{ 123, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			{ 124, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			// 蔦
			{ 119, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			{ 120, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			{ 127, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			{ 128, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			// ステージ遷移
			{ 129, {mapchip::TRANSITION}},	// ステージS(1)→ステージ1(2)
			{ 130, {mapchip::TRANSITION}},	// ステージ1(3)→ステージ2-2()
			{ 131, {mapchip::TRANSITION}},
			{ 132, {mapchip::TRANSITION}},
			{ 133, {mapchip::TRANSITION}},
			{ 134, {mapchip::TRANSITION}},
			{ 135, {mapchip::TRANSITION}},
			{ 136, {mapchip::TRANSITION}},
			{ 137, {mapchip::TRANSITION}},
			{ 138, {mapchip::TRANSITION}},
			{ 139, {mapchip::TRANSITION}},
			{ 140, {mapchip::TRANSITION}},
			{ 141, {mapchip::TRANSITION}},
			{ 142, {mapchip::TRANSITION}},
			{ 143, {mapchip::TRANSITION}},
			{ 144, {mapchip::TRANSITION}},
		};
		_chipCheck->LoadChipsMap(stage::KEY_NORMAL, stagechip);
		_chipCheck->ChangeStageKey(stage::KEY_NORMAL);	// 最初に呼び出すステージを登録
	}

	void MapChips::SetChipMember() {

	}

	void MapChips::ChangeMap() {
		// マップデータは読み込まれているか？
		if (_mapManager->IsLoad(_nextStage) == true) {
			std::string stagePath = DEFAULT_PATH + _nextStage + JSON_FORMAT;	// パスを作成
			TiledJsonLoad(_skey, DEFAULT_PATH, _skey);	// 対象の登録
		}
		/* ここにオブジェクトの初期化処理を挟む */

		/* ここでシナリオクラスを使ってオブジェクトの生成を行う */

		_skey = _nextStage;
		_nextStage = stage::CHANGE_NULL;
	}
}