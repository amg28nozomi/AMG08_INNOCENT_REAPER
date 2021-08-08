#pragma once
#include <unordered_map>
#include <string>

namespace se {

	class SoundDate {
	public:
		SoundDate(std::string fillname, int type);
		~SoundDate() = default;

		// �Q�b�^�[
		inline std::string& GetFill();
		inline int& GetHandleP() { return _handle; }
		inline int GetHandle() { return _handle; }
		inline int GetPlayType() { return _playType; }
	private:
		std::string _fillname;	// �t�@�C����/�p�X��
		int _handle;	// �T�E���h�n���h��
		int _playType;	// �Đ��^�C�v
	};

	class SoundServer {
	public:
		using SoundMap = std::unordered_map<std::string, SoundDate>;
		static void Init();
		static void Release();
		static void ClearSoundList();

		// SE�̓ǂݍ���
		static void LoadSoundMap(const SoundMap& soundMap);
		// �w�肵���T�E���h�n���h�����擾
		static int GetSound(const std::string& key);
		static int GetPlayType(const std::string& key);
		static int GetSound(const std::string& key, int* playtype);
	private:
		static SoundMap _soundList;
	};

}
