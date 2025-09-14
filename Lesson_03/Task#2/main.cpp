#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observed
{
public:
    void AddObserver(Observer* obs)
    {
        if (obs && std::find(observers_.begin(), observers_.end(), obs) == observers_.end())
        {
            observers_.push_back(obs);
        }
    }

    void RemoveObserver(Observer* obs)
    {
        std::vector<Observer*>::iterator it = std::remove(observers_.begin(), observers_.end(), obs);
        observers_.erase(it, observers_.end());
    }

    void warning(const std::string& message) const
    {
        notifyWarning(message);
    }

    void error(const std::string& message) const
    {
        notifyError(message);
    }

    void fatalError(const std::string& message) const
    {
        notifyFatal(message);
    }

private:
    void notifyWarning(const std::string& m) const
    {
        for (std::size_t i = 0; i < observers_.size(); ++i)
        {
            if (observers_[i]) observers_[i]->onWarning(m);
        }
    }
    void notifyError(const std::string& m) const
    {
        for (std::size_t i = 0; i < observers_.size(); ++i)
        {
            if (observers_[i]) observers_[i]->onError(m);
        }
    }
    void notifyFatal(const std::string& m) const
    {
        for (std::size_t i = 0; i < observers_.size(); ++i)
        {
            if (observers_[i]) observers_[i]->onFatalError(m);
        }
    }

private:
    std::vector<Observer*> observers_;
};

class WarningObserver : public Observer
{
public:
    void onWarning(const std::string& message) override
    {
        std::cout << "[WARN] " << message << std::endl;
    }
};

class ErrorObserver : public Observer
{
public:
    explicit ErrorObserver(const std::string& path) : path_(path) {}
    void onError(const std::string& message) override
    {
        std::ofstream out(path_, std::ios::app);
        if (out) out << "[ERROR] " << message << std::endl;
    }
private:
    std::string path_;
};

class FatalObserver : public Observer
{
public:
    explicit FatalObserver(const std::string& path) : path_(path) {}
    void onFatalError(const std::string& message) override
    {
        std::cout << "[FATAL] " << message << std::endl;
        std::ofstream out(path_, std::ios::app);
        if (out) out << "[FATAL] " << message << std::endl;
    }
private:
    std::string path_;
};

int main()
{
    Observed logger;

    WarningObserver w;
    ErrorObserver e("errors.txt");
    FatalObserver f("fatal.txt");

    logger.AddObserver(&w);
    logger.AddObserver(&e);
    logger.AddObserver(&f);

    logger.warning("Low disk space");
    logger.error("File not found");
    logger.fatalError("Out of memory");

    logger.RemoveObserver(&w);

    return 0;
}
