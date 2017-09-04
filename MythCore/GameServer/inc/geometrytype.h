#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include<algorithm>
class CMythSize;
class CMythPoint;
class CMythRect;
// CMythSize ��
/*
	CMythSize �Ƚ����� MFC CSize, ʵ�����������  mCX �� mCY ��Ա�ǹ����ģ�����
	CMythSize �ṩ��ά�����������ݵĳ�Ա����
*/
class CMythSize
{
public:
	int	mCX;			//! ������
	int	mCY;			//! ������

public:
	// CMythSize ���캯��, Ĭ�Ϲ���һ�� CMythSize( 0, 0 ) ����
	CMythSize( ) : mCX( 0 ), mCY( 0 )						{	}

	// CMythSize ���캯��, ͨ��������������һ�� CMythSize ����
	CMythSize( int vCX, int vCY ) : mCX( vCX ), mCY( vCY )	{	}

public:
	// CMythSize ���캯��
	~CMythSize( )											{	}

public:
	// != ������, �ж�����CMythSize�Ƿ�һ��
	bool operator != ( const CMythSize& rSize ) const			
		{ return mCX != rSize.mCX || mCY != rSize.mCY; }

	// == ������, �ж�����CMythSize�Ƿ�һ��
	bool operator == ( const CMythSize& rSize ) const			
		{ return mCX == rSize.mCX && mCY == rSize.mCY; }

	// -= ������, ��CMythSize�����ȥһ��CMythSize����
	void operator -= ( const CMythSize& rSize )
		{ mCX -= rSize.mCX; mCY -= rSize.mCY; }

	// += ������, ��CMythSize�������һ��rSize����
	void operator += ( const CMythSize& rSize )
		{ mCX += rSize.mCX; mCY += rSize.mCY; }

	// - ������, �����������rPoint��ֵ��ȥ��CMythSize�����ֵ
	CMythPoint operator - ( const CMythPoint& rPoint ) const;

	// - ������, �����������rRect��ֵ��ȥ��CMythSize�����ֵ
	CMythRect operator - ( const CMythRect& rRect ) const;
		
	// - ������, ����������ظ�CMythSize�����ֵ��ȥCMythSize����󣬹����CMythSize���� */
	CMythSize operator - ( const CMythSize& rSize ) const
		{ return CMythSize( mCX - rSize.mCX, mCY - rSize.mCY ); }

	// - ������, ����������ظ�CMythSize�����ȡ���󣬹����CMythSize����
	CMythSize operator - ( ) const
		{ return CMythSize( -mCX, -mCY ); }

	// + ������, �����������rPoint��ֵ���ϸ�CMythSize�����ֵ
	CMythPoint operator + ( const CMythPoint& rPoint ) const;

	// + ������, �����������rRect��ֵ���ϸ�CMythSize�����ֵ
	CMythRect operator + ( const CMythRect& rRect ) const;

	// + ������, ����������ظ�CMythSize�����ֵ����rSize�󣬹����CMythSize����
	CMythSize operator + ( const CMythSize& rSize ) const
		{ return CMythSize( mCX + rSize.mCX, mCY + rSize.mCY ); }
};

// CMythPoint ��
/*
	CMythPoint �Ƚ����� MFC CPoint, ʵ�����������  mX �� mY ��Ա�ǹ����ģ�����
	CMythPoint �ṩ��ά�����������ݵĳ�Ա����
*/
class CMythPoint
{
public:
	int		mX;
	int		mY;

public:
	// CMythPoint ���캯��, Ĭ�Ϲ���һ�� CMythPoint( 0, 0 ) ����
	CMythPoint( ) { }//: mX( 0 ), mY( 0 )						{	}
	// CMythPoint ���캯��, ͨ��������������һ�� CMythPoint ����
	CMythPoint( int vX, int vY ) : mX( vX ), mY( vY )	{	}
public:
	// CMythPoint ���캯��
	~CMythPoint( )										{	}

public:
	// ��Ա���� Offset, �������������ֱ����vXOffset, vYOffset
	void offset( int vXOffset, int vYOffset )
		{ mX += vXOffset; mY += vYOffset; }

