#include "MapChips.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "ResourceServer.h"
#include "Collision.h"
#include "Vector2.h"
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

	MapChips::MapChips(Game& game, std::string& filePath, std::string& tiledFileName) : _game(game), _debugAABB(Vector2(), Vector2()) {
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
		_worldPosition = { WINDOW_W / 2, WINDOW_H / 2 };
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
		for (layer = 0; layer < _mapSizeLayer; ++layer) {
			for (y = 0; y < _mapSize.second; ++y) {
				for (x = 0; x < _mapSize.first; ++x) {
					int  layerStart = _mapSize.first * _mapSize.second * layer;
					int index = y * _mapSize.first + x;
					int posX = x * _chipSize.first - _worldPosition.IntX();//(_worldPosition.IntX() - HALF_WINDOW_W);
					int posY = y * _chipSize.second - (_worldPosition.IntY() - HALF_WINDOW_H);
					int no = _mapData[layerStart + index];
					--no;

					if (0 <= no) {
						auto gh = graph::ResourceServer::GetHandles(CHIP_KEY, no);
						DrawGraph(posX, posY, gh, TRUE);

#ifdef _DEBUG
						// デバッグ用：当たり判定の描画
						/*if (CheckHit(x, y)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							DrawBox(posX, posY, posX + _chipSize.first, posY + _chipSize.second, GetColor(255, 0, 0), TRUE);
							SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
						}*/
#endif
					}
				}
			}
		}

		/* 検証用 */
		auto minx = _debugAABB.GetMin().IntX();
		auto miny = _debugAABB.GetMin().IntY();
		auto maxx = _debugAABB.GetMax().IntX();
		auto maxy = _debugAABB.GetMax().IntY();

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				auto chipMinX = x * _chipSize.first;// _worldPosition.GetX();
				auto chipMinY = y * _chipSize.second;// _worldPosition.GetY();
				auto chipMaxX = x * _chipSize.first + _chipSize.first;// - _worldPosition.GetX();
				auto chipMaxY = y * _chipSize.second + _chipSize.second;// _worldPosition.GetY();

				AABB mapchip({ static_cast<double>(chipMinX) , static_cast<double>(y) }, { static_cast<double>(chipMaxX), static_cast<double>(chipMaxY) });

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				mapchip.DrawBox(GetColor(255, 255, 0));
				SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
			}
		}
		/* 検証用 */
	}

	void MapChips::WorldClanp() {
		auto mapW = _mapSize.first;
		auto mapH = _mapSize.second;
		auto chipW = _chipSize.first;
		auto chipH = _chipSize.second;

		// ワールド座標は画面内に収まっているかどうか？
		if (_worldPosition.GetX() < 0) { _worldPosition.GetPX() = 0; }
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

		auto cwindowW = _worldPosition.GetX() - HALF_WINDOW_W;
		auto cwindowH = _worldPosition.GetY() - WINDOW_H; //- _mapSize.second * _chipSize.second;	// 画面内に収まっているか？

		auto mapY = -_mapSize.second * _chipSize.second;


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

	void MapChips::WorldUpdate(Vector2 pos) {
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

	// 当たり判定の取得
	bool MapChips::IsHit(AABB box, double g) {
		int x, y;

		auto gs = static_cast<int>(g);

		auto minx = box.GetMin().IntX();
		auto miny = box.GetMin().IntY() + gs;
		auto maxx = box.GetMax().IntX();
		auto maxy = box.GetMax().IntY() + gs;

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				// マップチップと接触しているかどうか？
				int chip_no = CheckHit(x, y);
				// チップ番号が0かどうか
				if (chip_no != 0) {
					return true;
				}
			}
		}
		// 当たっていない
		return false;
	}

	bool MapChips::IsHit(AABB box, Vector2& pos, Vector2& move) {
		int x, y;

		auto vectorX = move.GetX();
		auto vectorY = move.GetY();

		auto minx = box.GetMin().IntX() + move.IntX();
		auto miny = box.GetMin().IntY() + move.IntY();
		auto maxx = box.GetMax().IntX() + move.IntX();
		auto maxy = box.GetMax().IntY() + move.IntY();

		/* 検証用 */
		_debugAABB = { {static_cast<double>(minx), static_cast<double>(miny)}, {static_cast<double>(maxx), static_cast<double>(maxy)} };
		/* 検証用 */

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				// マップチップと接触しているかどうか？
				int chip_no = CheckHit(x, y);
				// チップ番号が0かどうか
				if (chip_no != 0) {
					// 新規追加
					auto chipMinX = x * _chipSize.first;
					auto chipMinY = y * _chipSize.second;
					auto chipMaxX = x * _chipSize.first + _chipSize.first;
					auto chipMaxY = y * _chipSize.second + _chipSize.second;

					AABB mapchip({ static_cast<double>(chipMinX) , static_cast<double>(chipMinY) }, { static_cast<double>(chipMaxX), static_cast<double>(chipMaxY)});
					AABB boxcol({ box.GetMin().GetX() + move.GetX(), box.GetMin().GetY() + move.GetY() },
						{ box.GetMax().GetX() + move.GetX(), box.GetMax().GetY() + move.GetY() });
					
					// x座標のめり込み判定
					// if (vectorX < 0 && boxcol.HitCheck(mapchip) == true) {
					if (vectorX < 0 && mapchip.SideCheck(boxcol) == true) {
						// めり込んでいるピクセルを算出し、座標を置き換える
						//auto m = (chipMaxX - minx);
						//auto vec1 = _chipSize.first - m;

						//pos.GetPX() = pos.GetX() + vec1;
						//// pos.GetPX() = chipMaxX + boxcol.GetCenter().GetX();//pos.GetX() + vec1;
						//move.GetPX() = 0;
						
						// move.GetPX() = (_chipSize.first - m) / vectorX;
					} if (0 < vectorX && mapchip.SideCheck(boxcol) == true) {
						// auto n = chipMaxX - maxx;
						/*auto n = maxx - chipMaxX;
						auto vec2 = _chipSize.first - n;
						move.GetPX() = pos.GetPX() - vec2;
						move.GetPX() = 0;*/
					}
					// y座標のめり込み判定
					if (vectorY < 0 && mapchip.HitCheck(mapchip) == true) {
						move.GetPY() = 0;
					} else if (0 < vectorY && mapchip.HitCheck(mapchip) == true) {
						move.GetPY() = 0;
					}
					return true;
				}
			}
		}
		return false;
	}
}