#include "geometrytype.h"

// ************************************************************ //
// CMythSize
// ************************************************************ //

CMythPoint CMythSize::operator - (const CMythPoint& rPoint) const
{
	return rPoint - *this;
}

CMythRect CMythSize::operator - (const CMythRect& rRect) const
{
	return rRect - *this;
}

CMythPoint CMythSize::operator + (const CMythPoint& rPoint) const
{
	return rPoint + *this;
}

CMythRect CMythSize::operator + (const CMythRect& rRect) const
{
	return rRect + *this;
}

// ************************************************************ //
// CMythPoint
// 2D зјБъ
// ************************************************************ //

CMythRect CMythPoint::operator - (const CMythRect& rRect) const
{
	return rRect - *this;
}

CMythRect CMythPoint::operator + (const CMythRect& rRect) const
{
	return rRect + *this;
}

bool CMythRect::intersectRect(const CMythRect& rSrcRect1, const CMythRect& rSrcRect2)
{
	if (rSrcRect1.isRectEmpty() || rSrcRect2.isRectEmpty())
		return false;

	int tLeft = std::max(rSrcRect1.mTopLeft.mX, rSrcRect2.mTopLeft.mX);
	int tTop = std::max(rSrcRect1.mTopLeft.mY, rSrcRect2.mTopLeft.mY);
	int tRight = std::min(rSrcRect1.mBottomRight.mX, rSrcRect2.mBottomRight.mX);
	int tBottom = std::min(rSrcRect1.mBottomRight.mY, rSrcRect2.mBottomRight.mY);

	if (CMythRect(tLeft, tTop, tRight, tBottom, false).isRectEmpty())
	{
		setRectEmpty();
		return false;
	}

	mTopLeft.mX = tLeft;
	mTopLeft.mY = tTop;
	mBottomRight.mX = tRight;
	mBottomRight.mY = tBottom;
	return true;
}

void CMythRect::normalizeRect()
{
	mTopLeft.mX = std::min(mTopLeft.mX, mBottomRight.mX);
	mTopLeft.mY = std::min(mTopLeft.mY, mBottomRight.mY);
	mBottomRight.mX = std::max(mTopLeft.mX, mBottomRight.mX);
	mBottomRight.mY = std::max(mTopLeft.mY, mBottomRight.mY);
}

bool CMythRect::ptInRect(const CMythPoint& rPoint) const
{
	if (isRectEmpty())
		return false;

	if (rPoint.mX < mTopLeft.mX || rPoint.mX > mBottomRight.mX)
		return false;

	if (rPoint.mY < mTopLeft.mY || rPoint.mY > mBottomRight.mY)
		return false;

	return true;
}

bool CMythRect::unionRect(const CMythRect& rSrcRectl, const CMythRect& rSrcRect2)
{
	int tLeft = rSrcRectl.mTopLeft.mX < rSrcRect2.mTopLeft.mX ? rSrcRectl.mTopLeft.mX : rSrcRect2.mTopLeft.mX;
	int tTop = rSrcRectl.mTopLeft.mY < rSrcRect2.mTopLeft.mY ? rSrcRectl.mTopLeft.mY : rSrcRect2.mTopLeft.mY;
	int tRight = rSrcRectl.mBottomRight.mX > rSrcRect2.mBottomRight.mX ? rSrcRectl.mBottomRight.mX : rSrcRect2.mBottomRight.mX;
	int tBottom = rSrcRectl.mBottomRight.mY > rSrcRect2.mBottomRight.mY ? rSrcRectl.mBottomRight.mY : rSrcRect2.mBottomRight.mY;

	mTopLeft.mX = tLeft;
	mTopLeft.mY = tTop;
	mBottomRight.mX = tRight;
	mBottomRight.mY = tBottom;
	return true;
}

int CMythRect::subtractRect(const CMythRect& rDesRect, CMythRect* pRectList)
{
	CMythRect tRectResult;
	if (tRectResult.intersectRect(*this, rDesRect) == false)
	{
		pRectList[0] = rDesRect;
		return 1;
	}

	CMythRect tTopRect;
	tTopRect.mTopLeft = rDesRect.mTopLeft;
	tTopRect.mBottomRight.mX = rDesRect.mBottomRight.mX;
	tTopRect.mBottomRight.mY = mTopLeft.mY - 1;

	CMythRect tLeftRect;
	tLeftRect.mTopLeft.mX = rDesRect.mTopLeft.mX;
	tLeftRect.mTopLeft.mY = std::max(mTopLeft.mY, rDesRect.mTopLeft.mY);
	tLeftRect.mBottomRight.mX = mTopLeft.mX - 1;
	tLeftRect.mBottomRight.mY = std::min(mBottomRight.mY, rDesRect.mBottomRight.mY);

	CMythRect tBottomRect;
	tBottomRect.mTopLeft.mX = rDesRect.mTopLeft.mX;
	tBottomRect.mTopLeft.mY = mBottomRight.mY + 1;
	tBottomRect.mBottomRight = rDesRect.mBottomRight;

	CMythRect tRightRect;
	tRightRect.mTopLeft.mX = mBottomRight.mX + 1;
	tRightRect.mTopLeft.mY = std::max(mTopLeft.mY, rDesRect.mTopLeft.mY);
	tRightRect.mBottomRight.mX = rDesRect.mBottomRight.mX;
	tRightRect.mBottomRight.mY = std::min(mBottomRight.mY, rDesRect.mBottomRight.mY);
	int tRectIndex = 0;
	if (tTopRect.isRectEmpty() == false)
		pRectList[tRectIndex++] = tTopRect;

	if (tLeftRect.isRectEmpty() == false)
		pRectList[tRectIndex++] = tLeftRect;

	if (tBottomRect.isRectEmpty() == false)
		pRectList[tRectIndex++] = tBottomRect;

	if (tRightRect.isRectEmpty() == false)
		pRectList[tRectIndex++] = tRightRect;

	return tRectIndex;
}
