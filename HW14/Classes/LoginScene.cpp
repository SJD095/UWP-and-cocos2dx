#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}





// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
    this->addChild(textField, 2);

    auto button = Button::create();
    button->setTitleText("Login");
    button->setTitleFontSize(30);
    button->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	button->addClickEventListener(CC_CALLBACK_1(LoginScene::login, this));
    this->addChild(button, 2);

	auto auto_button = Button::create();
	auto_button->setTitleText("Auto Login");
	auto_button->setTitleFontSize(30);
	auto_button->setPosition(Size(visibleWidth / 2, visibleHeight / 2 - 40));
	auto_button->addClickEventListener(CC_CALLBACK_1(LoginScene::auto_login, this));
	this->addChild(auto_button, 2);

    return true;
}
//点击Login的正常登录
void LoginScene::login(cocos2d::Ref* pSender)
{
	string username = "username=" + textField->getStringValue();
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::OnHttpRequestCompleted, this));

	database->setStringForKey("cookies", textField->getStringValue());

	const char* postData = username.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST username");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
void LoginScene::OnHttpRequestCompleted(HttpClient *sender, HttpResponse * response)
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
	//存储返回HTTP响应报文的header部分
	string header_data = "";

	std::vector<char> *buffer2 = response->getResponseHeader();
	for (int i = 0; i < buffer2->size(); i++)
	{
		header_data += (*buffer2)[i];
	}

	int subscript = header_data.find("GAMESESSIONID");
	string cookie = "";
	for (; header_data[subscript] != '\r'; subscript++)
	{
		cookie += header_data[subscript];
	}
	//存储登陆后服务器为用户分配的cookie，下次运行应用可直接点击Auto Login登陆
	database->setStringForKey("cookies", cookie);

//  header sample
//  Server: Apache - Coyote / 1.1
//	Set - Cookie : GAMESESSIONID = 8ca610711bc37b6c2ddf9ae08fcc6dfb
//	Content - Type : application / json; charset = UTF - 8
//	Transfer - Encoding: chunked
//	Date : Sun, 29 May 2016 13 : 53 : 35 GMT

	rapidjson::Document d;
	d.Parse<0>(response_data.c_str());

	if (d["result"].GetBool() == true)
	{
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.8, scene, Color3B(0, 0, 0)));
	}
	
	return;
}
//使用cookie登陆
void LoginScene::auto_login(cocos2d::Ref* pSender)
{
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	//事实上这里发送cookie的部分并没有起到效果，服务器似乎并不检查Login阶段发送的cookie，如果不设置username,返回结果部分result对应值一定为false
	headers.push_back("Cookies: " + database->getStringForKey("cookies"));
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::OnHttpRequestCompleted2, this));

	request->setTag("POST cookies");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginScene::OnHttpRequestCompleted2(HttpClient *sender, HttpResponse * response)
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

	//这里由于POST到服务器的数据段没有提供username，故返回json字符串result对应的值必为false,但在GameScene中的操作均会发送cookie，不涉及用户名，所以取消对result对应值的检验
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.8, scene, Color3B(0, 0, 0)));

	return;
}

