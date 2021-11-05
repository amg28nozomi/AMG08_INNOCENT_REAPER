/*****************************************************************//**
 * @file   Transition.h
 * @brief  ステージ遷移情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {
  /** ステージ遷移情報 */
  class Transition {
  public:
    /**
     * @brief           コンストラクタ
     * @param stageKey  遷移ステージキー
     * @param position  遷移後の座標
     * @param direction 遷移後の向き
     */
    Transition(std::string stageKey, Vector2 position, bool direction);
    /**
     * @brief           デストラクタ
     */
    ~Transition() = default;
    /**
     * @brief           ステージキーの取得
     * @return          ステージキーを返す
     */
    inline std::string NextKey() { return _nextKey; }
    /**
     * @brief           座標の取得
     * @return          座標を返す
     */
    inline Vector2 NextPosition() { return _position; }
    /**
     * @brief           向きの取得
     * @return          向きを返す
     */
    inline bool NextDirection() { return _direction; }
  private:
    std::string _nextKey; //!< 遷移先のキー
    Vector2 _position;    //!< 遷移後の座標(自機)
    bool _direction;      //!< 向き
  };
}