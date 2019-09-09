#include "world.h"
#include "projectile.h"
#include "foreach.h"
#include "textnode.h"
#include "button.h"
#include "datatables.h"
#include "sun.h"
#include "soundnode.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

namespace
{
	const std::vector<PlantData> Table = initializePlantData();
}

const sf::Vector2f TileSize(81.1111111111, 100.f);
const sf::Vector2f TileOffset(-30.f, -25.f);

World::World(sf::RenderTarget& outputTarget, TextureHolder& textures, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(outputTarget)
	, mSceneTexture()
	, mWorldView(outputTarget.getDefaultView())
	, mTextures(textures)
	, mFonts(fonts)
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(-50.f, -50.f, mWorldView.getSize().x + 50.f, mWorldView.getSize().y)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f)
	, mZombieSpawnPoints()
	, mActiveEnemies()
	, mPlants()
	, mLives(5)
	, mLifeDisplay(nullptr)
	, mWavesDisplay(nullptr)
	, mWaves(0)
	, mSpawnTime(sf::seconds(0.f))
	, mCurWave(1)
	, mIsButtonSelected(false)
	, mSelectedPlantType(-1)
	, mSun(200)
	, mSunDisplay(nullptr)
	, mContainer()
	, mSelectedPlant(-1)
	, mElapsedSunTime(sf::seconds(0.f))
	, mTimeToNextSun(sf::seconds(0.f))
	, mIsStartSequenceOver(false)
	, mStartTime(sf::seconds(0.f))
	, mStartWords(nullptr)
	, mLevelProgressBar(nullptr)
	, mLevelProgressHead(nullptr)
	, mTotalLevelTime(sf::seconds(22.f))
	, mElapsedLevelTime(sf::seconds(0.f))
	, mWaveTimes()
	, mHugeWave(nullptr)
	, mIsTextAttached(false)
	, mSounds(sounds)
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

	loadTextures();
	initializeTileMap();
	intializeButtons();
	intializeLevelData();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);

	mSounds.play(Sounds::ReadySetPlant);
}

void World::update(sf::Time dt)
{
	if (mIsStartSequenceOver)
	{
		destroyEntitiesOutsideView();
		updatePlants();

		while (!mCommandQueue.isEmpty())
			mSceneGraph.onCommand(mCommandQueue.pop(), dt);

		mSceneGraph.removeWrecks();
		spawnEnemies(dt);
		spawnSun(dt);
		updateTexts();
		updateButtons();
		updateLevelProgress(dt);
		showWave();

		mSceneGraph.update(dt, mCommandQueue);

		handleCollisions();
	}
	else
		startSequence(dt);

	updateSounds();
}

void World::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(mSceneGraph);

	mTarget.draw(mContainer);
	drawPlantPlacement();
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

bool World::hasPlayerFinished() const
{
	if (mZombieSpawnPoints.size() == 0 && mActiveEnemies.size() == 0)
		return true;
	else
		return false;
}

bool World::hasZombiesReachedEnd() const
{
	FOREACH(Zombie* enemy, mActiveEnemies)
	{
		if (!getBattlefieldBounds().contains(enemy->getPosition()))
			return true;
	}
	return false;
}

