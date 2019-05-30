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

#ifndef TWINY_H
#define TWINY_H

#include "happyhttp.h"
#include "base64.h"

#include <stdlib.h>

#define tw_version "0.0.5"

using namespace std;
void OnBegin(const happyhttp::Response* r, void* userdata);
void OnData(const happyhttp::Response* r, void* userdata, const unsigned char* data, int n);
void OnComplete(const happyhttp::Response* r, void* userdata);

extern string rcFilePath;
extern string tw_uname;
extern string tw_pass;
extern string tw_status;

bool checkLen(std::string mes);

void sPost(string where, string params);
void setStatus(string mes);
string toPercent(std::string str);
string tagParse(string tag, string text, int pos);
void firstRun();
void parseString(string str);
string inPass();
#endif // TWINY_H
