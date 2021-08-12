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
		// ResourceServer�ŉ摜�ǂݍ���
		graph::ResourceServer::LoadGraphList(mapchip);
		// �X�N���[�����W������
		_worldPosition = { WINDOW_W / 2, WINDOW_H / 2 };
	}

	MapChips::~MapChips() {
		// ������
		_mapData.clear();

	}

	void MapChips::Init() {

	}

	void MapChips::Process() {
		WorldClanp();
		


		// �v���C���[����X�N���[�����W���擾���đ��
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
		// �`�揈��
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
						// �f�o�b�O�p�F�����蔻��̕`��
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

		/* ���ؗp */
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
		/* ���ؗp */
	}

	void MapChips::WorldClanp() {
		auto mapW = _mapSize.first;
		auto mapH = _mapSize.second;
		auto chipW = _chipSize.first;
		auto chipH = _chipSize.second;

		// ���[���h���W�͉�ʓ��Ɏ��܂��Ă��邩�ǂ����H
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
		// �Ώۂ͕`��͈͓��Ɏ��܂��Ă��邩�ǂ����H
		if (-960 <= scrPosX <= 960) {
			// x���W���X�V
			auto fixX = scrPosX + 960;
			pos.GetPX() = fixX;
			// y���W���X�V���邩�ǂ���
			if (-540 <= scrPosY <= 540) {
				auto fixY = scrPosY + 540;
				// �~�����l�͉��H�i�����j
				// auto xx = _worldPosition.GetY() - HALF_WINDOW_H;
				pos.GetPY() = fixY;
			}
			return true;
		}

		auto cwindowW = _worldPosition.GetX() - HALF_WINDOW_W;
		auto cwindowH = _worldPosition.GetY() - WINDOW_H; //- _mapSize.second * _chipSize.second;	// ��ʓ��Ɏ��܂��Ă��邩�H

		auto mapY = -_mapSize.second * _chipSize.second;


		//// 0��菬�����ꍇ�͏C��
		//if (cwindowW < 0 ) return false;

		//auto x = pos.GetX() - cwindowW;
		//auto y = pos.GetY() - cwindowH;

		//// x���W�͎��܂��Ă��邩�ǂ����H
		//if (0 <= x <= WINDOW_W) {
		//	// �]��Z�ō��W��ϊ�
		//	pos.GetPX() = pos.IntX() % (WINDOW_W + 1);
		//}
		
		// auto y = pos.GetY() - cwindowH;
		/*auto x = pos.GetX() - _worldPosition.GetX();
		auto y = pos.GetY() - _worldPosition.GetY();*/
		// �Ώۂ̍��W����J�������W���������l�̓v���X���ǂ����H
		//if (x < HALF_WINDOW_W) return false;
		// if (x < HALF_WINDOW_W && y <= HALF_WINDOW_H) return false;
		
		//if (x < 0 && y < 0) {
		//	// �X�N���[�����W���ɑ��݂��Ȃ�
		//	return false;
		//}
		// �Ώۂ͌��݂̃X�N���[�����W��ɑ��݂��Ă��邩�H
		//if (0 <= x <= WINDOW_W) {
		//	// ���[���h���W����X�N���[�����W�ɏC��
		//	pos.GetPX() = x;//pos.GetX() - (_worldPosition.GetX() / 2);
		//	return true;
		//}

		//if (0 <= x <= WINDOW_W && 0 <= y <= WINDOW_H) {
		//	// ���[���h���W����X�N���[�����W�ɏC��
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

		// �}�b�v�̃T�C�Y����json����擾
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
		// ���C���[���f�[�^�̎擾
		for (int i = 0; i < static_cast<int>(aLayers.size()); ++i) {

			picojson::object jsLayer = aLayers[i].get<picojson::object>();
			if (jsLayer["type"].get<std::string>() == "tilelayer") {
				picojson::array aData = jsLayer["data"].get<picojson::array>();

				for (int y = 0; y < _mapSize.second; ++y) {

					for (int x = 0; x < _mapSize.first; ++x) {
						int layerStart = _mapSize.first * _mapSize.second * layer;
						int index = y * _mapSize.first + x;
						_mapData[layerStart + index] = static_cast<int>(aData[index].get<double>());

						// vector�z��Ɏg�p����Ă���}�b�v�`�b�v��o�^���邩�ǂ���
						int mapchip_no = static_cast<int>(aData[index].get<double>());
						// 0�̏ꍇ�͏������X�L�b�v
						if (mapchip_no == 0) continue;
						// _mapChipsType�ɒl�����邩�ǂ���������
						auto it = std::find(_mapChipsType.begin(), _mapChipsType.end(), mapchip_no);
						// �q�b�g���Ȃ������ꍇ�́A�����ɗv�f��ǉ�
						if (it == _mapChipsType.end()) {
							_mapChipsType.emplace_back(mapchip_no);
						}

						//// �v�f����̏ꍇ�́A�����ɒǉ�
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
		// �`�b�v�ԍ����Ⴂ���ɕ��ѕς���
		sort(_mapChipsType.begin(), _mapChipsType.end());
		// ������-1��ǉ�
		_mapChipsType.emplace_back(-1);

		return 1;
	}

	int MapChips::CheckHit(int x, int y) {
		
		auto mapSizeW = _mapSize.first;
		auto mapSizeH = _mapSize.second;

		if (0 <= x && x < mapSizeW && 0 <= y && y < mapSizeH) {
			int chip_no = _mapData[y * mapSizeW + x];
			// ������ID���ǂ����̔���
			auto i = 0;
			while (_mapChipsType[i] != -1) {
				if (chip_no == _mapChipsType[i]) {
					// ���������ꍇ�A���̃`�b�v�ԍ���Ԃ�
					return chip_no;
				}
				++i;
			}
		}
		// ������Ȃ������ꍇ��0��Ԃ�
		return 0;
	}

	// �����蔻��̎擾
	bool MapChips::IsHit(AABB box, double g) {
		int x, y;

		auto gs = static_cast<int>(g);

		auto minx = box.GetMin().IntX();
		auto miny = box.GetMin().IntY() + gs;
		auto maxx = box.GetMax().IntX();
		auto maxy = box.GetMax().IntY() + gs;

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
				int chip_no = CheckHit(x, y);
				// �`�b�v�ԍ���0���ǂ���
				if (chip_no != 0) {
					return true;
				}
			}
		}
		// �������Ă��Ȃ�
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

		/* ���ؗp */
		_debugAABB = { {static_cast<double>(minx), static_cast<double>(miny)}, {static_cast<double>(maxx), static_cast<double>(maxy)} };
		/* ���ؗp */

		for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
			for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
				// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
				int chip_no = CheckHit(x, y);
				// �`�b�v�ԍ���0���ǂ���
				if (chip_no != 0) {
					// �V�K�ǉ�
					auto chipMinX = x * _chipSize.first;
					auto chipMinY = y * _chipSize.second;
					auto chipMaxX = x * _chipSize.first + _chipSize.first;
					auto chipMaxY = y * _chipSize.second + _chipSize.second;

					AABB mapchip({ static_cast<double>(chipMinX) , static_cast<double>(chipMinY) }, { static_cast<double>(chipMaxX), static_cast<double>(chipMaxY)});
					AABB boxcol({ box.GetMin().GetX() + move.GetX(), box.GetMin().GetY() + move.GetY() },
						{ box.GetMax().GetX() + move.GetX(), box.GetMax().GetY() + move.GetY() });
					
					// x���W�̂߂荞�ݔ���
					// if (vectorX < 0 && boxcol.HitCheck(mapchip) == true) {
					if (vectorX < 0 && mapchip.SideCheck(boxcol) == true) {
						// �߂荞��ł���s�N�Z�����Z�o���A���W��u��������
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
					// y���W�̂߂荞�ݔ���
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