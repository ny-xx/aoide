
#include <string>
#include "irrklang/irrKlang.h"


using namespace irrklang;
using namespace std;

class MPlayer_Actionhandler
{
	public:
	//virtual MPlayer_Actionhandler(){};
	//virtual MPlayer_Actionhandler(){};
	virtual void onTrackFinished() = 0;
	virtual void onTrackPlays() = 0;
};


class MPlayer : public ISoundStopEventReceiver
{
public:
	MPlayer();
	~MPlayer();
	
	void play(string path);
	void resume();
	void pause();
	void stop();
	void seek(unsigned int ms);  //0 begin, 1 end	
	void setVol(float percent); //0 min, 1 max
	bool isPaused();
	bool isFinished();

	int getTime(){if(snd)return snd->getPlayPosition();else return 0;};
	int getPlayLength(){if(snd) return snd->getPlayLength();};
	void setActionHandler(MPlayer_Actionhandler *ah){action = ah;};
	virtual void OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData);
private:
	float vol;
	bool bResume;
	ISoundEngine *engine;
	ISound* snd;
	MPlayer_Actionhandler *action;

	void changeTimeThread();
};
