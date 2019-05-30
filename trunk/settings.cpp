/***************************************************************************
 *   Copyright (C) 2009-2010 by Andrey Esin                                *
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

#include "twiny.h"
#include "settings.h"

using namespace std;

void applySettings(string uname, std::string pass) {
    tw_uname = uname;
    tw_pass = pass;
    UnamePassEmpty();
}

void parseString(string str) {
    size_t pos;
    pos = str.find(' ');
    pos = str.find(' ', pos + 1);
    if (str.find("Username") == 0)
        tw_uname = base64_decode(str.substr(pos + 1, str.length() - pos));
    if (str.find("Password") == 0)
        tw_pass = base64_decode(str.substr(pos + 1, str.length() - pos));
}

void loadSettings() {
    ifstream rcFile;
    rcFile.open(rcFilePath.c_str(), ifstream::in);
    if (rcFile.is_open()) {
        while (!rcFile.eof()) {
            string str;
            getline(rcFile, str);
            if (!str.empty())
                parseString(str);
        }
        rcFile.close();
    } else
        cout << "Unable to open .twinyrc";
    if (tw_uname.empty() || tw_pass.empty())
        UnamePassEmpty();
}

void saveSettings() {
    ofstream rcFile;
    rcFile.open(rcFilePath.c_str(), ifstream::out);
    if (rcFile.is_open()) {
        rcFile << "Username = " << base64_encode(tw_uname) << endl;
        rcFile << "Password = " << base64_encode(tw_pass) << endl;
        rcFile.close();
    } else
        cout << "Error: Unable to open .twinyrc" << endl;
}

void UnamePassEmpty() {
    if (tw_uname.empty()) {
        cout << "Twiny didn\'t found your username or e-mail, so please enter them: ";
        cin >> tw_uname;
    }

    if (tw_pass.empty()) {
        cout << "Twiny didn\'t found your password, so please enter them: ";
        tw_pass = inPass();
    }
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

void firstRun() {
    cout << "It's first run of Twiny, so you must enter your username or e-mail: ";
    cin >> tw_uname;
    cout << "and your password: ";
    tw_pass = inPass();
    applySettings(tw_uname, tw_pass);
    saveSettings();
}
