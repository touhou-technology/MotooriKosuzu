/*是的就这样（
为bot提供接口
*/
#include <dpp/dpp.h>

namespace MK {
	class MotooriKosuze;
	class Function;
}

class MK::MotooriKosuze {
public:
	MotooriKosuze();
	void GetInstance();
private:
	dpp::activity* m_rubot;
	Function* m_Fn;
};