void World::loadTextures()
{

}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		if (matchesCategories(pair, Category::Projectile, Category::Zombie))
		{
			auto& bullet = static_cast<Projectile&>(*pair.first);
			auto& enemy = static_cast<Zombie&>(*pair.second);

			enemy.damage(bullet.getDamage());
			bullet.destroy();

			if (enemy.getHitpoints() == 0)
				enemy.setPosition(enemy.getPosition().x - 120, enemy.getPosition().y);
		}

		else if (matchesCategories(pair, Category::Plant, Category::Zombie))
		{
			auto& plant = static_cast<Plant&>(*pair.first);
			auto& enemy = static_cast<Zombie&>(*pair.second);

			if (plant.getPosition().x < enemy.getPosition().x && (((int)(enemy.getPosition().y) / (int)TileSize.y) * (int)TileSize.y) == (((int)(plant.getPosition().y) / (int)TileSize.y) * (int)TileSize.y))
			{
				enemy.setVelocity(0.f, 0.f);
				enemy.setIsEating(true);
				if (plant.getHitpoints() % 50 == 0)
					mSounds.play(Sounds::Eating);
				plant.damage(1.f);

				if (plant.getHitpoints() == 0)
				{
					mTileMap[plant.getPosition().y / (int)TileSize.y][plant.getPosition().x / (int)TileSize.x] = 0;
				}
			}
		}
	}
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == LowerGround) ? Category::SceneLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	sf::Texture& background = mTextures.get(Textures::Map);
	sf::Texture& word = mTextures.get(Textures::Words);
	sf::Texture& progressbar = mTextures.get(Textures::ProgressBar);
	sf::Texture& progressmeter = mTextures.get(Textures::ProgressMeter);
	sf::Texture& progressparts = mTextures.get(Textures::ProgressParts);

	std::unique_ptr<SpriteNode> grass(new SpriteNode(background));
	grass->setPosition(mWorldView.getSize().x / 2, mWorldView.getSize().y / 2);
	mSceneLayers[Background]->attachChild(std::move(grass));

	std::unique_ptr<TextNode> sun(new TextNode(mFonts, ""));
	mSunDisplay = sun.get();
	mSunDisplay->setSize(16u);
	mSunDisplay->setPosition(73.f, 72.f);
	mSunDisplay->setColor(sf::Color::Black);
	mSceneLayers[Background]->attachChild(std::move(sun));

	std::unique_ptr<SpriteNode> words(new SpriteNode(word));
	mStartWords = words.get();
	mStartWords->setPosition(mWorldView.getSize().x / 2, mWorldView.getSize().y / 2);
	mSceneLayers[Background]->attachChild(std::move(words));

	std::unique_ptr<SpriteNode> meter(new SpriteNode(progressmeter));
	meter->setPosition(705.f, 585.5);
	mSceneLayers[Background]->attachChild(std::move(meter));

	std::unique_ptr<SpriteNode> bar(new SpriteNode(progressbar));
	mLevelProgressBar = bar.get();
	mLevelProgressBar->setPosition(-100, -100);
	mSceneLayers[Background]->attachChild(std::move(bar));

	for (int i = 0; i < mWaveTimes.size(); i++)
	{
		std::unique_ptr<SpriteNode> flag(new SpriteNode(progressparts));
		flag->setTextureRect(sf::IntRect(0, 0, 23, 31));
		flag->setPosition(626 + (1 - mWaveTimes[i] / mTotalLevelTime) * progressmeter.getSize().x + 12.5, 572.5);
		mSceneLayers[Background]->attachChild(std::move(flag));
	}

	std::unique_ptr<SpriteNode> levelprogress(new SpriteNode(progressparts));
	levelprogress->setPosition(711.f, 594.5);
	levelprogress->setTextureRect(sf::IntRect(64, 10, 86, 11));
	mSceneLayers[Background]->attachChild(std::move(levelprogress));

	std::unique_ptr<SpriteNode> head(new SpriteNode(progressparts));
	mLevelProgressHead = head.get();
	mLevelProgressHead->setPosition(-100.f, -100.f);
	mLevelProgressHead->setTextureRect(sf::IntRect(27, 0, 23, 24));
	mSceneLayers[Background]->attachChild(std::move(head));

	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

	addEnemies();
}

void World::addEnemies()
{
	std::fstream list("Data/level1.zombies");
	assert(list.is_open());

	std::istreambuf_iterator<char> i(list);
	std::istreambuf_iterator<char> e;

	int n = 0;
	while (list.peek() != '\n' && i != e)
	{
		n *= 10;
		n += *i - '0';
		i++;
	}

	for (int x = 0; x < n; x++)
	{
		i++;
		int type = *i - '0';
		i++;
		i++;

		int delay = 0;
		while (list.peek() != '\n' && i != e)
		{
			delay *= 10;
			delay += *i - '0';
			i++;
		}

		addZombie(Zombie::Type(type), 850.f, randomInt(0, 4) * 100 + 104.5, sf::seconds(delay));
	}

	std::reverse(mZombieSpawnPoints.begin(), mZombieSpawnPoints.end());
}

void World::addZombie(Zombie::Type type, float relX, float relY, sf::Time delay)
{
	SpawnPoint spawn(type, relX, relY, delay);
	mZombieSpawnPoints.push_back(spawn);
}

