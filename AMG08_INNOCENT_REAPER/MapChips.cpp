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

	// �[
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

	constexpr auto CHIP_THORM_LEFT1 = 20;	// �����[(xmin)
	constexpr auto CHIP_THORM_LEFT2 = 40;	// �����[(xmax)
	constexpr auto CHIP_THORM_LEFT3 = 0;
	constexpr auto CHIP_THORM_LEFT4 = 40;

	constexpr auto CHIP_THORM_TOP1 = 0;
	constexpr auto CHIP_THORM_TOP2 = 40;
	constexpr auto CHIP_THORM_TOP3 = 0;
	constexpr auto CHIP_THORM_TOP4 = 20;


	constexpr auto CHIP_THORM_RIGHT1 = 0;
	constexpr auto CHIP_THORM_RIGHT2 = 20;

	// ��
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

		// �X�N���[�����W������
		_worldPosition = { WINDOW_W / 2, WINDOW_H / 2 };
		_worldLast = _worldPosition;
	}

	MapChips::~MapChips() {
		// ������
		_mapManager->StageMapClear();

	}

	void MapChips::Init() {

	}

	void MapChips::Process() {
		// �L�[���X�V���ꂽ�ꍇ�̓}�b�v����؂�ւ���
		if (_nextStage != stage::CHANGE_NULL) {
			ChangeMap();
		}
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
		auto mswidth = _nowMap.MapSizeWidth();
		auto msheight = _nowMap.MapSizeHeight();
		auto cswidth = _nowMap.ChipSizeWidth();
		auto csheight = _nowMap.ChipSizeHeight();

		// �`�揈��
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

					// �����蔻����擾
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

						// �f�o�b�O�p�F�����蔻��̕`��
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

		/* ���ؗp */
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
		/* ���ؗp */
	}

	void MapChips::WorldClanp() {
		auto mapWidth = _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth();
		auto mapHeight = _nowMap.MapSizeHeight() * _nowMap.ChipSizeHeight();

		// ���[���h���W�͉�ʓ��Ɏ��܂��Ă��邩�ǂ����H
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
		// ���W�̓��[���h���W�̏���Ɏ��܂��Ă��邩
		auto mapWidth = _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth();
		auto mapHeight = _nowMap.MapSizeHeight() * _nowMap.ChipSizeHeight();

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

		// ���[���hY���W�̓X�N���[���J�n�n�_�𒴂��Ă��邩�H
		if (HALF_WINDOW_H < _worldPosition.GetY() && _worldPosition.GetY() < scrY - HALF_WINDOW_H) { return true; }
		return false;
	}

	void MapChips::WorldUpdate(Vector2 pos) {
		// �O�t���[���̍��W�Ƃ��ēo�^
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

		// �}�b�v�̃T�C�Y����json����擾
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
		// ���C���[���f�[�^�̎擾
		for (int i = 0; i < static_cast<int>(aLayers.size()); ++i) {

			picojson::object jsLayer = aLayers[i].get<picojson::object>();
			if (jsLayer["type"].get<std::string>() == "tilelayer") {
				picojson::array aData = jsLayer["data"].get<picojson::array>();

				for (int y = 0; y < mapsizeHeight; ++y) {

					for (int x = 0; x < mapsizeWidth; ++x) {
						int layerStart = mapsizeWidth * mapsizeHeight * layer;
						int index = y * mapsizeWidth + x;
						mapdata[layerStart + index] = static_cast<int>(aData[index].get<double>());

						// vector�z��Ɏg�p����Ă���}�b�v�`�b�v��o�^���邩�ǂ���
						int mapchip_no = static_cast<int>(aData[index].get<double>());
						// 0�̏ꍇ�͏������X�L�b�v
						if (mapchip_no == 0) continue;
						// _mapChipsType�ɒl�����邩�ǂ���������
						auto it = std::find(chiptype.begin(), chiptype.end(), mapchip_no);
						// �q�b�g���Ȃ������ꍇ�́A�����ɗv�f��ǉ�
						if (it == chiptype.end()) {
							chiptype.emplace_back(mapchip_no);
						}
					}
				}
				++layer;
			}
		}
		// �`�b�v�ԍ����Ⴂ���ɕ��ѕς���
		sort(chiptype.begin(), chiptype.end());
		// ������-1��ǉ�
		chiptype.emplace_back(-1);

		// �z�u���̓o�^
		MapDataManager::JsonMapData jmd{
			{ skey, { std::make_pair(mapsizeWidth, mapsizeHeight), std::make_tuple(chipCount, chipCountW, chipCountH),
					  std::make_pair(chipSizeW, chipSizeH), mapSizeLayer, fileChips, mapdata, chiptype }},
		};
		_mapManager->LoadStageMap(jmd);	// �}�b�v�}�l�[�W���[�ɓo�^

		// ResourceServer�Ń}�b�v�`�b�v�摜�̓ǂݍ���
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
			// ������ID���ǂ����̔���
			auto i = 0;
			while (chiptype[i] != -1) {
				if (chip_no == chiptype[i]) {
					// ���������ꍇ�A���̃`�b�v�ԍ���Ԃ�
					return chip_no;
				}
				++i;
			}
		}
		// ������Ȃ������ꍇ��0��Ԃ�
		return 0;
	}

	int MapChips::IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos) {
		auto gs = static_cast<int>(g);

		auto footMinX = box.GetMin().IntX();
		auto footMaxX = box.GetMax().IntX();	// �����̍��W
		auto footMinY = box.GetMin().IntY() + gs;
		auto footMaxY = box.GetMax().IntY() + gs;
		Vector2 m = { box.GetMin().GetX(), box.GetMin().GetY() + g };
		Vector2 n = { box.GetMax().GetX(), box.GetMax().GetY() + g };
		AABB mn = { m, n, box.GetCollisionFlg() };


		auto csh = _nowMap.ChipSizeHeight();
		auto csw = _nowMap.ChipSizeWidth();

		int x, y;

		// �����蔻��ƐڐG����\���̂���͈͂Ƃ̂ݔ�����s��
		for (y = footMinY / csh; y <= footMaxY / csh; ++y) {
			for (x = footMinX / csw; x <= footMaxX / csw; ++x) {
				int chip_no = CheckHit(x, y);	// ���̏ꏊ�ɂ͉��̃`�b�v�����邩�H
				if (chip_no != 0) {
					if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON) continue;	// �����蔻�肪�Ȃ��ꍇ�͔�����
					// �Փ˔���
					auto c = _chipCheck->ChipCollision(chip_no);

					double minX = x * csw + c.GetMin().IntX();
					double maxX = x * csw + c.GetMax().IntX();
					double minY = y * csh + c.GetMin().IntY();
					double maxY = y * csh + c.GetMax().IntY();
					Vector2 cmin = { minX, minY };
					Vector2 cmax = { maxX, maxY };
					AABB cbox = { cmin, cmax, true };

					// ����͈͓��Ɏ��܂��Ă��邩�H
					if (mn.GetMin().GetX() < cbox.GetMax().GetX() && cbox.GetMin().GetX() < mn.GetMax().GetX()) {
						// �����l�����̏ꍇ�i�������A���ƐڐG���Ă��邩�H�j
						if (0 < g) {
							// �����͖��܂��Ă��Ȃ����H
							if (mn.GetMin().GetY() < cbox.GetMin().GetY() &&
								// �}�b�v�`�b�v�̏㕔�ɑΏۂ̉������������܂��Ă��邩�ǂ���
								cbox.GetMin().GetY() < mn.GetMax().GetY()) {
								auto cavep = box.GetHeightMax();
								pos.GetPY() = minY - cavep;
								// �ʏ픻��`�b�v�̏ꍇ�A���W���X�V����
								if (_chipCheck->IsChipType(chip_no) == mapchip::NORMAL) *lastpos = { maxX , pos.GetY() };
								return _chipCheck->IsChipType(chip_no);
							}
							/*if (mn.GetMin().GetY() < cbox.GetMax().GetY() && cbox.GetMin().GetY() < mn.GetMax().GetY()) return true;*/
						} if (g < 0) {
							// �����x�����̏ꍇ�i�j
								// �v���C���[�̉����̓}�b�v�`�b�v�̉������傫����
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

	// �����蔻��̎擾
	//bool MapChips::IsHit(AABB box, double g) {
	//	int x, y;

	//	auto gs = static_cast<int>(g);

	//	auto minx = box.GetMin().IntX();
	//	auto miny = box.GetMin().IntY() + gs;
	//	auto maxx = box.GetMax().IntX();
	//	auto maxy = box.GetMax().IntY() + gs;
	//	
	//	// �Ώۂ̓����蔻��
	//	Vector2 m = { static_cast<double>(minx), static_cast<double>(miny) };
	//	Vector2 n = { static_cast<double>(maxx), static_cast<double>(maxy) };
	//	AABB mn = { m, n, true };

	//	for (y = miny / _chipSize.second; y <= maxy / _chipSize.second; ++y) {
	//		for (x = minx / _chipSize.first; x <= maxx / _chipSize.first; ++x) {
	//			// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
	//			int chip_no = CheckHit(x, y);

	//			// �`�b�v�ԍ���0���ǂ���
	//			if (chip_no != 0) {
	//				auto c = _chipCheck->ChipCollision(chip_no);
	//				auto minX = c.GetMin().IntX();
	//				auto maxX = c.GetMax().IntX();
	//				auto minY = c.GetMin().IntY();
	//				auto maxY = c.GetMax().IntY();

	//				// �V�K�ǉ�
	//				Vector2 chipMin = { static_cast<double>(x * _chipSize.first + minX), static_cast<double>(y * _chipSize.second + minY) };
	//				Vector2 chipMax = { static_cast<double>(x * _chipSize.first + maxX), static_cast<double>(y * _chipSize.second + maxY) };
	//				AABB chipbox = { chipMin, chipMax, true };
	//				if (mn.HitCheck(chipbox)) return true;
	//				
	//			}
	//		}
	//	}
	//	// �������Ă��Ȃ�
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


			/* ���ؗp */
			// _debugAABB = { {static_cast<double>(minx), static_cast<double>(miny)}, {static_cast<double>(maxx), static_cast<double>(maxy)} };
			/* ���ؗp */

		for (y = miny / _nowMap.ChipSizeHeight(); y <= maxy / _nowMap.ChipSizeHeight(); ++y) {
			for (x = minx / _nowMap.ChipSizeWidth(); x <= maxx / _nowMap.ChipSizeWidth(); ++x) {
				// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
				int chip_no = CheckHit(x, y);
				// �`�b�v�ԍ���0���ǂ���
				if (chip_no != 0) {
					if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON ) continue;	// �����蔻�肪�Ȃ��ꍇ�͔�����
					// �����蔻����擾
					auto c = _chipCheck->ChipCollision(chip_no);
					auto minX = c.GetMin().IntX();
					auto maxX = c.GetMax().IntX();
					auto minY = c.GetMin().IntY();
					auto maxY = c.GetMax().IntY();

					// �V�K�ǉ�
					auto chipMinX = x * _nowMap.ChipSizeWidth() + minX;
					auto chipMinY = y * _nowMap.ChipSizeHeight() + minY;
					auto chipMaxX = x * _nowMap.ChipSizeWidth() + maxX;
					auto chipMaxY = y * _nowMap.ChipSizeHeight() + maxY;

					if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
						if (vectorY < 0) {
							if (miny < chipMinY && chipMinY < maxy) {
								auto cave = box.GetHeightMin();
								move.GetPY() = 0;	// �ړ��ʏ�����
								pos.GetPY() = chipMaxY + cave;
							}
						}
						else if (0 < vectorY) {
							if (chipMaxY < maxy && miny < chipMaxY) {
								auto cave = box.GetHeightMin();
								move.GetPY() = 0;	// �ړ��ʏ�����
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
					// �����ɍX�V���������Ă��邩�ǂ����H
					//if (isUpdate == false) {
					//	// ���̂ݔ���i�ړ��ʂ�x�̂݉��Z�j
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
					//	// �X�V���������Ă���ꍇ�͏����𔽓]������
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

					// �ړ��x�N�g�����A������s���o�[�W����
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
					//			move.GetPY() = 0;	// �ړ��ʏ�����
					//			pos.GetPY() = chipMaxY + cave;
					//		}
					//		else if (chipMaxY < box.GetMax().IntY() && box.GetMin().IntY() < chipMaxY) {
					//			auto cave = box.GetHeightMin();
					//			move.GetPY() = 0;	// �ړ��ʏ�����
					//			pos.GetPY() = chipMinY + cave;
					//		}
					//	}
					//}

					//// �c�̂ݏC���L��ver
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
	


					// �͈͓��Ɏ��܂��Ă��邩�H
					//if (minp.GetX() < chipMaxX && chipMinX < maxp.GetX()) {
					//	if (vectorY < 0) {
					//		// �W�����v�A�N�V�������ɓV��ɂ߂荞��ł��邩�H
					//		// �V���maxy�����Ώۂ�miny���������Ă��A
					//		if (minp.GetY() < chipMaxY && chipMinY < maxp.GetY()) {
					//			auto cave = box.GetHeightMin();
					//			move.GetPY() = 0;	// �ړ��ʏ�����
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
					// //�����o������
					//if (thisbox.HitDirection(mapchip)) {
					//	move.GetPX() = thisbox.HitDirection(mapchip);
					//}

					// x���W�̂߂荞�ݔ���
					// if (vectorX < 0 && boxcol.HitCheck(mapchip) == true) {
					// ���ړ���Ԃ���
					// ���܂��Ă��邩�H
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
					//	// �߂荞��ł���s�N�Z�����Z�o���A���W��u��������
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
					// y���W�̂߂荞�ݔ���
					// ���͎��܂��Ă��邩�H
					// �W�����v

					//if (vectorY < 0 && (chipMinX <= minx && minx <= chipMaxX) || (chipMinX <= maxx && maxx <= chipMaxX)) {
					//	if (miny < chipMaxY) {
					//		pos.GetPY() = chipMaxY - 1;
					//		move.GetPY() = 0;
					//	}
					//}
					//// ��������
					//if (0 < vectorY && (chipMinX <= minx && minx <= chipMaxX) || (chipMinX <= maxx && maxx <= chipMaxX)) {
					//	// �Ώۂ̃{�b�N�X���̓}�b�v�`�b�v��ɂ߂荞��ł��邩
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
		// �e�퓖���蔻���o�^����
		// �����蔻����C������`�b�v�ԍ���o�^
		ChipHitCheck::ChipsMap stagechip {
			// ����:�C������`�b�v�ԍ��A�E��:�C�����������蔻��+�`�b�v�Ɍ��ʂ��������邩�ǂ���
			// ��
			{  2, {0, 40, 20 ,40, mapchip::THORM}},
			{  6, {0, 5}},	// �[�̃`�b�v�ɂ͔�����������Ȃ�
			{  7, {35, 40}},

			// ���[
			{  9, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 17, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 25, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 33, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 41, {CHIP_RIGHT1, CHIP_RIGHT2}},
			// �E�[
			{ 16, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 24, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 32, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 40, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 48, {CHIP_LEFT1, CHIP_LEFT2}},
			// �V��
			{ 49, {CHIP_TIP1, CHIP_TIP2, CHIP_TIP5, CHIP_TIP6}},
			{ 50, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 51, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 52, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 53, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 54, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 55, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 56, {CHIP_TIP3, CHIP_TIP4, CHIP_TIP5, CHIP_TIP6}},

			// �ǉ��̃}�b�v�`�b�v�i�[�j
			{ 58, {0, 40, 0, 5}},
			{ 59, {0, 40, 0, 5}},
			// { 61, {0, 10, 0, 10}},

			// 2�ʐ�p�}�b�v�`�b�v
			// ���[
			{ 65, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 73, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 97, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 105, {CHIP_RIGHT1, CHIP_RIGHT2}},
			{ 113, {CHIP_RIGHT1, CHIP_RIGHT2}},
			// �E�[
			{ 72, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 80, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 100, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 108, {CHIP_LEFT1, CHIP_LEFT2}},
			{ 116, {CHIP_LEFT1, CHIP_LEFT2}},


			// �V��
			{ 81, {CHIP_TIP1, CHIP_TIP2, CHIP_TIP5, CHIP_TIP6}},
			{ 82, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 83, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 84, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 85, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 86, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 87, {CHIP_UP1, CHIP_UP2, CHIP_UP3, CHIP_UP4}},
			{ 88, {CHIP_TIP3, CHIP_TIP4, CHIP_TIP5, CHIP_TIP6}},

			// ���̑�
			{ 90, {0, 40, 0, 5}},
			{ 91, {0, 40, 0, 5}},

			// ��(��)
			{ 95, {0, 6}},
			{ 96, {35, 40}},
			{ 103, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			{ 104, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			{ 111, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			{ 112, {CHIP_THORM1, CHIP_THORM2, CHIP_THORM3, CHIP_THORM4, mapchip::THORM}},
			// �����[
			{ 101, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 109, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 117, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 125, {CHIP_THORM_LEFT1, CHIP_THORM_LEFT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			// ���E�[
			{ 102, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 110, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 118, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			{ 126, {CHIP_THORM_RIGHT1, CHIP_THORM_RIGHT2, CHIP_THORM_LEFT3, CHIP_THORM_LEFT4, mapchip::THORM}},
			// ��(�V��)
			{ 121, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			{ 122, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			{ 123, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			{ 124, {CHIP_THORM_TOP1, CHIP_THORM_TOP2, CHIP_THORM_TOP3, CHIP_THORM_TOP4, mapchip::THORM}},
			// ��
			{ 119, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			{ 120, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			{ 127, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			{ 128, {CHIP_IVY1, CHIP_IVY2, mapchip::IVY, mapchip::HIT_OFF}},
			// �X�e�[�W�J��
			{ 129, {mapchip::TRANSITION}},	// �X�e�[�WS(1)���X�e�[�W1(2)
			{ 130, {mapchip::TRANSITION}},	// �X�e�[�W1(3)���X�e�[�W2-2()
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
		_chipCheck->ChangeStageKey(stage::KEY_NORMAL);	// �ŏ��ɌĂяo���X�e�[�W��o�^
	}

	void MapChips::SetChipMember() {

	}

	void MapChips::ChangeMap() {
		// �}�b�v�f�[�^�͓ǂݍ��܂�Ă��邩�H
		if (_mapManager->IsLoad(_nextStage) == true) {
			std::string stagePath = DEFAULT_PATH + _nextStage + JSON_FORMAT;	// �p�X���쐬
			TiledJsonLoad(_skey, DEFAULT_PATH, _skey);	// �Ώۂ̓o�^
		}
		/* �����ɃI�u�W�F�N�g�̏��������������� */

		/* �����ŃV�i���I�N���X���g���ăI�u�W�F�N�g�̐������s�� */

		_skey = _nextStage;
		_nextStage = stage::CHANGE_NULL;
	}
}