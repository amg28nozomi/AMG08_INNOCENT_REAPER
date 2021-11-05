/*****************************************************************//**
 * @file   SoldierDoll.h
 * @brief  ソルジャードール（エネミーベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
  /** エネミーのコンパイル時定数 */
  namespace enemy {
    /** ソルジャードールのコンパイル時定数 */
    namespace soldier {
      constexpr auto ATTACK_SE = "sd_attack_se";       //!< 切りつけSE
      constexpr auto ATTACK_VOICE = "sd_attack_voice"; //!< 切り付け声SE
      constexpr auto DOWN = "sd_down";                 //!< 倒れた際のSE
      constexpr auto ESCAPE_SE = "sd_escape_se";       //!< 逃避時のSE
      constexpr auto ESCAPE_VOICE = "sd_escape_voice"; //!< 逃避時ボイス
      constexpr auto MOVE = "sd_move";                 //!< 足音
    }
  }
  constexpr auto SOLDIER_IMAGE_W = 160;                //!< ソルジャードールの幅
  constexpr auto SOLDIER_IMAGE_H = 160;                //!< ソルジャードールの高さ
  /** ソルジャードール */
  class SoldierDoll : public EnemyBase {
  public:
    /*
     * @brief             コンストラクタ
     * @param game        ゲームクラスの参照
     */
    SoldierDoll(Game& game);
    /**
     * @brief             デストラクタ
     */
    ~SoldierDoll() override;
    /**
     * @brief             初期化処理
     */
    void Init() override;
    /**
     * @brief             更新処理
     */
    void Process() override;
    /**
     * @brief             オブジェクト情報の登録
     * @param objValue    オブジェクト情報
     */
    void SetParameter(ObjectValue objValue) override;
  private:
    /**
     * @brief             移動処理
     */
    void Move() override;
    /**
     * @brief             巡回処理
     */
    void Patrol();
    /**
     * @brief             巡回開始
     */
    void PatrolOn() override;
    /**
     * @brief             逃避処理
     */
    void Escape();
    /**
     * @brief             逃避開始
     */
    void EscapeOn() override;
    /**
     * @brief             攻撃開始
     */
    void AttackOn() override;
    /**
     * @brief             攻撃処理
     */
    void Attack();
    /**
     * @brief             待機状態の設定
     * @param stay        待機時間
     */
    void ChangeIdol(int stay) override;
    /**
     * @brief             座標等の更新
     */
    void PositionUpdate() override;
    /**
     * @brief             状態の切り替え
     */
    void StateUpdate();
    /**
     * @brief             アクション
     */
    void Action() override;
    /**
     * @brief             死亡処理
     */
    void Death() override;
    /**
     * @brief             効果音の停止処理
     * @return            trueを返す
     */
    bool StopSound();
    /**
     * @brief             現在の当たり判定の取得
     * @param key         現在の状態を表すキー
     * @return            抜け殻ではない場合は通常の当たり判定を返す
     *                    抜け殻の場合は抜け殻用の当たり判定を返す
     */
    AABB NowCollision(std::string key) override;
    /**
     * @brief             自機アクションボックスとの衝突判定
     * @param cKey        対象の現在の状態(キー)
     * @param aCollision  対象の当たり判定ボックス
     * @param direction   対象の向きフラグ
     */
    void CollisionHit(const std::string cKey, Collision aCollision, bool direction) override;
  };
}