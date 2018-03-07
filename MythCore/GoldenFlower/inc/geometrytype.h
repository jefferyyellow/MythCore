#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include<algorithm>
class CMythSize;
class CMythPoint;
class CMythRect;
// CMythSize 类
/*
	CMythSize 比较类似 MFC CSize, 实现了相对坐标  mCX 和 mCY 成员是公开的，另外
	CMythSize 提供了维护这两个数据的成员函数
*/
class CMythSize
{
public:
	int	mCX;			//! 横坐标
	int	mCY;			//! 纵坐标

public:
	// CMythSize 构造函数, 默认构造一个 CMythSize( 0, 0 ) 对象
	CMythSize( ) : mCX( 0 ), mCY( 0 )						{	}

	// CMythSize 构造函数, 通过两个整数构造一个 CMythSize 对象
	CMythSize( int vCX, int vCY ) : mCX( vCX ), mCY( vCY )	{	}

public:
	// CMythSize 析造函数
	~CMythSize( )											{	}

public:
	// != 操作符, 判断两个CMythSize是否不一样
	bool operator != ( const CMythSize& rSize ) const			
		{ return mCX != rSize.mCX || mCY != rSize.mCY; }

	// == 操作符, 判断两个CMythSize是否一样
	bool operator == ( const CMythSize& rSize ) const			
		{ return mCX == rSize.mCX && mCY == rSize.mCY; }

	// -= 操作符, 该CMythSize对象减去一个CMythSize对象
	void operator -= ( const CMythSize& rSize )
		{ mCX -= rSize.mCX; mCY -= rSize.mCY; }

	// += 操作符, 该CMythSize对象加上一个rSize对象
	void operator += ( const CMythSize& rSize )
		{ mCX += rSize.mCX; mCY += rSize.mCY; }

	// - 操作符, 这个操作返回rPoint的值减去该CMythSize对象的值
	CMythPoint operator - ( const CMythPoint& rPoint ) const;

	// - 操作符, 这个操作返回rRect的值减去该CMythSize对象的值
	CMythRect operator - ( const CMythRect& rRect ) const;
		
	// - 操作符, 这个操作返回该CMythSize对象的值减去CMythSize对象后，构造的CMythSize拷贝 */
	CMythSize operator - ( const CMythSize& rSize ) const
		{ return CMythSize( mCX - rSize.mCX, mCY - rSize.mCY ); }

	// - 操作符, 这个操作返回该CMythSize对象的取反后，构造的CMythSize拷贝
	CMythSize operator - ( ) const
		{ return CMythSize( -mCX, -mCY ); }

	// + 操作符, 这个操作返回rPoint的值加上该CMythSize对象的值
	CMythPoint operator + ( const CMythPoint& rPoint ) const;

	// + 操作符, 这个操作返回rRect的值加上该CMythSize对象的值
	CMythRect operator + ( const CMythRect& rRect ) const;

	// + 操作符, 这个操作返回该CMythSize对象的值加上rSize后，构造的CMythSize拷贝
	CMythSize operator + ( const CMythSize& rSize ) const
		{ return CMythSize( mCX + rSize.mCX, mCY + rSize.mCY ); }
};

// CMythPoint 类
/*
	CMythPoint 比较类似 MFC CPoint, 实现了相对坐标  mX 和 mY 成员是公开的，另外
	CMythPoint 提供了维护这两个数据的成员函数
*/
class CMythPoint
{
public:
	int		mX;
	int		mY;

public:
	// CMythPoint 构造函数, 默认构造一个 CMythPoint( 0, 0 ) 对象
	CMythPoint( ) { }//: mX( 0 ), mY( 0 )						{	}
	// CMythPoint 构造函数, 通过两个整数构造一个 CMythPoint 对象
	CMythPoint( int vX, int vY ) : mX( vX ), mY( vY )	{	}
public:
	// CMythPoint 析造函数
	~CMythPoint( )										{	}

public:
	// 成员函数 Offset, 横坐标和纵坐标分别加上vXOffset, vYOffset
	void offset( int vXOffset, int vYOffset )
		{ mX += vXOffset; mY += vYOffset; }

