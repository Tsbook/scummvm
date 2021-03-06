/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "titanic/game/sgt/basin.h"

namespace Titanic {

BEGIN_MESSAGE_MAP(CBasin, CSGTStateRoom)
	ON_MESSAGE(TurnOn)
	ON_MESSAGE(TurnOff)
	ON_MESSAGE(MovieEndMsg)
END_MESSAGE_MAP()

void CBasin::save(SimpleFile *file, int indent) {
	file->writeNumberLine(1, indent);
	CSGTStateRoom::save(file, indent);
}

void CBasin::load(SimpleFile *file) {
	file->readNumber();
	CSGTStateRoom::load(file);
}

bool CBasin::TurnOn(CTurnOn *msg) {
	if (_statics->_v10 == "Open" && _statics->_v11 == "Closed"
			&& _statics->_v2 == "Closed") {
		setVisible(true);
		_statics->_v11 = "Open";
		_fieldE0 = 0;
		_startFrame = 0;
		_endFrame = 6;
		playMovie(0, 6, MOVIE_GAMESTATE);
		playSound("b#13.wav");
	}

	return true;
}

bool CBasin::TurnOff(CTurnOff *msg) {
	if (_statics->_v11 == "Open") {
		_statics->_v11 = "Closed";
		_fieldE0 = 1;
		_startFrame = 8;
		_endFrame = 14;
		playMovie(8, 14, MOVIE_NOTIFY_OBJECT | MOVIE_GAMESTATE);
		playSound("b#13.wav");
	}

	return true;
}

bool CBasin::MovieEndMsg(CMovieEndMsg *msg) {
	if (_statics->_v11 == "Closed")
		setVisible(false);

	return true;
}

} // End of namespace Titanic
