/*****************************************************************//**
 * @file   SoundServer.h
 * @brief  サウンドの管理を行うサウンドサーバ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "SoundData.h"
#include <unordered_map>
#include <string>

namespace se {
  /** サウンドサーバ */
  class SoundServer {
  public:
    /** サウンド情報を格納する連想配列 */
    using SoundMap = std::unordered_map<std::string, SoundData>;
    /**
     * @brief          初期化処理
     */
    static void Init();
    /**
     * @brief          サウンドハンドルの解放
     */
    static void Release();
    /**
     * @brief          コンテナの解放
     */
    static void ClearSoundList();
    /**
     * @brief          サウンドの読み込み
     * @param soundMap サウンド情報
     */
    static void LoadSoundMap(const SoundMap& soundMap);
    /**
     * @brief          サウンドハンドルの取得
     * @param key      キー
     * @return         サウンドハンドルを返す
     *                 キーが登録されていない場合は-1を返す
     */
    static int GetSound(const std::string& key);
    /**
     * @brief          再生タイプの取得
     * @param key      キー
     * @return         再生タイプを返す
     *                 キーが登録されていない場合は-1を返す
     *                  
     */
    static int GetPlayType(const std::string& key);
    /**
     * @brief          サウンドの取得
     * @param key      キー
     * @param playType 再生タイプ
     * @return         サウンドハンドルを返す
     *                 キーが登録されていない場合は-1を返す
     */
    static int GetSound(const std::string& key, int* playType);
  private:
    static SoundMap _soundList; //!< サウンド情報
  };
}