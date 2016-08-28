#include "Thunder.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Thunder::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



bool Thunder::init() {
    if ( !Layer::init() ) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();

    auto bgsprite = Sprite::create("background.jpg");
    bgsprite->setPosition(visibleSize / 2);
    // bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, \
    //     visibleSize.height / bgsprite->getContentSize().height);
    this->addChild(bgsprite, 0);

    player = Sprite::create("player.png");
    player->setPosition(visibleSize.width / 2, player->getContentSize().height + 5);
    player->setName("player");
    this->addChild(player, 1);

    addEnemy(5);

    preloadMusic();
    playBgm();

    addTouchListener();
    addKeyboardListener();
    addCustomListener();

	

    // TODO
    // add schedule
	schedule(schedule_selector(Thunder::update), 0.01f, kRepeatForever, 0);
	schedule(schedule_selector(Thunder::update2), 0.01f, kRepeatForever, 0);
	schedule(schedule_selector(Thunder::fires), 0.1f, kRepeatForever, 0);
    
    return true;
}

void Thunder::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("bgm.mp3");
	
}

void Thunder::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("bgm.mp3", true);
}

void Thunder::addEnemy(int n) {
    enemys.resize(n * 3);
    for(unsigned i = 0; i < 3; ++i) {
        char enemyPath[20];
        sprintf(enemyPath, "stone%d.png", 3 - i);
        double width  = (visibleSize.width - 20) / (n + 1.0),
               height = visibleSize.height - (50 * (i + 1));
        for(int j = 0; j < n; ++j) {
            auto enemy = Sprite::create(enemyPath);
            enemy->setAnchorPoint(Vec2(0.5, 0.5));
            enemy->setScale(0.5, 0.5);
            enemy->setPosition(width * (j + 1), height);
            enemys[i * n + j] = enemy;
			addChild(enemy);
        }
    }
}

void Thunder::addTouchListener(){
    // 
}

void Thunder::addKeyboardListener() {
	auto keyboardlistener = EventListenerKeyboard::create();
	keyboardlistener -> onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	keyboardlistener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, player);
}

void Thunder::update2(float f) {
	if (leftMove == true && player -> getPosition().x > 40)
	{
		player->setPosition(player->getPosition().x - 2, player->getPosition().y);
	}

	if (rightMove == true && player -> getPosition().x < visibleSize.width)
	{
		player->setPosition(player->getPosition().x + 2, player->getPosition().y);
	}

	if (upMove == true && player->getPosition().y < visibleSize.height)
	{
		player->setPosition(player->getPosition().x, player->getPosition().y + 2);
	}

	if (downMove == true && player->getPosition().y > 30)
	{
		player->setPosition(player->getPosition().x, player->getPosition().y - 2);
	}

	for (int j = 0; j < enemys.size(); j++)
	{
		if (enemys[j] != NULL)
		{
			for (unsigned i = 0; i < bullets.size(); i++)
			{

				if (bullets[i] != NULL && bullets[i]->getPosition().getDistance(enemys[j]->getPosition()) < 30)
				{
					EventCustom e("meet");
					e.setUserData(&j);
					_eventDispatcher->dispatchEvent(&e);
					this->removeChild(bullets[i]);
					bullets[i] = NULL;
					auto audio = SimpleAudioEngine::getInstance();
					audio->playEffect("explore.wav");
					break;
				}
			}

 			if (enemys[j] != NULL && player->getPosition().getDistance(enemys[j]->getPosition()) < 30)
			{
				Director::getInstance()->pause();
			}
		}
	}
}

void Thunder::update(float f) {

	static int dir = 1;
	count += f;
	if (count > 1.0) {
		moved = false;
		dir = -1;
	}
	if (count > 2.0) {
		dir = 1;
	}

	if (count > 3.0) {
		count = 0;
		moved = true;
	}
	
	if (moved == false) {
		

		for (unsigned i = 0; i < enemys.size(); i++) {
			if (enemys[i] != NULL) {
				enemys[i]->setPosition(enemys[i]->getPosition() + Vec2(dir, 0));
			}
		}
	}

	if (moved == true)
	{
		for (unsigned i = 0; i < enemys.size(); i++) {
			if (enemys[i] != NULL) {
				Vec2 enemyPos = enemys[i]->getPosition();
				Vec2 dire = player->getPosition() - enemyPos;
				dire.normalize();
				if (enemys[i] != NULL) enemys[i] ->setPosition(enemys[i]->getPosition() + 0.7 * dire);
			}
		}
	}

	for (unsigned i = 0; i < bullets.size(); i++) {
		if (bullets[i] != NULL) {
			bullets[i]->setPosition(bullets[i]->getPositionX(), bullets[i]->getPositionY() + 5);
			if (bullets[i]->getPositionY() > visibleSize.height) {
				this->removeChild(bullets[i]);
				bullets[i] = NULL;
			}
		}
	}
}

void Thunder::fires(float f) {
	if (fire == true)
	{
		auto bullet = Sprite::create("bullet.png");
		bullets.push_back(bullet);
		bullets.back()->setPosition(player->getPosition());
		addChild(bullets.back());

		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("fire.wav");
	}
}

void Thunder::addCustomListener() {
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	_eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

void Thunder::meet(EventCustom* event)
{
	
	auto p = (int*)(event -> getUserData());
	this->removeChild(enemys[*p]);
	enemys[*p] = NULL;
}

bool Thunder::onTouchBegan(Touch *touch, Event *unused_event) {
    //
	return true;
}

void Thunder::onTouchMoved(Touch *touch, Event *unused_event) {
    //
}

void Thunder::onTouchEnded(Touch *touch, Event *unused_event) {
    //
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {

    switch (code) {
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
			upMove = true;
            break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			downMove = true;
			break;
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
			leftMove = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
			rightMove = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            fire = true;
            break;
        default:
            break;
    }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		upMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		downMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		leftMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		rightMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		fire = false;
		break;
	default:
		break;
	}
}
