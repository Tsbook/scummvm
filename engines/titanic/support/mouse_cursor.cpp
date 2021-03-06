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

#include "graphics/cursorman.h"
#include "titanic/support/mouse_cursor.h"
#include "titanic/support/transparency_surface.h"
#include "titanic/support/video_surface.h"
#include "titanic/titanic.h"

namespace Titanic {

#define CURSOR_SIZE 64

static const int CURSOR_DATA[NUM_CURSORS][4] = {
	{ 1, 136, 19, 18 },
	{ 2, 139, 1, 1 },
	{ 3, 140, 32, 1 },
	{ 4, 137, 13, 0 },
	{ 5, 145, 13, 0 },
	{ 6, 144, 13, 22 },
	{ 7, 137, 14, 0 },
	{ 8, 148, 22, 40 },
	{ 9, 136, 19, 18 },
	{ 10, 143, 11, 11 },
	{ 11, 146, 11, 11 },
	{ 12, 136, 19, 18 },
	{ 13, 136, 19, 25 },
	{ 14, 136, 13, 22 },
	{ 15, 138, 20, 28 }
};

CMouseCursor::CursorEntry::~CursorEntry() {
	delete _videoSurface;
	delete _transSurface;
}

CMouseCursor::CMouseCursor(CScreenManager *screenManager) : 
		_screenManager(screenManager), _cursorId(CURSOR_HOURGLASS),
		_setCursorCount(0), _fieldE4(0), _fieldE8(0) {
	loadCursorImages();
	setCursor(CURSOR_ARROW);
}

CMouseCursor::~CMouseCursor() {
}

void CMouseCursor::loadCursorImages() {
	const CResourceKey key("ycursors.avi");

	// Iterate through getting each cursor
	for (int idx = 0; idx < NUM_CURSORS; ++idx) {
		assert(CURSOR_DATA[idx][0] == (idx + 1));
		_cursors[idx]._centroid = Common::Point(CURSOR_DATA[idx][2],
			CURSOR_DATA[idx][3]);

		// Create the surface
		CVideoSurface *surface = _screenManager->createSurface(CURSOR_SIZE, CURSOR_SIZE);
		_cursors[idx]._videoSurface = surface;

		// Open the cursors video and move to the given frame
		OSMovie movie(key, surface);
		movie.setFrame(idx);
		
		Graphics::ManagedSurface *transSurface = movie.duplicateTransparency();
		_cursors[idx]._transSurface = transSurface;
		surface->setTransparencySurface(transSurface);
	}
}

void CMouseCursor::show() {
	CursorMan.showMouse(true);
}

void CMouseCursor::hide() {
	CursorMan.showMouse(false);
}

void CMouseCursor::setCursor(CursorId cursorId) {
	++_setCursorCount;

	if (cursorId != _cursorId) {
		// The original cursors supported partial alpha when rendering the cursor.
		// Since we're using the ScummVM CursorMan, we can't do that, so we need
		// to build up a surface of the cursor with even partially transparent
		// pixels as wholy transparent
		CursorEntry &ce = _cursors[cursorId - 1];
		CVideoSurface &srcSurface = *ce._videoSurface;
		srcSurface.lock();

		Graphics::ManagedSurface surface(CURSOR_SIZE, CURSOR_SIZE, g_system->getScreenFormat());
		const uint16 *srcP = srcSurface.getPixels();
		CTransparencySurface transSurface(&ce._transSurface->rawSurface(), TRANS_DEFAULT);
		uint16 *destP = (uint16 *)surface.getPixels();

		for (int y = 0; y < CURSOR_SIZE; ++y) {
			transSurface.setRow(y);
			transSurface.setCol(0);

			for (int x = 0; x < CURSOR_SIZE; ++x, ++srcP, ++destP) {
				*destP = transSurface.isPixelTransparent() ? srcSurface.getTransparencyColor() : *srcP;
				transSurface.moveX();
			}
		}

		srcSurface.unlock();

		// Set the cursor
		_cursorId = cursorId;
		CursorMan.replaceCursor(surface.getPixels(), CURSOR_SIZE, CURSOR_SIZE,
			ce._centroid.x, ce._centroid.y, srcSurface.getTransparencyColor(), false, &g_vm->_screen->format);
	}
}

void CMouseCursor::update() {
	// No implementation needed
}

void CMouseCursor::lockE4() {
	_fieldE4 = 0;
	CScreenManager::_screenManagerPtr->_inputHandler->incLockCount();
}

void CMouseCursor::unlockE4() {
	_fieldE4 = 1;
	_fieldE8 = 0;
	CScreenManager::_screenManagerPtr->_inputHandler->decLockCount();
}

void CMouseCursor::setPosition(const Point &pt, double rate) {
	assert(rate >= 0.0 && rate <= 1.0);
	
	// TODO: Figure out use of the rate parameter
	g_system->warpMouse(pt.x, pt.y);
}

} // End of namespace Titanic
