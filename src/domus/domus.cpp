/**
 * @file domus.cpp
 * @brief Defines main() for the domusd program.
 *
 * Copyright (c) 2012 George Sudarkoff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */


#include "domus.hpp"

#include <iostream>
#include <string>

namespace domus {
    extern const char * gitVersion();
    extern std::string sysInfo();
}

#include <boost/program_options.hpp>

namespace po = boost::program_options;

void show_help_text(po::options_description options) {
    std::cout << options << std::endl;
};

int main(int argc, char* argv[]) {
    po::options_description general_options("General options");
    po::positional_options_description positional_options;

    general_options.add_options()
    ("help", "produce this help message")
    ("version", "print version information")
    ("sysinfo", "print system information")
    ;

    if( argc == 1 )
        std::cout << argv[0] << " --help for help and startup options" << std::endl;

    {
        po::variables_map params;
        po::store(po::parse_command_line(argc, argv, general_options), params);
        po::notify(params);

        if (params.count("help")) {
            show_help_text(general_options);
            return 0;
        }
        if (params.count("version")) {
            std::cout << domus::Version() << "-"
                      << domus::gitVersion() << std::endl;
            return 0;
        }
        if (params.count("sysinfo")) {
            std::cout << domus::sysInfo() << std::endl;
            return 0;
        }
    }

    return 0;
}
