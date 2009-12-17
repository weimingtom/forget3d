/*****************************************************************************
 * Copyright (C) 2009 The Forget3D Project by Martin Foo (i25ffz@hotmail.com)
 * ALL RIGHTS RESERVED
 *
 * License I
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITHOUT a fee is granted under following requirements:
 * - You make no money using this software.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * Licence II
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITH a fee is granted under following requirements:
 * - As soon as you make money using this software, you have to pay a
 *   licence fee. Until this point of time, you can use this software
 *   without a fee.
 * 	 Please contact Martin Foo (i25ffz@hotmail.com) for further details.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHORS
 * BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 * INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE, SAVINGS OR
 * REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR NOT THE AUTHORS HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *****************************************************************************/

#ifndef F3D_MATHLIB_H_
#define F3D_MATHLIB_H_

#include "f3d.h"

namespace F3D {

    /**
     * Quaternion class for all games using F3D, from rsn:
     *  Author:  Brett Porter
     *  Email:   brettporter@yahoo.com
     *  Website: http://rsn.gamedev.net/pl3d
     */
    class Quaternion {
    public:
        /**
         * Constructor. Retrieves values from angles.
         */
        Quaternion( const float *angles ) {
#if (defined(DEBUG) && defined(MATH_DEBUG))
            printf("Quaternion constructor with angles...\n");
#endif
            fromAngles( angles );
        }

        /**
         * Cosntructor. Retrieves values from an interpolation between two other quaternions.
         */
        Quaternion( const Quaternion& q1, Quaternion& q2, float interp ) {
#if (defined(DEBUG) && defined(MATH_DEBUG))
            printf("Quaternion constructor with q1, q2, interp...\n");
#endif
            slerp( q1, q2, interp );
        }

        /**
         * Set values from angles.
         *     angles (x,y,z) Euler rotation angles.
         */
        void fromAngles( const float *angles );

        /**
         * Set values from an interpolation between two other quaternions.
         * This will also modify the second quaternion if it is backwards.
         * q1, q2	The quaternions to interpolate between
         * interp	A value from 0.0-1.0 indicating the linear interpolation parameter.
        */
        void slerp( const Quaternion& q1, Quaternion& q2, float interp );

        /*	Get a value from the quaternion. */
        float operator[]( int index ) const {
            return m_quat[index];
        }

        /*	Set the inverse of the quaternion. */
        void inverse();

    private:
        //	Quaternion data
        float m_quat[4];
    };

    inline void Quaternion::inverse() {
        m_quat[0] = -m_quat[0];
        m_quat[1] = -m_quat[1];
        m_quat[2] = -m_quat[2];
        m_quat[3] = -m_quat[3];
    }

    /**
     * Matrix class for all games using F3D, from rsn:
     *  Author:  Brett Porter
     *  Email:   brettporter@yahoo.com
     *  Website: http://rsn.gamedev.net/pl3d
     */
    class Matrix {
    public:
        /*	Constructor. */
        Matrix();

        /*	Destructor. */
        ~Matrix();

        /*	Set to identity. */
        void loadIdentity();

        /*	Set the values of the matrix. */
        void set( const float *matrix );

        /*	Post-multiply by another matrix. */
        void postMultiply( const float *m2 );

        /*	Set the translation of the current matrix. Will erase any previous values. */
        void setTranslation( const float *translation );

        /*	Set the inverse translation of the current matrix. Will erase any previous values. */
        void setInverseTranslation( const float *translation );

        /*	Make a rotation matrix from Euler angles. The 4th row and column are unmodified. */
        void setRotationRadians( const float *angles );

        /*	Make a rotation matrix from Euler angles. The 4th row and column are unmodified. */
        void setRotationDegrees( const float *angles );

        /*	Make a rotation matrix from a quaternion. The 4th row and column are unmodified. */
        void setRotationQuaternion( const Quaternion& quat );

