/*****************************************************************//**
 * @file   StageUi.h
 * @brief  ステージUI（パーティクル画像のサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Particle_Image.h"
#include <vector>

namespace inr {
  /** ステージUI */
  class StageUi : public Particle_Image {
  public:
    /**
     * @brief          コンストラクタ
     * @param game     ゲームクラスの参照
     */
    StageUi(Game& game);
    /**
     * @brief          デストラクタ
     */
    ~StageUi() = default;
    /**
     * @brief          初期化処理
     */
    void Init() override;
    /**
     * @brief          更新処理
     */
    void Process() override;
    /**
     * @brief          描画処理
     */
    void Draw() override;
    /**
     * @brief          UI情報の登録
     * @param graph    画像キー
     * @param pos      座標
     * @param pal      透明度
     */
    void SetParameter(std::string graph, Vector2 pos, int pal = 0);
    /**
     * @brief          描画アイコンの切り替え
     * @param stageKey ステージ
     * @return         アイコン番号を返す
     */
    bool ChangeNumber(const std::string stageKey);
    /**
     * @brief          待ち時間のカウント処理
     * @return         減算処理に切り替えた場合はtrueを返す
     *                 それ以外の場合はfalseを返す
     */
    bool IntervalCount();
    /**
     * @brief          描画の開始
     * @return         開始に成功した場合はtrueを返す
     *                 失敗した場合はfalseを返す
     */
    bool DrawStart() override;
    /**
     * @brief          描画処理フラグの取得
     * @return         描画処理フラグを返す
     */
    inline bool FadeDraw() { return _fadeDraw; }
  private:
    int _gNumber;      //!< 何番目の画像を描画するか
    int _alive;        //!< 生存フレーム
    bool _fadeDraw;    //!< 描画処理を実行したか
  };
}