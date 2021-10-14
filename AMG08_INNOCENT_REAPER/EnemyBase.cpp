/*****************************************************************//**
 * \file   EnemyBase.cpp
 * \brief  敵のスーパークラス（オブジェクトベースのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "SoulSkin.h"
#include "ChipNumber.h"

using std::string;

namespace inr {
	// コンストラクタ
	EnemyBase::EnemyBase(Game& game) : ObjectBase(game), _searchBox(Vector2(), Vector2()) {
		// 各種パラメータの初期化
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair("NULL", key::SOUND_NUM);
		_position = {};
		_moveVector = {};
		_actionX = 0;
		_patrolX = 0;
		_aInterval = 0;
		_aFrame = 0;
		_aCount = 0;
		_stay = 0;
		_direction = false;
		_changeGraph = true;
		_drawStop = false;
		_isAction = false;
		_soul = nullptr;
		Init();
	}
	// デストラクタ
	EnemyBase::~EnemyBase() {
	}
	// 初期化
	void EnemyBase::Init() {
	}
	// 描画
	void EnemyBase::Draw() {
		// 描画座標の算出
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// 画像の読み込みおよび描画
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
		// 当たり判定の描画
		DrawDebugBox(_searchBox);
		if (_mainCollision.GetCollisionFlag() == true) DrawDebugBox(_mainCollision);
#endif
	}
	// アニメーションカウンタの制御
	void EnemyBase::AnimationCount() {
		// 魂が空かつ、アニメーション番号が最大の場合は処理を終了
		if (_soul == nullptr && IsAnimationMax() == true) return;
		ObjectBase::AnimationCount();	// カウント処理呼び出し
	}
	// 索敵処理
	bool EnemyBase::SearchPlayer() {
		// 魂が空ではない時
		if (_soul != nullptr) {
			// プレイヤークラスの参照からメインの当たり判定を取得
			auto player = _game.GetObjectServer()->GetPlayer();
			const auto& pbox = player->GetMainCollision();
			// 索敵範囲と自機は衝突しているか？
			if (_searchBox.HitCheck(pbox)) {
				// 自機は自分と同じ方向にいるか？
				if (_direction == SearchPosition()) {
					return true;	// 発見した
				}
			}
		}
		return false;				// 発見できなかった
	}
	// プレイヤーが左右どちらにいるか判定
	bool EnemyBase::SearchPosition() {
		auto pl = _game.GetObjectServer()->GetPlayer();
		auto px = pl->GetPosition().GetX();		// 自機のx座標
		auto myx = _position.GetX();			// 自身のx座標
		// 自身のx座標から自機のx座標を引き、左右どちらにいるかを算出する
		auto way = myx - px;
		if (way < 0) return false;	// 右側に居る
		return true;				// 左側に居る
	}
	// 座標等の更新
	void EnemyBase::PositionUpdate() {
	}
	// 逃走状態への遷移
	void EnemyBase::EscapeOn() {
	}
	// 巡回状態への遷移処理
	void EnemyBase::PatrolOn() {
		_searchBox.SetCollisionFlag() = true;	// 索敵処理の再開
		_changeGraph = true;					
		_aState = ActionState::PATROL;			// 巡回状態に遷移
	}
	// 攻撃状態への遷移処理
	void EnemyBase::AttackOn() {
	}
	// 状態遷移
	void EnemyBase::ChangeState(ActionState nextstate, std::string key) {
		// 各種パラメータの切り替え
		_changeGraph = true;
		_aState = nextstate;
		_divKey.first = key;
	}
	// 待機状態への調整
	void EnemyBase::ChangeIdol(int stay) {
	}
	// オブジェクト情報の登録
	void EnemyBase::SetParameter(ObjectValue objValue) {
		_oValue = objValue;		// オブジェクト情報の登録
		// 各種パラメータの登録
		_position = _oValue.Positions()[0];
		_direction = _oValue.Direction();
		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);
	}
	// 自機アクションとの衝突判定
	void EnemyBase::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
	}
	// 抜け殻かどうか
	bool EnemyBase::IsEmpty() {
		if (_soul == nullptr) return true;	// 抜け殻
		return false;						// 抜け殻ではない
	}
	// 死亡処理
	void EnemyBase::Death() {
		_soul->SetSpwan(_position);	// 自身の座標に魂を実体化する
		_soul->OwnerNull();			// 所有者の初期化
		_soul.reset();				// 魂の所有権を手放す
	}
	// アクション状態に移行するか
	void EnemyBase::Action() {
		// 条件を満たしてないため終了
		if (_aState == ActionState::WAKEUP || _isAction == true) return;
		// 自機を発見した場合はアクション状態に移行
		if (SearchPlayer() == true) {
			_isAction = true;
		// 魂が空の場合はアクション移動量を0にする
		} else if (_soul == nullptr) _actionX = 0;
	}
	// 急所の生成(干渉可能範囲の算出)
	AABB EnemyBase::VitalPart(Collision& col, int vital) {
		// 座標を算出
		Vector2 vitalMin(0.0, col.GetMin().GetY());
		Vector2 vitalMax(0.0, col.GetMax().GetY());
		// 向きに応じてx座標に修正を加える
		switch (_direction) {
		case enemy::MOVE_LEFT:
			vitalMin.GetPX() = col.GetMax().GetX() - vital;
			vitalMax.GetPX() = col.GetMax().GetX();
			break;
		case enemy::MOVE_RIGHT:
			vitalMin.GetPX() = col.GetMin().GetX();
			vitalMax.GetPX() = col.GetMin().GetX() + vital;
			break;
		}
		// 当たりの生成
		return AABB(vitalMin, vitalMax, true);
	}
	// 攻撃範囲の算出
	AABB EnemyBase::DamageBox(int fix) {
		// ベクトル作成
		auto damageMin(_mainCollision.GetMin());
		auto damageMax(_mainCollision.GetMax());
		// 向きに応じて範囲修正
		switch (_direction) {
		case enemy::MOVE_LEFT:
			damageMax.GetPX() -= fix;
			break;
		case enemy::MOVE_RIGHT:
			damageMin.GetPX() += fix;
			break;
		}
		// 当たり判定の修正
		return AABB(damageMin, damageMax, _mainCollision.GetCollisionFlag());
	}
	// 立っているマップチップとの判定
	bool EnemyBase::IsStandChip() {
		auto nowcol = NowCollision(_divKey.first);	// 現在の当たり判定を取得
		// 接触しているマップチップを判別する
		auto chipType = _game.GetMapChips()->IsStand(nowcol, _position, _gravity, &_lastChip);
		switch (chipType) {
			// 蔦 or 通常チップの場合
		case mapchip::IVY:
		case mapchip::NONE:
			return false;	// 効果無し
		case mapchip::THORM:
			if (_soul != nullptr) Death();	// 魂がある場合は死亡処理を呼び出し
			return true;	// 効果有り
		default:
			return true;
		}
	}
	// 効果音の再生
	bool EnemyBase::PlaySe(const std::string key) {
		auto sound = SoundResearch(key);	// ハンドルの取得
		if (sound == -1) return false;		// ハンドルの読み込みに失敗した場合は終了する
		// 音源を再生する
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
		return true;
	}
	// 抜け殻時の当たり判定
	AABB EnemyBase::EmptyBox() {
		return NowCollision(_divKey.first);
	}
	// 保持している魂の生成
	bool EnemyBase::SoulPop() {
		if (_soul == nullptr) return false;	// 既に死んでいる
		Death();	// 死亡
		return true;
	}
}