        /*	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified. */
        void setInverseRotationRadians( const float *angles );

        /*	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified. */
        void setInverseRotationDegrees( const float *angles );

        /*	Get the matrix data. */
        const float *getMatrix() const {
            return m_matrix;
        }

        /*	Translate a vector by the inverse of the translation part of this matrix. */
        void inverseTranslateVect( float *pVect ) const;

        /*	Rotate a vector by the inverse of the rotation part of this matrix. */
        void inverseRotateVect( float *pVect ) const;

    private:
        //	Matrix data, stored in column-major order
        float m_matrix[16];
    };

    inline void Matrix::set( const float *matrix ) {
        memcpy( m_matrix, matrix, sizeof( float )*16 );
    }

    inline void Matrix::loadIdentity() {
        memset( m_matrix, 0, sizeof( float )*16 );
        m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = 1;
    }

    inline void Matrix::inverseRotateVect( float *pVect ) const {
        float vec[3];

        vec[0] = pVect[0]*m_matrix[0]+pVect[1]*m_matrix[1]+pVect[2]*m_matrix[2];
        vec[1] = pVect[0]*m_matrix[4]+pVect[1]*m_matrix[5]+pVect[2]*m_matrix[6];
        vec[2] = pVect[0]*m_matrix[8]+pVect[1]*m_matrix[9]+pVect[2]*m_matrix[10];

        memcpy( pVect, vec, sizeof( float )*3 );
    }

    inline void Matrix::inverseTranslateVect( float *pVect ) const {
        pVect[0] = pVect[0]-m_matrix[12];
        pVect[1] = pVect[1]-m_matrix[13];
        pVect[2] = pVect[2]-m_matrix[14];
    }

    /**
     * Vector class for all games using F3D, from rsn:
     *  Author:  Brett Porter
     *  Email:   brettporter@yahoo.com
     *  Website: http://rsn.gamedev.net/pl3d
     */
    class Vector {
    public:
        /*	Constructor. (0, 0, 0, 1) */
        Vector();

        /*	Constructor. 3 float values. */
        Vector( const float *vector );

        /*	Destructor. */
        ~Vector();

        /*	Retrieve vector. */
        const float *getVector() const {
            return m_vector;
        }

        /*	Transform the vector by a matrix. */
        void transform( const Matrix *m );

        /*	Transform the vector by a matrix, not including the scaling or transformation elements (use only top-left 3x3 of matrix). */
        void transform3( const Matrix *m );

        /*	Set the values of the vector. Takes 3 float values. */
        void set( const float *vector );

        /*	Translate by another vector. */
        void add( const Vector& v );

        /*	Reset to (0, 0, 0, 1). */
        void reset();

        /*	Get the length of the vector. */
        float length() const;

        /*	Normalize (make it a unit vector). */
        void normalize();

        const float operator[]( int index ) const {
            return m_vector[index];
        }

    private:
        //	Vector data
        float m_vector[4];
    };

    inline void Vector::reset() {
        m_vector[0] = m_vector[1] = m_vector[2] = 0;
        m_vector[3] = 1;
    }

    inline void Vector::set( const float *values ) {
        m_vector[0] = values[0];
        m_vector[1] = values[1];
        m_vector[2] = values[2];
    }

    inline void Vector::add( const Vector& v ) {
        m_vector[0] += v.m_vector[0];
        m_vector[1] += v.m_vector[1];
        m_vector[2] += v.m_vector[2];
        m_vector[3] += v.m_vector[3];
    }

    inline void Vector::normalize() {
        float len = length();

        m_vector[0] /= len;
        m_vector[1] /= len;
        m_vector[2] /= len;
    }

    inline float Vector::length() const {
        return ( float )sqrt( m_vector[0]*m_vector[0]+m_vector[1]*m_vector[1]+m_vector[2]*m_vector[2] );
    }

}

#endif /* F3D_MATHLIB_H_ */
