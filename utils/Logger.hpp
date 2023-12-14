#include <vector>
#include <iostream>
#include <string>
#include <fstream>

std::string currentDateTime();

class Logger {

public:

    enum class Error : int {
        OK,
        ERROR_TYPE_1,
        ERROR_TYPE_2,
        ERROR_TYPE_3,
        ERROR_TYPE_4
    };

private:

    std::vector<std::pair<Error, std::string>> errors;
    const size_t MAX_BUFFER_SIZE = 100;
    std::ofstream logFile;

    void dumpToStream(std::ostream& o) const;

public:

    Logger(const std::string& filename = "");
    ~Logger();

    void set_error(Error err, std::string msg = "");
    void writeToFile();
    inline void printErrors() const { this->dumpToStream(std::cerr); }
    inline std::pair<Error, std::string> last_error() const { return this->errors.back(); }
};
