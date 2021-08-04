// Singleton.cpp

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

// EOF

