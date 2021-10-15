#pragma once
#include <vector>
#include "Vector2.h"

namespace inr {

	namespace oscenario {
		constexpr auto OBJ_NULL = -1;
		constexpr auto OBJ_PLAYER = 0;
		constexpr auto OBJ_SOLDIER_DOLL = 1;
		constexpr auto OBJ_BIG_DOLL = 2;
		constexpr auto OBJ_CROW_DOLL = 3;
		constexpr auto OBJ_SOUL = 4;
		constexpr auto OBJ_LEVER = 5;
		constexpr auto OBJ_BLOCK = 6;
		constexpr auto OBJ_CRYSTAL = 7; // quartz
		constexpr auto OBJ_DOOR = 8;
		constexpr auto OBJ_ITEM = 9;

		namespace type {
			constexpr auto PLAYER = 0;
			constexpr auto ENEMY = 1;
			constexpr auto SOUL = 2;
			constexpr auto GIMMICK = 3;
			constexpr auto ITEM = 4;
		}

		namespace gimmick {
			constexpr auto TYPE_NULL = -1;
			constexpr auto TYPE_LEVER = 0;
			constexpr auto TYPE_CRYSTAL = 1;
			constexpr auto TYPE_BLOCK = 2;
			constexpr auto TYPE_DOOR = 3;

			constexpr auto FLAG_NULL = -1;
			constexpr auto FLAG_FALSE = 0;
			constexpr auto FLAG_TRUE = 1;

			namespace crystal {
				constexpr auto TYPE_NULL = -1;
				constexpr auto TYPE_STAGE_0 = 0;
				constexpr auto TYPE_STAGE_2 = 1;

				constexpr auto DOOR_NULL = -1;
				constexpr auto DOOR_RED = 0;
				constexpr auto DOOR_BLUE = 1;
			}
		}
	}

	/** ギミックの情報 */
	class GimmickValue {
	public:
		/**
		 * @brief	コンストラクタ
		 * @param ギミックの種類
		 * @param ギミックのフラグ
		 * @param 水晶の種類
		 * @param ドアの種類
		 */
		GimmickValue(int type = oscenario::gimmick::TYPE_NULL, int flag = oscenario::gimmick::FLAG_NULL, int ctype = oscenario::gimmick::crystal::TYPE_NULL, std::vector<int> types = { oscenario::gimmick::crystal::DOOR_NULL });
		inline int GimmickType() { return _gimmickType; }
		inline int GimmickFlag() { return _gimmickFlag; }
		inline int CrystalType() { return _crystalType; }
		inline std::vector<int> DoorType() { return _dtype; }
		inline void AddFlag(int flag) { _gimmickFlag = flag; }
	private:
		int _gimmickType;	// ギミックの種類（-1:対象外　0:レバー　1:水晶　2:　3:ドア）
		int _gimmickFlag;	// ギミックのフラグ（-1:対象外　0:false　1:true）
		int _crystalType;	// クリスタルの種類(-1:対象外、0:ステージ0、1:ステージ2)
		std::vector<int> _dtype;	// ドアの種類

	};

	// 登録情報
	class ObjectValue {
	public:
		/**
		 * @brief	コンストラクタ
		 */
		ObjectValue();
		/**
		 * @brief	コンストラクタ
		 * @param 生成するクラス
		 * @param 生成地点
		 * @param 魂の色(0:なし　1:赤　2:青)
		 * @param ギミック情報
		 */
		ObjectValue(int classtype, Vector2 xy, bool direction = false, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		/**
		 * @brief	コンストラクタ
		 * @param 生成するクラス
		 * @param 生成地点を格納したコンテナ
		 * @param 魂の色(0:なし　1:赤　2:青)
		 * @param ギミック情報
		 */
		ObjectValue(int classtype, std::vector<Vector2> xy, bool direction = false, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		/**
		 * @brief	デストラクタ
		 */
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline int SoulType() { return _soulType; }
		inline int GimmickType() { return _gimmick.GimmickType(); }
		inline int GimmickFlag() { return _gimmick.GimmickFlag(); }
		inline int CrystalType() { return _gimmick.CrystalType(); }
		inline int ObjectType() { return _objType; }
		inline bool Direction() { return _direction; }
		inline std::vector<Vector2> Positions() { return _spawnPos; }
		inline std::vector<int> DoorType() { return _gimmick.DoorType(); }

		void PositionsUpdate(Vector2 newpos);
		void FlagUpdate(int flag, int soul = 0);
		inline void SoulUpdate(int soul) { _soulType = soul; }
		inline void DirectionUpdate(bool newdir) { _direction = newdir; }
		
	private:
		int _class;	// 生成するクラスは何か
		std::vector<Vector2> _spawnPos;	// 出現地点
		bool _direction;	// 向き
		int _soulType;	// 魂の有無（0:未所持　1:赤　2:青）
		GimmickValue _gimmick;	// ギミックの登録情報
		int _objType;	// オブジェクトの種類

		int IsObjectType(int classname);	// このオブジェクトは何か？
	};
}