void World::spawnEnemies(sf::Time dt)
{
	mSpawnTime += dt;

	while (!mZombieSpawnPoints.empty() && mSpawnTime > mZombieSpawnPoints.back().delay)
	{
		SpawnPoint spawn = mZombieSpawnPoints.back();

		std::unique_ptr<Zombie> enemy(new Zombie(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);

		mSceneLayers[ForeGround]->attachChild(std::move(enemy));

		mZombieSpawnPoints.pop_back();
		mSpawnTime = sf::seconds(0.f);
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.remove();
	});

	Command command2;
	command2.category = Category::Zombie;
	command2.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.remove();
	});


	mCommandQueue.push(command);
	mCommandQueue.push(command2);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.left -= 50;
	bounds.top -= 50;
	bounds.width += 150;

	return bounds;
}

void World::startSequence(sf::Time dt)
{
	mStartTime += dt;

	if (mStartTime < sf::seconds(6.f))
	{
		if (mStartTime > sf::seconds(4.f))
			mStartWords->setTextureRect(sf::IntRect(570, 0, 285, 103));
		else if (mStartTime > sf::seconds(2.f))
			mStartWords->setTextureRect(sf::IntRect(285, 0, 285, 103));
		else
			mStartWords->setTextureRect(sf::IntRect(0, 0, 285, 103));

	}
	else
	{
		mIsStartSequenceOver = true;

		mSceneLayers[Background]->detachChild(*mStartWords);
	}
}

