#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include "audio.hpp"

SoftwareSerial musicChipSerial(-1, MUSIC_CHIP_PIN);
SoftwareSerial effectChipSerial(-1, EFFECT_CHIP_PIN);

DFRobotDFPlayerMini musicPlayer;
DFRobotDFPlayerMini effectPlayer;

void InitAudio()
{
  musicChipSerial.begin(9600);
  effectChipSerial.begin(9600);

  musicPlayer.begin(musicChipSerial, false);
  effectPlayer.begin(effectChipSerial, false);

  musicPlayer.volume(DEFAULT_VOLUME);
  effectPlayer.volume(DEFAULT_VOLUME);
}

void StartMusic(MUSIC music){
  musicPlayer.playFolder(1, music);
  musicPlayer.enableLoop();
}

void StopMusic(){
  musicPlayer.stop();
}

void PlayEffect(EFFECT effect){
  effectPlayer.playFolder(1, effect);
}
