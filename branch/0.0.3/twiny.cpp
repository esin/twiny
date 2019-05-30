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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "twiny.h"

#define tw_version "0.0.3"

string tw_uname;
string tw_pass;
string tw_status;
string rcFilePath;

string resp; //Response of server

bool forInfo = false;
int status = 0;

void OnBegin(const happyhttp::Response* r, void* userdata) {
    status = r->getstatus();
}

void OnData(const happyhttp::Response* r, void* userdata, const unsigned char* data, int n) {
    //  fwrite(data, 1, n, stdout);
    resp.append((const char*) (data));
}

void OnComplete(const happyhttp::Response* r, void* userdata) {
    switch (status) {
        case 200:
            if (forInfo) {
                parseInfo(resp);
                resp.clear();
                break;
            }
            cout << "OK" << endl;
            saveSettings();
            break;
        case 401:
        {
            cout << "Error: Wrong username or password." << endl;
            if (yesNo("You can try to enter new username/e-mail again. Do you? [y/n]: ")) {
                tw_uname.clear();
                tw_pass.clear();
                UnamePassEmpty();
                setStatus(tw_status);
            }
            break;
        }
        case 403:
            cout << "Error: You have reached the limit of updating status." << endl;
                       exit(0);
            break;
        case 500:
            cout << "Internal Server Error" << endl;
            exit(0);
            break;

        default:
            cout << "Twiny didn't found description of error" << endl;
            cout << "Please send this to author: " << endl;
            cout << r->getstatus() << " " << r->getreason() << endl;
    }


    /*
    # 1,000 total updates per day, on any and all devices (web, mobile web, phone, API, etc. )
    # 250 total direct messages per day, on any and all devices
    # 150 API requests per hour
     */
}

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

bool checkLen(std::string mes) {
    int len = mes.length();
    if (len > 140 || len < 1) {
        cout << "Status is too long (" << len << " chars), but can be only 140 chars." << endl;
        return false;
    }
    return true;
}

void applySettings(std::string uname, std::string pass) {
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
        rcFile << "Username = " << base64_encode(reinterpret_cast<const unsigned char*> (tw_uname.c_str()), tw_uname.length()) << endl;
        rcFile << "Password = " << base64_encode(reinterpret_cast<const unsigned char*> (tw_pass.c_str()), tw_pass.length()) << endl;
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

string genStr(int argc, char* argv[]) {
    string str;
    string str2;
    for (int i = 1; i < argc; i++)
        str += str2.assign(argv[i]) + ' ';
    str = str.erase(str.length() - 1, 1);
    return str;
}

string toPercent(std::string str) {
    string ret;
    static const char HEX[17] = "0123456789ABCDEF";
    string incSymb = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int b = 0, e;

    while ((e = str.find_first_not_of(incSymb, b)) != string::npos) {
        ret.append(str, b, e - b);

        ret += '%';
        ret += HEX[(str[e] >> 4) & 0xF];
        ret += HEX[(str[e] >> 0) & 0xF];

        b = e + 1;
    }
    ret.append(str, b, str.length() - b);

    return ret;
}


#include <termios.h>

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

void setStatus(std::string mes) {
    string params = "status=" + toPercent(mes) + "&source=_twiny_";
    int len = params.length();

    string enc;
    enc = tw_uname + ':' + tw_pass;
    enc = base64_encode(reinterpret_cast<const unsigned char*> (enc.c_str()), enc.length());

    happyhttp::Connection conn("www.twitter.com", 80);
    conn.setcallbacks(OnBegin, 0, OnComplete, 0);

    conn.putrequest("POST", "/statuses/update.xml");
    conn.putheader("Authorization", "Basic " + enc);
    conn.putheader("Accept", "text/plain");
    conn.putheader("Accept-Charset", "utf-8");
    conn.putheader("User-Agent", "_twiny_");
    conn.putheader("X-Twitter-Client", "_twiny_");
    conn.putheader("X-Twitter-Client-Version", tw_version);
    conn.putheader("X-Twitter-Client-URL", "http://twiny.googlecode.com/files/twiny.xml");
    conn.putheader("Connection", "close");
    conn.putheader("Content-Length", len);

    conn.endheaders();

    conn.send((const unsigned char*) params.c_str(), len);

    while (conn.outstanding())
        conn.pump();
}

string tagParse(string tag, string text) {
    string ret = "";
    size_t lt = text.find("<" + tag + ">", 0) + tag.length() + 2;
    size_t rt = text.find("</" + tag + ">", 0);
    ret = text.substr(lt, rt - lt);
    ret = HTMLUnescape(ret);
    return ret;
}

void parseInfo(std::string source) {
    /*      Tag name, value*/
    multimap<string, string> mmm;

    mmm.insert(pair<string, string > ("Screen Name", tagParse("screen_name", source)));
    mmm.insert(pair<string, string > ("Following", tagParse("friends_count", source)));
    mmm.insert(pair<string, string > ("Followers", tagParse("followers_count", source)));
    mmm.insert(pair<string, string > ("URL", tagParse("url", source)));
    mmm.insert(pair<string, string > ("Name", tagParse("name", source)));
    mmm.insert(pair<string, string > ("Description", tagParse("description", source)));
    mmm.insert(pair<string, string > ("Time Zone", tagParse("time_zone", source)));


    showInfo(mmm);
}

void showInfo(multimap<string, string> mInfo) {
    multimap<string, string>::iterator it;
    for (it = mInfo.begin(); it != mInfo.end(); it++)
        cout << "\033[1m" << (*it).first << "\033[0m:\t" << (*it).second << endl;
}

void getInfo(string userN) { //For getting info about user.
    forInfo = true;
    happyhttp::Connection conn("www.twitter.com", 80);
    conn.setcallbacks(OnBegin, OnData, OnComplete, 0);
    string req = "/users/show/" + userN + ".xml";
    conn.request("GET", req.c_str(), 0, 0, 0);

    while (conn.outstanding())
        conn.pump();
    forInfo = false;
}

bool isNum(string str) {
    for (int i = 0; i < str.length(); i++)
        if (!isdigit(str.at(i)))
            return false;
    return true;
}

string HTMLUnescape(string str) {
    string ret;
    int i = 0;
    while (i < str.length()) {
        if (str.at(i) == '&' && str.at(i + 1) == '#') {
            string code;
            for (int j = i + 2; j < i + 6 + 2; j++) {
                if (str.at(j) == ';')
                    break;
                code += str.at(j);
            }
            if (isNum(code)) {
                istringstream i(code);
                unsigned int c;
                i >> c;
                if (c >= 0x800) {
                    ret += 0xE0 | c >> 12 & 0x0F;
                    ret += 0x80 | c >> 6 & 0x3F;
                    ret += 0x80 | c & 0x3F;

                } else if (c >= 0x80) {
                    ret += 0xC0 | c >> 6 & 0x1F;
                    ret += 0x80 | c & 0x3F;
                } else {
                    ret += c;
                }
            }
            i += code.length() + 2;

        } else {
            ret += str.at(i);
        }
        i++;
    }
    return ret;
}


// /info user
// new status
// 