	// 成员函数 Offset, 横坐标和纵坐标分别加上rPoint.mX, rPoint.mY
	void offset( const CMythPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

	// 成员函数 Offset, 横坐标和纵坐标分别偏移rSize.mX, rSize.mY
	void offset( const CMythSize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }
	
	void clear( ) { mX = -1; mY = -1; }

	// 成员函数 LimitDistance, 判断目标点与本点距离是否在vAmbit内
	bool limitDistance( const CMythPoint& rDesPos, unsigned int vAmbit )
	{	
		if( (unsigned int )abs( mX - rDesPos.mX ) > vAmbit )
		{
			return false;
		}
		else if( ( unsigned int )abs( mY - rDesPos.mY ) > vAmbit )
		{
			return false;
		}

		return true;
	}

	int length( const CMythPoint& rDesPos )
	{
		return (std::max)( abs( mY - rDesPos.mY ), abs( mX - rDesPos.mX ) );
	}
public:
	// != 操作符, 判断两个CMythPoint是否不一样 */
	bool operator != ( const CMythPoint& rPoint ) const
		{ return mX != rPoint.mX || mY != rPoint.mY; }

	// == 操作符, 判断两个CMythPoint是否一样 */
	bool operator == ( const CMythPoint& rPoint ) const
		{ return mX == rPoint.mX && mY == rPoint.mY; }

	// += 操作符, 该操作加上一个CMythSize对象 */
	void operator += ( const CMythSize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

	// += 操作符, 该操作加上一个CMythPoint对象 */
	void operator += ( const CMythPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

	// -= 操作符, 该操作减去一个CMythSize对象 */
 	void operator -= ( const CMythSize& rSize )
		{ mX -= rSize.mCX; mY -= rSize.mCY; }

	// -= 操作符, 该操作减去一个CMythPoint对象 */
 	void operator -= ( const CMythPoint& rPoint )
		{ mX -= rPoint.mX; mY -= rPoint.mY; }

	// - 操作符, 该操作返回该CMythPoint对象减去一个CMythPoint对象后，构造的CMythPoint拷贝 */
	CMythPoint operator - ( const CMythPoint& rPoint ) const
		{ return CMythPoint( mX - rPoint.mX, mY - rPoint.mY ); }

	// - 操作符, 该操作返回该CMythPoint对象减去一个CMythSize对象后，构造的CMythPoint拷贝
	CMythPoint operator - ( const CMythSize& rSize ) const
		{ return CMythPoint( mX - rSize.mCX, mY - rSize.mCY ); }

	// - 操作符, 该操作返回CMythRect对象减去该CMythPoint对象后，构造的CMythRect拷贝
	CMythRect operator - ( const CMythRect& rRect ) const;

	// - 操作符, 该操作返回该CMythPoint对象取反后，构造的CMythPoint拷贝
	CMythPoint operator - ( ) const
		{ return CMythPoint( -mX, -mY ); }

	// + 操作符, 该操作返回该CMythPoint对象加上一个CMythPoint对象后，构造的CMythPoint拷贝
	CMythPoint operator + ( const CMythPoint& rPoint ) const
		{ return CMythPoint( mX + rPoint.mX, mY + rPoint.mY ); }

	// + 操作符, 该操作返回该CMythPoint对象加上一个CMythSize对象后，构造的CMythPoint拷贝
	CMythPoint operator + ( const CMythSize& rSize ) const
		{ return CMythPoint( mX + rSize.mCX, mY + rSize.mCY ); }

	// + 操作符, 该操作返回CMythRect对象加上该CMythPoint对象后，构造的CMythRect拷贝
	CMythRect operator + ( const CMythRect& rRect ) const;
};

// CMythRect 类,因为vs环境里有CRect，所以加前缀，避免重名
/*
	CMythRect 比较类似 MFC CMythRect, 实现了相对矩形坐标  mTopLeft 和 mBottomRight 成员是公开的，另外
	CMythRect 提供了维护这两个数据的成员函数, CMythRect的构造函数默认将调用NormalizeRect, 该函数将使
	得一个CMythRect对象正常化, 比如:mTopLeft( 10, 10 ) 和 mBottomRight( 20, 20 )是一个正常的矩形, 但
	一个mTopLeft( 20, 20 ) 和 mBottomRight( 10, 10 )是一个不正常的矩形, NormalizeRect将使得这样的矩
	形正常化，一个不正常的矩形在使用某些CMythRect的成员函数时可能会发生错误！
*/
class CMythRect
{
public:
	CMythPoint	mTopLeft;					//! CMythPoint 型,矩形的左上角
	CMythPoint	mBottomRight;				//! CMythPoint 型,矩形的右下角

public:
	// CMythRect 构造函数, 默认构造一个 CMythRect( 0, 0, 0, 0 ) 对象
	CMythRect( )	{ normalizeRect( ); }

	// \brief CMythRect 构造函数, 通过4个整数构造 CMythRect 对象, 
	CMythRect( int vLeft, int vTop, int vRight, int vBottom, bool vNormalize = true ) : mTopLeft( vLeft, vTop ), mBottomRight( vRight, vBottom )
		{ if ( vNormalize ) normalizeRect( ); }

	// CMythRect 构造函数, 通过1个 CMythPoint 和一个 CMythSize 构造 CMythRect 对象, 
	CMythRect( const CMythPoint& rPoint, const CMythSize& rSize, bool vNormalize = true ) : mTopLeft( rPoint ), mBottomRight( rPoint + rSize )
		{ if ( vNormalize ) normalizeRect( ); }

	// CMythRect 构造函数, 通过2个 CMythPoint 构造 CMythRect 对象, 
	CMythRect( const CMythPoint& rTopLeft, const CMythPoint& rBottomRight, bool vNormalize = true ) : mTopLeft( rTopLeft ), mBottomRight( rBottomRight )
		{ if ( vNormalize ) normalizeRect( ); }
public:
	// CMythRect 析造函数
	~CMythRect( ) {	}

public:
	// 成员函数 BottomLeft, 
	CMythPoint bottomLeft( ) const
		{ return CMythPoint( mTopLeft.mX, mBottomRight.mY );	}

	// 成员函数 TopRight, 
	CMythPoint topRight( ) const
		{ return CMythPoint( mBottomRight.mX, mTopLeft.mY );	}

	// 成员函数 BottomRight, 
	const CMythPoint& bottomRight() const
		{ return mBottomRight; }

	CMythPoint& bottomRight()
		{ return mBottomRight; }

	// 成员函数 TopLeft, 
	const CMythPoint& topLeft() const
		{ return mTopLeft; }

	CMythPoint& topLeft()
		{ return mTopLeft; }

	// 成员函数 CenterPoint, 
	CMythPoint centerPoint( ) const
		{ return CMythPoint( ( mTopLeft.mX + mBottomRight.mX ) >> 1, ( mTopLeft.mY + mBottomRight.mY ) >> 1 ); }

	// 成员函数 EqualRect, 判断两个对象是否相等
	bool equalRect( const CMythRect& rRect )	const
		{ return mTopLeft == rRect.mTopLeft && mBottomRight == rRect.mBottomRight; }

	// 成员函数 DeflateRect, 缩小该 CMythRect 对象, 缩小指的是像中心靠近
	void deflateRect( int vX, int vY )
		{ mTopLeft.mX += vX; mTopLeft.mY += vY; mBottomRight.mX -= vX; mBottomRight.mY -= vY; }

	// 成员函数 DeflateRect, 缩小该 CMythRect 对象, 缩小指的是像中心靠近
	void deflateRect( const CMythSize& rSize )
	{ 
		mTopLeft.mX		+= rSize.mCX; mTopLeft.mY		+= rSize.mCY; 
		mBottomRight.mX	-= rSize.mCX; mBottomRight.mY	-= rSize.mCY; 
	}

	// 成员函数 DeflateRect, 缩小该 CMythRect 对象, 缩小指的是像中心靠近
	void deflateRect( const CMythRect& rRect )
	{
		mTopLeft.mX		+= rRect.mTopLeft.mX;		mTopLeft.mY		+= rRect.mTopLeft.mY;		
		mBottomRight.mX -= rRect.mBottomRight.mX;	mBottomRight.mY -= rRect.mBottomRight.mY; 
	}

	// 成员函数 DeflateRect, 缩小该 CMythRect 对象, 缩小指的是像中心靠近
	void deflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX += vLeft; mTopLeft.mY += vTop; mBottomRight.mX -= vRight; mBottomRight.mY -= vBottom; }

	// 成员函数 InflateRect, 扩大该 CMythRect 对象, 扩大指的是像中心发散
	void inflateRect( int vX, int vY )
		{ mTopLeft.mX -= vX; mTopLeft.mY -= vY; mBottomRight.mX += vX; mBottomRight.mY += vY; }

	// 成员函数 InflateRect, 扩大该 CMythRect 对象, 扩大指的是像中心发散
	void inflateRect( const CMythSize& rSize )
	{ 
		mTopLeft.mX		-= rSize.mCX;	mTopLeft.mY		-= rSize.mCY;			
		mBottomRight.mX += rSize.mCX;	mBottomRight.mY	+= rSize.mCY; 
	}

	// 成员函数 InflateRect, 扩大该 CMythRect 对象, 扩大指的是像中心发散
	void inflateRect( const CMythRect& rRect )
	{
		mTopLeft.mX		-= rRect.mTopLeft.mX;		mTopLeft.mY		-= rRect.mTopLeft.mY;	
		mBottomRight.mX += rRect.mBottomRight.mX;	mBottomRight.mY += rRect.mBottomRight.mY; 
	}

	// 成员函数 InflateRect, 扩大该 CMythRect 对象, 扩大指的是像中心发散
	void inflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX -= vLeft; mTopLeft.mY -= vTop; mBottomRight.mX += vRight; mBottomRight.mY += vBottom; }

	//! 成员函数 Height, 返回该 CMythRect 对象的高度
	/*! \return 返回该 CMythRect 对象的高度 */
	int height( ) const
		{ return mBottomRight.mY - mTopLeft.mY + 1; }

	// 成员函数 Width, 返回该 CMythRect 对象的宽度
	int	width( ) const	
		{ return mBottomRight.mX - mTopLeft.mX + 1; }

	// 成员函数 IsRectEmpty, 返回该 CMythRect 对象的宽和高是不是全大于0
	bool isRectEmpty( ) const
		{ return width( ) <= 0 || height( ) <= 0; }

	// 成员函数 IsRectNull, 返回该 CMythRect 对象的四个边是否全为0
	bool isRectNull( ) const
		{ return mTopLeft.mX == 0 && mTopLeft.mY == 0 && mBottomRight.mX == 0 && mBottomRight.mY == 0; }

	// 成员函数 MoveToX, 对其该 CMythRect 对象到vX,纵坐标不变
	void moveToX( int vX )	
		{ mBottomRight.mX = vX + width( ); mTopLeft.mX = vX; }

	// 成员函数 MoveToY, 对其该 CMythRect 对象到vY,横坐标不变
	void moveToY( int vY )	
		{ mBottomRight.mY = vY + height( ); mTopLeft.mY = vY; }

	// 成员函数 MoveToXY, 对其该 CMythRect 对象到vX, vY
	void moveToXY( int vX, int vY )
	{
		mBottomRight.mX = vX + width( );	mTopLeft.mX	= vX; 
		mBottomRight.mY = vY + height( ); 	mTopLeft.mY	= vY;  
	}

	// 成员函数 MoveToXY, 对其该 CMythRect 对象到rPoint
	void moveToXY( const CMythPoint& rPoint )
	{
		mBottomRight.mX = rPoint.mX + width( ); 	mTopLeft.mX = rPoint.mX; 
		mBottomRight.mY = rPoint.mY + height( );	mTopLeft.mY = rPoint.mY; 
	}

	// 成员函数 OffsetRect, 移动该 CMythRect 对象
	void offsetRect( int vX, int vY )					 
		{ *this += CMythPoint( vX, vY ); }

	// 成员函数 OffsetRect, 移动该 CMythRect 对象
	void offsetRect( const CMythPoint& rPoint )
		{ *this += rPoint; }

	// 成员函数 OffsetRect, 移动该 CMythRect 对象
	void offsetRect( const CMythSize& rSize )				 
		{ *this += rSize; }	

	// 成员函数 SetRect, 设置 CMythRect 对象
	void setRect( int vLeft, int vTop, int vRight, int vBottom )
	{
		mTopLeft.mX		= vLeft;	mTopLeft.mY		= vTop; 
		mBottomRight.mX = vRight;	mBottomRight.mY = vBottom; 
	}

	// 成员函数 SetRect, 通过1个 CMythPoint 和1个 CMythSize 设置 CMythRect 对象, 
	void setRect( const CMythPoint& rPoint, const CMythSize& rSize )
		{ mTopLeft = rPoint; mBottomRight = rPoint + rSize; }

	// 成员函数 SetRect, 通过2个 CMythPoint 设置 CMythRect 对象, 
	void setRect( const CMythPoint& rTopLeft, const CMythPoint& rBottomRight )
		{ mTopLeft = rTopLeft; mBottomRight	= rBottomRight; }

	// 成员函数 SetRectEmpty, 将该 CMythRect 对象置0, 
	void setRectEmpty( )
		{ mTopLeft.mX = 0; mTopLeft.mY = 0; mBottomRight.mX = 0; mBottomRight.mY = 0; }

	// 成员函数 Size, 返回该 CMythRect 对象的宽度和高度
	CMythSize size( ) const
		{ return CMythSize( width( ), height( ) ); }

	// 成员函数 NormalizeRect, 正常化该 CMythRect 对象
	void normalizeRect( );

	// 成员函数 PtInRect, 判断指定坐标是否在该 CMythRect 对象中
	bool ptInRect( const CMythPoint&	rPoint ) const;

	// 成员函数 IntersectRect, 计算两个 CMythRect 对象的交集
	bool intersectRect( const CMythRect& rSrcRect1, const CMythRect& rSrcRect2 );

	// 成员函数 UnionRect, 计算两个 CMythRect 对象的并集
	bool unionRect( const CMythRect& rSrcRectl, const CMythRect& rSrcRect2 );

	//! 矩形相减
	int subtractRect( const CMythRect& rDesRect, CMythRect* pRectList );

public:
	// != 操作符, 判断两个 CMythRect 对象是否不相等
	bool operator != ( const CMythRect& rRect ) const
		{ return !equalRect( rRect ); }

	// == 操作符, 判断两个 CMythRect 对象是否相等
	bool operator == ( const CMythRect& rRect ) const
		{ return equalRect( rRect ); }

	// &= 操作符, 计算该 CMythRect 对象和 指定 CMythRect 对象的交集
	void operator &= ( const CMythRect& rRect )
		{ intersectRect( *this, rRect ); }

	// |= 操作符, 计算该 CMythRect 对象和 指定 CMythRect 对象的并集
	void operator |= ( const CMythRect& rRect )
		{ unionRect( *this, rRect ); }

	// & 操作符, 计算该 CMythRect 对象和 指定 CMythRect 对象的交集
	CMythRect operator & ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult;
		tRectResult.intersectRect( *this, rRect );
		return tRectResult;
	}

