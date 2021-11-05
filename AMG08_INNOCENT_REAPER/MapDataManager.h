/*****************************************************************//**
 * @file   MapDataManager.h
 * @brief  マップ情報の管理を行うマップデータマネージャ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>

namespace inr {
  /** 二重インクルード防止 */
  class Game;
  class MapData;
  /** マップデータ管理用のクラス */
  class MapDataManager {
  public:
    /**
     * @brief             コンストラクタ
     * @param game        ゲームクラスの参照
     */
    MapDataManager(Game& game);
    /**
     * @brief             デストラクタ
     */
    ~MapDataManager();
    /** マップ情報を管理するコンテナ */
    using JsonMapData = std::unordered_map<std::string, MapData>;
    /**
     * @brief             マップ情報の一括読み込み
     * @param jsonMapData マップ情報を格納した連想配列
     */
    void LoadStageMap(JsonMapData& jsonMapData);
    /**
     * @brief             指定したマップ情報の取得
     * @param stage       ステージキー
     * @param mapData     マップ情報
     * @return            読み込みに成功した場合はtrueを返す
     *                    マップ情報の取り出しに失敗した場合はfalseを返す
     */
    bool GetStageMap(const std::string stage, MapData& mapData);
    /**
     * @brief             読み込み処理を行うかの判定
     * @param key         ステージキー
     * @return            読み込みを行う場合はtrueを返す
     *                    既に読み込んでいる場合はfalseを返す
     */
    bool IsLoad(const std::string key);
    /**
     * @brief             コンテナの開放
     */
    void StageMapClear();
  private:
    JsonMapData _maps; //!< マップ情報を管理するコンテナ
    Game& _game;       //!< ゲームクラスの参照
  };
}