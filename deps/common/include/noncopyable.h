#pragma once

class noncopyable{
protected:
	noncopyable() = default;
	~noncopyable() = default;

private:
	noncopyable(noncopyable const&) = delete;
	noncopyable const& operator=(noncopyable const&) = delete;
};
