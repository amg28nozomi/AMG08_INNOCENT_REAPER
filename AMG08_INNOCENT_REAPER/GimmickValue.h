/*****************************************************************//**
 * @file   GimmickValue.h
 * @brief   ギミック情報クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>

namespace inr {
  /** オブジェクト情報のコンパイル時定数 */
  namespace oscenario {
    /** ギミックのコンパイル時定数 */
    namespace gimmick {
      constexpr auto TYPE_NULL = -1;     //!< 該当なし
      constexpr auto TYPE_LEVER = 0;     //!< レバー
      constexpr auto TYPE_CRYSTAL = 1;   //!< 水晶
      constexpr auto TYPE_BLOCK = 2;     //!< 壊れる岩
      constexpr auto TYPE_DOOR = 3;      //!< ドア
      // ギミックフラグ
      constexpr auto FLAG_NULL = -1;     //!< NULL
      constexpr auto FLAG_FALSE = 0;     //!< ギミックフラグ(偽)
      constexpr auto FLAG_TRUE = 1;      //!< ギミックフラグ(真)
      /** 水晶のコンパイル時定数 */
      namespace crystal {
        constexpr auto TYPE_NULL = -1;   //!< 該当なし
        constexpr auto TYPE_STAGE_0 = 0; //!< ステージ0用
        constexpr auto TYPE_STAGE_2 = 1; //!< ステージ2用
        // ドア
        constexpr auto DOOR_NULL = -1;   //!< 該当なし
        constexpr auto DOOR_RED = 0;     //!< 赤ドア
        constexpr auto DOOR_BLUE = 1;    //!< 青ドア
      }
    }
  }
  /** ギミックの情報 */
  class GimmickValue {
  public:
    /**
     * @brief        コンストラクタ
     * @param  type  ギミックの種類
     * @param  flag  ギミックのフラグ
     * @param  cType 水晶の種類
     * @param  types ドアの種類
     */
    GimmickValue(int type = oscenario::gimmick::TYPE_NULL, int flag = oscenario::gimmick::FLAG_NULL, int cType = oscenario::gimmick::crystal::TYPE_NULL, std::vector<int> types = { oscenario::gimmick::crystal::DOOR_NULL });
    /**
     * @brief        ギミックタイプの取得
     * @return       ギミックタイプを返す
     */
    inline int GimmickType() { return _gimmickType; }
    /**
     * @brief        ギミックフラグの取得
     * @return       ギミックフラグを返す
     */
    inline int GimmickFlag() { return _gimmickFlag; }
    /**
     * @brief        水晶タイプの取得
     * @return       水晶タイプを返す
     */
    inline int CrystalType() { return _crystalType; }
    /**
     * @brief        ドアの取得
     * @return       ドアを返す
     */
    inline std::vector<int> DoorType() { return _dtype; }
    /**
     * @brief        ギミックフラグの更新
     * @param flag   フラグ
     */
    inline void AddFlag(int flag) { _gimmickFlag = flag; }
  private:
    int _gimmickType;        //!< ギミックの種類（-1:対象外　0:レバー　1:水晶　2:　3:ドア）
    int _gimmickFlag;        //!< ギミックのフラグ（-1:対象外　0:false　1:true）
    int _crystalType;        //!< クリスタルの種類(-1:対象外、0:ステージ0、1:ステージ2)
    std::vector<int> _dtype; //!< ドアの種類
  };
}