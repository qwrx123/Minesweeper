#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <Dsound.h>
#include <objbase.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "ole32.lib")


class songManager
{
public:
enum songTypes
{
    NONE,
    TITLE,
    GAME,
    DEATH
};
enum effectTypes
{
	START,
	CLICK,
	WIN,
	LOSE
};
private:
struct waveFile {
	char ChunkID[4];
	int ChunkSize;
	char Format[4];
	char Subchunk1ID[4];
	int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	short BitsPerSample;
	char Subchunk2ID[4];
	int Subchunk2Size;
};
LPDIRECTSOUND8 lpDevice;
LPDIRECTSOUNDBUFFER8 pclickEffectBuffer = NULL;
LPDIRECTSOUNDBUFFER8 ploseEffectBuffer = NULL;
LPDIRECTSOUNDBUFFER8 pstartEffectBuffer = NULL;
LPDIRECTSOUNDBUFFER8 pwinEffectBuffer = NULL;
int musicVolume;
int effectVolume;
public:
songManager(HWND hwnd);
~songManager();
void changeSongVolume(int volume);
void changeEffectVolume(int volume);
void switchSongType(songManager::songTypes swapType);
void playEffectSound(songManager::effectTypes playEffect);
void setGameSong(int song);
private:
void changeVolume(LPDIRECTSOUNDBUFFER8 theSoundBuffer, int volume);
HRESULT CreateSecondaryBuffer(LPDIRECTSOUND8 lpDirectSound, LPDIRECTSOUNDBUFFER8* ppDsb8, LPCWSTR lpName);
};