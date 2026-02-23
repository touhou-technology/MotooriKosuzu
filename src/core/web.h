#ifndef WEB_H
#define WEB_H
#include <nlohmann/json.hpp>
#include <vector>

class UseAPI {
  public:
	enum class TranslateAPI { deepl = 0, google = 1 };

	UseAPI() = default;

 std::vector<std::string> ReflectionAPI();
};

#endif /* WEB_H */
