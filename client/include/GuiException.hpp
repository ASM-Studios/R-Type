#pragma once

#include <string>
#include <exception>
#include "Logger.hpp"

namespace GUI {
    class GuiException : public std::exception {
    private:
        std::string _msg;

    public:
        GuiException(std::string const &msg) : _msg(msg) {}
        virtual ~GuiException() throw() {}
        virtual const char *what() const throw() { return _msg.c_str(); }
    };

    class guiActionException : public std::exception {
    private:
        std::string _msg;

    public:
        guiActionException(std::string const &msg) : _msg(msg) {}
        virtual ~guiActionException() throw() {}
        virtual const char *what() const throw() { return _msg.c_str(); }
    };
}
