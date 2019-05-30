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

#ifndef WIN32
#include <termios.h>
#endif

#include "twiny.h"
#include "settings.h"
#include "common.h"

bool yesNo(string question) {
    cout << question;
    string answer;
    cin >> answer;
    if ((answer.at(0) == 'y') || (answer.at(0) == 'Y') || (answer.substr(1, 3) == "yes"))
        return true;
    else if ((answer.at(0) == 'n') || (answer.at(0) == 'N') || (answer.substr(1, 3) == "no"))
        return false;
    else return yesNo(question);
}

string genStr(int argc, char* argv[]) {
    string str;
    string str2;
    for (int i = 1; i < argc; i++)
        str += str2.assign(argv[i]) + ' ';
    str = str.erase(str.length() - 1, 1);
    return str;
}

#ifndef WIN32
struct termios save;

void echoOff() {
    struct termios term;
    tcgetattr(0, &save);
    term = save;
    term.c_lflag &= (~ECHO);
    tcsetattr(0, TCSANOW, &term);
}

void echoOn() {
    tcsetattr(0, TCSANOW, &save);
}
#endif

string inPass() {
    string str;
    char c;

#ifndef WIN32
    echoOff();
#endif

    for (;;) {
#ifdef WIN32
        c = getch();
#else
        c = getchar();
#endif
        if ((c == '\r' || c == '\n') && !str.empty())
            break;
        str += c;
    }
#ifndef WIN32
    echoOn();
#endif
    str = str.substr(1, str.length() - 1);
    cout << endl;
    return str;
}
