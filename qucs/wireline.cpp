/***************************************************************************
 * Copyright (C) 2008 by Bastien ROUCARIES <bastien.roucaries@gmail.com>   *       
 *                                                                         *
 * This is free software; you can redistribute it and/or modify            *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2, or (at your option)     *
 * any later version.                                                      *
 *                                                                         *
 * This software is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this package; see the file COPYING.  If not, write to        *
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,   *
 * Boston, MA 02110-1301, USA.                                             *
 ***************************************************************************/

#include "wireline.h"

/* made configurable */
unsigned int WireLine::m_adjust = 3;

/*! Return bounding rectangle arround a wireline */
QRectF WireLine::boundingRect() const
{
   QRectF rect;
   rect.setTopLeft(this->p1());
   rect.setBottomRight(this->p2());
   rect = rect.normalized();
   rect.adjust(-this->m_adjust, -this->m_adjust, +this->m_adjust, +this->m_adjust);
   return rect;
}
