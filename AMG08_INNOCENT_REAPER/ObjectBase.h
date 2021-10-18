/*****************************************************************//**
 * @file   ObjectBase.h
 * @brief  オブジェクトのスーパークラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <DxLib.h>
#include "Collision.h"
#include "AABB.h"
#include "ResourceServer.h"
#include "ObjectValue.h"

/** 二重インクルード防止 */
class Vector2;

namespace {
	/** キー:状態キー　値(左辺):総モーションフレーム数　値(右辺):SEの再生フレーム */
	using KeyDatas = std::unordered_map<std::string, std::pair<int, int>>;
	/** 左辺:画像キー　右辺:サウンドキー */
	using Keys = std::pair<std::string, std::string>;
}
namespace inr {
	constexpr auto FRAME_G = 0.5;		//!< 1フレーム事の重力加速値
	constexpr auto MAX_G = 15.0;		//!< 重力値の上限
	constexpr auto SE_NUM = 0;			//!< SEフレーム無し
	/** 二重インクルード防止 */
	class Game;
	class ObjectServer;
	/** オブジェクトベース */
	class ObjectBase {
		/** フレンド指定 */
		friend class ObjectServer;
	protected:
		/**
		 * @brief				オブジェクトの種類を表す列挙型
		 *							オブジェクトベース　自機　敵　魂　ギミック
		 */
		enum class ObjectType {
			OBJECT_BASE, PLAYER, ENEMY, SOUL, GIMMICK
		};
		Game& _game;					//!< ゲーム参照
		ObjectType _type;			//!< オブジェクトタイプ
		int _aCount;					//!< アニメーションの添え字
		double _speed;				//!< 移動速度
		double _gravity;			//!< 重力による加速度
		bool _direction;			//!< 向きフラグ
		bool _changeGraph;		//!< 画像の切り替わりフラグ
		bool _stand;					//!< 地面に立っているかどうか
		bool _delete;					//!< デリートするか否か？（true：ObjectServerから消去　false:生存中）
		bool _isJump;					//!< ジャンプフラグ
		ObjectValue _oValue;	//!< 生成時の情報
		Vector2 _position;		//!< 座標
		Vector2 _lastChip;		//!< 最後に立っていたマップチップ
		AABB _mainCollision;	//!< キャラの当たり判定
		Keys _divKey;					//!< 左辺:グラフィック用、右辺:サウンド用
		KeyDatas _motionKey;	//!< first:キー, s.first:総フレーム数, SEの再生時間
		/**
		 * @brief							ResourceServerからグラフィックハンドル取得
		 * @param gh					int型のアドレス
		 * @return						画像切り替え処理がある場合はtrueを返す
		 *										通常の読み込みの場合はfalseを返す
		 */
		virtual bool GraphResearch(int* gh);
		/**
		 * @brief							マップチップの上に立っているか？
		 * @return						立っている場合はtrueを返す
		 *										立っていない場合はfalseを返す
		 */
		virtual bool IsStandChip();
		/**
		 * @brief							総モーションフレーム数の取得
		 * @param key					状態キー
		 * @return						ヒットした場合は総フレーム数を返す
		 *										ヒットしなかった場合は-1を返す
		 */
		virtual int GetSize(const std::string& key);
		/**
		 * @brief							アニメーションカウンタの増減
		 */
		virtual void AnimationCount();
		/**
		 * @brief							アニメーションの描画間隔
		 * @return						間隔を返す
		 */
		int AnimationInterval();
		/**
		 * @brief							現在のアニメーション番号の取得
		 * @return						アニメーション番号を返す
		 */
		int AnimationNumber();
		/**
		 * @brief							アニメーション番号は最大か
		 * @return						最大の場合はtrueを返す
		 *										最大ではない場合はfalseを返す
		 */
		bool IsAnimationMax();
		/**
		 * @brief							アニメーションカウントは最大か
		 * @return						最大の場合はtrueを返す
		 *										最大ではない場合はfalseを返す
		 */
		bool AnimationCountMax();
		/**
		 * @brief							SoundServerからサウンドハンドルを取得する
		 * @param key					キー
		 * @return						取得したサウンドハンドルを返す
		 *										取得に失敗した場合は-1を返す
		 */
		virtual int SoundResearch(const std::string& key);
		/**
		 * @brief							再生フレーム数の取得
		 * @param key					キー
		 * @return						ヒットした場合は再生フレーム数を返す
		 *										ヒットしなかった場合は-1を返す
		 */
		virtual int GetSoundFrame(const std::string& key);
		/**
		 * @brief							現在の当たり判定の取得
		 * @param	key					現在の状態を表すキー
		 * @return						空の当たり判定ボックスを返す
		 */
		virtual AABB NowCollision(std::string key);
		/**
		 * @brief							このオブジェクトは自機かどうか
		 * @return						自機の場合はtrueを返す
		 *										自機ではない場合はfalseを返す
		 */
		bool ThisPlayer();
		/**
		 * @brief							ギミックとの衝突判定
		 * @param move				移動両
		 */
		void GimmickCheck(Vector2& move);
#ifdef _DEBUG
		/**
		 * @brief							当たり判定ボックスの描画(デバッグモード限定)
		 * @param cb					描画する判定ボックス
		 * @param color				色
		 */
		void DrawDebugBox(Collision cb, int color = GetColor(255, 0, 255));
#endif
	public:
		/**
		 * @brief							コンストラクタ
		 * @param game				ゲームクラスの参照
		 */
		ObjectBase(Game& game);
		/**
		 * @brief							デストラクタ
		 */
		virtual ~ObjectBase();
		/**
		 * @brief							初期化処理
		 */
		virtual void Init();
		/**
		 * @brief							更新処理
		 */
		virtual void Process();
		/**
		 * @brief							描画処理
		 */
		virtual void Draw();
		/**
		 * @brief							向きの取得
		 * @return						向きフラグを返す
		 */
		inline bool GetDirection() { return _direction; }
		/**
		 * @brief							消去フラグの取得
		 * @return						消去フラグを返す
		 */
		inline bool IsDelete() { return _delete; }
		/**
		 * @brief							オブジェクトタイプの取得
		 * @return						オブジェクトタイプを返す
		 */
		inline ObjectType GetType() { return _type; }
		/**
		 * @brief							座標の取得
		 * @return						座標を返す
		 */
		inline Vector2 GetPosition() { return _position; }
		/**
		 * @brief							移動量の取得
		 * @return						空の移動ベクトルを返す
		 */
		inline virtual Vector2 GetMoveVector() { return Vector2(); }
		/**
		 * @brief							通常当たり判定ボックスの取得
		 * @return						通常当たり判定ボックスを返す
		 */
		inline AABB& GetMainCollision() { return _mainCollision; }
		/**
		 * @brief							オブジェクト情報の取得
		 * @return						オブジェクト情報を返す
		 */
		inline ObjectValue GetObjectValue() { return _oValue; }
		/**
		 * @brief							死亡判定
		 * @return						falseを返す
		 */
		virtual bool IsDead();
		/**
		 * @brief							オブジェクト情報の登録
		 * @param ObjectValue オブジェクト情報
		 */
		virtual void SetParameter(ObjectValue objValue);
		/**
		 * @brief							状態の設定
		 * @param spawn				生成座標
		 * @param soulcolor		魂の色
		 */
		virtual void SetStatus(Vector2 spawn, std::string soulcolor) { return; }
		/**
		 * @brief							移動速度の設定
		 * @param speed				移動速度
		 */
		virtual void SetSpeed(double speed) { return; }
		/**
		 * @brief							消去フラグの変更
		 */
		void Del();
		/** 当たり判定を格納する連想配列(キー:状態　値:当たり判定ボックス) */
		using boxs = std::unordered_map<std::string, AABB>;
	};
}
