#include "MapChips.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "ResourceServer.h"
#include "Collision.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include "Scenario.h"
#include "ModeServer.h"
#include "ModeMain.h"
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
	constexpr auto CHIP_UP4 = 6;

	// �[
	constexpr auto CHIP_TIP1 = 35;
	constexpr auto CHIP_TIP2 = 40;
	constexpr auto CHIP_TIP3 = 0;
	constexpr auto CHIP_TIP4 = 6;

	constexpr auto CHIP_TIP5 = 0;
	constexpr auto CHIP_TIP6 = 6;

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


	constexpr auto CHECK_MIN = 1;
	constexpr auto CHECK_MAX = 5;
}

namespace inr {

	constexpr auto CHIP_KEY = "chips";

	MapChips::MapChips(Game& game) : _game(game), _nowMap() {
		_chipCheck = std::make_unique<ChipHitCheck>();	// �`�b�v�����蔻��C��
		_mapManager = std::make_unique<MapDataManager>(_game.GetGame());
		_stageTransition = std::make_unique<StageTransition>(_game.GetGame());
		_stageTransition->Init();	// �������������s��
		SetChipsMap();	// �}�b�v�`�b�v�̓����蔻���o�^

		_worldPosition = { DEFAULT_W, DEFAULT_H };
		_worldLast = _worldPosition;
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
		_mapManager->StageMapClear();
	}

	void MapChips::Process() {
		_worldLast = _worldPosition;
		// �L�[���X�V���ꂽ�ꍇ�̓}�b�v����؂�ւ���
		WorldClanp();
	}