	// | 操作符, 计算该 CMythRect 对象和 指定 CMythRect 对象的并集
	CMythRect operator | ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult;
		tRectResult.unionRect( *this, rRect );
		return tRectResult;
	}

	// - 操作符, 把该 CMythRect 反向偏移 rPoint 个单位
	CMythRect operator - ( const CMythPoint& rPoint ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( -rPoint );
		return tRectResult;
	}

	// - 操作符, 把该 CMythRect 反向偏移 rSize 个单位
	CMythRect operator - ( const CMythSize& rSize ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( -rSize );
		return tRectResult;
	}

	// - 操作符, 缩小该 CMythRect 对象，缩小的尺寸由 rRect 指定
	CMythRect operator - ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.deflateRect( rRect );
		return tRectResult;
	}

	// + 操作符, 把该 CMythRect 正向偏移 rPoint 个单位
	CMythRect operator + ( const CMythPoint& rPoint ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( rPoint );
		return tRectResult;
	}

	// + 操作符, 把该 CMythRect 正向偏移 rSize 个单位
	CMythRect operator + ( const CMythSize& rSize ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( rSize );
		return tRectResult;
	}

	// + 操作符, 扩大该 CMythRect 对象，扩大的尺寸由 rRect 指定
	CMythRect operator + ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.inflateRect( rRect );
		return tRectResult;
	}

	// += 操作符, 把该 CMythRect 正向偏移 rPoint 个单位
	void operator += ( const CMythPoint& rPoint )
		{ mTopLeft += rPoint; mBottomRight += rPoint; }

	// += 操作符, 把该 CMythRect 正向偏移 rSize 个单位
	void operator += ( const CMythSize& rSize )
		{ mTopLeft += rSize; mBottomRight += rSize; }

	// += 操作符, 扩大该 CMythRect 对象，扩大的尺寸由 rRect 指定
	void operator += ( const CMythRect& rRect )
		{ inflateRect( rRect );	}

	// += 操作符, 把该 CMythRect 反向偏移 rPoint 个单位
	void operator -= ( const CMythPoint& rPoint )
		{ mTopLeft -= rPoint; mBottomRight -= rPoint; }

	// += 操作符, 把该 CMythRect 反向偏移 rSize 个单位
	void operator -= ( const CMythSize& rSize )
		{ mTopLeft -= rSize; mBottomRight -= rSize;	}

	// += 操作符, 缩小该 CMythRect 对象，缩小的尺寸由 rRect 指定
	void operator -= ( const CMythRect& rRect )
		{ deflateRect( rRect );	}
};

#endif