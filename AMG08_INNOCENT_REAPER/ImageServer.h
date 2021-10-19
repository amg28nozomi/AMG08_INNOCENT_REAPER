/*****************************************************************//**
 * @file   ImageServer.h
 * @brief  イメージサーバクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ImageValue.h"

namespace inr {
	/** 画像のコンパイル時定数 */
	namespace image {
		/**   テキスト検索用キー */
		namespace number {
			constexpr auto NUM = -1;	//!< テキスト画像なし
		}
	}
	/** 二重インクルード防止 */
	class Particle_Image;
	class ImageValue;
	class Game;
	/** イメージサーバ */
	class ImageServer {
	public:
		/**
		 * @brief					コンストラクタ
		 * @param game		ゲームクラスの参照
		 */
		ImageServer(Game& game);
		/**
		 * @brief					デストラクタ
		 */
		~ImageServer();
		/**
		 * @brief					初期化処理
		 * @return				trueを返す
		 */
		bool Init();
		/**
		 * @brief					更新処理
		 * @return				更新処理を完了した場合はtrueを返す
		 *								中断した場合はfalseを返す
		 */
		bool Process();
		/**
		 * @brief					描画処理
		 * @return				描画処理を完了した場合はtrueを返す
		 *								コンテナが空の場合はfalseを返す
		 */
		bool Draw();
		/**
		 * @brief					描画画像の切り替え
		 * @param nextKey	次に描画する画像の登録キー
		 * @return				切り替えに成功した場合はtrueを返す
		 *								失敗した場合はfalseを返す
		 */
		bool ImageChange(const int nextKey);
		/**
		 * @brief					登録処理を行うかの判定
		 * @param number	登録番号
		 * @return				登録処理を行う場合はtrueを返す
		 *								既に登録されている場合はfalseを返す
		 */
		bool IsResister(const int number);
		/**
		 * @brief					画像の登録
		 * @param number	登録番号
		 * @param image		画像のシェアードポインタ
		 * @return				trueを返す
		 */
		bool AddImage(const int number, std::shared_ptr<Particle_Image> image);
		/**
		 * @brief					画像の読み込み処理を行うか
		 * @return				コンテナが空の場合はtrueを返す
		 *								要素がある場合はfalseを返す
		 */
		bool IsLoad() { return _images.empty(); }
		/**
		 * @brief					活動状態の判定・移行処理
		 * @return				活動状態に移行した場合はtrueを返す
		 *								失敗した場合はfalseを返す
		 */
		bool IsActive();
		/**
		 * @brief					活動状態の取得
		 * @return				活動フラグを返す
		 */
		bool Active() { return _active; }
	private:
		Game& _game;																											//!< ゲームクラスの参照
		int _imageKey;																										//!< 検索用キー
		int _changeKey;																										//!< 切り替え用キー
		bool _active;																											//!< 処理を実行するかどうか
		bool _input;																											//!< 入力処理を受け付けるか否か
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;	//!< 画像を管理するための連想配列
		/**
		 * @brief					画像の開放
		 * @return				trueを返す
		 */
		bool ImageClear();
		/**
		 * @brief					画像の初期化処理呼び出し
		 * @return				初期化に成功した場合はtrueを返す
		 *								失敗した場合はfalseを返す
		 */
		bool ImageInit();
		/**
		 * @brief					検索用キーの切り替え
		 * @return				切り替えに成功した場合はtrueを返す
		 *								失敗した場合はfalseを返す
		 */
		bool ChangeKey();
		/**
		 * @brief					入力処理
		 * @return				処理に成功した場合はtrueを返す
		 *								失敗した場合はfalseを返す
		 */
		bool Input();
	};
}

