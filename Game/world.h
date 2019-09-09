#pragma once

#include "resourceholder.h"
#include "resourceidentifiers.h"
#include "scenenode.h"
#include "spritenode.h"
#include "plant.h"
#include "zombie.h"
#include "command.h"
#include "commandqueue.h"
#include "container.h"
#include "soundplayer.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Window\Event.hpp>

#include <array>
#include <queue>

namespace sf
{
	class RenderTarget;
}

class World : private sf::NonCopyable
{
public:
	explicit							World(sf::RenderTarget& outputTarget, TextureHolder& textures, FontHolder& fonts, SoundPlayer& sounds);
	void								update(sf::Time dt);
	void								draw();
	void								handleEvent(sf::Event event);
	void								setMousePosition(sf::Vector2i position);
	void								addSun(int sun);

	CommandQueue&						getCommandQueue();

	bool 								hasPlayerFinished() const;
	bool								hasZombiesReachedEnd() const;

	void								screenshot();

	float								getElapsedTime() const;

private:
	void								loadTextures();
	void								handleCollisions();

	void								initializeTileMap();
	void								intializeButtons();
	void								intializeLevelData();

	void								buildScene();
	void								addEnemies();
	void								addZombie(Zombie::Type type, float relX, float relY, sf::Time delay);
	void								spawnEnemies(sf::Time dt);
	void								destroyEntitiesOutsideView();
	void								drawPlantPlacement();
	void								updatePlants();
	void								updateTexts();
	void								updateButtons();
	void								updateSounds();
	void								showWave();
	void								updateLevelProgress(sf::Time dt);
	void								spawnSun(sf::Time dt);
	void								collectSun(sf::Vector2i mousePosition);
	sf::FloatRect						getViewBounds() const;
	sf::FloatRect						getBattlefieldBounds() const;

	void								startSequence(sf::Time dt);

private:
	enum Layer
	{
		Background,
		LowerGround,
		UpperGround,
		ForeGround,
		LayerCount
	};

	struct SpawnPoint
	{
		SpawnPoint(Zombie::Type type, float x, float y, sf::Time delay)
			: type(type)
			, x(x)
			, y(y)
			, delay(delay)
		{
		}

		Zombie::Type type;
		float x;
		float y;
		sf::Time delay;
	};


private:
	sf::RenderTarget&					mTarget;
	sf::RenderTexture					mSceneTexture;
	sf::View							mWorldView;
	TextureHolder&						mTextures;
	FontHolder&							mFonts;

	SceneNode							mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;
	CommandQueue						mCommandQueue;

	sf::FloatRect						mWorldBounds;
	sf::Vector2f						mSpawnPosition;

	std::vector<SpawnPoint>				mZombieSpawnPoints;
	std::vector<Zombie*>					mActiveEnemies;
	std::vector<Plant*>					mPlants;

	std::vector<std::vector<int>>		mTileMap;

	TextNode*							mLifeDisplay;
	int									mLives;

	TextNode*							mSunDisplay;
	int									mSun;

	TextNode*							mWavesDisplay;
	int									mWaves;

	sf::Time							mSpawnTime;
	int									mCurWave;

	sf::Vector2i						mMousePosition;

	int									mSelectedPlant;

	bool								mIsButtonSelected;
	int									mSelectedPlantType;

	GUI::Container						mContainer;

	sf::Time							mElapsedSunTime;
	sf::Time							mTimeToNextSun;

	bool								mIsStartSequenceOver;
	sf::Time							mStartTime;
	SpriteNode*							mStartWords;

	SpriteNode*							mLevelProgressBar;
	SpriteNode*							mLevelProgressHead;
	sf::Time							mElapsedLevelTime;

	sf::Time							mTotalLevelTime;
	std::vector<sf::Time>				mWaveTimes;

	SpriteNode*							mHugeWave;
	bool								mIsTextAttached;

	SoundPlayer&						mSounds;
};