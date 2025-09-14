#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>

class LogMessage
{
public:
    enum class Type
    {
        Warning,
        Error,
        Fatal,
        Unknown
    };

    LogMessage(Type t, const std::string& m) : type_(t), msg_(m) {}

    Type type() const
    {
        return type_;
    }

    const std::string& message() const
    {
        return msg_;
    }

private:
    Type type_;
    std::string msg_;
};

class Handler
{
public:
    explicit Handler(std::unique_ptr<Handler> next) : next_(std::move(next)) {}
    virtual ~Handler() = default;

    void handle(const LogMessage& lm)
    {
        if (process(lm))
        {
            return;
        }
        if (next_)
        {
            next_->handle(lm);
            return;
        }
        // Если дошли до конца
        throw std::runtime_error("Unhandled log message: " + lm.message());
    }

protected:
    virtual bool process(const LogMessage& lm) = 0;

private:
    std::unique_ptr<Handler> next_;
};

// ФАТАЛЬНАЯ
class FatalHandler : public Handler
{
public:
    using Handler::Handler;

protected:
    bool process(const LogMessage& lm) override
    {
        if (lm.type() != LogMessage::Type::Fatal)
        {
            return false;
        }
        throw std::runtime_error(lm.message());
    }
};

// ОШИБКА
class ErrorHandler : public Handler
{
public:
    ErrorHandler(const std::string& path, std::unique_ptr<Handler> next)
        : Handler(std::move(next)), path_(path)
    {
    }

protected:
    bool process(const LogMessage& lm) override
    {
        if (lm.type() != LogMessage::Type::Error)
        {
            return false;
        }
        std::ofstream out(path_, std::ios::app);
        if (out)
        {
            out << "[ERROR] " << lm.message() << std::endl;
        }
        return true;
    }

private:
    std::string path_;
};

// ПРЕДУПРЕЖДЕНИЕ
class WarningHandler : public Handler
{
public:
    using Handler::Handler;

protected:
    bool process(const LogMessage& lm) override
    {
        if (lm.type() != LogMessage::Type::Warning)
        {
            return false;
        }
        std::cout << "[WARN] " << lm.message() << std::endl;
        return true;
    }
};

// исключение
class UnknownHandler : public Handler
{
public:
    using Handler::Handler;

protected:
    bool process(const LogMessage& lm) override
    {
        if (lm.type() != LogMessage::Type::Unknown)
        {
            return false;
        }
        throw std::runtime_error("Unknown message: " + lm.message());
    }
};

int main()
{
    // цепочка
    std::unique_ptr<Handler> chain =
        std::unique_ptr<Handler>(
            new FatalHandler(
                std::unique_ptr<Handler>(
                    new ErrorHandler("errors.log",
                        std::unique_ptr<Handler>(
                            new WarningHandler(
                                std::unique_ptr<Handler>(
                                    new UnknownHandler(nullptr))))))));

    try
    {
        chain->handle(LogMessage(LogMessage::Type::Warning, "Disk is almost full"));
        chain->handle(LogMessage(LogMessage::Type::Error, "Config missing"));
        // исключение и завершение работы
        chain->handle(LogMessage(LogMessage::Type::Fatal, "Critical failure"));
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    // исключение
    try
    {
        chain->handle(LogMessage(LogMessage::Type::Unknown, "???"));
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
