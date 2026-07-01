#ifndef WEB_H
#define WEB_H
#include <nlohmann/json.hpp>
#include <variant>
#include <vector>

class UseAPI {
  public:
	enum class TranslateAPI { deepl = 0, google = 1 };

	UseAPI() = default;

	// Reflection Translate API to String
	static consteval std::vector<std::string> ReflectionAPI();
};

#endif /* WEB_H */
