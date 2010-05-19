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

#include "mathlib.h"

namespace F3D {

    /**
     * Quaternion class for all games using F3D, from rsn:
     *  Author:  Brett Porter
     *  Email:   brettporter@yahoo.com
     *  Website: http://rsn.gamedev.net/pl3d
     */
    void Quaternion::fromAngles( const float *angles ) {
        float angle;
        double sr, sp, sy, cr, cp, cy;

        angle = angles[2]*0.5f;
        sy = sin( angle );
        cy = cos( angle );
        angle = angles[1]*0.5f;
        sp = sin( angle );
        cp = cos( angle );
        angle = angles[0]*0.5f;
        sr = sin( angle );
        cr = cos( angle );

        double crcp = cr*cp;
        double srsp = sr*sp;

        m_quat[0] = ( float )( sr*cp*cy-cr*sp*sy );
        m_quat[1] = ( float )( cr*sp*cy+sr*cp*sy );
        m_quat[2] = ( float )( crcp*sy-srsp*cy );
        m_quat[3] = ( float )( crcp*cy+srsp*sy );
    }

    void Quaternion::slerp( const Quaternion& q1, Quaternion& q2, float interp ) {
        // Decide if one of the quaternions is backwards
        int i;
        float a = 0, b = 0;
        for ( i = 0; i < 4; i++ ) {
            a += ( q1[i]-q2[i] )*( q1[i]-q2[i] );
            b += ( q1[i]+q2[i] )*( q1[i]+q2[i] );
        }
        if ( a > b )
            q2.inverse();

        float cosom = q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3];
        double sclq1, sclq2;

