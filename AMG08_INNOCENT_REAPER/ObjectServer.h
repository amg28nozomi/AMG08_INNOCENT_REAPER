/*****************************************************************//**
 * @file   ObjectServer.h
 * @brief  オブジェクトの管理を行うオブジェクトサーバ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ObjectBase.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"

namespace inr{
  /** 二重インクルード防止 */
  class EnemyBase;
  class CrowDoll;
  class GimmickBase;
  /** オブジェクトサーバ */
  class ObjectServer {
  public:
    /**
     * @brief     コンストラクタ
     */
    ObjectServer();
    /**
     * @brief     デストラクタ
     */
    ~ObjectServer();
    /**
     * @brief     コンテナの初期化
     */
    void Clear();
    /**
     * @brief     オブジェクトの登録
     * @param obj オブジェクトのシェアードポインタ
     */
    void Add(std::shared_ptr<ObjectBase> obj);
    /**
     * @brief     更新処理
     */
    void Process();
    /**
     * @brief     描画処理
     */
    void Draw();
    /**
     * @brief     オブジェクトの開放処理
     */
    void DeleteObject();
    /**
     * @brief     自機の取得
     * @return    自機のシェアードポインタを返す
     */
    std::shared_ptr<Player> GetPlayer();
    /**
     * @brief     全オブジェクトの取得
     * @return    全オブジェクトを返す
     */
    std::vector<std::shared_ptr<ObjectBase>> GetObjects() { return _objects; }
    /**
     * @brief     全エネミーの取得
     * @return    全エネミーを返す
     */
    std::vector<std::shared_ptr<EnemyBase>> GetEnemys();
    /**
     * @brief     自機は生成されているか？
     * @return    生成されている場合はtrueを返す
     *            生成されていない場合はfalseを返す
     */
    bool IsPlayer();
    /**
     * @brief     自機のオブジェクトを削除
     */
    void ObjectsClear();
    /**
     * @brief     全オブジェクトの解放
     */
    inline void AllClear() { _objects.clear(); }
    std::shared_ptr<SoulSkin> GetSoul();
    /**
     * @brief     消去処理フラグの起動
     */
    inline void DelOn() { _delete = true; }
    /**
     * @brief     消去フラグの取得
     * @return    消去フラグを返す
     */
    inline bool DelFlag() { return _delete; }
  private:
    bool _updateFlg;                                   //!< 更新があるか
    bool _delete;                                      //!< 消去フラグ
    std::vector<std::shared_ptr<ObjectBase>> _objects; //!< オブジェクト格納用コンテナ
    std::vector<std::shared_ptr<ObjectBase>> _addObj;  //!< 登録予約
    std::vector<std::shared_ptr<ObjectBase>> _delObj;  //!< 消去予約
  };
}