	// ��Ա���� Offset, �������������ֱ����rPoint.mX, rPoint.mY
	void offset( const CMythPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

	// ��Ա���� Offset, �������������ֱ�ƫ��rSize.mX, rSize.mY
	void offset( const CMythSize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }
	
	void clear( ) { mX = -1; mY = -1; }

	// ��Ա���� LimitDistance, �ж�Ŀ����뱾������Ƿ���vAmbit��
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
	// != ������, �ж�����CMythPoint�Ƿ�һ�� */
	bool operator != ( const CMythPoint& rPoint ) const
		{ return mX != rPoint.mX || mY != rPoint.mY; }

	// == ������, �ж�����CMythPoint�Ƿ�һ�� */
	bool operator == ( const CMythPoint& rPoint ) const
		{ return mX == rPoint.mX && mY == rPoint.mY; }

	// += ������, �ò�������һ��CMythSize���� */
	void operator += ( const CMythSize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

	// += ������, �ò�������һ��CMythPoint���� */
	void operator += ( const CMythPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

	// -= ������, �ò�����ȥһ��CMythSize���� */
 	void operator -= ( const CMythSize& rSize )
		{ mX -= rSize.mCX; mY -= rSize.mCY; }

	// -= ������, �ò�����ȥһ��CMythPoint���� */
 	void operator -= ( const CMythPoint& rPoint )
		{ mX -= rPoint.mX; mY -= rPoint.mY; }

	// - ������, �ò������ظ�CMythPoint�����ȥһ��CMythPoint����󣬹����CMythPoint���� */
	CMythPoint operator - ( const CMythPoint& rPoint ) const
		{ return CMythPoint( mX - rPoint.mX, mY - rPoint.mY ); }

	// - ������, �ò������ظ�CMythPoint�����ȥһ��CMythSize����󣬹����CMythPoint����
	CMythPoint operator - ( const CMythSize& rSize ) const
		{ return CMythPoint( mX - rSize.mCX, mY - rSize.mCY ); }

	// - ������, �ò�������CMythRect�����ȥ��CMythPoint����󣬹����CMythRect����
	CMythRect operator - ( const CMythRect& rRect ) const;

	// - ������, �ò������ظ�CMythPoint����ȡ���󣬹����CMythPoint����
	CMythPoint operator - ( ) const
		{ return CMythPoint( -mX, -mY ); }

	// + ������, �ò������ظ�CMythPoint�������һ��CMythPoint����󣬹����CMythPoint����
	CMythPoint operator + ( const CMythPoint& rPoint ) const
		{ return CMythPoint( mX + rPoint.mX, mY + rPoint.mY ); }

	// + ������, �ò������ظ�CMythPoint�������һ��CMythSize����󣬹����CMythPoint����
	CMythPoint operator + ( const CMythSize& rSize ) const
		{ return CMythPoint( mX + rSize.mCX, mY + rSize.mCY ); }

	// + ������, �ò�������CMythRect������ϸ�CMythPoint����󣬹����CMythRect����
	CMythRect operator + ( const CMythRect& rRect ) const;
};

// CMythRect ��,��Ϊvs��������CRect�����Լ�ǰ׺����������
/*
	CMythRect �Ƚ����� MFC CMythRect, ʵ������Ծ�������  mTopLeft �� mBottomRight ��Ա�ǹ����ģ�����
	CMythRect �ṩ��ά�����������ݵĳ�Ա����, CMythRect�Ĺ��캯��Ĭ�Ͻ�����NormalizeRect, �ú�����ʹ
	��һ��CMythRect����������, ����:mTopLeft( 10, 10 ) �� mBottomRight( 20, 20 )��һ�������ľ���, ��
	һ��mTopLeft( 20, 20 ) �� mBottomRight( 10, 10 )��һ���������ľ���, NormalizeRect��ʹ�������ľ�
	����������һ���������ľ�����ʹ��ĳЩCMythRect�ĳ�Ա����ʱ���ܻᷢ������
*/
class CMythRect
{
public:
	CMythPoint	mTopLeft;					//! CMythPoint ��,���ε����Ͻ�
	CMythPoint	mBottomRight;				//! CMythPoint ��,���ε����½�

public:
	// CMythRect ���캯��, Ĭ�Ϲ���һ�� CMythRect( 0, 0, 0, 0 ) ����
	CMythRect( )	{ normalizeRect( ); }

	// \brief CMythRect ���캯��, ͨ��4���������� CMythRect ����, 
	CMythRect( int vLeft, int vTop, int vRight, int vBottom, bool vNormalize = true ) : mTopLeft( vLeft, vTop ), mBottomRight( vRight, vBottom )
		{ if ( vNormalize ) normalizeRect( ); }

	// CMythRect ���캯��, ͨ��1�� CMythPoint ��һ�� CMythSize ���� CMythRect ����, 
	CMythRect( const CMythPoint& rPoint, const CMythSize& rSize, bool vNormalize = true ) : mTopLeft( rPoint ), mBottomRight( rPoint + rSize )
		{ if ( vNormalize ) normalizeRect( ); }

	// CMythRect ���캯��, ͨ��2�� CMythPoint ���� CMythRect ����, 
	CMythRect( const CMythPoint& rTopLeft, const CMythPoint& rBottomRight, bool vNormalize = true ) : mTopLeft( rTopLeft ), mBottomRight( rBottomRight )
		{ if ( vNormalize ) normalizeRect( ); }
public:
	// CMythRect ���캯��
	~CMythRect( ) {	}

public:
	// ��Ա���� BottomLeft, 
	CMythPoint bottomLeft( ) const
		{ return CMythPoint( mTopLeft.mX, mBottomRight.mY );	}

	// ��Ա���� TopRight, 
	CMythPoint topRight( ) const
		{ return CMythPoint( mBottomRight.mX, mTopLeft.mY );	}

	// ��Ա���� BottomRight, 
	const CMythPoint& bottomRight() const
		{ return mBottomRight; }

	CMythPoint& bottomRight()
		{ return mBottomRight; }

	// ��Ա���� TopLeft, 
	const CMythPoint& topLeft() const
		{ return mTopLeft; }

	CMythPoint& topLeft()
		{ return mTopLeft; }

	// ��Ա���� CenterPoint, 
	CMythPoint centerPoint( ) const
		{ return CMythPoint( ( mTopLeft.mX + mBottomRight.mX ) >> 1, ( mTopLeft.mY + mBottomRight.mY ) >> 1 ); }

	// ��Ա���� EqualRect, �ж����������Ƿ����
	bool equalRect( const CMythRect& rRect )	const
		{ return mTopLeft == rRect.mTopLeft && mBottomRight == rRect.mBottomRight; }

	// ��Ա���� DeflateRect, ��С�� CMythRect ����, ��Сָ���������Ŀ���
	void deflateRect( int vX, int vY )
		{ mTopLeft.mX += vX; mTopLeft.mY += vY; mBottomRight.mX -= vX; mBottomRight.mY -= vY; }

	// ��Ա���� DeflateRect, ��С�� CMythRect ����, ��Сָ���������Ŀ���
	void deflateRect( const CMythSize& rSize )
	{ 
		mTopLeft.mX		+= rSize.mCX; mTopLeft.mY		+= rSize.mCY; 
		mBottomRight.mX	-= rSize.mCX; mBottomRight.mY	-= rSize.mCY; 
	}

	// ��Ա���� DeflateRect, ��С�� CMythRect ����, ��Сָ���������Ŀ���
	void deflateRect( const CMythRect& rRect )
	{
		mTopLeft.mX		+= rRect.mTopLeft.mX;		mTopLeft.mY		+= rRect.mTopLeft.mY;		
		mBottomRight.mX -= rRect.mBottomRight.mX;	mBottomRight.mY -= rRect.mBottomRight.mY; 
	}

	// ��Ա���� DeflateRect, ��С�� CMythRect ����, ��Сָ���������Ŀ���
	void deflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX += vLeft; mTopLeft.mY += vTop; mBottomRight.mX -= vRight; mBottomRight.mY -= vBottom; }

	// ��Ա���� InflateRect, ����� CMythRect ����, ����ָ���������ķ�ɢ
	void inflateRect( int vX, int vY )
		{ mTopLeft.mX -= vX; mTopLeft.mY -= vY; mBottomRight.mX += vX; mBottomRight.mY += vY; }

	// ��Ա���� InflateRect, ����� CMythRect ����, ����ָ���������ķ�ɢ
	void inflateRect( const CMythSize& rSize )
	{ 
		mTopLeft.mX		-= rSize.mCX;	mTopLeft.mY		-= rSize.mCY;			
		mBottomRight.mX += rSize.mCX;	mBottomRight.mY	+= rSize.mCY; 
	}

	// ��Ա���� InflateRect, ����� CMythRect ����, ����ָ���������ķ�ɢ
	void inflateRect( const CMythRect& rRect )
	{
		mTopLeft.mX		-= rRect.mTopLeft.mX;		mTopLeft.mY		-= rRect.mTopLeft.mY;	
		mBottomRight.mX += rRect.mBottomRight.mX;	mBottomRight.mY += rRect.mBottomRight.mY; 
	}

	// ��Ա���� InflateRect, ����� CMythRect ����, ����ָ���������ķ�ɢ
	void inflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX -= vLeft; mTopLeft.mY -= vTop; mBottomRight.mX += vRight; mBottomRight.mY += vBottom; }

	//! ��Ա���� Height, ���ظ� CMythRect ����ĸ߶�
	/*! \return ���ظ� CMythRect ����ĸ߶� */
	int height( ) const
		{ return mBottomRight.mY - mTopLeft.mY + 1; }

	// ��Ա���� Width, ���ظ� CMythRect ����Ŀ��
	int	width( ) const	
		{ return mBottomRight.mX - mTopLeft.mX + 1; }

	// ��Ա���� IsRectEmpty, ���ظ� CMythRect ����Ŀ�͸��ǲ���ȫ����0
	bool isRectEmpty( ) const
		{ return width( ) <= 0 || height( ) <= 0; }

	// ��Ա���� IsRectNull, ���ظ� CMythRect ������ĸ����Ƿ�ȫΪ0
	bool isRectNull( ) const
		{ return mTopLeft.mX == 0 && mTopLeft.mY == 0 && mBottomRight.mX == 0 && mBottomRight.mY == 0; }

	// ��Ա���� MoveToX, ����� CMythRect ����vX,�����겻��
	void moveToX( int vX )	
		{ mBottomRight.mX = vX + width( ); mTopLeft.mX = vX; }

	// ��Ա���� MoveToY, ����� CMythRect ����vY,�����겻��
	void moveToY( int vY )	
		{ mBottomRight.mY = vY + height( ); mTopLeft.mY = vY; }

	// ��Ա���� MoveToXY, ����� CMythRect ����vX, vY
	void moveToXY( int vX, int vY )
	{
		mBottomRight.mX = vX + width( );	mTopLeft.mX	= vX; 
		mBottomRight.mY = vY + height( ); 	mTopLeft.mY	= vY;  
	}

	// ��Ա���� MoveToXY, ����� CMythRect ����rPoint
	void moveToXY( const CMythPoint& rPoint )
	{
		mBottomRight.mX = rPoint.mX + width( ); 	mTopLeft.mX = rPoint.mX; 
		mBottomRight.mY = rPoint.mY + height( );	mTopLeft.mY = rPoint.mY; 
	}

	// ��Ա���� OffsetRect, �ƶ��� CMythRect ����
	void offsetRect( int vX, int vY )					 
		{ *this += CMythPoint( vX, vY ); }

	// ��Ա���� OffsetRect, �ƶ��� CMythRect ����
	void offsetRect( const CMythPoint& rPoint )
		{ *this += rPoint; }

	// ��Ա���� OffsetRect, �ƶ��� CMythRect ����
	void offsetRect( const CMythSize& rSize )				 
		{ *this += rSize; }	

	// ��Ա���� SetRect, ���� CMythRect ����
	void setRect( int vLeft, int vTop, int vRight, int vBottom )
	{
		mTopLeft.mX		= vLeft;	mTopLeft.mY		= vTop; 
		mBottomRight.mX = vRight;	mBottomRight.mY = vBottom; 
	}

	// ��Ա���� SetRect, ͨ��1�� CMythPoint ��1�� CMythSize ���� CMythRect ����, 
	void setRect( const CMythPoint& rPoint, const CMythSize& rSize )
		{ mTopLeft = rPoint; mBottomRight = rPoint + rSize; }

	// ��Ա���� SetRect, ͨ��2�� CMythPoint ���� CMythRect ����, 
	void setRect( const CMythPoint& rTopLeft, const CMythPoint& rBottomRight )
		{ mTopLeft = rTopLeft; mBottomRight	= rBottomRight; }

	// ��Ա���� SetRectEmpty, ���� CMythRect ������0, 
	void setRectEmpty( )
		{ mTopLeft.mX = 0; mTopLeft.mY = 0; mBottomRight.mX = 0; mBottomRight.mY = 0; }

	// ��Ա���� Size, ���ظ� CMythRect ����Ŀ�Ⱥ͸߶�
	CMythSize size( ) const
		{ return CMythSize( width( ), height( ) ); }

	// ��Ա���� NormalizeRect, �������� CMythRect ����
	void normalizeRect( );

	// ��Ա���� PtInRect, �ж�ָ�������Ƿ��ڸ� CMythRect ������
	bool ptInRect( const CMythPoint&	rPoint ) const;

	// ��Ա���� IntersectRect, �������� CMythRect ����Ľ���
	bool intersectRect( const CMythRect& rSrcRect1, const CMythRect& rSrcRect2 );

	// ��Ա���� UnionRect, �������� CMythRect ����Ĳ���
	bool unionRect( const CMythRect& rSrcRectl, const CMythRect& rSrcRect2 );

	//! �������
	int subtractRect( const CMythRect& rDesRect, CMythRect* pRectList );

public:
	// != ������, �ж����� CMythRect �����Ƿ����
	bool operator != ( const CMythRect& rRect ) const
		{ return !equalRect( rRect ); }

	// == ������, �ж����� CMythRect �����Ƿ����
	bool operator == ( const CMythRect& rRect ) const
		{ return equalRect( rRect ); }

	// &= ������, ����� CMythRect ����� ָ�� CMythRect ����Ľ���
	void operator &= ( const CMythRect& rRect )
		{ intersectRect( *this, rRect ); }

	// |= ������, ����� CMythRect ����� ָ�� CMythRect ����Ĳ���
	void operator |= ( const CMythRect& rRect )
		{ unionRect( *this, rRect ); }

	// & ������, ����� CMythRect ����� ָ�� CMythRect ����Ľ���
	CMythRect operator & ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult;
		tRectResult.intersectRect( *this, rRect );
		return tRectResult;
	}

