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

#include <iostream>

using namespace std;


string parseUName(string text, bool full = false) {
    string ret = "";
    for (int i = 0; i < text.length(); i++) {
        if (!isalnum(text.at(i)) && text.at(i) != '_')
            break;
        ret += text.at(i);
    }
    if (full && !ret.empty()) {
        ret = '@' + ret;
        ret = "\033[4m" + ret + "\033[0m";
    }
    return ret.empty() ? text : ret;
}

string parseShaTag(string text, bool full = false) { // For parsing #tags
    string ret = "";
    string exclChars = "!?.,{}()[];#\'";
    for (int i = 0; i < text.length(); i++) {
        if (exclChars.find(text.at(i), 0) >= 0 &&
                exclChars.find(text.at(i), 0) <= exclChars.length())
            break;
        ret += text.at(i);
    }

    if (full && !ret.empty()) {
        ret = '#' + ret;
        ret = "\033[1m" + ret + "\033[0m";
    }
    return ret.empty() ? text : ret;
}

string parseMessage(string in) {
    string ret = "";
    unsigned int i = 0;
    while (i < in.length()) {
        if (in.at(i) == '@') {
            string n = in.substr(i + 1, in.find_first_of(' ', i) - i);
            ret += parseUName(n, true);
            i += parseUName(n, false).length();
        } else if (in.at(i) == '#') {
            string n = in.substr(i + 1, in.find_first_of(' ', i) - i);
            ret += parseShaTag(n, true);
            i += parseShaTag(n, false).length();
        } else
            ret += in.at(i);
        i++;
    }
    return ret.empty() ? in : ret;
}
