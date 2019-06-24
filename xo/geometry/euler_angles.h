#pragma once

#include "quat_type.h"
#include "xo/utility/smart_enum.h"

namespace xo
{
	/// Euler order for Euler angle conversions
	xo_smart_enum_class( euler_order, xyz, xzy, yxz, yzx, zxy, zyx );

	template< typename T > using euler_angles = vec3_< radian_< T > >;

	template< angle_unit U, typename T > quat_<T> quat_from_x_angle( const angle_<U, T>& a )
	{ T ha = T( 0.5 ) * a.rad_value(); return quat_<T>( std::cos( ha ), std::sin( ha ), T( 0 ), T( 0 ) ); }

	template< angle_unit U, typename T > quat_<T> quat_from_y_angle( const angle_<U, T>& a )
	{ T ha = T( 0.5 ) * a.rad_value(); return quat_<T>( std::cos( ha ), T( 0 ), std::sin( ha ), T( 0 ) ); }

	template< angle_unit U, typename T > quat_<T> quat_from_z_angle( const angle_<U, T>& a )
	{ T ha = T( 0.5 ) * a.rad_value(); return quat_<T>( std::cos( ha ), T( 0 ), T( 0 ), std::sin( ha ) ); }

	/// make quaternion from Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( const angle_<U, T>& x, const angle_<U, T>& y, const angle_<U, T>& z, euler_order eo = euler_order::xyz ) {
		quat_<T> qx = quat_from_x_angle( x );
		quat_<T> qy = quat_from_y_angle( y );
		quat_<T> qz = quat_from_z_angle( z );

		// #todo: more efficient
		switch ( eo )
		{
		case xo::euler_order::xyz: return qx * qy * qz;
		case xo::euler_order::xzy: return qx * qz * qy;
		case xo::euler_order::yxz: return qy * qx * qz;
		case xo::euler_order::yzx: return qy * qz * qx;
		case xo::euler_order::zxy: return qz * qx * qy;
		case xo::euler_order::zyx: return qz * qy * qx;
		default: xo_error( "quat_from_euler: invalid euler_order" );
		}
	}

	/// make quaternion from vec3 of Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( const vec3_< angle_<U, T> >& eu, euler_order eo = euler_order::xyz ) {
		return quat_from_euler( eu.x, eu.y, eu.z, eo );
	}

	// #todo: verify / replace
	template< typename T > radian_< T > pitch( const quat_<T>& q ) {
		T tx = T( 2 ) * q.x, tz = T( 2 ) * q.z;
		T twx = tx * q.w, txx = tx * q.x, tyz = tz * q.y, tzz = tz * q.z;
		return radian_< T >( atan2( tyz + twx, T( 1 ) - ( txx + tzz ) ) );
	}

	// #todo: verify / replace
	template< typename T > radian_< T > yaw( const quat_<T>& q ) {
		T tx = T( 2 ) * q.x, ty = T( 2 ) * q.y, tz = T( 2 ) * q.z;
		T twy = ty * q.w, txx = tx * q.x, txz = tz * q.x, tyy = ty * q.y;
		return radian_< T >( atan2( txz + twy, T( 1 ) - ( txx + tyy ) ) );
	}

	// #todo: verify / replace
	template< typename T > radian_< T > roll( const quat_<T>& q ) {
		T ty = T( 2 ) * q.y, tz = T( 2 ) * q.z;
		T twz = tz * q.w, txy = ty * q.x, tyy = ty * q.y, tzz = tz * q.z;
		return radian_< T >( atan2( txy + twz, T( 1 ) - ( tyy + tzz ) ) );
	}

	/// get Euler angles from quat
	// #todo: add Euler order as argument, use if constexpr
	template< typename T > euler_angles< T > euler_from_quat( const quat_<T>& q )
	{
		// #todo: verify / replace
		return euler_angles< T >( pitch( q ), yaw( q ), roll( q ) );
	}

}