        if (( 1.0+cosom ) > 0.00000001 ) {
            if (( 1.0-cosom ) > 0.00000001 ) {
                double omega = acos( cosom );
                double sinom = sin( omega );
                sclq1 = sin(( 1.0-interp )*omega )/sinom;
                sclq2 = sin( interp*omega )/sinom;
            } else {
                sclq1 = 1.0-interp;
                sclq2 = interp;
            }
            for ( i = 0; i < 4; i++ )
                m_quat[i] = ( float )( sclq1*q1[i]+sclq2*q2[i] );
        } else {
            m_quat[0] = -q1[1];
            m_quat[1] = q1[0];
            m_quat[2] = -q1[3];
            m_quat[3] = q1[2];

            sclq1 = sin(( 1.0-interp ) * 0.5 * M_PI );
            sclq2 = sin( interp * 0.5 * M_PI );
            for ( i = 0; i < 3; i++ )
                m_quat[i] = ( float )( sclq1*q1[i]+sclq2*m_quat[i] );
        }
    }

    /**
     * Matrix class for all games using F3D, from rsn:
     *  Author:  Brett Porter
     *  Email:   brettporter@yahoo.com
     *  Website: http://rsn.gamedev.net/pl3d
     */
    Matrix::Matrix() {
#if (defined(DEBUG) && defined(MATH_DEBUG))
        printf("Matrix constructor...\n");
#endif
        loadIdentity();
    }

    Matrix::~Matrix() {
#if (defined(DEBUG) && defined(MATH_DEBUG))
        printf("Matrix destructor...\n");
#endif
    }

    void Matrix::postMultiply( const float *m2 ) {
        float newMatrix[16];
        const float *m1 = m_matrix;//, *m2 = matrix.m_matrix;

        newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
        newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
        newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
        newMatrix[3] = 0;

        newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
        newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
        newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
        newMatrix[7] = 0;

        newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
        newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
        newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
        newMatrix[11] = 0;

        newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
        newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
        newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
        newMatrix[15] = 1;

        set( newMatrix );
    }

    void Matrix::setTranslation( float trans_x, float trans_y, float trans_z ) {
        m_matrix[12] = trans_x;
        m_matrix[13] = trans_y;
        m_matrix[14] = trans_z;
    }

    void Matrix::setInverseTranslation( const float *translation ) {
        m_matrix[12] = -translation[0];
        m_matrix[13] = -translation[1];
        m_matrix[14] = -translation[2];
    }

    void Matrix::setRotationDegrees( const float *angles ) {
        float vec[3];

        vec[0] = ( float )( angles[0]*180.0/M_PI );
        vec[1] = ( float )( angles[1]*180.0/M_PI );
        vec[2] = ( float )( angles[2]*180.0/M_PI );

        setRotationRadians( vec[0], vec[1], vec[2] );
    }

    void Matrix::setInverseRotationDegrees( const float *angles ) {
        float vec[3];

        vec[0] = ( float )( angles[0]*180.0/M_PI );
        vec[1] = ( float )( angles[1]*180.0/M_PI );
        vec[2] = ( float )( angles[2]*180.0/M_PI );

        setInverseRotationRadians( vec[0], vec[1], vec[2] );
    }

    void Matrix::setRotationRadians( float angle_x, float angle_y, float angle_z ) {
        double cr = cos( angle_x );
        double sr = sin( angle_x );
        double cp = cos( angle_y );
        double sp = sin( angle_y );
        double cy = cos( angle_z );
        double sy = sin( angle_z );

        m_matrix[0] = ( float )( cp*cy );
        m_matrix[1] = ( float )( cp*sy );
        m_matrix[2] = ( float )( -sp );

        double srsp = sr*sp;
        double crsp = cr*sp;

        m_matrix[4] = ( float )( srsp*cy-cr*sy );
        m_matrix[5] = ( float )( srsp*sy+cr*cy );
        m_matrix[6] = ( float )( sr*cp );

        m_matrix[8] = ( float )( crsp*cy+sr*sy );
        m_matrix[9] = ( float )( crsp*sy-sr*cy );
        m_matrix[10] = ( float )( cr*cp );
    }

    void Matrix::setInverseRotationRadians( float anglex, float angley, float anglez ) {
        double cr = cos( anglex );
        double sr = sin( anglex );
        double cp = cos( angley );
        double sp = sin( angley );
        double cy = cos( anglez );
        double sy = sin( anglez );

        m_matrix[0] = ( float )( cp*cy );
        m_matrix[4] = ( float )( cp*sy );
        m_matrix[8] = ( float )( -sp );

        double srsp = sr*sp;
        double crsp = cr*sp;

        m_matrix[1] = ( float )( srsp*cy-cr*sy );
        m_matrix[5] = ( float )( srsp*sy+cr*cy );
        m_matrix[9] = ( float )( sr*cp );

        m_matrix[2] = ( float )( crsp*cy+sr*sy );
        m_matrix[6] = ( float )( crsp*sy-sr*cy );
        m_matrix[10] = ( float )( cr*cp );
    }

    void Matrix::setRotationQuaternion( const Quaternion& quat ) {
        m_matrix[0] = ( float )( 1.0 - 2.0*quat[1]*quat[1] - 2.0*quat[2]*quat[2] );
        m_matrix[1] = ( float )( 2.0*quat[0]*quat[1] + 2.0*quat[3]*quat[2] );
        m_matrix[2] = ( float )( 2.0*quat[0]*quat[2] - 2.0*quat[3]*quat[1] );

        m_matrix[4] = ( float )( 2.0*quat[0]*quat[1] - 2.0*quat[3]*quat[2] );
        m_matrix[5] = ( float )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[2]*quat[2] );
        m_matrix[6] = ( float )( 2.0*quat[1]*quat[2] + 2.0*quat[3]*quat[0] );

        m_matrix[8] = ( float )( 2.0*quat[0]*quat[2] + 2.0*quat[3]*quat[1] );
        m_matrix[9] = ( float )( 2.0*quat[1]*quat[2] - 2.0*quat[3]*quat[0] );
        m_matrix[10] = ( float )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[1]*quat[1] );
    }

    /**
     * Vector class for all games using F3D, from rsn:
     *  Author:  Brett Porter
     *  Email:   brettporter@yahoo.com
     *  Website: http://rsn.gamedev.net/pl3d
     */
    Vector::Vector() {
#if (defined(DEBUG) && defined(MATH_DEBUG))
        printf("Vector constructor...\n");
#endif
        reset();
    }

    Vector::Vector( float vec_x, float vec_y, float vec_z  ) {
#if (defined(DEBUG) && defined(MATH_DEBUG))
        printf("Vector constructor with values[vec_x, vec_y, vec_z]...\n");
#endif
        set( vec_x, vec_y, vec_z );
        m_vector[3] = 1;
    }

    Vector::~Vector() {
#if (defined(DEBUG) && defined(MATH_DEBUG))
        printf("Vector destructor...\n");
#endif
    }

    void Vector::transform(const Matrix *m ) {
        double vector[4];
        const float *matrix = m->getMatrix();

        vector[0] = m_vector[0]*matrix[0]+m_vector[1]*matrix[4]+m_vector[2]*matrix[8]+matrix[12];
        vector[1] = m_vector[0]*matrix[1]+m_vector[1]*matrix[5]+m_vector[2]*matrix[9]+matrix[13];
        vector[2] = m_vector[0]*matrix[2]+m_vector[1]*matrix[6]+m_vector[2]*matrix[10]+matrix[14];
        vector[3] = m_vector[0]*matrix[3]+m_vector[1]*matrix[7]+m_vector[2]*matrix[11]+matrix[15];

        m_vector[0] = ( float )( vector[0] );
        m_vector[1] = ( float )( vector[1] );
        m_vector[2] = ( float )( vector[2] );
        m_vector[3] = ( float )( vector[3] );
    }

    void Vector::transform3(const Matrix *m ) {
        double vector[3];
        const float *matrix = m->getMatrix();

        vector[0] = m_vector[0]*matrix[0]+m_vector[1]*matrix[4]+m_vector[2]*matrix[8];
        vector[1] = m_vector[0]*matrix[1]+m_vector[1]*matrix[5]+m_vector[2]*matrix[9];
        vector[2] = m_vector[0]*matrix[2]+m_vector[1]*matrix[6]+m_vector[2]*matrix[10];

        m_vector[0] = ( float )( vector[0] );
        m_vector[1] = ( float )( vector[1] );
        m_vector[2] = ( float )( vector[2] );
        m_vector[3] = 1;
    }
}
