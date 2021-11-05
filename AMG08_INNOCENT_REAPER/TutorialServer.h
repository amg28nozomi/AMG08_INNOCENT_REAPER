/*****************************************************************//**
 * @file   TutorialServer.h
 * @brief  チュートリアル画像を管理するサーバ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
  /** チュートリアル情報 */
  class TutorialImage;
  /** チュートリアルサーバ */
  class TutorialServer {
  public:
    /**
     * @brief        コンストラクタ
     */
    TutorialServer();
    /**
     * @brief        デストラクタ
     */
    ~TutorialServer();
    /**
     * @brief        更新処理
     */
    void Process();
    /**
     * @brief        描画処理
     * 
     */
    void Draw();
    /**
     * @brief        コンテナの解放
     * @return       trueを返す
     */
    bool Clear();
    /**
     * @brief        チュートリアル画像の登録
     * @param tImage チュートリアル画像
     */
    void Add(std::unique_ptr<TutorialImage> tImage);
  private:
    bool _isActive;                                      //!< 活動状態フラグ
    std::vector<std::unique_ptr<TutorialImage>> _images; //!< 画像用コンテナ
  };
}