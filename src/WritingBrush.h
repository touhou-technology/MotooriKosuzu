/*🖊的作用你应该晓得
* 作为书记更改
*/
#pragma once
#include <json/json.h>
#include <httplib.h>

//配置
namespace Pen {
	class ConfigPen {
	public:
		static Json::Value ReadFile(std::string Path);
	};

	class WebPen {
	public:

	};
}