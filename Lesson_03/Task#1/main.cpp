#include <iostream>
#include <fstream>
#include <string>

class LogCommand
{
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand
{
public:
    void print(const std::string& message) override
    {
        std::cout << message << std::endl;
    }
};

class FileLogCommand : public LogCommand
{
public:
    explicit FileLogCommand(const std::string& path)
        : path_(path)
    {
    }

    void print(const std::string& message) override
    {
        std::ofstream out(path_, std::ios::app);
        if (out)
        {
            out << message << std::endl;
        }
    }

private:
    std::string path_;
};

void print(LogCommand& cmd)
{
    cmd.print("Hello from Command!");
}

int main()
{
    ConsoleLogCommand toConsole;
    print(toConsole);

    FileLogCommand toFile("log.txt");
    print(toFile);

    return 0;
}