void World::updatePlants()
{
	mActiveEnemies.clear();

	Command enemyCollector;
	enemyCollector.category = Category::Zombie;
	enemyCollector.action = derivedAction<Zombie>([this](Zombie& enemy, sf::Time)
	{
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	Command PlantCollector;
	PlantCollector.category = Category::Plant;
	PlantCollector.action = derivedAction<Plant>([this](Plant& plant, sf::Time)
	{
		if (!plant.isDestroyed())
			mPlants.push_back(&plant);
	});


	Command rangeChecker;
	rangeChecker.category = Category::Plant;
	rangeChecker.action = derivedAction<Plant>([this](Plant& plant, sf::Time)
	{
		if (plant.getType() == Plant::Sunflower)
		{
			plant.setZombieInRange(true);
			return;
		}
		else
		{
			FOREACH(Zombie* enemy, mActiveEnemies)
			{
				float eDistance = distance(plant, *enemy);
				if (eDistance < plant.getRange() && (((int)(enemy->getPosition().y) / (int)TileSize.y) * (int)TileSize.y) == (((int)(plant.getPosition().y) / (int)TileSize.y) * (int)TileSize.y))
				{
					if (enemy->getPosition().x > plant.getPosition().x)
					{
						plant.setZombieInRange(true);
						return;
					}
				}
			}
			plant.setZombieInRange(false);
		}
	});

	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(PlantCollector);
	mCommandQueue.push(rangeChecker);
	mPlants.clear();
}

void World::initializeTileMap()
{
	std::fstream map("Data/map.map");
	assert(map.is_open());

	std::vector<int> v;

	for (std::istreambuf_iterator<char> i(map), e; i != e; i++)
	{
		if (map.peek() == '\n')
		{
			mTileMap.push_back(v);
			v.clear();
		}
		else
		{
			v.push_back(*i - '0');
		}
	}

}

void World::handleEvent(sf::Event event)
{
	if (mIsStartSequenceOver)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (mMousePosition.y <= (mTileMap.size() * TileSize.y) + TileOffset.y && mMousePosition.x <= (mTileMap[0].size() * TileSize.x) + TileOffset.x)
				{
					sf::Vector2i pos = mMousePosition;
					pos.x /= (int)TileSize.x;
					pos.y /= (int)TileSize.y;
					pos.x *= (int)TileSize.x;
					pos.y *= (int)TileSize.y;


					if (mTileMap[pos.y / (int)TileSize.y][pos.x / (int)TileSize.x] == 0 && mIsButtonSelected)
					{
						std::unique_ptr<Plant> Plant(new Plant(Plant::Type(mSelectedPlantType), mTextures, mFonts));
						Plant->setPosition(pos.x + TileSize.x / 2 + TileOffset.x, pos.y + TileSize.y / 2 + TileOffset.y);

						mSun -= Plant->getPrice();

						if (Plant->getType() == Plant::Sunflower)
							mSceneLayers[LowerGround]->attachChild(std::move(Plant));
						else
							mSceneLayers[UpperGround]->attachChild(std::move(Plant));

						mSounds.play(Sounds::Plant, sf::Vector2f(pos.x, pos.y));

						mTileMap[pos.y / (int)TileSize.y][pos.x / (int)TileSize.x] = 2;
						mIsButtonSelected = false;
					}

					else if (!mIsButtonSelected)
						collectSun(mMousePosition);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
				mIsButtonSelected = false;
		}

		mContainer.setMousePosition(mMousePosition);
		mContainer.handleEvent(event);
	}
}

void World::setMousePosition(sf::Vector2i position)
{
	mMousePosition = position;
}

void World::addSun(int sun)
{
	mSun += sun;
}

void World::drawPlantPlacement()
{
	if (mIsButtonSelected)
	{
		if (mMousePosition.y <= (mTileMap.size() * TileSize.y) + TileOffset.y && mMousePosition.x <= (mTileMap[0].size() * TileSize.x) + TileOffset.x)
		{
			sf::Vector2i pos;
			pos.x = mMousePosition.x / (int)TileSize.x;
			pos.y = mMousePosition.y / (int)TileSize.y;
			pos.x *= (int)TileSize.x;
			pos.y *= (int)TileSize.y;

			if (mTileMap[pos.y / (int)TileSize.y][pos.x / (int)TileSize.x] == 0)
			{
				sf::Sprite placeholder(mTextures.get(Table[mSelectedPlantType].texture), Table[mSelectedPlantType].textureRect);
				centerOrigin(placeholder);
				placeholder.setPosition(pos.x + TileSize.x / 2 + TileOffset.x, pos.y + TileSize.y / 2 + TileOffset.y);
				placeholder.setColor(sf::Color(255, 255, 255, 122));

				sf::Sprite foreground(mTextures.get(Table[mSelectedPlantType].texture), Table[mSelectedPlantType].textureRect);
				centerOrigin(foreground);
				foreground.setPosition(mMousePosition.x, mMousePosition.y);

				mTarget.draw(placeholder);
				mTarget.draw(foreground);
			}
		}
	}
}

void World::spawnSun(sf::Time dt)
{
	if (mTimeToNextSun == sf::seconds(0))
		mTimeToNextSun = sf::seconds(randomInt(5, 25));

	mElapsedSunTime += dt;

	if (mElapsedSunTime > mTimeToNextSun)
	{
		std::unique_ptr<Sun> sun(new Sun(mTextures, randomInt(1, 4) * 100 + 50));
		sun->setPosition(randomInt(1, 8) * 80 + 80 + TileOffset.x, 0);

		mSceneLayers[UpperGround]->attachChild(std::move(sun));

		mTimeToNextSun = sf::seconds(randomInt(5, 25));
		mElapsedSunTime = sf::seconds(0.f);
	}
}

void World::collectSun(sf::Vector2i mousePosition)
{
	Command sunCollector;
	sunCollector.category = Category::Sun;
	sunCollector.action = derivedAction<Sun>([this, mousePosition](Sun& sun, sf::Time)
	{
		if (sun.getBoundingRect().contains(mousePosition.x, mousePosition.y))
		{
			mSun += 25;
			mSounds.play(Sounds::SunClick);
			sun.remove();
		}
	});

	mCommandQueue.push(sunCollector);
}

void World::updateTexts()
{
	mSunDisplay->setString(toString(mSun));
}

void World::updateButtons()
{
	for (int i = 0; i < 2; i++)
	{
		if (mSun < Table[i].price)
			mContainer.toggleDisable(i, true);
		else
			mContainer.toggleDisable(i, false);
	}
}

void World::intializeButtons()
{
	auto peashooter = std::make_shared<GUI::Button>(mFonts, mTextures, mSounds, GUI::Button::PlantButton);
	peashooter->setPosition(137.f, 43.f);
	peashooter->hasIcon(true);
	peashooter->setContent(mTextures.get(Textures::PlantIcon));
	peashooter->setImageOffset(sf::Vector2f(-1.f, 3.f));
	peashooter->setCallback([this]()
	{
		mIsButtonSelected = true;
		mSelectedPlantType = Plant::Basic;
	});

	auto sunflower = std::make_shared<GUI::Button>(mFonts, mTextures, mSounds, GUI::Button::PlantButton);
	sunflower->setPosition(185.5, 43.f);
	sunflower->hasIcon(true);
	sunflower->setContent(mTextures.get(Textures::SunflowerIcon));
	sunflower->setImageOffset(sf::Vector2f(-1.f, 3.f));
	sunflower->setCallback([this]()
	{
		mIsButtonSelected = true;
		mSelectedPlantType = Plant::Sunflower;
	});

	mContainer.pack(peashooter);
	mContainer.pack(sunflower);
}

void World::updateLevelProgress(sf::Time dt)
{
	mElapsedLevelTime += dt;

	sf::Texture& bar = mTextures.get(Textures::ProgressBar);
	float width = bar.getSize().x;

	if (mElapsedLevelTime < mTotalLevelTime)
	{
		mLevelProgressBar->setTextureRect(sf::IntRect((1 - (mElapsedLevelTime / mTotalLevelTime)) * width, 0, mElapsedLevelTime / mTotalLevelTime * width, bar.getSize().y));
		mLevelProgressBar->setPosition(626 + (1 - (mElapsedLevelTime / mTotalLevelTime)) * width + mLevelProgressBar->getBoundingRect().width / 2, 585.f);

		mLevelProgressHead->setPosition(626 + (1 - (mElapsedLevelTime / mTotalLevelTime)) * width, 583.f);
	}
}

void World::intializeLevelData()
{
	std::fstream data("Data/level1.dat");
	assert(data.is_open());

	std::istreambuf_iterator<char> i(data);
	std::istreambuf_iterator<char> e;

	int time = 0;

	while (data.peek() != '\n' && i != e)
	{
		time *= 10;
		time += *i - '0';
		i++;
	}

	mTotalLevelTime = sf::seconds(time);

	int n = 0;
	i++;
	n = *i - '0';

	for (int x = 0; x < n; x++)
	{
		time = 0;
		i++;
		i++;

		while (data.peek() != '\n' && i != e)
		{
			time *= 10;
			time += *i - '0';
			i++;
		}

		mWaveTimes.push_back(sf::seconds(time));
	}
}

void World::showWave()
{
	for (int i = 0; i < mWaveTimes.size(); i++)
	{
		if (mElapsedLevelTime >= mWaveTimes[i] - sf::seconds(4.f) && mElapsedLevelTime <= mWaveTimes[i] + sf::seconds(1.5f) && !mIsTextAttached)
		{
			sf::Texture& hugewave = mTextures.get(Textures::HugeWave);

			std::unique_ptr<SpriteNode> wave(new SpriteNode(hugewave));
			mHugeWave = wave.get();
			mHugeWave->setPosition(mWorldView.getSize().x / 2, mWorldView.getSize().y / 2);
			mSceneLayers[ForeGround]->attachChild(std::move(wave));
			mIsTextAttached = true;

			mSounds.play(Sounds::HugeWave, sf::Vector2f(400.f, 300.f));

			return;
		}
		else if (mElapsedLevelTime>=mWaveTimes[i] + sf::seconds(1.5f) && mHugeWave != nullptr && mIsTextAttached)
		{
			mSceneLayers[ForeGround]->detachChild(*mHugeWave);
			mHugeWave = nullptr;
			mIsTextAttached = false;
		}
	}
}

void World::updateSounds()
{
	mSounds.setListenerPosition(sf::Vector2f(400.f, 300.f));

	mSounds.removeStoppedSounds();
}

void World::screenshot()
{
	sf::Image image = mSceneTexture.getTexture().copyToImage();
	std::cout << "asd";
	for (int i = 1; i < image.getSize().x - 1; i++)
	{
		for (int j = 1; j < image.getSize().y - 1; j++)
		{
			sf::Color c(0, 0, 0);
			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					c += image.getPixel(i + u, j + v);
				}
			}
			c = sf::Color(c.r / 9, c.g / 9, c.b / 9);
			c += image.getPixel(i, j);
			c = sf::Color(c.r / 2, c.g / 2, c.b / 2);
			image.setPixel(i, j, c);
		}
	}
	if (image.saveToFile("screenshot.png"))
		std::cout << "saved";
	else
		throw("Could not save file");
}

float World::getElapsedTime() const
{
	return mElapsedLevelTime.asMilliseconds();
}
