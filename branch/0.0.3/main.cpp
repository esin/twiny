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
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>

#include "twiny.h"

using namespace std;

void printUsage() {
    cout << "Twiny didn't found new status" << endl;
    cout << "Usage: twiny new status" << endl;
}

void isItU(string uname) {
    getInfo(uname);
    if (!yesNo("Is it information really about you? [y/n]: "))
        firstRun();
}

void firstRun() {
    cout << "It's first run of Twiny, so you must enter your username or e-mail: ";
    cin >> tw_uname;
    cout << "and your password: ";
    tw_pass = inPass();
    isItU(tw_uname);
    applySettings(tw_uname, tw_pass);
    saveSettings();
}

string getHome() {
    char* home = getenv("HOME");
    if (home)
        return string(getenv("HOME")) + '/';
    return string();
}

void checkFR() {
    string path = getHome() + ".twinyrc";
    rcFilePath = path;
    if (access(path.c_str(), 0) == 0)
        loadSettings();
    else
        firstRun();
}


void runCommand(vector<string> commands) {
    string comm;
    comm.clear();
    string param = "";
    param.clear();
    for (int i = 0; i < commands.size(); i++) {

        if (comm.empty()) {
            comm = commands.at(i);
            continue;
        }
        if (!comm.empty())
            param += commands.at(i) + ' ';
    }
    if (!comm.empty()) {
        // Commands with parametrs.
        if (!param.empty()) {
            param.erase(param.length() - 1, 1);

            if (comm == "/info")
                getInfo(param);
            if (comm == "/twit")
                setStatus(param);
        }
        // Commands without parametrs.
        if (comm == "/quit") exit(0);
        if (comm == "/info" && param.empty() && !tw_uname.empty()) getInfo(tw_uname);
        if (comm == "/help") printUsage();
    }
}

void parseCommand(string in) {
    vector<string> comms;

    if (!in.empty()) {
        if (in.at(in.length() - 1) != ' ') in += ' ';
        while (in.find(' ') <= in.length() + 1) {
            size_t pos = in.find(' ');
            string comm = in.substr(0, pos);
            comms.push_back(comm);
            in.erase(0, pos + 1);
        }
        runCommand(comms);
    }
}

void inputCommand() {
    for (;;) {
        cout << "\033[1mω>\033[0m ";
        char in[250];
        cin.getline(in, 250);
        cout << "\n\033M";
        parseCommand(in);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        checkFR(); // Check for first run.
        tw_status = genStr(argc, argv);
        if (checkLen(tw_status)) {

            try {
                setStatus(tw_status);
            } catch (happyhttp::Wobbly& e) {
                cout << "Network exception: " << e.what() << endl;
            }

        }
    } else {
        checkFR(); // Check for first run.
        inputCommand();
    }

    return 0;
}
