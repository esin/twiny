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
    /*
    Limits are:
    # 1,000 total updates per day, on any and all devices (web, mobile web, phone, API, etc. )
    # 250 total direct messages per day, on any and all devices
    # 150 API requests per hour
     */

#include "twiny.h"
#include "settings.h"
#include "common.h"
#include "parsers.h"

string tw_uname;
string tw_pass;
string tw_status;
string rcFilePath;

string resp;

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
        case 500:
            cout << "Error: " << tagParse("error", resp, 0) << endl;
        default:
            cout << "I didn't found description of error" << endl;
            cout << "Please send this to author: " << endl;
            cout << r->getstatus() << " " << r->getreason() << endl;
    }
}

bool checkLen(string mes) {

    int len = 0;
    for (int i = 0; i < mes.length(); i++)
        if ((int)mes.at(i) < 0) len++;
    len /= 2;
    for (int i = 0; i < mes.length(); i++)
        if ((int)mes.at(i) > 0) len++;

    if (len > 140) {
        cout << "Status is too long (" << len << " chars), but can be only 140 chars." << endl;
        return false;
    }

    if (len < 1) {
        cout << "Status is too short, i don't know what to send." << endl;
        return false;
    }
    return true;
}

string toPercent(string str) {
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

string tagParse(string tag, string text, int pos = 0) {
    string ret = "";
    size_t lt = text.find("<" + tag + ">", pos) + tag.length() + 2;
    size_t rt = text.find("</" + tag + ">", pos);
    ret = text.substr(lt, rt - lt);
    return ret;
}

void setStatus(string mes) { // For setting new status. From this function all began :)
    if (checkLen(mes)) {
        string params = "status=" + toPercent(mes) + "&source=_twiny_";
        sPost("/statuses/update.xml", params);
    }
}

void sPost(string where = "", string params = "") {
    string enc;
    enc = tw_uname + ':' + tw_pass;
    enc = base64_encode(enc);
    int len = params.length();

    happyhttp::Connection conn("www.twitter.com", 80);
    conn.setcallbacks(OnBegin, OnData, OnComplete, 0);

    conn.putrequest("POST", where);
    conn.putheader("Authorization", "Basic " + enc);
    conn.putheader("Accept", "text/plain");
    conn.putheader("Accept-Charset", "utf-8");
    conn.putheader("User-Agent", "tωiny");
    conn.putheader("X-Twitter-Client", "tωiny");
    conn.putheader("X-Twitter-Client-Version", tw_version);
    conn.putheader("X-Twitter-Client-URL", "http://twiny.googlecode.com/files/twiny.xml");
    conn.putheader("Connection", "close");
    conn.putheader("Content-Length", len);

    conn.endheaders();

    conn.send((const unsigned char*) params.c_str(), len);

    while (conn.outstanding())
        conn.pump();
}

void printUsage() {
    cout << "T\033[1mω\033[0miny version " << tw_version << endl;
    cout << "I am small console tool for twitting" << endl;
    cout << endl;
    cout << "Error: You didn't enter new status" << endl;
    cout << "Usage: twiny new status" << endl;
    cout << endl;
    cout << "For all bugs, feel free to write: gmlastik@gmail.com" << endl;
    cout << "or other contact information on http://twiny.googlecode.com" << endl;
}
int main(int argc, char* argv[]) {
    if (argc > 1) {
        checkFR(); // Check for first run.
        tw_status = genStr(argc, argv);
        try {
            setStatus(tw_status);
        } catch (happyhttp::Wobbly& e) {
            cout << "Wooow!Network exception: " << e.what() << endl;
        }
    } else {
        printUsage();
    }
    return 0;
}
