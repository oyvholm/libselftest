/*
 * binbuf.h
 * File ID: 9bfca018-4a58-11f1-aef8-83850402c3ce
 *
 * (C)opyleft 2026- Øyvind A. Holm <sunny@sunbase.org>
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _BINBUF_H
#define _BINBUF_H

struct binbuf {
	size_t alloc;
	size_t len;
	char *buf;
};

void binbuf_init(struct binbuf *sb);
void binbuf_free(struct binbuf *sb);

#endif /* ifndef _BINBUF_H */

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
