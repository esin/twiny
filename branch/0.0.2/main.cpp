/***************************************************************************
 *   Copyright (C) 2009 by Andrey Esin                                     *
 *   gmlastik@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *

 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "twiny.h"

#define version 0.0.2

void printUsage() {
    std::cout << "Error: You didn't enter new status" << std::endl;
    std::cout << "Usage: twiny new status" << std::endl;
}

std::string getHome() {
    char* home = getenv("HOME");
    if (home)
        return std::string(getenv("HOME")) + '/';
    return std::string();
}

void checkFR() {
    std::fstream rc;
    std::string path = getHome() + ".twinyrc";
    setRCFileP(path);
    rc.open(path.c_str(), std::ios::in);

    if (!rc.is_open()) {
        std::string uname, pass;
        std::cout << "It's first run of Twiny, so you must enter your username or e-mail: ";
        std::cin >> uname;
        std::cout << "and your password: ";
        std::cin >> pass;
        applySettings(uname, pass);
        saveSettings();
    } else {
        rc.close();
        loadSettings();
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        checkFR(); // Check for first run.

        std::string twStatus = genStr(argc, argv);
        if (checkLen(twStatus)) {

            try {
                setStatus(twStatus);
            }
            catch (happyhttp::Wobbly& e) {
                std::cout << "Network exception: " << e.what() << std::endl;
            }
            
        }
    } else {
        printUsage();
    }
    return 0;
}