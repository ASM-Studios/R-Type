#include "SystemMetrics.hpp"

namespace GUI {
    SystemMetrics::SystemMetrics(GUI::WindowManager& windowManager, const sf::RenderWindow *window)
        : memoryUsage(0), cpuUsage(0.0f), _windowManager(windowManager) {
        _windowManager._addText("ping", "PING: 0 ms", {static_cast<float>(window->getSize().x - 250), 50.F});
        _windowManager._addText("memoryUsage", "Memory: 0 KB", {static_cast<float>(window->getSize().x - 250), 90.F});
        _windowManager._addText("cpuUsage", "CPU: 0%", {static_cast<float>(window->getSize().x - 250), 130.F});
    }

    SystemMetrics::~SystemMetrics() {}

    void SystemMetrics::update() {
        if (_timePerTick < 5.0F) {
            _timePerTick += 0.1F;
            return;
        }
        _timePerTick = 0;
        memoryUsage = getMemoryUsage();
        cpuUsage = getCPUUsage();

        double const memoryUsageGB = static_cast<double>(memoryUsage) /  (1024 * 1024);;
        char buffer[64];

        snprintf(buffer, sizeof(buffer), "Memory: %.2f GB", memoryUsageGB);
        if (auto memoryText = _windowManager._getText("memoryUsage")) {
            memoryText->setString(buffer);
        }

        snprintf(buffer, sizeof(buffer), "CPU: %.0f%%", cpuUsage);
        if (auto cpuText = _windowManager._getText("cpuUsage")) {
            cpuText->setString(buffer);
        }
        snprintf(buffer, sizeof(buffer), "PING: %d ms", pingValue);
        if (auto pingText = _windowManager._getText("ping")) {
            pingText->setString(buffer);
        }
    }


    size_t SystemMetrics::getMemoryUsage()
    {
        #ifdef _WIN32
            PROCESS_MEMORY_COUNTERS memCounter;
            if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter))) {
                return memCounter.WorkingSetSize / 1024;
            }
        #elif __linux__
            struct rusage usage;
            if (getrusage(RUSAGE_SELF, &usage) == 0) {
                return usage.ru_maxrss;
            }
        #endif
            return 0;
    }

    float SystemMetrics::getCPUUsage() {
        #ifdef _WIN32
            static FILETIME prevSysIdle, prevSysKernel, prevSysUser;
            FILETIME sysIdle, sysKernel, sysUser;

            if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser)) {
                return -1.0f;
            }

            if (memcmp(&prevSysIdle, &sysIdle, sizeof(FILETIME)) != 0) {
                ULONGLONG sysIdleDiff = (sysIdle.dwLowDateTime - prevSysIdle.dwLowDateTime) +
                                        ((sysIdle.dwHighDateTime - prevSysIdle.dwHighDateTime) << 32);
                ULONGLONG sysKernelDiff = (sysKernel.dwLowDateTime - prevSysKernel.dwLowDateTime) +
                                          ((sysKernel.dwHighDateTime - prevSysKernel.dwHighDateTime) << 32);
                ULONGLONG sysUserDiff = (sysUser.dwLowDateTime - prevSysUser.dwLowDateTime) +
                                        ((sysUser.dwHighDateTime - prevSysUser.dwHighDateTime) << 32);

                ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
                if (sysTotal == 0)
                    return 0.0f;
                float cpuUsage = (sysTotal - sysIdleDiff) / (float)sysTotal * 100.0f;

                prevSysIdle = sysIdle;
                prevSysKernel = sysKernel;
                prevSysUser = sysUser;

                return cpuUsage;
            }
        #elif __linux__
            static long lastTotalUser = 0, lastTotalUserLow = 0, lastTotalSys = 0, lastTotalIdle = 0;


            std::ifstream file("/proc/stat");
            if (!file.is_open()) {
                std::cerr << "Failed to open /proc/stat" << std::endl;
                return 0.0f;
            }
            std::string cpuLabel;
            long user, nice, system, idle, iowait, irq, softirq, steal;
            file >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
            file.close();

            if (cpuLabel != "cpu") {
                std::cerr << "Unexpected label in /proc/stat: " << cpuLabel << std::endl;
                return 0.0f;
            }
            long totalUser = user;
            long totalUserLow = nice;
            long totalSys = system;
            long totalIdle = idle;
            long total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
                         (totalSys - lastTotalSys);
            long totalIdleDiff = totalIdle - lastTotalIdle;
            if ((total + totalIdleDiff) == 0) {
                return 0.0f;
            }

            float cpuUsage = (total * 100.0f) / (total + totalIdleDiff);

            lastTotalUser = totalUser;
            lastTotalUserLow = totalUserLow;
            lastTotalSys = totalSys;
            lastTotalIdle = totalIdle;
            return cpuUsage;
        #endif
            return 0.0f;
    }
}