/*****************************************************************//**
 * @file   RedGreenBlue.h
 * @brief  RedGreenBlueカラーモデルクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {
  /** RBGカラーモデルのコンパイル時定数 */
  namespace rgb {
    constexpr auto ADD = true;      //!< 加算
    constexpr auto SUB = false;     //!< 減算
    constexpr auto MIN_BLEND = 0;   //!< 下限
    constexpr auto MAX_BLEND = 255; //!< 上限
  }
  /** RedGreenBlueカラーモデルクラス */
  class RedGreenBlue {
  public:
    /**
     * @brief        コンストラクタ
     */
    RedGreenBlue();
    /**
     * @brief        コンストラクタ
     * @param  red   赤
     * @param  green 緑
     * @param  blue  青
     */
    RedGreenBlue(double red, double green, double blue);
    /**
     * @brief        デストラクタ
     */
    ~RedGreenBlue() = default;
    /**
     * @brief        RGB値(赤)の取得
     * @return       RGB値(赤)を返す
     */
    inline int Red() { return  static_cast<int>(_red); }
    /**
     * @brief        RGB値(緑)の取得
     * @return       RGB値(緑)を返す
     */
    inline int Green() { return static_cast<int>(_green); }
    /**
     * @brief        RGB値(青)の取得
     * @return       RGB値(青)を返す
     */
    inline int Blue() { return static_cast<int>(_blue); }
    /**
     * @brief        RGB値を下限に修正
     */
    void Min();
    /**
     * @brief        RGB値を上限に修正
     */
    void Max();
    /**
     * @brief        RGB値は上限に到達しているか？
     * @return       上限に到達している場合はtrueを返す
     *               到達していない場合はfalseを返す
     */
    inline bool IsBrendMax() { return _red == rgb::MAX_BLEND; }
    /**
     * @brief        RGB値の一括更新
     * @param type   処理タイプ
     * @param value  値
     */
    void Update(bool type, double value);
    /**
     * @brief        RGB値の更新
     * @param type   処理タイプ
     * @param rValue 赤の値
     * @param gValue 緑の値
     * @param bValue 青の値
     */
    void Update(bool type, double rValue, double gValue, double bValue);
  private:
    double _red;     //!< 輝度(赤)
    double _green;   //!< 輝度(緑)
    double _blue;    //!< 輝度(青)
    /**
     * @brief        指定したRGB値の修正
     * @param value  修正対象のポインタ
     */
    void SetBlend(double* value);
  };
}