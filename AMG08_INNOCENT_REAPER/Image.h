/*****************************************************************//**
 * @file   Image.h
 * @brief  画像のスーパークラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {
  /** 背景のコンパイル時定数 */
  namespace background {
    // 背景素材の各種情報
    // 封印の祭壇
    constexpr auto BACK_GROUND_S = "back_groundS"; //!< 封印の祭壇の画像キー
    constexpr auto ALTAR = "back_altar";           //!< 封印の祭壇(前景)の画像キー
    constexpr auto STAGES_HEIGHT = 2160;           //!< 封印の祭壇のサイズ
    // 荒れた町
    constexpr auto BACK_GROUND_1 = "back_ground1"; //!< 荒れた町の画像キー
    constexpr auto REMAINS = "back_remains";       //!< 荒れた町(前景)の画像キー
    constexpr auto STAGE1_MAP_HEIGHT = 2160;       //!< 荒れた町のサイズ
    constexpr auto REMAINS_WIDTH = 600;            //!< 荒れた町(前景)の幅
    // 契りの森
    constexpr auto BACK_GROUND_2 = "back_ground2"; //!< 契りの森画像キー
    constexpr auto STAGE2_MAP_HEIGHT = 2160;       //!< 契りの森のサイズ
    // ボスステージ
    constexpr auto BACK_GROUND_B = "back_groundB"; //!< ボスステージの画像キー
  }
  /** エンディングのコンパイル時定数 */
  namespace end {
    // スタッフロール
    constexpr auto STAFF_ROLL = "staff_roll";      //!< スタッフロールの画像キー    
    constexpr auto STAFF_ROLL_WIDTH = 1920;        //!< スタッフロールの幅
    constexpr auto STAFF_ROLL_HEIGHT = 11880;      //!< スタッフロールの高さ
  }
  // アニメーション素材
  // タイトルロゴ
  constexpr auto TITLE_LOGO = "title_rogo";        //!< タイトルロゴの画像キー
  constexpr auto TITLE_IMAGE_W = 1920;             //!< タイトルロゴの幅
  constexpr auto TITLE_IMAGE_H = 1080;             //!< タイトルロゴの高さ
  // チームロゴ
  constexpr auto TEAM_LOGO = "team_logo";          //!< チームロゴの画像キー
  constexpr auto TEAM_IMAGE = 140;                 //!< チームロゴの画像サイズ
  // タイトル背景
  constexpr auto TITLE_BG = "title_back_ground";   //!< タイトル背景の画像キー
  constexpr auto TITLE_IMAGE_WIDTH = 1920;         //!< タイトル背景の幅
  constexpr auto TITLE_IMAGE_HEIGHT = 1080;        //!< タイトル背景の高さ
  // 各種タイトルUIの画像キー
  constexpr auto TITLE_START1 = "ui_start1";       //!< スタートUI(1)の画像キー
  constexpr auto TITLE_START2 = "ui_start2";       //!< スタートUI(2)の画像キー
  constexpr auto TITLE_EXIT1 = "ui_exit1";         //!< 終了UI(1)の画像キー
  constexpr auto TITLE_EXIT2 = "ui_exit2";         //!< 終了UI(2)の画像キー
  // タイトルUIのサイズ
  constexpr auto TITLE_UI_HEIGHT = 166;            //!< タイトルUIの高さ
  constexpr auto TITLE_START_WIDTH = 372;          //!< スタートUIの幅
  constexpr auto TITLE_EXIT_WIDTH = 298;           //!< 終了UIの幅
  /** 二重インクルード防止 */
  class Game;
  /** 画像のスーパークラス */
  class Image {
  protected:
    Game& _game;           //!<　ゲーム参照
    Vector2 _pos;          //!< 座標
    std::string _graphKey; //!< 画像キー
  public:
    /**
     * @brief      コンストラクタ
     * @param game ゲームクラスの参照
     */
    Image(Game& game);
    /**
     * @brief      デストラクタ
     */
    ~Image() = default;
    /**
     * @brief      初期化処理
     */
    virtual void Init();
    /**
     * @brief      更新処理
     */
    virtual void Process();
    /**
     * @brief      描画処理
     */
    virtual void Draw();
    /**
     * @brief      フラグの起動
     */
    virtual void FlagOn();
  };
}