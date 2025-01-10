#pragma once
#include <dpp/dpp.h>
#include <coroutine>
#include <queue>      

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

	TranslateVoice(std::coroutine_handle<promise_type>&& h)
		:handle(h) {

	}

	TranslateVoice(TranslateVoice&& h)
		:handle(h.handle) {
		h.handle = nullptr;
	};

	~TranslateVoice()
	{
		//!自行销毁
		if (handle)
			handle.destroy();
	}

	TranslateVoice& operator=(const TranslateVoice&) = delete;
	TranslateVoice& operator=(TranslateVoice&& s) {
		handle = s.handle;
		s.handle = nullptr;
		return *this;
	}

	struct promise_type {
		promise_type() = default;
		~promise_type() = default;

		//!生成协程返回值
		auto get_return_object() {
			return TranslateVoice{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		std::suspend_never initial_suspend() {
			return {};
		}

		void return_value(char* v) {
			return_data_ = v;
			return;
		}

		std::suspend_always yield_value(char* v) {
			std::cout << "yield_value invoked." << std::endl;
			return_data_ = v;
			return {};
		}

		std::suspend_always final_suspend() noexcept {
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

	char* get();

	void reset_handle(std::coroutine_handle<promise_type> new_handle);

	void test_move_next();

	Specification& Specification_Reset();
	void Specification_Reset(Specification param);

	std::coroutine_handle<promise_type> handle;
private:
	Specification m_Speci;
	std::vector<TranslateVoice> TranslateVoice_vector;
};

class S_TranslateVoiceConfig {
public:
	static void Init();

private:
	static void Slashcommand();
	static void Voice();
	static void AutoComplete();
};