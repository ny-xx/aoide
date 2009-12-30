#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/xtime.hpp>
#include "mplayer.hh"

using namespace std;



MPlayer::MPlayer() : snd(NULL)
{
	bResume = false;				//nothing to resume, you had to start
	engine = createIrrKlangDevice();
		boost::function<void()> f  = boost::bind(&MPlayer::changeTimeThread, this);
	boost::thread th(f);
	cerr << "after thread start " << endl;
}



MPlayer::~MPlayer()
{
	if(snd)
		snd->drop();
	engine->drop();
}

void MPlayer::play(string path)
{
	cerr << "MPlayer::play" << endl;
	if(snd)
	{
		snd->stop();
		snd->setSoundStopEventReceiver(NULL, 0);
		snd->drop();
	}
	if(path=="") return;

	snd = engine->play2D(path.c_str(), false, false, true);
	cerr << "playing " << path.c_str() << endl;
	snd->setSoundStopEventReceiver(this, 0);
	snd->setVolume(vol);
	bResume = true;
}	

void MPlayer::resume()
{
	cerr << "MPlayer::resume" << endl;
	if(bResume)
	{
		if(snd)
			snd->setIsPaused(false);
	}	
}

void MPlayer::pause()
{
	cerr << "MPlayer::pause" << endl;
	bResume  = true;
	if(snd)
		snd->setIsPaused();
}

void MPlayer::stop()
{
	cerr << "MPlayer::stop" << endl;
	bResume = false;
	if(snd)
	{
		snd->setSoundStopEventReceiver(NULL, 0);
		snd->stop();
	}
}

void MPlayer::seek(unsigned int ms)
{
	if(snd)
		snd->setPlayPosition(ms);
}

bool MPlayer::isPaused()
{
	cerr << "MPlayer::isPaused" << endl;
	if(snd)
		return snd->getIsPaused();
	return false;
}

bool MPlayer::isFinished()
{
	if(snd)
		return snd->isFinished();
	return false;
}
	

void MPlayer::setVol(float percent)
{
	vol = percent;
	if(snd)
		snd->setVolume(percent);
}


void MPlayer::OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData)
{
	bResume=false;
	if(action)
		action->onTrackFinished();
}

void MPlayer::changeTimeThread()
{		
	boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    

	for(;;)
	{
		xt.sec += 1;
		boost::thread::sleep(xt);
		if(action)
			action->onTrackPlays();
	};//
}