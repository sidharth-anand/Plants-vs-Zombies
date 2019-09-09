#include "musicplayer.h"

MusicPlayer::MusicPlayer()
	: mMusic()
	, mFilenames()
	, mVolume(100.f)
{
	mFilenames[Music::Menu] = "Resources/Music/menu.ogg";
	mFilenames[Music::Grasswalk] = "Resources/Music/grasswalk.ogg";
	mFilenames[Music::Lose] = "Resources/Music/losemusic.ogg";
	mFilenames[Music::Win] = "Resources/Music/winmusic.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
	mMusic.setVolume(volume);
}

int MusicPlayer::getVolume() const
{
	return (int)mVolume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}

void MusicPlayer::setLooped(bool looped)
{
	mMusic.setLoop(looped);
}
