#pragma once
#include <dpp/dpp.h>
#include <coroutine>

class InitVoice {
public:
	static void Init();
};

class TranslateVoice {
public:
	struct promise_type;
	struct Specification {
		std::string language;
		std::string model;
		dpp::snowflake ID;
	};

	TranslateVoice(const TranslateVoice&) = delete;

	TranslateVoice(std::coroutine_handle<promise_type> h)
		:handle(h) {
	};

	~TranslateVoice()
	{
		//!自行销毁
		if (handle)
			handle.destroy();
	}

	struct promise_type {
		promise_type() = default;
		~promise_type() = default;

		//!生成协程返回值
		auto get_return_object(){
			return TranslateVoice{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		std::suspend_always initial_suspend(){
			return {};
		}

		void return_value(char* v){
			return_data_ = v;
			return;
		}

		std::suspend_always yield_value(char* v){
			std::cout << "yield_value invoked." << std::endl;
			return_data_ = v;
			return {};
		}

		std::suspend_always final_suspend() noexcept{
			std::cout << "final_suspend invoked." << std::endl;
			return {};
		}
		//
		void unhandled_exception()
		{
			std::exit(1);
		}
		//返回值
		char* return_data_;
	};//END promise

	void send(const dpp::voice_receive_t& event);

	Specification& Specification_Reset();
	void Specification_Reset(Specification param);

private:
	Specification m_Speci;
	std::coroutine_handle<promise_type> handle;
};

class S_TranslateVoiceConfig {
public:
	static void Init();

private:
	static void Slashcommand();
	static void Voice();
	static void AutoComplete();
};