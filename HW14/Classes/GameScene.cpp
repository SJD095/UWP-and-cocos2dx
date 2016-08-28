#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <sstream>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	submit_button->addClickEventListener(CC_CALLBACK_1(GameScene::submit, this));
    this->addChild(submit_button, 2);

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
	rank_button->addClickEventListener(CC_CALLBACK_1(GameScene::top, this));
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
    this->addChild(rank_button, 2);

    return true;
}

void GameScene::submit(cocos2d::Ref* pSender)
{
	string username = "score=" + score_field->getStringValue();
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	if (database -> getStringForKey("cookies") != "Gakki") headers.push_back("Cookies: " + database->getStringForKey("cookies"));
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::OnHttpRequestCompleted, this));

	const char* postData = username.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST score");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::OnHttpRequestCompleted(HttpClient *sender, HttpResponse * response)
{
	//just for debug
	if (!response)
	{
		return;
	}

	string response_data = "";

	std::vector<char> *buffer = response->getResponseData();
	for (int i = 0; i < buffer->size(); i++)
	{
		response_data += (*buffer)[i];
	}
}

void GameScene::top(cocos2d::Ref* pSender)
{
	search_count++;
	HttpRequest* request = new HttpRequest();
	char tmp[10];
	itoa(search_count, tmp, 10);
	string url = "http://localhost:8080/rank?top=10&rand=" + (string)tmp;
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	//这里从本地获取之前存入的cookie，也就是上一个用户登录时保存的cookie
	if (database->getStringForKey("cookies") != "" ) headers.push_back("Cookies: " + database->getStringForKey("cookies"));
	string test = database->getStringForKey("cookies");
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::OnHttpRequestCompleted2, this));

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::OnHttpRequestCompleted2(HttpClient *sender, HttpResponse * response)
{
	if (!response)
	{
		return;
	}

	string response_data = "";

	std::vector<char> *buffer = response->getResponseData();
	for (int i = 0; i < buffer->size(); i++)
	{
		response_data += (*buffer)[i];
	}

	rapidjson::Document d;
	d.Parse<0>(response_data.c_str());
	//首先替换'|'方便后面istringstream读取player:score对
	string rank = d["info"].GetString();
	for (int i = 0; i < rank.size(); i++)
	{
		if (rank[i] == '|') rank[i] = ' ';
	}

	std::istringstream players(rank);
	string rank_result = "";
	string something;
	//一次读入一组player:score对，以回车结束
	while (players >> something)
	{
		rank_result += something + '\n';
	}

	rank_field->setText(rank_result);
}