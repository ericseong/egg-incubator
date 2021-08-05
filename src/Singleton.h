// Singleton.h

#ifndef __SINGLETON_H__
#define __SINGLETON_H__
 
template<typename T> class Singleton {
private:
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;
protected:
	Singleton() {} 
public:
	static T& getInstance() { 
		static T instance;
		return instance;
	}
};

#endif
// EOF

