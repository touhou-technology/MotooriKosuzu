//主要的对象模型
//Pen为代理类


#pragma once
#include <unordered_map>

#include <dpp/dpp.h>

//随着时间小时的键值对应的消息映射
class {

};

//贴纸-存放翻译频道对象(信息队列，消息映射)
class test {
public:
	test();

	~test() = default;
public:


private:
	std::unordered_map<dpp::snowflake, dpp::snowflake> Link;
};

//webhook version
class ObjWeb {
public:
	ObjWeb(dpp::webhook WK);

	~ObjWeb() = default;
public:


private:
	std::pair<dpp::snowflake, dpp::webhook> Send;

	std::unordered_map<dpp::snowflake, dpp::snowflake> Link;
};