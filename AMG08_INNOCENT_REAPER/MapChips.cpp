#include "MapChips.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "ResourceServer.h"
#include "Collision.h"
#include "Vector2.h"
// #include "MapData.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include <DxLib.h>

#include "picojson/picojson.h"

namespace {
	constexpr auto FIRST = 0;
	constexpr auto SECOND = 1;
	constexpr auto THIRD = 2;
}

namespace inr {

	constexpr auto CHIP_KEY = "chips";

	MapChips::MapChips(Game& game, std::string& filePath, std::string& tiledFileName) : _game(game) {
		TiledJsonLoad(filePath, tiledFileName + ".json");

		auto chipCountW = std::get<SECOND>(_chipCount);
		auto chipCountH = std::get<THIRD>(_chipCount);
		auto chipSizeW = _chipSize.first;
		auto chipSizeH = _chipSize.second;
		auto chipAllNum = chipCountW * chipCountH;

		auto filename = filePath + _fileChips;

		const graph::ResourceServer::DivGraphMap mapchip {
			{CHIP_KEY, {(filename).c_str(), chipCountW, chipCountH, chipAllNum, chipSizeW, chipSizeH}},
		};
		// ResourceServerで画像読み込み
		graph::ResourceServer::LoadGraphList(mapchip);
		// スクリーン座標初期化
		_scrPosition = std::make_pair(0, 0);

	}

	MapChips::~MapChips() {
		// 各種初期化
		_mapData.clear();

	}

	void MapChips::Init() {

	}

	void MapChips::Process() {
		// 真壁講師のものを流用（バグが出次第修正予定）
		auto scrX = &_scrPosition.first;
		auto scrY = &_scrPosition.second;
		auto mapW = _mapSize.first;
		auto mapH = _mapSize.second;
		auto chipW = _chipSize.first;
		auto chipH = _chipSize.second;


		// プレイヤーからスクリーン座標を取得して代入
		/*if (*scrX < 0) { *scrX = 0; }
		if (*scrX > mapW * chipW - WINDOW_W) *scrX = mapW * chipW + WINDOW_W; 
		if (*scrY < 0) { *scrY = 0; }
		if (*scrY > mapH * chipH - WINDOW_H) *scrY = mapH * chipH + WINDOW_H;*/
	}

	void MapChips::Draw() {
#ifdef _DEBUG
		DrawFormatString(0, 300, GetColor(255, 0, 0), "scr.x = %d\n", _scrPosition.first);
		DrawFormatString(0, 325, GetColor(255, 0, 0), "scr.y = %d\n", _scrPosition.second);
#endif
		// 描画処理
		int x, y, layer;
		for (layer = 0; layer < _mapSizeLayer; ++layer) {
			for (y = 0; y < _mapSize.second; ++y) {
				for (x = 0; x < _mapSize.first; ++x) {
					int  layerStart = _mapSize.first * _mapSize.second * layer;
					int index = y * _mapSize.first + x;
					int posX = x * _chipSize.first - _scrPosition.first;
					int posY = y * _chipSize.second - _scrPosition.second;
					int no = _mapData[layerStart + index];
					--no;

					if (0 <= no) {
						auto gh = graph::ResourceServer::GetHandles(CHIP_KEY, no);
						DrawGraph(posX, posY, gh, TRUE);

#ifdef _DEBUG
						// デバッグ用：当たり判定の描画
						if (CheckHit(x, y)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							DrawBox(posX, posY, posX + _chipSize.first, posY + _chipSize.second, GetColor(255, 0, 0), TRUE);
							SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
						}
#endif
					}
				}
			}
		}
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
		_mapSize.first = static_cast<int>(jsRoot["width"].get<double>());
		_mapSize.second = static_cast<int>(jsRoot["height"].get<double>());

		picojson::array aTileSets = jsRoot["tilesets"].get<picojson::array>();
		picojson::object jsTile = aTileSets[0].get<picojson::object>();

		auto chipCount = &std::get<FIRST>(_chipCount);
		auto chipCountW = &std::get<SECOND>(_chipCount);
		auto chipCountH = &std::get<THIRD>(_chipCount);
		auto chipSizeW = &_chipSize.first;
		auto chipSizeH = &_chipSize.second;

		*chipCount = static_cast<int>(jsTile["tilecount"].get<double>());
		*chipCountW = static_cast<int>(jsTile["columns"].get<double>());
		*chipCountH = (*chipCount / *chipCountW);
		*chipSizeW = static_cast<int>(jsRoot["tilewidth"].get<double>());
		*chipSizeH = static_cast<int>(jsRoot["tileheight"].get<double>());
		_fileChips = jsTile["image"].get<std::string>();

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
		// _mapChipsType.swap(addChipsType);
		sort(_mapChipsType.begin(), _mapChipsType.end());
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

	bool MapChips::IsHit(AABB box, Vector2& move) {
		int x, y;

		auto vectorX = move.GetX();
		auto vectorY = move.GetY();

		auto minx = box.GetMin().IntX() + move.IntX();
		auto miny = box.GetMin().IntY() + move.IntY();
		auto maxx = box.GetMax().IntX() + move.IntX();
		auto maxy = box.GetMax().IntY() + move.IntY();

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				// マップチップと接触しているかどうか？
				int chip_no = CheckHit(x, y);
				// チップ番号が0かどうか
				if (chip_no != 0) {
					AABB mapchip({ static_cast<double>(x) , static_cast<double>(y) }, { static_cast<double>(x + _chipSize.first), static_cast<double>(y + _chipSize.second)});
					AABB boxcol({ box.GetMin().GetX() + move.GetX(), box.GetMin().GetY() + move.GetY() },
						{ box.GetMax().GetX() + move.GetX(), box.GetMax().GetY() + move.GetY() });
					
					// 移動ベクトルがプラスかつ、
					if (vectorX < 0 && boxcol.HitCheck(mapchip) == true) {
						move.GetPX() = -move.GetX();
					} else if (0 < vectorX && boxcol.HitCheck(mapchip) == true) {
						move.GetPX() = -move.GetX();
					}
					// y座標で接触しているかどうか判定
					if (vectorY < 0 && mapchip.HitCheck(mapchip) == true) {
						move.GetPY() = 0;
					} else if (0 < vectorY && mapchip.HitCheck(mapchip) == true) {
						move.GetPY() = 0;
					}

					// 当たっている場合は移動ベクトルを0にする
					// 移動量がプラスの場合、移動量を減らす
					/*if (move.GetX() < 0) move.GetPX() = -0.5;
					else if (0 < move.GetX()) move.GetPX() = -0.5;
					if (0 < move.GetY()) move.GetPY() = +0.5;
					else  if (move.GetY() < 0) move.GetPY() = -0.5;*/

					//if (move.GetX() < 0) collision.GetMin().GetPX() = x * _chipSize.first + _chipSize.first;
					//if (0 < mx) collision.GetMax().GetPX() = x * _chipSize.first - (PLAYER_WIDTH / 2);
					//if (0 < my) collision.GetMin().GetPY() = y * _chipSize.second - (PLAYER_HIGHT / 2);
					//if (my < 0) collision.GetMax().GetPY() = y * _chipSize.second + _chipSize.second;
					return true;
				}
			}
		}
		return false;
	}
}