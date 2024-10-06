#include <chrono>
#include <fcntl.h>
#include <filesystem>
#include <fmt/format.h>
#include <random>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <csignal>
#include <mutex>

auto myPipe = "my.fifo";

#ifdef __linux__
auto racketPath = "/usr/bin/racket";
#endif

#ifdef __APPLE__
auto racketPath = "/opt/homebrew/bin/racket";
#endif

std::mutex printMutex;
void mutexPrint(std::string_view s)
{
    std::lock_guard<std::mutex> lock(printMutex);
    fmt::print("{}", s);
}

void ERROR(std::string_view s)
{
    fmt::print("Failed: {} -> {} {}\n", s, errno, strerror(errno));
    std::exit(1);
}

bool file_exists(const char* name)
{
    struct stat buffer;
    return (stat(name, &buffer) == 0);
}

std::string getPath(std::string_view s)
{
    return std::filesystem::weakly_canonical(std::filesystem::path(s)).parent_path().string();
}

void listenThread(FILE* fd)
{
    fmt::print("Started listening \n");

    char* line = nullptr;
    size_t len = 0;

    ssize_t nread = 1;

    for (;;) {
        nread = getline(&line, &len, fd);

        if (nread == 0) {
            break;
        }

        // rm ending \n
        if (line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        mutexPrint(fmt::format("Read {} bytes -> {}\n", nread, line));
        free(line);
        line = nullptr;
        len = 0;
    }
    fmt::print("SHOULD NOT BE REACHED!");
}

void startListening(FILE* fd)
{
    auto* t = new std::thread(listenThread, fd);
    t->detach();
}

int main(int argc, char* argv[])
{
    if (file_exists(myPipe)) {
        if (unlink(myPipe) != 0) {
            ERROR("delete old pipe");
        }
    }

    if (mkfifo(myPipe, S_IRWXU) != 0) {
        ERROR("create pipe");
    }

    // THE MAGIC that tell the system to kill a child once the main process quit/teminrated.
    signal(SIGCHLD, SIG_IGN);

    // Invoke racket's pipe reader
    std::string cmd = fmt::format("{} {}/{}", racketPath, getPath(argv[0]), "randomReceiver.rkt");
    FILE* rktReceiver = popen(cmd.c_str(), "r");
    if (!rktReceiver) {
        ERROR("invoke racket");
    }
    startListening(rktReceiver);

    // Note: this will freeze if the reader has not been created
    int out_fd = open(myPipe, O_WRONLY);
    if (out_fd < 0) {
        ERROR("open pipe");
    }

    // RNG
    std::default_random_engine rng;
    std::uniform_int_distribution<int> getRandom(INT_MIN, INT_MAX);

    for (;;) {
        auto s = fmt::format("{}\n", getRandom(rng));
        mutexPrint(fmt::format("\nBegin write -> {}", s.c_str()));
        if (write(out_fd, s.c_str(), s.size()) < 0) {
            perror("write");
        }

        fsync(out_fd);

        mutexPrint(fmt::format("write {} bytes\n", s.size()));

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}