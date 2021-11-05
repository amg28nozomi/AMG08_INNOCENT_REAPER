/*****************************************************************//**
 * @file   ObjectValue.h
 * @brief  オブジェクト情報クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include "Vector2.h"
#include "GimmickValue.h"

namespace inr {
  /** オブジェクト情報のコンパイル時定数 */
  namespace oscenario {
    constexpr auto OBJ_NULL = -1;        //!< 該当なし
    constexpr auto OBJ_PLAYER = 0;       //!< 自機
    constexpr auto OBJ_SOLDIER_DOLL = 1; //!< ソルジャードール
    constexpr auto OBJ_BIG_DOLL = 2;     //!< ビッグドール
    constexpr auto OBJ_CROW_DOLL = 3;    //!< クロウドール
    constexpr auto OBJ_SOUL = 4;         //!< 魂
    constexpr auto OBJ_LEVER = 5;        //!< レバー
    constexpr auto OBJ_BLOCK = 6;        //!< 壊れる岩
    constexpr auto OBJ_CRYSTAL = 7;      //!< 水晶
    constexpr auto OBJ_DOOR = 8;         //!< ドア
    constexpr auto OBJ_ITEM = 9;         //!< アイテム
    /** オブジェクトタイプのコンパイル時定数 */
    namespace type {
      constexpr auto PLAYER = 0;         //!< 自機
      constexpr auto ENEMY = 1;          //!< 敵
      constexpr auto SOUL = 2;           //!< 魂
      constexpr auto GIMMICK = 3;        //!< ギミック
      constexpr auto ITEM = 4;           //!< アイテム
    }
  }
  /** オブジェクト情報 */
  class ObjectValue {
  public:
    /**
     * @brief  コンストラクタ
     */
    ObjectValue();
    /**
     * @brief           コンストラクタ
     * @param classType 生成するクラス
     * @param xy        生成地点
     * @param direction 向きフラグ
     * @param soulColor 魂の色(0:なし　1:赤　2:青)
     * @param gValue    ギミック情報
     */
    ObjectValue(int classType, Vector2 xy, bool direction = false, int soulColor = 0, GimmickValue gValue = GimmickValue());
    /**
     * @brief           コンストラクタ
     * @param classType 生成するクラス
     * @param xy        生成地点を格納したコンテナ
     * @param direction 向きフラグ
     * @param soulColor 魂の色(0:なし　1:赤　2:青)
     * @param gValue    ギミック情報
     */
    ObjectValue(int classType, std::vector<Vector2> xy, bool direction = false, int soulColor = 0, GimmickValue gValue = GimmickValue());
    /**
     * @brief           デストラクタ
     */
    ~ObjectValue() = default;
    /**
     * @brief           クラス番号の取得
     * @return          クラス番号を返す
     */
    inline int ClassName() { return _class; }
    /**
     * @brief           魂情報の取得
     * @return          魂情報を返す
     */
    inline int SoulType() { return _soulType; }
    /**
     * @brief           ギミックタイプの取得
     * @return          ギミックタイプを返す
     */
    inline int GimmickType() { return _gimmick.GimmickType(); }
    /**
     * @brief           ギミックフラグの取得
     * @return          ギミックフラグを返す
     */
    inline int GimmickFlag() { return _gimmick.GimmickFlag(); }
    /**
     * @brief           水晶タイプの取得
     * @return          水晶タイプを返す
     */
    inline int CrystalType() { return _gimmick.CrystalType(); }
    /**
     * @brief           オブジェクトタイプの取得
     * @return          オブジェクトタイプを返す
     */
    inline int ObjectType() { return _objType; }
    /**
     * @brief           向きフラグの取得
     * @return          向きフラグを返す
     */
    inline bool Direction() { return _direction; }
    /**
     * @brief           出現座標の取得
     * @return          出現座標を返す
     */
    inline std::vector<Vector2> Positions() { return _spawnPos; }
    /**
     * @brief           ドアタイプの取得
     * @return          ドアタイプを返す
     */
    inline std::vector<int> DoorType() { return _gimmick.DoorType(); }
    /**
     * @brief           座標の更新
     * @param newPos    更新後の座標
     */
    void PositionsUpdate(Vector2 newPos);
    /**
     * @brief           ギミックフラグと魂情報の更新
     * @param flag      フラグ
     * @param soul      魂
     */
    void FlagUpdate(int flag, int soul = 0);
    /**
     * @brief           魂状態の更新
     * @param soul      魂
     */
    inline void SoulUpdate(int soul) { _soulType = soul; }
    /**
     * @brief            向きフラグの更新
     * @param newDir     向き
     */
    inline void DirectionUpdate(bool newDir) { _direction = newDir; }
  private:
    int _class;                     //!< 生成するクラスは何か
    int _objType;                   //!< オブジェクトの種類
    int _soulType;                  //!< 魂の有無（0:未所持　1:赤　2:青）
    std::vector<Vector2> _spawnPos; //!< 出現地点
    bool _direction;                //!< 向き
    GimmickValue _gimmick;          //!< ギミックの登録情報
    /**
     * @brief           オブジェクトタイプの判定
     * @param className クラス番号
     * @return          オブジェクトタイプを返す
     */
    int IsObjectType(int className);
  };
}