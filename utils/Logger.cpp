#include <ctime>
#include <cstdio>
#include <filesystem>
#include "logger.hpp"


std::string currentDateTime() {
    const int BUFFER_SIZE = 20; // YYYY-MM-DD_HH-MM-SS\0
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);

    char buffer[BUFFER_SIZE];
    std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d_%02d-%02d-%02d",
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);

    return std::string(buffer);
}


void Logger::dumpToStream(std::ostream& o) const {
    for (const auto& [error, message] : errors) {
        o << message << " (" << static_cast<int>(error) << ")" << std::endl;
    }
}


Logger::Logger(const std::string& filename) {
    std::filesystem::path logPath;

    if (filename.size() == 0) { logPath = "./logs" / std::filesystem::path("logs_" + currentDateTime() + ".txt"); }
    else                      { logPath = std::filesystem::path(filename); }

    logFile.open(logPath, std::ios::out);

    if (!logFile.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier de log." << std::endl;
    }
}


Logger::~Logger() {
    writeToFile();
    logFile.close();
}


void Logger::set_error(Error err, std::string msg) {
    msg = "[" + currentDateTime() + "] - " + msg;
    errors.push_back({err, msg});
    if (errors.size() >= MAX_BUFFER_SIZE) {
        writeToFile();
    }
}


void Logger::writeToFile() {
    if (!logFile.is_open()) return;
    this->dumpToStream(logFile);
    errors.clear();
}
