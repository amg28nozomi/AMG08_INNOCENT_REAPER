/*****************************************************************//**
 * @file   Lever.h
 * @brief  レバークラス（ギミックベースクラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
  /** ギミックのコンパイル時定数 */
  namespace gimmick {
    /** ドアのコンパイル時定数 */
    namespace door {
      // ドア番号
      constexpr auto NOT_DOOR = -1; //!< 該当なし
      constexpr auto D_LEVER = 1;   //!< レバー
      constexpr auto D_RED = 2;     //!< 赤
      constexpr auto D_BLUE = 3;    //!< 青
      constexpr auto D_BOSS = 4;    //!< ボス
    }
  }
  /** 二重インクルード防止 */
  class Door;
  /** レバー */
  class Lever : public GimmickBase {
  public:
    /**
     * @brief          コンストラクタ
     * @param game     ゲームクラスの参照
     */
    Lever(Game& game);
    /**
     * @brief          デストラクタ
     */
    ~Lever() = default;
    /**
     * @brief          更新処理
     */
    void Process() override;
    /**
     * @brief          描画処理
     */
    void Draw() override;
    /**
     * @brief          ドアの開放処理
     */
    void OpenDoor();
    /**
     * @brief          オブジェクト情報の登録
     * @param objValue オブジェクト情報
     */
    void SetParameter(ObjectValue objValue) override;
    /**
     * @brief          オブジェクト情報の更新
     */
    void ObjValueUpdate() override;
  private:
    std::shared_ptr<Door> _door; //!< 扉
  };
}