/*****************************************************************//**
 * @file   Pause_UI.h
 * @brief  ポーズ用UIクラス（パーティクル画像のサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Particle_Image.h"

namespace inr {
  /** システムのコンパイル時定数 */
  namespace system {
    constexpr auto CURSOR_MOVE = "cursor_move"; //!< カーソル画像
    // 画像番号
    constexpr auto CONTINUE = 0;                //!< コンティニュー
    constexpr auto CONTROLS = 1;                //!< 操作説明
    constexpr auto QUIT_TO_TITLE = 2;           //!< タイトルに戻る
  }
  /** ポーズ用UI */
  class Pause_UI : public Particle_Image {
  public:
    /**
     * @brief      コンストラクタ
     * @param game ゲームクラスの参照
     */
    Pause_UI(Game& game);
    /**
     * @brief      デストラクタ
     */
    ~Pause_UI() = default;
    /**
     * @brief      初期化処理
     */
    void Init() override;
    /**
     * @brief      更新処理
     */
    void Process() override;
    /**
     * @brief      画像および座標の変更
     * @param type 上下入力情報
     */
    void ChangePosition(bool type) override;
    /**
     * @brief      移動フラグの取得
     * @return     移動フラグを返す
     */
    bool IsMove() { return _move; }
    /**
     * @brief      UI番号の取得  
     * @return     UI番号を返す
     */
    int UiNumber() { return _no; }
  private:
    int _no;       //!< UI番号
    int _upDown;   //!< 上下どちらに移動するか（1:上、-1:下）
    double _nextY; //!< 移動先
    bool _move;    //!< 移動中かどうか（TRUEの時のみ、移動処理受付）
  };
}