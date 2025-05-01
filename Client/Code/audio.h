#pragma once
/*ma_engine audio;
std::vector<ma_sound*> sounds = {};
//Audio Management -------------------------------------
enum SFX {
	BUTTON, BUILD, DESTROY, MARCH, PLACE_UNIT, AMBIENT_1, AMBIENT_2, AMBIENT_3
};

struct Sound {
	std::string name;
	int versions = 0;
	Sound(const char* n, int v = 1) {
		name = n;
		versions = v;
	}
};

std::vector<const char*> music = { };
std::vector<Sound> sfx = {
	Sound("button"),
	Sound("button"),
	Sound("destroy"),
	Sound("march"),
	Sound("place_unit"),
	Sound("ocean"),
	Sound("forest"),
	Sound("mountain")
};

float getVolume(SFX s) {
	if (s >= AMBIENT_1) {
		return UI.sounds[s - AMBIENT_1].volume;
	}
	if (s == BUTTON) {
		return 1;
	}
	return .4;
}

std::vector<ma_sound*> ambience = {};
bool started[3] = {};

void manageAmbience() {
	for (int i = 0; i < 3; i++) {
		AmbientSound* sound = &UI.sounds[i];
		if (sound->volume < sound->target) {
			sound->volume += .01;
			if (sound->volume > sound->target) {
				sound->volume = sound->target;
			}
		}
		if (sound->volume > sound->target) {
			sound->volume -= .05;
			if (sound->volume < sound->target) {
				sound->volume = sound->target;
			}
		}
	}
	if (ambience.size() == 0) {
		for (int i = 0; i < 3; i++) {
			ma_sound* sound = new ma_sound();
			ambience.push_back(sound);
		}
	}
	int i = 0;
	for (ma_sound* sound : ambience) {
		if (!(started[i])) {
			started[i] = true;
			//ma_sound_uninit(sound);
			std::string str = "./Sounds/" + sfx[AMBIENT_1 + i].name + ".mp3";
			char* c = const_cast<char*>(str.c_str());
			ma_result res = ma_sound_init_from_file(&audio, c, MA_SOUND_FLAG_ASYNC, NULL, NULL, sound);
			ma_sound_set_looping(sound, true);
			res = ma_sound_start(sound);
		}
		ma_sound_set_volume(sound, getVolume((SFX)(AMBIENT_1 + i)));
		i++;
	}
}

void playSound(SFX s) {
	//ma_sound_set_pan() to p an audio -1 is left, 1 is right
	ma_sound* sound = new ma_sound();
	sounds.push_back(sound);
	std::string str = "./Sounds/" + sfx[s].name;
	if (sfx[s].versions > 1) {
		int ran = 1 + rand() % sfx[s].versions;
		str += to_str(ran);
	}
	str += ".mp3";
	char* c = const_cast<char*>(str.c_str());
	ma_result res = ma_sound_init_from_file(&audio, c, MA_SOUND_FLAG_ASYNC, NULL, NULL, sound);
	if (res == MA_SUCCESS) {
		ma_sound_set_volume(sound, getVolume(s));
		res = ma_sound_start(sound);
	}
	else {
		delete sound;
		sounds.pop_back();
	}
	std::vector<ma_sound*> newSounds = {};
	for (int i = 0; i < sounds.size(); i++) {
		if (sounds[i] != nullptr && !(sounds[i]->atEnd)) {
			newSounds.push_back(sounds[i]);
		}
		else {
			ma_sound_uninit(sounds[i]);
			delete sounds[i];
			sounds[i] = nullptr;
		}
	}
	sounds = newSounds;
}
*/