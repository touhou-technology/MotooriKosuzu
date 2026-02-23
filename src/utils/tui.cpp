#include <iostream>
#include <memory>
#include <vector>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

template <typename T>
class ConcreteCommand : public Command {
public:
    explicit ConcreteCommand(T s)
	: strategy(std::move(s)){};

	void execute() override {
		strategy.init();
	}

private:
    T strategy;
};

class PrintTest {
public:
    PrintTest() = default;
    void init() {
        std::cout << "Test\n";
    }
};

class PrintNum {
public:
    PrintNum() = default;
    void init() { std::cout << value << std::endl; }

private:
	int value = {3};
};

class TestVoid {
public:
    int value = 0;
	void init() {

	}
};

int main(int argc, char *argv[]) {
	std::vector<std::unique_ptr<Command>> commands;
	commands.push_back(std::make_unique<ConcreteCommand<PrintTest>>(PrintTest{}));
	commands.push_back(std::make_unique<ConcreteCommand<PrintNum>>(PrintNum{}));
	commands.push_back(
		               std::make_unique<ConcreteCommand<TestVoid>>(TestVoid{}));

	for (auto &cmd : commands) {
		cmd->execute();

	}
    return 0;
}
