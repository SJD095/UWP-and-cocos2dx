#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Breakout :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Breakout);

    //“Ù¿÷º”‘ÿ”Î≤•∑≈
    Sprite* player;
	PhysicsWorld* m_world;
	Size visibleSize;
	Vector<PhysicsBody*> enemys;

    void preloadMusic();
    void playBgm();

    void addBackground();
    void addEdge();
	void addPlayer();

	void addContactListener();
	void addKeyboardListener();

	void update(float f);
	void update2(float f);
	void update3(float f);

	bool onConcactBegan(PhysicsContact& contact);
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

	void newEnemys();
	void addEnemy(int type, Point p);

	int time_limit = 30;
	cocos2d::Label* time;
	cocos2d::ProgressTimer* pT;
	int shoot_allow = 0;
	bool fire = false;
	void Breakout::fires(float f);
	std::vector<Sprite*> bullets;
};