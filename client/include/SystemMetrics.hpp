#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "WindowManager.hpp"

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif __linux__
    #include <sys/resource.h>
    #include <unistd.h>
    #include <fstream>
#endif

extern int pingValue;

namespace GUI {class WindowManager;
    class SystemMetrics {
        public:
        SystemMetrics() = delete;
        explicit SystemMetrics(GUI::WindowManager& windowManager, const sf::RenderWindow *window);
        ~SystemMetrics();

        void update();

        private:
        size_t memoryUsage;
        float cpuUsage;
        WindowManager& _windowManager;
        float _timePerTick = 0;

        size_t getMemoryUsage();
        float getCPUUsage();
    };

}