	// | ������, ����� CMythRect ����� ָ�� CMythRect ����Ĳ���
	CMythRect operator | ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult;
		tRectResult.unionRect( *this, rRect );
		return tRectResult;
	}

	// - ������, �Ѹ� CMythRect ����ƫ�� rPoint ����λ
	CMythRect operator - ( const CMythPoint& rPoint ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( -rPoint );
		return tRectResult;
	}

	// - ������, �Ѹ� CMythRect ����ƫ�� rSize ����λ
	CMythRect operator - ( const CMythSize& rSize ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( -rSize );
		return tRectResult;
	}

	// - ������, ��С�� CMythRect ������С�ĳߴ��� rRect ָ��
	CMythRect operator - ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.deflateRect( rRect );
		return tRectResult;
	}

	// + ������, �Ѹ� CMythRect ����ƫ�� rPoint ����λ
	CMythRect operator + ( const CMythPoint& rPoint ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( rPoint );
		return tRectResult;
	}

	// + ������, �Ѹ� CMythRect ����ƫ�� rSize ����λ
	CMythRect operator + ( const CMythSize& rSize ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.offsetRect( rSize );
		return tRectResult;
	}

	// + ������, ����� CMythRect ��������ĳߴ��� rRect ָ��
	CMythRect operator + ( const CMythRect& rRect ) const
	{
		CMythRect tRectResult = *this;
		tRectResult.inflateRect( rRect );
		return tRectResult;
	}

	// += ������, �Ѹ� CMythRect ����ƫ�� rPoint ����λ
	void operator += ( const CMythPoint& rPoint )
		{ mTopLeft += rPoint; mBottomRight += rPoint; }

	// += ������, �Ѹ� CMythRect ����ƫ�� rSize ����λ
	void operator += ( const CMythSize& rSize )
		{ mTopLeft += rSize; mBottomRight += rSize; }

	// += ������, ����� CMythRect ��������ĳߴ��� rRect ָ��
	void operator += ( const CMythRect& rRect )
		{ inflateRect( rRect );	}

	// += ������, �Ѹ� CMythRect ����ƫ�� rPoint ����λ
	void operator -= ( const CMythPoint& rPoint )
		{ mTopLeft -= rPoint; mBottomRight -= rPoint; }

	// += ������, �Ѹ� CMythRect ����ƫ�� rSize ����λ
	void operator -= ( const CMythSize& rSize )
		{ mTopLeft -= rSize; mBottomRight -= rSize;	}

	// += ������, ��С�� CMythRect ������С�ĳߴ��� rRect ָ��
	void operator -= ( const CMythRect& rRect )
		{ deflateRect( rRect );	}
};

#endif