	void MapChips::Draw() {
#ifdef _DEBUG
		/*DrawFormatString(0, 300, GetColor(255, 0, 0), "worldPosition.x = %d\n", _worldPosition.IntX());
		DrawFormatString(0, 325, GetColor(255, 0, 0), "worldPosition.y = %d\n", _worldPosition.IntY());*/
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

						if (no < 128) {	// �J�ڃ`�b�v�̏ꍇ�͏������s��Ȃ�
							auto gh = graph::ResourceServer::GetHandles(stage::KEY_NORMAL, no);
							DrawGraph(posX, posY, gh, TRUE);
						}

#ifdef _DEBUG
						if (_game.IsDebugMode() == true) {
							auto chiptype = _chipCheck->IsChipType(no + 1);
							std::tuple<int, int, int> colortype = std::make_tuple(255, 0, 0);
							if (chiptype == mapchip::IVY) colortype = std::make_tuple(255, 255, 0);
							if (chiptype == mapchip::THORM) colortype = std::make_tuple(255, 255, 255);
							if (chiptype == mapchip::TRANSITION) colortype = std::make_tuple(0, 0, 255);

							// �f�o�b�O�p�F�����蔻��̕`��
							if (CheckHit(x, y)) {
								SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
								DrawBox(posX + minX, posY + minY, posX + maxX, posY + maxY, GetColor(std::get<0>(colortype), std::get<1>(colortype), std::get<2>(colortype)), TRUE);
								// DrawBox(posX, posY, posX + _chipSize.first, posY + _chipSize.second, GetColor(255, 0, 0), TRUE);
								SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
							}
						}
#endif
					}
				}
			}
		}
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
		return false;
	}

	bool MapChips::IsScrollX() {
		auto mapW = _nowMap.MapSizeWidth();//_mapSize.first;
		auto chipW = _nowMap.ChipSizeWidth();//_chipSize.first;

		auto scrX = mapW * chipW;

		if (HALF_WINDOW_W < _worldPosition.GetX()) { return true; }
		else if (scrX - HALF_WINDOW_W < _worldPosition.GetX()) { return true; }
		return false;
	}

	bool MapChips::IsScrollY() {
		auto mapH = _nowMap.MapSizeHeight();
		auto chipH = _nowMap.ChipSizeHeight();
		auto scrY = mapH * chipH;

		// ���[���hY���W�̓X�N���[���J�n�n�_�𒴂��Ă��邩�H
		if (HALF_WINDOW_H < _worldPosition.GetY() && _worldPosition.GetY() < scrY - HALF_WINDOW_H) { return true; }
		return false;
	}

	void MapChips::WorldUpdate(Vector2 pos) {
		if (_game.GetModeServer()->GetModeMain()->BossFlag() == true) return;	//�@�t���O���I���̏ꍇ�͍X�V���s��Ȃ�
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

	int MapChips::IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos, bool flag) {
		//auto gs = static_cast<int>(g);

		//auto footMinX = box.GetMin().IntX();
		//auto footMaxX = box.GetMax().IntX();	// �����̍��W
		//auto footMinY = box.GetMin().IntY() + gs;
		//auto footMaxY = box.GetMax().IntY() + gs;
		//Vector2 m = { box.GetMin().GetX(), box.GetMin().GetY() + g };
		//Vector2 n = { box.GetMax().GetX(), box.GetMax().GetY() + g };
		//// �Ώۂ̓����蔻��
		//AABB mn = { m, n, box.GetCollisionFlg() };

		auto gs = static_cast<int>(g / CHECK_MAX);
		auto gr = g / CHECK_MAX;
		auto footMinX = box.GetMin().IntX();
		auto footMaxX = box.GetMax().IntX();	// �����̍��W


		for (auto num = CHECK_MIN; num < CHECK_MAX + 1; ++num) {
			auto footMinY = static_cast<int>(box.GetMin().IntY() + (gr * num));
			auto footMaxY = static_cast<int>(box.GetMax().IntY() + (gr * num));

			auto grvity = gr * num;

			Vector2 m = { box.GetMin().GetX(), box.GetMin().GetY() + grvity };
			Vector2 n = { box.GetMax().GetX(), box.GetMax().GetY() + grvity };
			// �Ώۂ̓����蔻��
			AABB mn = { m, n, box.GetCollisionFlag() };


			auto csh = _nowMap.ChipSizeHeight();
			auto csw = _nowMap.ChipSizeWidth();

			int x, y;

			// �����蔻��ƐڐG����\���̂���͈͂Ƃ̂ݔ�����s��
			for (y = footMinY / csh; y <= footMaxY / csh; ++y) {
				for (x = footMinX / csw; x <= footMaxX / csw; ++x) {
					int chip_no = CheckHit(x, y);	// ���̏ꏊ�ɂ͉��̃`�b�v�����邩�H
					if (chip_no != 0) {
						if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON && _chipCheck->IsChipType(chip_no) != mapchip::TRANSITION) continue;	// �����蔻�肪�Ȃ��ꍇ�͔�����
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
							auto chiptype = _chipCheck->IsChipType(chip_no);
							// �����l�����̏ꍇ�i�������A���ƐڐG���Ă��邩�H�j
							if (0 < grvity) {
								// �����͖��܂��Ă��Ȃ����H
								if (mn.GetMin().GetY() < cbox.GetMin().GetY() &&
									// �}�b�v�`�b�v�̏㕔�ɑΏۂ̉������������܂��Ă��邩�ǂ���
									cbox.GetMin().GetY() < mn.GetMax().GetY()) {
									if (chiptype == mapchip::TRANSITION) {
										// �J�ڃ`�b�v�̏ꍇ�͑J�ڏ��������s
										if (flag == true) TransitionResearch(chip_no);
										continue;
									}
									else {
										auto cavep = box.GetHeightMax();
										pos.GetPY() = minY - cavep;
										// �ʏ픻��`�b�v�̏ꍇ�A���W���X�V����
										if (chiptype == mapchip::NORMAL) *lastpos = { maxX , pos.GetY() };
										return chiptype;
									}
								}
								/*if (mn.GetMin().GetY() < cbox.GetMax().GetY() && cbox.GetMin().GetY() < mn.GetMax().GetY()) return true;*/
							}
							else if (grvity < 0) {
								// �����x�����̏ꍇ�i�j
									// �v���C���[�̉����̓}�b�v�`�b�v�̉������傫����
								if (cbox.GetMin().GetY() < mn.GetMax().GetY() && mn.GetMin().GetY() < cbox.GetMax().GetY()) {
									if (chiptype == mapchip::TRANSITION) {
										if (flag == true) TransitionResearch(chip_no);
										continue;
									}
									else {
										auto cavep = box.GetHeightMin();
										pos.GetPY() = maxY + cavep;
										return chiptype;
									}
								}

							}
						}
					}
				}
			}
		}
		return mapchip::TYPE_NULL;
	}
		

	int MapChips::IsHit(AABB box, Vector2& pos, Vector2& move, bool direction) {
		int x, y;

		auto thisbox = box;

		auto vectorX = move.GetX() / CHECK_MAX;
		auto vectorY = move.GetY() / CHECK_MAX;

		auto startX = static_cast<int>(box.GetMin().GetX() + move.GetX());
		auto startY = static_cast<int>(box.GetMin().GetY() + move.GetY());
		auto endY = static_cast<int>(box.GetMax().GetY() + move.GetY());
		auto endX = static_cast<int>(box.GetMax().GetX() + move.GetX());

		for (auto num = CHECK_MIN; num < CHECK_MAX + 1; ++num) {
			Vector2 move_s = { (vectorX * num), (vectorY * num) };
			Vector2 movepos = { pos + move_s };
			thisbox.Update(movepos, direction);

			auto minx = thisbox.GetMin().IntX();
			auto miny = thisbox.GetMin().IntY();
			auto maxx = thisbox.GetMax().IntX();
			auto maxy = thisbox.GetMax().IntY();

			for (y = startY / _nowMap.ChipSizeHeight(); y <= endY / _nowMap.ChipSizeHeight(); ++y) {
				for (x = startX / _nowMap.ChipSizeWidth(); x <= endX / _nowMap.ChipSizeWidth(); ++x) {
					// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
					int chip_no = CheckHit(x, y);
					// �`�b�v�ԍ���0���ǂ���
					if (chip_no != 0) {
						if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON) continue;	// �����蔻�肪�Ȃ��ꍇ�͔�����

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

						auto chiptype = _chipCheck->IsChipType(chip_no);
						auto hittype = _chipCheck->IsHitType(chip_no);

						if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
							if (vectorY * CHECK_MAX < 0) {
								if (miny < chipMaxY && chipMaxY < maxy) {
									auto cave = box.GetHeightMin();
									move.GetPY() = 0;	// �ړ��ʏ�����
									pos.GetPY() = chipMaxY + cave;
								}
							}
							else if (0 < vectorY * CHECK_MAX) {
								if (chipMinY < maxY && miny < chipMinY) {
									auto cave = box.GetHeightMin();
									move.GetPY() = 0;	// �ړ��ʏ�����
									pos.GetPY() = chipMinY + cave;
								}
							}
						}

						if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
							if (vectorX < 0 * CHECK_MAX) {
								if (minx < chipMaxX && chipMinX < maxx) {
									auto cave = box.GetWidthMin();
									move.GetPX() = 0;
									pos.GetPX() = chipMaxX + cave;
									return _chipCheck->IsChipType(chip_no);
								}
							}
							if (0 < vectorX * CHECK_MAX) {
								if (chipMinX < maxx && minx < chipMaxX) {
									auto cave = box.GetWidthMin();
									move.GetPX() = 0;
									pos.GetPX() = chipMinX - cave;
									return _chipCheck->IsChipType(chip_no);
								}
							}
						}
					}
				}
			}
		}
		return mapchip::NONE;
	}

	int MapChips::IsHit(AABB box, Vector2& pos, Vector2& move, bool direction, bool* isGran) {
		int x, y;

		auto thisbox = box;

		auto vectorX = move.GetX() / CHECK_MAX;
		auto vectorY = move.GetY() / CHECK_MAX;

		auto startX = static_cast<int>(box.GetMin().GetX() + move.GetX());
		auto startY = static_cast<int>(box.GetMin().GetY() + move.GetY());
		auto endY = static_cast<int>(box.GetMax().GetY() + move.GetY());
		auto endX = static_cast<int>(box.GetMax().GetX() + move.GetX());

		for (auto num = CHECK_MIN; num < CHECK_MAX + 1; ++num) {
			Vector2 move_s = { (vectorX * num), (vectorY * num) };
			Vector2 movepos = { pos + move_s };
			thisbox.Update(movepos, direction);

			auto minx = thisbox.GetMin().IntX();
			auto miny = thisbox.GetMin().IntY();
			auto maxx = thisbox.GetMax().IntX();
			auto maxy = thisbox.GetMax().IntY();

			for (y = startY / _nowMap.ChipSizeHeight(); y <= endY / _nowMap.ChipSizeHeight(); ++y) {
				for (x = startX / _nowMap.ChipSizeWidth(); x <= endX / _nowMap.ChipSizeWidth(); ++x) {
					// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
					int chip_no = CheckHit(x, y);
					// �`�b�v�ԍ���0���ǂ���
					if (chip_no != 0) {
						// if (_chipCheck->IsHitType(chip_no) != mapchip::HIT_ON) continue;	// �����蔻�肪�Ȃ��ꍇ�͔�����
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

						auto chiptype = _chipCheck->IsChipType(chip_no);
						auto hittype = _chipCheck->IsHitType(chip_no);

						if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
							if (vectorY * num < 0) {
								if (miny < chipMaxY && chipMaxY < maxy) {
									// < chipMinY && chipMinY < maxy) {
									if (hittype == mapchip::HIT_ON){// num == CHECK_MAX) {
										auto cave = box.GetHeightMin();
										move.GetPY() = 0;	// �ړ��ʏ�����
										pos.GetPY() = chipMaxY + cave;
									}
									else if (chiptype == mapchip::TYPE_IVX) *isGran = true;
									else if (chiptype == mapchip::TRANSITION) TransitionResearch(chip_no);
								}
							}
							else if (0 < vectorY * num) {
								if (chipMinY < maxY && miny < chipMinY) {
									if (hittype == mapchip::HIT_ON){/* && num == CHECK_MAX) {*/
										auto cave = box.GetHeightMin();
										move.GetPY() = 0;	// �ړ��ʏ�����
										pos.GetPY() = chipMinY + cave;
									}
									else if (chiptype == mapchip::TYPE_IVX) *isGran = true;
									else if (chiptype == mapchip::TRANSITION) TransitionResearch(chip_no);
								}
							}
						}

						if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
							if (vectorX * num < 0) {
								if (minx < chipMaxX && chipMinX < maxx) {
									if (hittype == mapchip::HIT_ON) {
										auto cave = box.GetWidthMin();
										move.GetPX() = 0;
										pos.GetPX() = chipMaxX + cave;
										return _chipCheck->IsChipType(chip_no);
									}
									else if (chiptype == mapchip::TYPE_IVX) *isGran = true;
									else if (chiptype == mapchip::TRANSITION) TransitionResearch(chip_no);
								}
							}
							if (0 < vectorX * num) {
								if (chipMinX < maxx && minx < chipMaxX) {
									if (hittype == mapchip::HIT_ON) {
										auto cave = box.GetWidthMin();
										move.GetPX() = 0;
										pos.GetPX() = chipMinX - cave;
										return _chipCheck->IsChipType(chip_no);
									}
									else if (chiptype == mapchip::TYPE_IVX) *isGran = true;
									else if (chiptype == mapchip::TRANSITION) TransitionResearch(chip_no);
								}
							}
						}
					}
				}
			}
		}
		return mapchip::NONE;
	}

	bool MapChips::HitIvy(AABB box, Vector2 pos, Vector2 move, std::pair<double, double>* ivx, bool direction) {
		pos = pos + move;
		box.Update(pos, direction);

		auto minx = box.GetMin().IntX();
		auto maxx = box.GetMax().IntX();
		auto miny = box.GetMin().IntY();
		auto maxy = box.GetMax().IntY();

		int x = 0, y = 0;
		for (y = miny / _nowMap.ChipSizeHeight(); y <= maxy / _nowMap.ChipSizeHeight(); ++y) {
			for (x = minx / _nowMap.ChipSizeWidth(); x <= maxx / _nowMap.ChipSizeWidth(); ++x) {
				// �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
				int chip_no = CheckHit(x, y);
				// �`�b�v�ԍ���0���ǂ���
				if (_chipCheck->IsChipType(chip_no) != mapchip::IVY) continue;

				auto c = _chipCheck->ChipCollision(chip_no);
				
				Vector2 cmin = { static_cast<double>(x * _nowMap.ChipSizeWidth() + c.GetMin().IntX()), static_cast<double>(y * _nowMap.ChipSizeHeight() + c.GetMin().IntY()) };
				Vector2 cmax = { static_cast<double>(x * _nowMap.ChipSizeWidth() + c.GetMax().IntX()), static_cast<double>(y * _nowMap.ChipSizeHeight() + c.GetMax().IntY()) };
				AABB cBox = { cmin, cmax, true };
				/*if (cBox.HitCheck(box) == true)*/
				*ivx = std::make_pair(cmin.GetX(), cmax.GetX());
				return true;
			}
		}
		return false;	// �q�b�g���Ȃ�����
	}

