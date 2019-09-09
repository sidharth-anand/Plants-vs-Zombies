#pragma once

namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		Grass,
		Plant,
		Bullet,
		Zombie,
		Map,
		Buttons,
		PlantIcon,
		ZombieDeath,
		ZombieNormal,
		Sun,
		PeaHit,
		Words,
		Eating,
		ProgressMeter,
		ProgressBar,
		ProgressParts,
		HugeWave,
		AdventureButton,
		MainMenu,
		OptionsButton,
		QuitButton,
		OptionsBG,
		OptionsBack,
		SliderKnob,
		SliderBar,
		Pause,
		Back,
		Restart,
		Win,
		Lose,
		TitleScreen,
		LoadingGrass,
		LoadingGrassRoll,
		Logo,
		Sunflower,
		SunflowerIcon,
		PeashooterAnimation,
		SunflowerAnimation,
		PlayerNameButton,
		ScoreButton,
		ScoreScreen,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		CSMS,
		HOT,
		Dwarven,
		BrianneTod,
	};
}

namespace Music
{
	enum ID
	{
		Menu,
		Grasswalk,
		Win,
		Lose,
	};
}

namespace Sounds
{
	enum ID
	{
		ButtonHover,
		ButtonClick,
		ButtonClickSmall,
		Plant,
		ReadySetPlant,
		Pause,
		SunClick,
		ButtonClickPlant,
		PeashooterFire,
		PeaSplat,
		HugeWave,
		Eating,
		ZombieFalling,
		None,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>			TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>					FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID>			SoundBufferHolder;