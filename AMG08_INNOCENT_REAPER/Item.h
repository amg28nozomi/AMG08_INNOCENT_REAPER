/*****************************************************************//**
 * @file   Item.h
 * @brief  アイテムクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>
#include "Particle_Image.h"
#include "Collision.h"
#include "ObjectValue.h"

namespace inr {
  /** アイテムのコンパイル時定数 */
  namespace item {
    // アイテム
    constexpr auto ITEM = "item_light"; //!< キー
    constexpr auto ITEM_IMAGE = 150;    //!< アイテムのサイズ
  }
  /** 二重インクルード防止 */
  class Game;
  /** アイテム */
  class Item {
  public:
    /**
     * @brief        コンストラクタ
     * @param game   ゲームクラスの参照
     */
    Item(Game& game);
    /**
     * @brief        デストラクタ
     */
    ~Item() = default;
    /**
     * @brief        初期化処理
     */
    void Init();
    /**
     * @brief        更新処理
     */
    void Process();
    /**
     * @brief        描画処理
     */
    void Draw();
    /**
     * @brief        オブジェクト情報の登録
     * @param ovalue オブジェクト情報
     */
    void SetParameter(ObjectValue ovalue);
    /**
     * @brief        オブジェクト情報の更新
     */
    void ObjValueUpdate();
    /**
     * @brief        入手フラグの取得
     * @return       入手フラグを返す
     */
    inline bool IsGet() { return _isGet; }
    /**
     * @brief        消去フラグの取得
     * @return       消去フラグを返す
     */
    inline bool IsDel() { return _del; }
    /**
     * @brief        オブジェクト情報の取得
     * @return       オブジェクト情報を返す
     */
    inline ObjectValue GetObjectValue() { return _oValue; }
  private:
    Game& _game;         //!< ゲームクラスの参照
    ObjectValue _oValue; //!< オブジェクト情報
    Collision _col;      //!< 当たり判定
    std::string _gkey;   //!< 画像キー
    Vector2 _position;   //!< 座標
    int _count;          //!< カウンタ
    int _maxFrame;       //!< アニメーションの描画フレーム数
    int _messageNo;      //!<　対応しているmessage番号
    bool _isGet;         //!< すでに入手されているか？
    bool _del;           //!< 消去フラグ
  };
}