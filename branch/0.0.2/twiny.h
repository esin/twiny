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

#include "happyhttp.h"
#include "base64.h"

void OnBegin( const happyhttp::Response* r, void* userdata );
void OnData( const happyhttp::Response* r, void* userdata, const unsigned char* data, int n );
void OnComplete( const happyhttp::Response* r, void* userdata );
bool checkLen(std::string mes);
void UnamePassEmpty();

std::string genStr(int argc, char* argv[]);
void setStatus(std::string mes);
void getInfo();

void applySettings(std::string uname, std::string pass);
void loadSettings();
void saveSettings();
void setRCFileP(std::string path);
