//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLMath/EulerAngles.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Quaternion.h>
#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

using namespace PLMath;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(EulerAngles) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() :
			/* some setup */
			PI(static_cast<float>(Math::Pi)),
			fX(0.0f),
			fY(0.0f),
			fZ(0.0f)
		{
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Objects for testing
		float PI;

		float fX, fY, fZ;
		Matrix3x3 mRotX, mRotY, mRotZ;
		Matrix3x3 mEulX, mEulY, mEulZ;
		Matrix3x3 mRot, mEul;
	};

	TEST(CHECK_MATRIX3X3_MACRO) {
		// checking testmacro: CHECK_MATRIX3X3
		Matrix3x3 m1, m2;

		m1.Set(1,2,3,4,5,6,7,8,9);
		m2.Set(1,2,3,4,5,6,7,8,9);

		CHECK_MATRIX3X3(m1, m2, 0.01f);
	}

	TEST_FIXTURE(ConstructTest, Matrix_Functions) {
		int steps = 10;

		// actual test
		for (float x=0.0; x<=2*PI; x+=PI/steps) {
			mRotX.FromEulerAngleX(x);

			for (float y=0.0; y<=2*PI; y+=PI/steps) {
				mRotY.FromEulerAngleY(y);

				for (float z=0.0; z<=2*PI; z+=PI/steps) {
					mRotZ.FromEulerAngleZ(z);

					mRot =  mRotX * mRotY * mRotZ;

					EulerAngles::FromMatrix(mRot, fX, fY, fZ);

					// rotate back to get expected result
					mEulX.FromEulerAngleX(fX);
					mEulY.FromEulerAngleY(fY);
					mEulZ.FromEulerAngleZ(fZ);

					mEul = mEulZ * mEulY * mEulX;

					//printf("x=%f y=%f z=%f\n", x, y, z);
					CHECK_MATRIX3X3(mEul, mRot, 0.0001f);

					EulerAngles::ToMatrix(fX, fY, fZ, mRot);

					CHECK_MATRIX3X3(mEul, mRot, 0.0001f);
				}
			}
		}
	}

	TEST_FIXTURE(ConstructTest, Matrix_Functions_SingleCase) {
            fX=PI/4;
            fY=0;
            fZ=0;

            mRotX.FromEulerAngleX(fX);
            mRotY.FromEulerAngleY(fY);
            mRotZ.FromEulerAngleZ(fZ);

            mRot =  mRotX * mRotY * mRotZ;

            EulerAngles::FromMatrix(mRot, fX, fY, fZ);

            mEulX.FromEulerAngleX(fX);
            mEulY.FromEulerAngleY(fY);
            mEulZ.FromEulerAngleZ(fZ);

            mEul = mEulZ * mEulY * mEulX;

            //printf("x=%f y=%f z=%f\n", fX, fY, fZ);
            CHECK_MATRIX3X3(mEul, mRot, 0.01f);
    }

	TEST_FIXTURE(ConstructTest, ToQuaternion) {
		Quaternion q;
		EulerAngles::ToQuaternion(fX, fY, fZ, q);

		CHECK_CLOSE(0.0f, q.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, q.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, q.z, Math::Epsilon);

		fX = static_cast<float>(Math::Pi2);
		fY = static_cast<float>(Math::Pi2);
		fZ = static_cast<float>(Math::Pi2);

		EulerAngles::ToQuaternion(fX, fY, fZ, q);

		CHECK_CLOSE(0.0f, q.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, q.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, q.z, Math::Epsilon);

		fX = static_cast<float>(Math::Pi);
		fY = static_cast<float>(Math::Pi);
		fZ = static_cast<float>(Math::Pi);

		EulerAngles::ToQuaternion(fX, fY, fZ, q);

		CHECK_CLOSE(0.0f, q.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, q.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, q.z, Math::Epsilon);

		//printf("%s\n", q.ToString().GetASCII());
	}

	TEST_FIXTURE(ConstructTest, FromQuaternion) {
		Quaternion q(mRotX);

		EulerAngles::FromQuaternion(q, fX, fY, fZ);

		CHECK_CLOSE(0.0f, fX, Math::Epsilon);
		CHECK_CLOSE(0.0f, fY, Math::Epsilon);
		CHECK_CLOSE(0.0f, fZ, Math::Epsilon);

		fX = 0;
		fY = 0;
		fZ = 0;

		EulerAngles::FromQuaternion(q, fX, fY, fZ);

		CHECK_CLOSE(0.0f, fX, Math::Epsilon);
		CHECK_CLOSE(0.0f, fY, Math::Epsilon);
		CHECK_CLOSE(0.0f, fZ, Math::Epsilon);

		//printf("%s\n", q.ToString().GetASCII());
	}
}