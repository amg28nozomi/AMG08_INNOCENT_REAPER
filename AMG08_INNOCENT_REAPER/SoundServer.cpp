#include "SoundServer.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <DxLib.h>

namespace se {

	SoundServer::SoundMap SoundServer::_soundList;

	SoundDate::SoundDate(std::string fillname, int type) : _fillname(fillname), _handle(0), _playType(type) {

	}

	std::string& SoundDate::GetFill() {
		std::string* fillname = &_fillname;
		return *fillname;
	}

	void SoundServer::Init() {
		ClearSoundList();
	}

	void SoundServer::Release() {
		ClearSoundList();
	} 

	void SoundServer::ClearSoundList() {
		for (auto&& sdl : _soundList) {
			auto sh = sdl.second.GetHandle();
			DeleteSoundMem(sh);
		}
		_soundList.clear();
	}

	void SoundServer::LoadSoundMap(const SoundMap& soundMap) {
		for (auto&& sm : soundMap) {
			auto& key = sm.first;	
			auto it = _soundList.find(key);
			if (it != _soundList.end()) {
				continue;
			}
			auto sd = sm.second;
			sd.GetHandleP() = LoadSoundMem(sd.GetFill().c_str());
			_soundList.emplace(key.c_str(), sd);
		}
	}

	int SoundServer::GetSound(const std::string& key) {
		auto it = _soundList.find(key);
		if (it == _soundList.end()) {
			return -1;
		}
		// auto sound = it->second.GetHandle();
		return it->second.GetHandle();
	}

	int SoundServer::GetSound(const std::string& key, int* playtype) {
		auto it = _soundList.find(key);
		if (it == _soundList.end()) {
			return -1;
		}
		*playtype = it->second.GetPlayType();
		return it->second.GetHandleP();
	}

	int SoundServer::GetPlayType(const std::string& key) {
		auto it = _soundList.find(key);
		if (it == _soundList.end()) {
			return -1;
		}
		return it->second.GetPlayType();
	}
}