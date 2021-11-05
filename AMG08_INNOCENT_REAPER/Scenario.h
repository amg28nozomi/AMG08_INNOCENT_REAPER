/*****************************************************************//**
 * @file   Scenario.h
 * @brief  シナリオクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "Vector2.h"

namespace inr {
  /** 二重インクルード防止 */
  class Game;
  class Player;
  class EnemyBase;
  class ObjectValue;
  class ImageValue;
  class TutorialValue;
  class Particle_Image;
  class TutorialImage;
  /** 画像情報を格納する連想配列 */
  using ImageMap = std::unordered_map<int, ImageValue>;
  /** シナリオ */
  class Scenario {
  public:
    /** オブジェクト情報を格納する連想配列(キー:ステージ　値:オブジェクト情報のコンテナ) */
    using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
    /**
     * @brief         コンストラクタ
     * @param game    ゲームクラスの参照
     */
    Scenario(Game& game);
    /**
     * @brief         デストラクタ
     */
    ~Scenario();
    /**
     * @brief         初期化処理
     */
    void Init();
    /**
     * @brief         シナリオの更新
     * @param key     ステージ
     */
    void ScenarioUpdate(std::string key);
    /**
     * @brief         オブジェクトの生成
     * @param key     ステージ
     * @return        trueを返す
     */
    bool AddObjects(const std::string key);
    /**
     * @brief         ボス戦開始時にブロックを生成する
     */
    void BossBlock();
  private:
    Game& _game;            //!< ゲームクラスの参照
    ScenarioMap _scenarios; //!< 各ステージのオブジェクト情報
    /**
     * @brief         自機の生成
     */
    void AddPlayer();
    /**
     * @brief         ソルジャードールの生成
     * @param oValue  オブジェクト情報
     */
    void AddSoldierDoll(ObjectValue oValue);
    /**
     * @brief         ビッグドールの生成
     * @param oValue  オブジェクト情報
     */
    void AddBigDoll(ObjectValue oValue);
    /**
     * @brief         クロウドールの生成
     * @param oValue  オブジェクト情報
     */
    void AddCrowDoll(ObjectValue oValue);
    /**
     * @brief         レバーの生成
     * @param oValue  オブジェクト情報
     */
    void AddLever(ObjectValue oValue);
    /**
     * @brief         水晶の生成
     * @param oValue  オブジェクト情報
     */
    void AddCrystal(ObjectValue oValue);
    /**
     * @brief         壊れる岩の生成
     * @param oValue  オブジェクト情報
     */
    void AddBlock(ObjectValue oValue);
    /**
     * @brief         ドアの生成
     * @param oValue  オブジェクト情報
     */
    void AddDoor(ObjectValue oValue);
    /**
     * @brief         アイテムの生成
     * @param oValue  オブジェクト情報
     */
    void AddItem(ObjectValue oValue);
    /**
     * @brief         画像の生成・取得
     * @param pValue  画像情報
     * @return        生成したが画像を返す
     */
    std::unique_ptr<Particle_Image> AddImage(ImageValue pValue);
    /**
     * @brief         チュートリアル画像の生成
     * @param key     生成した場合はtrueを返す
     *                それ以外の場合はfalseを返す
     */
    bool AddTutorials(const std::string key);
    /**
     * @brief         オブジェクト情報の登録
     * @param key     ステージ
     * @param oValues オブジェクト情報
     */
    void LoadObjectData(std::string key, std::vector<ObjectValue> oValues);  // 情報読み込み
    /**
     * @brief         画像情報の登録
     */
    void LoadImageData(ImageMap iMap);
    /**
     * @brief         対象のキーは読み込んだか？
     * @param key     ステージ
     * @return        登録を行った場合はtrueを返す
     *                既に登録されている場合はfalseを返す
     */
    bool IsLoad(const std::string key);
    /**
     * @brief         連想配列の初期化
     */
    void ClearScenario();
  };
}