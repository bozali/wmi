#pragma once


namespace wmi {


class NonCopyable
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;

private:
	NonCopyable(const NonCopyable&) = default;
	NonCopyable& operator=(const NonCopyable&) = default;
};

}
