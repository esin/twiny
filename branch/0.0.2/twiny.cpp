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

#include "twiny.h"

std::string tw_uname = "";
std::string tw_pass = "";
std::string tw_status = "";
std::string rcFilePath = "";

int status = 0;

void OnBegin(const happyhttp::Response* r, void* userdata) {
    status = r->getstatus();
}

void OnData(const happyhttp::Response* r, void* userdata, const unsigned char* data, int n) {
    //fwrite(data, 1, n, stdout);
}

void OnComplete(const happyhttp::Response* r, void* userdata) {
    if (status == 200) {
        std::cout << "OK" << std::endl;
        saveSettings();
    }
    if (status == 401) {
        std::cout << "Error: Wrong username or password." << std::endl;
        std::cout << "You can try to enter new username/e-mail again. Do you? [y/n]: ";
        std::string ans;
        std::cin >> ans;
        if (ans.at(0) == 'y' || ans.at(0) == 'Y') {
            tw_uname.clear();
            tw_pass.clear();
            UnamePassEmpty();
            setStatus(tw_status);
        }

    }
    if (status == 403)
        std::cout << "Error: You have reached the limit of updating status." << std::endl;

    /*
    # 1,000 total updates per day, on any and all devices (web, mobile web, phone, API, etc. )
    # 250 total direct messages per day, on any and all devices
    # 150 API requests per hour
     */
}

void setRCFileP(std::string path)
{
    rcFilePath = path;
}

bool checkLen(std::string mes) {
    int len = mes.length();
    if (len > 140 || len < 1) {
        std::cout << "Status is too long (" << len << " chars), but can be only 140 chars." << std::endl;
        return false;
    }
    return true;
}

void applySettings(std::string uname, std::string pass) {
    tw_uname = uname;
    tw_pass = pass;
    UnamePassEmpty();
}

std::string parseString(std::string str) {
    size_t pos;
    pos = str.find(' ');
    pos = str.find(' ', pos + 1);
    if (str.find("Username") == 0)
        tw_uname = base64_decode(str.substr(pos + 1, str.length() - pos));
    if (str.find("Password") == 0)
        tw_pass = base64_decode(str.substr(pos + 1, str.length() - pos));
}

void loadSettings() {
    std::string str;
    std::ifstream rcFile(rcFilePath.c_str());
    if (rcFile.is_open()) {
        while (!rcFile.eof()) {
            getline(rcFile, str);
            if (str.length())
                parseString(str);
        }
        rcFile.close();
    } else
        std::cout << "Unable to open .twinyrc";
    if (tw_uname.empty() || tw_pass.empty() == 0)
        UnamePassEmpty();
}

void saveSettings() {
    std::ofstream rcFile(rcFilePath.c_str());
    if (rcFile.is_open()) {
        rcFile << "Username = " << base64_encode(reinterpret_cast<const unsigned char*> (tw_uname.c_str()), tw_uname.length()) << std::endl;
        rcFile << "Password = " << base64_encode(reinterpret_cast<const unsigned char*> (tw_pass.c_str()), tw_pass.length()) << std::endl;
        rcFile.close();
    } else
        std::cout << "Error: Unable to open .twinyrc" << std::endl;
}

void UnamePassEmpty() {
    if (tw_uname.empty()) {
        std::cout << "Twiny didn\'t found your username or e-mail, so please enter them: ";
        std::cin >> tw_uname;
    }

    if (tw_pass.empty()) {
        std::cout << "Twiny didn\'t found your password, so please enter them: ";
        std::cin >> tw_pass;
    }
}

std::string genStr(int argc, char* argv[]) {
    std::string str;
    std::string str2;
    for (int i = 1; i < argc; i++)
        str += str2.assign(argv[i]) + ' ';
    str = str.erase(str.length() - 1, 1);
    return str;
}

void setStatus(std::string mes) {

    tw_status = mes;
    std::string params = "status=" + mes;
    int len = params.length();

    std::string enc;
    enc = tw_uname + ':' + tw_pass;
    enc = base64_encode(reinterpret_cast<const unsigned char*> (enc.c_str()), enc.length());

    happyhttp::Connection conn("www.twitter.com", 80);
    conn.setcallbacks(OnBegin, OnData, OnComplete, 0);

    conn.putrequest("POST", "/statuses/update.xml");
    conn.putheader("Authorization", "Basic " + enc);
    conn.putheader("Accept", "text/plain");
    conn.putheader("Accept-Charset", "utf-8");
    conn.putheader("Connection", "close");
    conn.putheader("Content-Length", len);

    conn.endheaders();

    conn.send((const unsigned char*) params.c_str(), len);

    while (conn.outstanding())
        conn.pump();
}

void getInfo() //For getting info about user.
{
    happyhttp::Connection conn("www.twitter.com", 80);
    conn.setcallbacks(OnBegin, OnData, 0, 0);

    std::string req = "/users/show/" + tw_uname + ".xml";
    conn.request("GET", req.c_str(), 0, 0, 0);

    while (conn.outstanding())
        conn.pump();
}
