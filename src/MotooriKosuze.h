/*是的就这样（
为bot提供接口
*/
#include <dpp/dpp.h>

namespace MK {
	class MotooriKosuze;
	class Function;
}

//该类设计提供配置文件路径、本体本身
class MK::MotooriKosuze {
public:
	MotooriKosuze();
	dpp::activity* GetM_robot();
private:
	dpp::activity* m_robot;
	std::string token;
};