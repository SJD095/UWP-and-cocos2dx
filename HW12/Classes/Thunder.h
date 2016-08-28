#ifndef __THUNDER_H__
#define __THUNDER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <vector>

USING_NS_CC;

class Thunder : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Thunder);

    void preloadMusic();
    void playBgm();
    
    void addTouchListener();
    void addKeyboardListener();

    void addEnemy(int n);
    void update(float f);
	void update2(float f);
    void fires(float f);
    void meet(EventCustom* event);
    void addCustomListener();

    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

	int move;
    Size visibleSize;
	Sprite* player;
	std::vector<Sprite*> bullets;
    std::vector<Sprite*> enemys;
	bool leftMove = false;
	bool rightMove = false;
	bool upMove = false;
	bool downMove = false;
	bool moved = true;
	bool fire = false;
	double count = 0;
};

#endif // __THUNDER_H__
