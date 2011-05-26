/*   _____                           
 * /\  _  \                     __    
 * \ \ \_\ \      __    __  __ /\_\   
 *  \ \  __ \   /'_ `\ /\ \/\ \\/\ \  
 *   \ \ \/\ \ /\ \_\ \\ \ \_\ \\ \ \ 
 *    \ \_\ \_\\ \____ \\ \____/ \ \_\
 *     \/_/\/_/ \/____\ \\/___/   \/_/
 *                /\____/             
 *                \_/__/              
 *
 * Copyright (c) 2011 Joshua Larouche
 * 
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Agui nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Agui/ResizableBorderLayout.hpp"

namespace agui
{
	ResizableBorderLayout::ResizableBorderLayout(void)
		: resizingNorth(true), resizingSouth(true),
		resizingWest(true), resizingEast(true),
		dragX(0), dragY(0), mouseResult(CENTER), oldMargin(0),
		constrainToCenter(true)
	{
	}

	ResizableBorderLayout::~ResizableBorderLayout(void)
	{
	}

	BorderLayout::BorderLayoutEnum ResizableBorderLayout::getPointRegion( const Point &p )
	{
		if(!getWidget(CENTER))
		{
			return CENTER;
		}

		Widget* north = getWidget(NORTH);
		Widget* south = getWidget(SOUTH);
		Widget* east = getWidget(EAST);
		Widget* west = getWidget(WEST);
		Widget* center = getWidget(CENTER);

		if(resizingWest && west)
		{
			if(p.getX() >= west->getWidth() + west->getLocation().getX()
				&& p.getX() <= center->getLocation().getX())
			{
				return WEST;
			}
		}

		if(resizingEast && east)
		{
			if(p.getX() <= east->getLocation().getX()
				&& p.getX() >= center->getLocation().getX() + center->getWidth())
			{
				return EAST;
			}
		}


		if(resizingNorth && north)
		{
			if(p.getY() >= north->getHeight() + north->getLocation().getY()
				&& p.getY() <= center->getLocation().getY())
			{
				return NORTH;
			}
		}

		if(resizingSouth && south)
		{
			if(p.getY() <= south->getLocation().getY()
				&& p.getY() >= center->getLocation().getY() + center->getHeight())
			{
				return SOUTH;
			}
		}

		return CENTER;
		
	}

	void ResizableBorderLayout::mouseDown( MouseEvent &mouseEvent )
	{
		mouseResult = getPointRegion(mouseEvent.getPosition());
		dragX = mouseEvent.getX();
		dragY = mouseEvent.getY();

		if(mouseResult == NORTH)
		{
			oldMargin = getWidget(NORTH)->getHeight();
		}
		else if(mouseResult == SOUTH)
		{
			oldMargin = getWidget(SOUTH)->getHeight();
		}
		else if(mouseResult == EAST)
		{
			oldMargin = getWidget(EAST)->getWidth();
		}
		else if(mouseResult == WEST)
		{
			oldMargin = getWidget(WEST)->getWidth();
		}
	}

	void ResizableBorderLayout::mouseDrag( MouseEvent &mouseEvent )
	{
		if(mouseResult == CENTER)
		{
			return;
		}

		int delta = mouseResult == NORTH || mouseResult == SOUTH ?
			mouseEvent.getY() - dragY : mouseEvent.getX() - dragX;

		if(mouseResult == SOUTH || mouseResult == EAST)
		{
			delta = -delta;
		}

		int deltaMargin = delta + oldMargin;

		if(constrainToCenter)
		{
			//North
			if( mouseResult == NORTH && deltaMargin > 
				getWidget(CENTER)->getLocation().getY() - 
				getHorizontalSpacing() + getWidget(CENTER)->getSize().getHeight())
			{
				deltaMargin = getWidget(CENTER)->getLocation().getY() -
					getHorizontalSpacing() + getWidget(CENTER)->getSize().getHeight();
			}

			//West
			if( mouseResult == WEST && deltaMargin > 
				getWidget(CENTER)->getLocation().getX() - 
				getVerticalSpacing() + getWidget(CENTER)->getSize().getWidth())
			{
				deltaMargin = getWidget(CENTER)->getLocation().getX() -
					getVerticalSpacing() + getWidget(CENTER)->getSize().getWidth();
			}

			//South
			if(mouseResult == SOUTH && deltaMargin > getInnerHeight() -
				getWidget(CENTER)->getLocation().getY() - getHorizontalSpacing())
			{
				deltaMargin = getInnerHeight() -  
					getWidget(CENTER)->getLocation().getY() - getHorizontalSpacing();
			}

			//East
			if(mouseResult == EAST && deltaMargin > getInnerWidth() - 
				getWidget(CENTER)->getLocation().getX() - getVerticalSpacing())
			{
				deltaMargin = getInnerWidth() -  
					getWidget(CENTER)->getLocation().getX() - getVerticalSpacing();
			}
		}
		
		setBorderMargin(mouseResult,deltaMargin);
	}

	void ResizableBorderLayout::setConstrainToCenter(bool constrain)
	{
		constrainToCenter = constrain;
		updateLayout();
	}

	bool ResizableBorderLayout::isConstrainedToCenter() const
	{
		return constrainToCenter;
	}

}