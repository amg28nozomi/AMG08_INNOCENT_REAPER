/*****************************************************************//**
 * @file   UI.h
 * @brief  UI（画像クラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include <memory>
#include <vector>
#include <queue>

namespace inr {
  /** UIのコンパイル時定数 */
  namespace ui {
    constexpr auto KEY_HP = "player_my_soul";     //!< HPUI
    constexpr auto KEY_RED = "player_red_soul";   //!< 赤い魂
    constexpr auto KEY_BLUE = "player_blue_soul"; //!< 青い魂
    constexpr auto KEY_BOX = "player_soul_box";   //!< ボックス
    constexpr auto HP_NUM = 5;                    //!< HPUIの総分割数
    constexpr auto HP_SIZE = 200;                 //!< HPUIの画像サイズ
  }
  /** 二重インクルード防止 */
  class Player;
  class SoulSkin;
  /** UIクラス */
  class UI : public Image {
  public:
    /**
     * @brief        コンストラクタ
     * @param game   ゲームクラスの参照
     */
    UI(Game& game);
    /**
     * @brief        デストラクタ
     */
    ~UI() = default;
    /**
     * @brief        初期化処理
     */
    void Init() override;
    /**
     * @brief        更新処理
     */
    void Process() override;
    /**
     * @brief        描画処理
     */
    void Draw() override;
    /**
     * @brief        リセット
     */
    void Reset();
    /**
     * @brief        自機ポインタの取得
     */
    void PlayerUpdate();
  private:
    std::shared_ptr<Player> _player;               //!< 自機のポインタ
    std::queue<std::shared_ptr<SoulSkin>> _uiSoul; //!< 1フレーム前の情報
    std::vector<std::string> _ghKeys;              //!< 画像キー
    std::vector<int> _count;                       //!< アニメーションカウント
    std::vector<int> _pal;                         //!< 透明度
    std::vector<bool> _active;                     //!< 対象は活性化しているか？
    /**
     * @brief        画像キーの取得
     * @param number 魂の色
     * @return       対応した画像キーを返す
     */
    std::string GetGraphKey(int number);
    /**
     * @brief        グラフィックハンドルの取得
     * @param key    画像キー
     * @param count  現在のカウント
     * @return       取得したグラフィックを返す
     *               失敗した場合は-1を返す
     */
    int GraphHandle(const std::string key, int count);
    /**
     * @brief        魂のサーチ
     */
    void SoulResearch();
    /**
     * @brief        自機の魂保有数が変わったかの判定
     * @param value  魂フラグ
     * @return       前フレームとの差分を返す
     */
    int IsSoulChange(bool value);
    /**
     * @brief        UIの削除
     */
    void Dels();
    /**
     * @brief        カウンタの加算処理
     */
    void ActiveCount();
    /**
     * @brief        UI画像の更新
     */
    void GraphUpdata();
    /**
     * @brief        UIボックスの描画処理
     */
    void DrawEmptyBox();
  };
}