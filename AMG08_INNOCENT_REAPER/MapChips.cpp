/*****************************************************************//**
 * @file   MapChips.cpp
 * @brief  �}�b�v�`�b�v�N���X
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "MapChips.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Player.h"
#include "ResourceServer.h"
#include "DivGraph.h"
#include "Collision.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include "ChipNumber.h"
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

  constexpr auto CHIP_THORM_LEFT1 = 20;  // �����[(xmin)
  constexpr auto CHIP_THORM_LEFT2 = 40;  // �����[(xmax)
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
    _chipCheck = std::make_unique<ChipHitCheck>(); // �`�b�v�����蔻��C��
    _mapManager = std::make_unique<MapDataManager>(_game.GetGame());
    _stageTransition = std::make_unique<StageTransition>(_game.GetGame());

    _stageTransition->Init(); // �������������s��
    SetChipsMap();            // �}�b�v�`�b�v�̓����蔻���o�^

    _worldPosition = { DEFAULT_W, DEFAULT_H };
    _worldLast = _worldPosition;
  }

  MapChips::~MapChips() {
    _mapManager->StageMapClear(); // �X�e�[�W���̉��
  }

  void MapChips::Init() {
    _mapManager->StageMapClear(); // �X�e�[�W���̉��
  }

  void MapChips::Process() {
    _worldLast = _worldPosition;
    WorldClanp(); // �L�[���X�V���ꂽ�ꍇ�̓}�b�v����؂�ւ���
  }

  void MapChips::Draw() {
    auto mapSizeWidth = _nowMap.MapSizeWidth();
    auto mapSizeHeight = _nowMap.MapSizeHeight();
    auto chipSizeWidth = _nowMap.ChipSizeWidth();
    auto chipSizeHeight = _nowMap.ChipSizeHeight();
    // �`�揈��
    int x, y, layer;

    int minY = _worldPosition.IntY() - 640;
    int maxY = _worldPosition.IntY() + 640;
    int minX = _worldPosition.IntX() - 1060;
    int maxX = _worldPosition.IntX() + 1060;

    int startY = minY / chipSizeHeight;
    int endY = maxY / chipSizeHeight;
    int startX = minX / chipSizeWidth;
    int endX = maxX / chipSizeWidth;

    if (startX < 0) startX = 0;
    if (mapSizeWidth < endX) endX = mapSizeWidth;
    if (startY < 0) startY = 0;
    if (mapSizeHeight < endY) endY = mapSizeHeight;

    for (layer = 0; layer < _nowMap.MapSizeLayer(); ++layer) {
      for (y = startY; y < endY; ++y) {
        for (x = startX; x < endX; ++x) {

          int layerStart = mapSizeWidth * mapSizeHeight * layer;
          int index = y * mapSizeWidth + x;
          int no = _nowMap.MapDatas(layerStart + index);
          // �����蔻����擾
          auto c = _chipCheck->ChipCollision(no);
          auto chipMinX = c.GetMin().IntX();
          auto chipMaxX = c.GetMax().IntX();
          auto chipMinY = c.GetMin().IntY();
          auto chipMaxY = c.GetMax().IntY();

          int posX = x * chipSizeWidth - _worldPosition.IntX() + HALF_WINDOW_W;
          int posY = y * chipSizeHeight - (_worldPosition.IntY() - HALF_WINDOW_H);
          --no;

          if (0 <= no) {
            // �J�ڃ`�b�v�̏ꍇ�͏������s��Ȃ�
            if (no < 128) {
              auto gh = graph::ResourceServer::GetHandles(stage::KEY_NORMAL, no);
              DrawGraph(posX, posY, gh, TRUE);
            }
#ifdef _DEBUG
            if (_game.IsDebugMode() == true) {
              auto chipType = _chipCheck->IsChipType(no + 1);
              std::tuple<int, int, int> colorType = std::make_tuple(255, 0, 0);
              if (chipType == mapchip::IVY) colorType = std::make_tuple(255, 255, 0);
              if (chipType == mapchip::THORM) colorType = std::make_tuple(255, 255, 255);
              if (chipType == mapchip::TRANSITION) colorType = std::make_tuple(0, 0, 255);
              // �f�o�b�O�p�F�����蔻��̕`��
              if (CheckHit(x, y)) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
                DrawBox(posX + chipMinX, posY + chipMinY, posX + chipMaxX, posY + chipMaxY, GetColor(std::get<0>(colorType), std::get<1>(colorType), std::get<2>(colorType)), TRUE);
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
        pos.GetPY() = fixY;
      }
      return true; // �N�����v����
    }
    return false;  // �N�����v���s
  }

  bool MapChips::IsScrollX() {
    auto mapW = _nowMap.MapSizeWidth();
    auto chipW = _nowMap.ChipSizeWidth();

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
    //�@�t���O���I���̏ꍇ�͍X�V���s��Ȃ�
    if (_game.GetModeServer()->GetModeMain()->BossFlag() == true) return;
    // �O�t���[���̍��W�Ƃ��ēo�^
    _worldLast = _worldPosition;

    _worldPosition.GetPX() = pos.GetX();
    _worldPosition.GetPY() = pos.GetY();
    WorldClanp();
  }

  std::string MapChips::StringFileLoad(std::string FileName) {
    FILE* fp;
    fopen_s(&fp, FileName.c_str(), "rb");

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

  int MapChips::TiledJsonLoad(std::string sKey, std::string filePath, std::string strFileName) {
    std::string strJson = StringFileLoad(filePath + strFileName);
    if (strJson == "") return 0;

    picojson::value json;
    picojson::parse(json, strJson);
    picojson::object jsRoot = json.get<picojson::object>();
    // �}�b�v�̃T�C�Y����json����擾
    auto mapSizeWidth  = static_cast<int>(jsRoot["width"].get<double>());
    auto mapSizeHeight = static_cast<int>(jsRoot["height"].get<double>());

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
    std::vector<int> mapData;
    mapData.resize(static_cast<int>(mapSizeLayer * mapSizeWidth * mapSizeHeight));
    layer = 0;

    std::vector<int> chipType;
    // ���C���[���f�[�^�̎擾
    for (int i = 0; i < static_cast<int>(aLayers.size()); ++i) {

      picojson::object jsLayer = aLayers[i].get<picojson::object>();
      if (jsLayer["type"].get<std::string>() == "tilelayer") {
        picojson::array aData = jsLayer["data"].get<picojson::array>();

        for (int y = 0; y < mapSizeHeight; ++y) {

          for (int x = 0; x < mapSizeWidth; ++x) {
            int layerStart = mapSizeWidth * mapSizeHeight * layer;
            int index = y * mapSizeWidth + x;
            mapData[layerStart + index] = static_cast<int>(aData[index].get<double>());
            // vector�z��Ɏg�p����Ă���}�b�v�`�b�v��o�^���邩�ǂ���
            int mapchipNo = static_cast<int>(aData[index].get<double>());
            // 0�̏ꍇ�͏������X�L�b�v
            if (mapchipNo == 0) continue;
            // _mapChipsType�ɒl�����邩�ǂ���������
            auto it = std::find(chipType.begin(), chipType.end(), mapchipNo);
            // �q�b�g���Ȃ������ꍇ�́A�����ɗv�f��ǉ�
            if (it == chipType.end()) {
              chipType.emplace_back(mapchipNo);
            }
          }
        }
        ++layer;
      }
    }
    // �`�b�v�ԍ����Ⴂ���ɕ��ѕς���
    sort(chipType.begin(), chipType.end());
    // ������-1��ǉ�
    chipType.emplace_back(-1);
    // �z�u���̓o�^
    MapDataManager::JsonMapData jmd {
      { sKey, { std::make_pair(mapSizeWidth, mapSizeHeight), std::make_tuple(chipCount, chipCountW, chipCountH),
            std::make_pair(chipSizeW, chipSizeH), mapSizeLayer, fileChips, mapData, chipType }},
    };
    _mapManager->LoadStageMap(jmd); // �}�b�v�}�l�[�W���[�ɓo�^
    // ResourceServer�Ń}�b�v�`�b�v�摜�̓ǂݍ���
    auto fileName = filePath + fileChips;
    auto chipAllNum = chipCountW * chipCountH;
    const graph::ResourceServer::DivGraphMap mapchip{
      { stage::KEY_NORMAL, {(fileName).c_str(), chipCountW, chipCountH, chipAllNum, chipSizeW, chipSizeH}},
    };
    graph::ResourceServer::LoadGraphList(mapchip);

    return 1;
  }

  int MapChips::CheckHit(int x, int y) {

    auto mapSizeW = _nowMap.MapSizeWidth();
    auto mapSizeH = _nowMap.MapSizeHeight();

    auto chips = _nowMap.MapDatas();
    auto chipType = _nowMap.ChipType();

    if (0 <= x && x < mapSizeW && 0 <= y && y < mapSizeH) {
      int chipNo = chips[y * mapSizeW + x];
      // ������ID���ǂ����̔���
      auto i = 0;
      while (chipType[i] != -1) {
        if (chipNo == chipType[i]) {
          // ���������ꍇ�A���̃`�b�v�ԍ���Ԃ�
          return chipNo;
        }
        ++i;
      }
    }
    // ������Ȃ������ꍇ��0��Ԃ�
    return 0;
  }

  int MapChips::IsStand(AABB box, Vector2& pos, double g, Vector2* lastPos, bool flag) {
    auto gs = static_cast<int>(g / CHECK_MAX);
    auto gr = g / CHECK_MAX;
    auto footMinX = box.GetMin().IntX();
    auto footMaxX = box.GetMax().IntX();  // �����̍��W


    for (auto num = CHECK_MIN; num < CHECK_MAX + 1; ++num) {
      auto footMinY = static_cast<int>(box.GetMin().IntY() + (gr * num));
      auto footMaxY = static_cast<int>(box.GetMax().IntY() + (gr * num));

      auto gravity = gr * num;

      Vector2 m = { box.GetMin().GetX(), box.GetMin().GetY() + gravity };
      Vector2 n = { box.GetMax().GetX(), box.GetMax().GetY() + gravity };
      // �Ώۂ̓����蔻��
      AABB mn = { m, n, box.GetCollisionFlag() };

      auto csh = _nowMap.ChipSizeHeight();
      auto csw = _nowMap.ChipSizeWidth();

      int x, y;
      // �����蔻��ƐڐG����\���̂���͈͂Ƃ̂ݔ�����s��
      for (y = footMinY / csh; y <= footMaxY / csh; ++y) {
        for (x = footMinX / csw; x <= footMaxX / csw; ++x) {
          int chipNo = CheckHit(x, y);  // ���̏ꏊ�ɂ͉��̃`�b�v�����邩�H
          if (chipNo != 0) {
            if (_chipCheck->IsHitType(chipNo) != mapchip::HIT_ON && _chipCheck->IsChipType(chipNo) != mapchip::TRANSITION) continue;  // �����蔻�肪�Ȃ��ꍇ�͔�����
            // �Փ˔���
            auto c = _chipCheck->ChipCollision(chipNo);

            double minX = x * csw + c.GetMin().IntX();
            double maxX = x * csw + c.GetMax().IntX();
            double minY = y * csh + c.GetMin().IntY();
            double maxY = y * csh + c.GetMax().IntY();
            Vector2 cMin = { minX, minY };
            Vector2 cMax = { maxX, maxY };
            AABB cBox = { cMin, cMax, true };

            // ����͈͓��Ɏ��܂��Ă��邩�H
            if (mn.GetMin().GetX() < cBox.GetMax().GetX() && cBox.GetMin().GetX() < mn.GetMax().GetX()) {
              auto chipType = _chipCheck->IsChipType(chipNo);
              // �����l�����̏ꍇ�i�������A���ƐڐG���Ă��邩�H�j
              if (0 < gravity) {
                // �����͖��܂��Ă��Ȃ����H
                if (mn.GetMin().GetY() < cBox.GetMin().GetY() &&
                  // �}�b�v�`�b�v�̏㕔�ɑΏۂ̉������������܂��Ă��邩�ǂ���
                  cBox.GetMin().GetY() < mn.GetMax().GetY()) {
                  if (chipType == mapchip::TRANSITION) {
                    // �J�ڃ`�b�v�̏ꍇ�͑J�ڏ��������s
                    if (flag == true) TransitionResearch(chipNo);
                    continue;
                  }
                  else {
                    auto cavep = box.GetHeightMax();
                    pos.GetPY() = minY - cavep;
                    // �ʏ픻��`�b�v�̏ꍇ�A���W���X�V����
                    if (chipType == mapchip::NORMAL) *lastPos = { maxX , pos.GetY() };
                    return chipType;
                  }
                }
              }
              else if (gravity < 0) {
                // �v���C���[�̉����̓}�b�v�`�b�v�̉������傫����
                if (cBox.GetMin().GetY() < mn.GetMax().GetY() && mn.GetMin().GetY() < cBox.GetMax().GetY()) {
                  if (chipType == mapchip::TRANSITION) {
                    if (flag == true) TransitionResearch(chipNo);
                    continue;
                  }
                  else {
                    auto cavep = box.GetHeightMin();
                    pos.GetPY() = maxY + cavep;
                    return chipType;
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

    auto thisBox = box;

    auto vectorX = move.GetX() / CHECK_MAX;
    auto vectorY = move.GetY() / CHECK_MAX;

    auto startX = static_cast<int>(box.GetMin().GetX() + move.GetX());
    auto startY = static_cast<int>(box.GetMin().GetY() + move.GetY());
    auto endY = static_cast<int>(box.GetMax().GetY() + move.GetY());
    auto endX = static_cast<int>(box.GetMax().GetX() + move.GetX());

    for (auto num = CHECK_MIN; num < CHECK_MAX + 1; ++num) {
      Vector2 moves = { (vectorX * num), (vectorY * num) };
      Vector2 movePos = { pos + moves };
      thisBox.Update(movePos, direction);

      auto minX = thisBox.GetMin().IntX();
      auto minY = thisBox.GetMin().IntY();
      auto maxX = thisBox.GetMax().IntX();
      auto maxY = thisBox.GetMax().IntY();

      for (y = startY / _nowMap.ChipSizeHeight(); y <= endY / _nowMap.ChipSizeHeight(); ++y) {
        for (x = startX / _nowMap.ChipSizeWidth(); x <= endX / _nowMap.ChipSizeWidth(); ++x) {
          // �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
          int chipNo = CheckHit(x, y);
          // �`�b�v�ԍ���0���ǂ���
          if (chipNo != 0) {
            if (_chipCheck->IsHitType(chipNo) != mapchip::HIT_ON) continue; // �����蔻�肪�Ȃ��ꍇ�͔�����
            auto chipCollision = _chipCheck->ChipCollision(chipNo);
            auto collisionMinX = chipCollision.GetMin().IntX();
            auto collisionMaxX = chipCollision.GetMax().IntX();
            auto collisionMinY = chipCollision.GetMin().IntY();
            auto collisionMaxY = chipCollision.GetMax().IntY();
            // �V�K�ǉ�
            auto chipMinX = x * _nowMap.ChipSizeWidth() + collisionMinX;
            auto chipMinY = y * _nowMap.ChipSizeHeight() + collisionMinY;
            auto chipMaxX = x * _nowMap.ChipSizeWidth() + collisionMaxX;
            auto chipMaxY = y * _nowMap.ChipSizeHeight() + collisionMaxY;

            auto chipType = _chipCheck->IsChipType(chipNo);
            auto hitType = _chipCheck->IsHitType(chipNo);

            if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
              if (vectorY * CHECK_MAX < 0) {
                if (minY < chipMaxY && chipMaxY < maxY) {
                  auto cave = box.GetHeightMin();
                  move.GetPY() = 0;  // �ړ��ʏ�����
                  pos.GetPY() = chipMaxY + cave;
                }
              }
              else if (0 < vectorY * CHECK_MAX) {
                if (chipMinY < collisionMaxY && minY < chipMinY) {
                  auto cave = box.GetHeightMin();
                  move.GetPY() = 0;  // �ړ��ʏ�����
                  pos.GetPY() = chipMinY + cave;
                }
              }
            }
            if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
              if (vectorX < 0 * CHECK_MAX) {
                if (minX < chipMaxX && chipMinX < maxX) {
                  auto cave = box.GetWidthMin();
                  move.GetPX() = 0;
                  pos.GetPX() = chipMaxX + cave;
                  return _chipCheck->IsChipType(chipNo);
                }
              }
              if (0 < vectorX * CHECK_MAX) {
                if (chipMinX < maxX && minX < chipMaxX) {
                  auto cave = box.GetWidthMin();
                  move.GetPX() = 0;
                  pos.GetPX() = chipMinX - cave;
                  return _chipCheck->IsChipType(chipNo);
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

    auto thisBox = box;

    auto vectorX = move.GetX() / CHECK_MAX;
    auto vectorY = move.GetY() / CHECK_MAX;

    auto startX = static_cast<int>(box.GetMin().GetX() + move.GetX());
    auto startY = static_cast<int>(box.GetMin().GetY() + move.GetY());
    auto endY = static_cast<int>(box.GetMax().GetY() + move.GetY());
    auto endX = static_cast<int>(box.GetMax().GetX() + move.GetX());

    for (auto num = CHECK_MIN; num < CHECK_MAX + 1; ++num) {
      Vector2 moves = { (vectorX * num), (vectorY * num) };
      Vector2 movePos = { pos + moves };
      thisBox.Update(movePos, direction);

      auto minX = thisBox.GetMin().IntX();
      auto minY = thisBox.GetMin().IntY();
      auto maxX = thisBox.GetMax().IntX();
      auto maxY = thisBox.GetMax().IntY();

      for (y = startY / _nowMap.ChipSizeHeight(); y <= endY / _nowMap.ChipSizeHeight(); ++y) {
        for (x = startX / _nowMap.ChipSizeWidth(); x <= endX / _nowMap.ChipSizeWidth(); ++x) {
          // �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
          int chip_no = CheckHit(x, y);
          // �`�b�v�ԍ���0���ǂ���
          if (chip_no != 0) {
            // �����蔻����擾
            auto chipCollision = _chipCheck->ChipCollision(chip_no);
            auto collisionMinX = chipCollision.GetMin().IntX();
            auto collisionMaxX = chipCollision.GetMax().IntX();
            auto collisionMinY = chipCollision.GetMin().IntY();
            auto collisionMaxY = chipCollision.GetMax().IntY();
            // �V�K�ǉ�
            auto chipMinX = x * _nowMap.ChipSizeWidth() + collisionMinX;
            auto chipMinY = y * _nowMap.ChipSizeHeight() + collisionMinY;
            auto chipMaxX = x * _nowMap.ChipSizeWidth() + collisionMaxX;
            auto chipMaxY = y * _nowMap.ChipSizeHeight() + collisionMaxY;

            auto chipType = _chipCheck->IsChipType(chip_no);
            auto hitType = _chipCheck->IsHitType(chip_no);

            if (box.GetMin().IntX() < chipMaxX && chipMinX < box.GetMax().IntX()) {
              if (vectorY * num < 0) {
                if (minY < chipMaxY && chipMaxY < maxY) {
                  if (hitType == mapchip::HIT_ON){
                    auto cave = box.GetHeightMin();
                    move.GetPY() = 0; // �ړ��ʏ�����
                    pos.GetPY() = chipMaxY + cave;
                  }
                  else if (chipType == mapchip::TYPE_IVX) *isGran = true;
                  else if (chipType == mapchip::TRANSITION) TransitionResearch(chip_no);
                }
              }
              else if (0 < vectorY * num) {
                if (chipMinY < collisionMaxY && minY < chipMinY) {
                  if (hitType == mapchip::HIT_ON){
                    auto cave = box.GetHeightMin();
                    move.GetPY() = 0; // �ړ��ʏ�����
                    pos.GetPY() = chipMinY + cave;
                  }
                  else if (chipType == mapchip::TYPE_IVX) *isGran = true;
                  else if (chipType == mapchip::TRANSITION) TransitionResearch(chip_no);
                }
              }
            }

            if (box.GetMin().GetY() < chipMaxY && chipMinY < box.GetMax().GetY()) {
              if (vectorX * num < 0) {
                if (minX < chipMaxX && chipMinX < maxX) {
                  if (hitType == mapchip::HIT_ON) {
                    auto cave = box.GetWidthMin();
                    move.GetPX() = 0;
                    pos.GetPX() = chipMaxX + cave;
                    return _chipCheck->IsChipType(chip_no);
                  }
                  else if (chipType == mapchip::TYPE_IVX) *isGran = true;
                  else if (chipType == mapchip::TRANSITION) TransitionResearch(chip_no);
                }
              }
              if (0 < vectorX * num) {
                if (chipMinX < maxX && minX < chipMaxX) {
                  if (hitType == mapchip::HIT_ON) {
                    auto cave = box.GetWidthMin();
                    move.GetPX() = 0;
                    pos.GetPX() = chipMinX - cave;
                    return _chipCheck->IsChipType(chip_no);
                  }
                  else if (chipType == mapchip::TYPE_IVX) *isGran = true;
                  else if (chipType == mapchip::TRANSITION) TransitionResearch(chip_no);
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

    auto minX = box.GetMin().IntX();
    auto maxX = box.GetMax().IntX();
    auto minY = box.GetMin().IntY();
    auto maxY = box.GetMax().IntY();

    int x = 0, y = 0;
    for (y = minY / _nowMap.ChipSizeHeight(); y <= maxY / _nowMap.ChipSizeHeight(); ++y) {
      for (x = minX / _nowMap.ChipSizeWidth(); x <= maxX / _nowMap.ChipSizeWidth(); ++x) {
        // �}�b�v�`�b�v�ƐڐG���Ă��邩�ǂ����H
        int chipNo = CheckHit(x, y);
        // �`�b�v�ԍ����ӂł͂Ȃ��ꍇ�͏������X�L�b�v
        if (_chipCheck->IsChipType(chipNo) != mapchip::IVY) continue;

        auto c = _chipCheck->ChipCollision(chipNo);
        
        Vector2 cMin = { static_cast<double>(x * _nowMap.ChipSizeWidth() + c.GetMin().IntX()), static_cast<double>(y * _nowMap.ChipSizeHeight() + c.GetMin().IntY()) };
        Vector2 cMax = { static_cast<double>(x * _nowMap.ChipSizeWidth() + c.GetMax().IntX()), static_cast<double>(y * _nowMap.ChipSizeHeight() + c.GetMax().IntY()) };
        AABB cBox = { cMin, cMax, true };

        *ivx = std::make_pair(cMin.GetX(), cMax.GetX());
        return true;
      }
    }
    return false; // �q�b�g���Ȃ�����
  }
  // �J�ڃ`�b�v�ƐڐG������
  bool MapChips::TransitionResearch(const int no) {
    // �J�ڃ`�b�v���̔���
    if (_stageTransition->IsHit(no)) {
      return true; // �ڐG����
    }
    return false;  // �ڐG���Ă��Ȃ�
  }

  void MapChips::SetChipsMap() {
    // �e�퓖���蔻���o�^����
    // �����蔻����C������`�b�v�ԍ���o�^
    ChipHitCheck::ChipsMap stageChip {
      // ����:�C������`�b�v�ԍ��A�E��:�C�����������蔻��+�`�b�v�Ɍ��ʂ��������邩�ǂ���
      // ��
      {  2, {0, 40, 20 ,40, mapchip::THORM}},
      {  6, {0, 6}},  // �[�̃`�b�v�ɂ͔�����������Ȃ�
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
      { 129, {mapchip::TRANSITION, mapchip::HIT_OFF}},
      { 130, {mapchip::TRANSITION, mapchip::HIT_OFF}},
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
    _chipCheck->LoadChipsMap(stage::KEY_NORMAL, stageChip);
    _chipCheck->ChangeStageKey(stage::KEY_NORMAL); // �ŏ��ɌĂяo���X�e�[�W��o�^
  }

  void MapChips::ChangeMap(std::string nextStage) {
    // �}�b�v�f�[�^�͓ǂݍ��܂�Ă��邩�H
    if (_mapManager->IsLoad(nextStage) == true) {
      // �ǂݍ��܂ꂢ�Ȃ��ꍇ��
      std::string stagePath = nextStage + JSON_FORMAT;   // �p�X���쐬
      TiledJsonLoad(nextStage, DEFAULT_PATH, stagePath); // �Ώۂ̓o�^
    }
    _mapManager->GetStageMap(nextStage, _nowMap);        // �z�u���̎擾
  }
}