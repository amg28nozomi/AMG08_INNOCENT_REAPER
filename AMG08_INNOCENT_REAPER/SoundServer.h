#pragma once
#include <unordered_map>
#include <string>

namespace se {

	class SoundDate {
	public:
		SoundDate(std::string fillname, int type);
		~SoundDate() = default;

		// ゲッター
		inline std::string& GetFill();
		inline int& GetHandleP() { return _handle; }
		inline int GetHandle() { return _handle; }
		inline int GetPlayType() { return _playType; }
	private:
		std::string _fillname;	// ファイル名/パス名
		int _handle;	// サウンドハンドル
		int _playType;	// 再生タイプ
	};

	class SoundServer {
	public:
		using SoundMap = std::unordered_map<std::string, SoundDate>;
		static void Init();
		static void Release();
		static void ClearSoundList();

		// SEの読み込み
		static void LoadSoundMap(const SoundMap& soundMap);
		// 指定したサウンドハンドルを取得
		static int GetSound(const std::string& key);
		static int GetPlayType(const std::string& key);
		static int GetSound(const std::string& key, int* playtype);
	private:
		static SoundMap _soundList;
	};

}