bool MapChips::TransitionResearch(const int no) {
	if (_stageTransition->IsHit(no)) {
		return true;
	}
	return false;
}

	void MapChips::SetChipsMap() {
		// �e�퓖���蔻���o�^����
		// �����蔻����C������`�b�v�ԍ���o�^
		ChipHitCheck::ChipsMap stagechip {
			// ����:�C������`�b�v�ԍ��A�E��:�C�����������蔻��+�`�b�v�Ɍ��ʂ��������邩�ǂ���
			// ��
			{  2, {0, 40, 20 ,40, mapchip::THORM}},
			{  6, {0, 6}},	// �[�̃`�b�v�ɂ͔�����������Ȃ�
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
			{ 58, {0, 40, 0, 6}},
			{ 59, {0, 40, 0, 6}},
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
			{ 90, {0, 40, 0, 6}},
			{ 91, {0, 40, 0, 6}},

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
			{ 129, {mapchip::TRANSITION, mapchip::HIT_OFF}},	// �X�e�[�WS(1)���X�e�[�W1(2)
			{ 130, {mapchip::TRANSITION, mapchip::HIT_OFF}},	// �X�e�[�W1(3)���X�e�[�W2-2()
			{ 131, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 132, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 133, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 134, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 135, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 136, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 137, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 138, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 139, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 140, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 141, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 142, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 143, {mapchip::TRANSITION, mapchip::HIT_OFF}},
			{ 144, {mapchip::TRANSITION, mapchip::HIT_OFF}},
		};
		_chipCheck->LoadChipsMap(stage::KEY_NORMAL, stagechip);
		_chipCheck->ChangeStageKey(stage::KEY_NORMAL);	// �ŏ��ɌĂяo���X�e�[�W��o�^
	}

	void MapChips::SetChipMember() {

	}

	void MapChips::ChangeMap(std::string nextStage) {
		// �}�b�v�f�[�^�͓ǂݍ��܂�Ă��邩�H
		if (_mapManager->IsLoad(nextStage) == true) {
			// �ǂݍ��܂ꂢ�Ȃ��ꍇ��
			std::string stagePath = nextStage + JSON_FORMAT;	// �p�X���쐬
			TiledJsonLoad(nextStage, DEFAULT_PATH, stagePath);	// �Ώۂ̓o�^
		}
		_mapManager->GetStageMap(nextStage, _nowMap);	// �z�u���̎擾
	}
}