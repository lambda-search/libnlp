
#pragma once

#include "tclap/CmdLine.h"

class CmdLineOutput : public TCLAP::StdOutput {
public:
    virtual void usage(TCLAP::CmdLineInterface &cmd) {
        std::cout << std::endl
                  << cmd.getMessage() << std::endl
                  << "Author: Carbo Kuo <byvoid@byvoid.com>" << std::endl
                  << "Bug Report: http://github.com/lambda-search/libnlp/issues"
                  << std::endl
                  << std::endl
                  << "Usage: " << std::endl
                  << std::endl;

        _shortUsage(cmd, std::cout);
        std::cout << std::endl;
        std::cout << "Options: " << std::endl << std::endl;
        _longUsage(cmd, std::cout);
        std::cout << std::endl;
    }

    virtual void version(TCLAP::CmdLineInterface &cmd) {
        std::cout << std::endl
                  << cmd.getMessage() << std::endl
                  << "Version: " << cmd.getVersion() << std::endl
                  << std::endl;
    }
};
