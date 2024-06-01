/*🖊的作用你应该晓得
* 作为书记更改
*/
#pragma once
#include <jsoncpp/json/json.h>
#include <iostream>

//配置
namespace Pen {
	class ConfigPen {
	public:
		static Json::Value ReadFile(std::string Path);
	};
}