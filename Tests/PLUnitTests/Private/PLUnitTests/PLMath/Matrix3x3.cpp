//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/EulerAngles.h>

using namespace PLMath;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Matrix3x3) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() {
			/* some setup */
			float val = 0.0f;
			for (int i=0; i<9; i++, val+=1.0f)
				fT[i]=val;
		}
		~ConstructTest() {
			/* some teardown */

		}

		// Container for testing
		float fT[9];
		Matrix3x3 mM, mMT;
		Vector3 vV3;
	};

	bool CheckAxisAngle(float fX, float fY, float fZ, float fAngle, float fExpectedAngle)
	{
		float fXT, fYT, fZT, fAngleT;
		Matrix3x3 mRot;

		// Degree to radian
		fAngle         = static_cast<float>(fAngle*Math::DegToRad);
		fExpectedAngle = static_cast<float>(fExpectedAngle*Math::DegToRad);

		mRot.FromAxisAngle(fX, fY, fZ, fAngle);
		mRot.ToAxisAngle(fXT, fYT, fZT, fAngleT);
		if (Math::AreEqual(fX, fXT) && Math::AreEqual(fY, fYT) && Math::AreEqual(fZ, fZT) &&
			Math::AreEqual(fAngleT, fExpectedAngle))
			return true; // Success
		else return false; // Error!
	}

	// Matrix constructors & copy
	TEST_FIXTURE(ConstructTest, Constructors_and_Getter) {
		// Matrix must be identity
		CHECK_EQUAL(1.0f, mM.xx);
		CHECK_EQUAL(0.0f, mM.xy);
		CHECK_EQUAL(0.0f, mM.xz);
		CHECK_EQUAL(0.0f, mM.yx);
		CHECK_EQUAL(1.0f, mM.yy);
		CHECK_EQUAL(0.0f, mM.yz);
		CHECK_EQUAL(0.0f, mM.zx);
		CHECK_EQUAL(0.0f, mM.zy);
		CHECK_EQUAL(1.0f, mM.zz);

		// Matrix3x3(const float[])
		mM = Matrix3x3(fT);
		// fM[9]
		CHECK_EQUAL(0.0f, mM.fM[0]);
		CHECK_EQUAL(3.0f, mM.fM[3]);
		CHECK_EQUAL(6.0f, mM.fM[6]);
		CHECK_EQUAL(1.0f, mM.fM[1]);
		CHECK_EQUAL(4.0f, mM.fM[4]);
		CHECK_EQUAL(7.0f, mM.fM[7]);
		CHECK_EQUAL(2.0f, mM.fM[2]);
		CHECK_EQUAL(5.0f, mM.fM[5]);
		CHECK_EQUAL(8.0f, mM.fM[8]);

		// The entry of the matrix in row r (0 <= r <= 2) and column c (0 <= c <= 2) is
		// stored at index i = r+3*c (0 <= i <= 8).
		CHECK_EQUAL(0.0f, mM.fM[0+3*0]);
		CHECK_EQUAL(3.0f, mM.fM[0+3*1]);
		CHECK_EQUAL(6.0f, mM.fM[0+3*2]);
		CHECK_EQUAL(1.0f, mM.fM[1+3*0]);
		CHECK_EQUAL(4.0f, mM.fM[1+3*1]);
		CHECK_EQUAL(7.0f, mM.fM[1+3*2]);
		CHECK_EQUAL(2.0f, mM.fM[2+3*0]);
		CHECK_EQUAL(5.0f, mM.fM[2+3*1]);
		CHECK_EQUAL(8.0f, mM.fM[2+3*2]);

		// xx, xy...
		CHECK_EQUAL(0.0f, mM.xx);
		CHECK_EQUAL(3.0f, mM.xy);
		CHECK_EQUAL(6.0f, mM.xz);
		CHECK_EQUAL(1.0f, mM.yx);
		CHECK_EQUAL(4.0f, mM.yy);
		CHECK_EQUAL(7.0f, mM.yz);
		CHECK_EQUAL(2.0f, mM.zx);
		CHECK_EQUAL(5.0f, mM.zy);
		CHECK_EQUAL(8.0f, mM.zz);

		// fM33[3][3]
		CHECK_EQUAL(0.0f, mM.fM33[0][0]);
		CHECK_EQUAL(3.0f, mM.fM33[1][0]);
		CHECK_EQUAL(6.0f, mM.fM33[2][0]);
		CHECK_EQUAL(1.0f, mM.fM33[0][1]);
		CHECK_EQUAL(4.0f, mM.fM33[1][1]);
		CHECK_EQUAL(7.0f, mM.fM33[2][1]);
		CHECK_EQUAL(2.0f, mM.fM33[0][2]);
		CHECK_EQUAL(5.0f, mM.fM33[1][2]);
		CHECK_EQUAL(8.0f, mM.fM33[2][2]);

		// Matrix3x3(float fXX, float fXY, float fXZ,
		//			 float fYX, float fYY, float fYZ,
		//			 float fZX, float fZY, float fZZ)
		mM = Matrix3x3(0.0f, 0.1f, 0.2f,
			1.0f, 1.1f, 1.2f,
			2.0f, 2.1f, 2.2f);
		CHECK_EQUAL(0.0f, mM.xx);
		CHECK_EQUAL(0.1f, mM.xy);
		CHECK_EQUAL(0.2f, mM.xz);
		CHECK_EQUAL(1.0f, mM.yx);
		CHECK_EQUAL(1.1f, mM.yy);
		CHECK_EQUAL(1.2f, mM.yz);
		CHECK_EQUAL(2.0f, mM.zx);
		CHECK_EQUAL(2.1f, mM.zy);
		CHECK_EQUAL(2.2f, mM.zz);
		CHECK_EQUAL(0.0f, mM.fM[0]);
		CHECK_EQUAL(0.1f, mM.fM[3]);
		CHECK_EQUAL(0.2f, mM.fM[6]);
		CHECK_EQUAL(1.0f, mM.fM[1]);
		CHECK_EQUAL(1.1f, mM.fM[4]);
		CHECK_EQUAL(1.2f, mM.fM[7]);
		CHECK_EQUAL(2.0f, mM.fM[2]);
		CHECK_EQUAL(2.1f, mM.fM[5]);
		CHECK_EQUAL(2.2f, mM.fM[8]);
	}

	// Static variables
	TEST_FIXTURE(ConstructTest, Static_variables) {
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.xx);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.xy);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.xz);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.yx);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.yy);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.yz);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.zx);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.zy);
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.zz);

		CHECK_EQUAL(1.0f, Matrix3x3::Identity.xx);
		CHECK_EQUAL(0.0f, Matrix3x3::Identity.xy);
		CHECK_EQUAL(0.0f, Matrix3x3::Identity.xz);
		CHECK_EQUAL(0.0f, Matrix3x3::Identity.yx);
		CHECK_EQUAL(1.0f, Matrix3x3::Identity.yy);
		CHECK_EQUAL(0.0f, Matrix3x3::Identity.yz);
		CHECK_EQUAL(0.0f, Matrix3x3::Identity.zx);
		CHECK_EQUAL(0.0f, Matrix3x3::Identity.zy);
		CHECK_EQUAL(1.0f, Matrix3x3::Identity.zz);
	}

	// Operators
	// * Vector3
	TEST_FIXTURE(ConstructTest, Multiplication_Vector3) {
		// 90°
		vV3 = Vector3(1.0f, 0.0f, 0.0f);
		mM.FromEulerAngleY(static_cast<float>(90.0f*Math::DegToRad));
		vV3 = mM*vV3;
		CHECK_CLOSE(0.0f, vV3.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV3.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV3.z, Math::Epsilon);

		// -90°
		vV3 = Vector3(1.0f, 0.0f, 0.0f);
		mM.FromEulerAngleY(-static_cast<float>(90.0f*Math::DegToRad));
		vV3 = mM*vV3;
		CHECK_CLOSE(0.0f, vV3.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV3.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV3.z, Math::Epsilon);
	}

	// Matrix operations
	// IsZero()
	TEST_FIXTURE(ConstructTest, IsZero) {
		mM = Matrix3x3::Zero;
		CHECK(mM.IsZero());

		mM.yz = 0.001f;
		CHECK(!(mM.IsZero()));

		mM.yz = static_cast<float>(Math::Epsilon);
		CHECK(mM.IsZero());
	}

	// IsTrueZero()
	TEST_FIXTURE(ConstructTest, IsTrueZero) {
		mM = Matrix3x3::Zero;
		CHECK(mM.IsTrueZero());

		mM.yz = 0.001f;
		CHECK(!(mM.IsTrueZero()));

		mM.yz = static_cast<float>(Math::Epsilon);
		CHECK(!(mM.IsTrueZero()));
	}

	// SetZero()
	TEST_FIXTURE(ConstructTest, SetZero) {
		mM.yz = 0.001f;
		mM.xx = 0.101f;
		mM.SetZero();
		CHECK_EQUAL(0.0f, mM.xx);
		CHECK_EQUAL(0.0f, mM.xy);
		CHECK_EQUAL(0.0f, mM.xz);
		CHECK_EQUAL(0.0f, mM.yx);
		CHECK_EQUAL(0.0f, mM.yy);
		CHECK_EQUAL(0.0f, mM.yz);
		CHECK_EQUAL(0.0f, mM.zx);
		CHECK_EQUAL(0.0f, mM.zy);
		CHECK_EQUAL(0.0f, mM.zz);
	}

	// IsIdentity()
	TEST_FIXTURE(ConstructTest, IsIdentity) {
		mM = Matrix3x3::Identity;
		CHECK(mM.IsIdentity());

		mM.yz = 0.001f;
		CHECK(!(mM.IsIdentity()));

		mM.yz = static_cast<float>(Math::Epsilon);
		CHECK(mM.IsIdentity());
	}

	// IsTrueIdentity()
	TEST_FIXTURE(ConstructTest, IsTrueIdentity) {
		mM = Matrix3x3::Identity;
		CHECK(mM.IsTrueIdentity());

		mM.yz = 0.001f;
		CHECK(!(mM.IsTrueIdentity()));

		mM.yz = static_cast<float>(Math::Epsilon);
		CHECK(!(mM.IsTrueIdentity()));
	}

	// SetIdentity()
	TEST_FIXTURE(ConstructTest, SetIdentity) {
		mM.yz = 0.001f;
		mM.xx = 0.101f;
		mM.SetIdentity();
		CHECK_EQUAL(1.0f, mM.xx);
		CHECK_EQUAL(0.0f, mM.xy);
		CHECK_EQUAL(0.0f, mM.xz);
		CHECK_EQUAL(0.0f, mM.yx);
		CHECK_EQUAL(1.0f, mM.yy);
		CHECK_EQUAL(0.0f, mM.yz);
		CHECK_EQUAL(0.0f, mM.zx);
		CHECK_EQUAL(0.0f, mM.zy);
		CHECK_EQUAL(1.0f, mM.zz);
	}

	// Set()
	TEST_FIXTURE(ConstructTest, Set) {
		mM.Set(-0.1f, -1.0f, -2.0f,
			-3.0f, -4.0f, -5.0f,
			-6.0f, -7.0f, -8.0f);
		CHECK_EQUAL(-0.1f, mM.xx);
		CHECK_EQUAL(-1.0f, mM.xy);
		CHECK_EQUAL(-2.0f, mM.xz);
		CHECK_EQUAL(-3.0f, mM.yx);
		CHECK_EQUAL(-4.0f, mM.yy);
		CHECK_EQUAL(-5.0f, mM.yz);
		CHECK_EQUAL(-6.0f, mM.zx);
		CHECK_EQUAL(-7.0f, mM.zy);
		CHECK_EQUAL(-8.0f, mM.zz);
	}

	// GetRow()
	TEST_FIXTURE(ConstructTest, GetRow) {
		mM = fT;
		vV3 = mM.GetRow(0);
		CHECK_EQUAL(0.0f, vV3.x);
		CHECK_EQUAL(3.0f, vV3.y);
		CHECK_EQUAL(6.0f, vV3.z);

		vV3 = mM.GetRow(1);
		CHECK_EQUAL(1.0f, vV3.x);
		CHECK_EQUAL(4.0f, vV3.y);
		CHECK_EQUAL(7.0f, vV3.z);

		vV3 = mM.GetRow(2);
		CHECK_EQUAL(2.0f, vV3.x);
		CHECK_EQUAL(5.0f, vV3.y);
		CHECK_EQUAL(8.0f, vV3.z);
	}

	// SetRow()
	TEST_FIXTURE(ConstructTest, SetRow) {
		mM.SetRow(0, Vector3(-0.1f, -3.0f, -6.0f));
		CHECK_EQUAL(-0.1f, mM.xx);
		CHECK_EQUAL(-3.0f, mM.xy);
		CHECK_EQUAL(-6.0f, mM.xz);

		mM.SetRow(1, Vector3(-1.0f, -4.0f, -7.0f));
		CHECK_EQUAL(-1.0f, mM.yx);
		CHECK_EQUAL(-4.0f, mM.yy);
		CHECK_EQUAL(-7.0f, mM.yz);

		mM.SetRow(2, Vector3(-2.0f, -5.0f, -8.0f));
		CHECK_EQUAL(-2.0f, mM.zx);
		CHECK_EQUAL(-5.0f, mM.zy);
		CHECK_EQUAL(-8.0f, mM.zz);
	}

	// GetColumn()
	TEST_FIXTURE(ConstructTest, GetColumn) {
		mM = fT;
		vV3 = mM.GetColumn(0);
		CHECK_EQUAL(0.0f, vV3.x);
		CHECK_EQUAL(1.0f, vV3.y);
		CHECK_EQUAL(2.0f, vV3.z);

		vV3 = mM.GetColumn(1);
		CHECK_EQUAL(3.0f, vV3.x);
		CHECK_EQUAL(4.0f, vV3.y);
		CHECK_EQUAL(5.0f, vV3.z);

		vV3 = mM.GetColumn(2);
		CHECK_EQUAL(6.0f, vV3.x);
		CHECK_EQUAL(7.0f, vV3.y);
		CHECK_EQUAL(8.0f, vV3.z);
	}

	// SetColumn()
	TEST_FIXTURE(ConstructTest, SetColumn) {
		mM.SetColumn(0, Vector3(-0.1f, -1.0f, -2.0f));
		CHECK_EQUAL(-0.1f, mM.xx);
		CHECK_EQUAL(-1.0f, mM.yx);
		CHECK_EQUAL(-2.0f, mM.zx);

		mM.SetColumn(1, Vector3(-3.0f, -4.0f, -5.0f));
		CHECK_EQUAL(-3.0f, mM.xy);
		CHECK_EQUAL(-4.0f, mM.yy);
		CHECK_EQUAL(-5.0f, mM.zy);

		mM.SetColumn(2, Vector3(-6.0f, -7.0f, -8.0f));
		CHECK_EQUAL(-6.0f, mM.xz);
		CHECK_EQUAL(-7.0f, mM.yz);
		CHECK_EQUAL(-8.0f, mM.zz);
	}

	// IsSymmetric()
	TEST_FIXTURE(ConstructTest, IsSymmetric) {
		mM = Matrix3x3::Identity;
		CHECK(mM.IsSymmetric());

		mM.xx = 4.0f;
		CHECK(mM.IsSymmetric());

		mM.xy = 4.0f;
		CHECK(!(mM.IsSymmetric()));
	}

	// IsOrthogonal()
	TEST_FIXTURE(ConstructTest, IsOrthogonal) {
		mM = Matrix3x3::Identity;
		CHECK(mM.IsOrthogonal());

		mM.yy = 4.0f;
		CHECK(!(mM.IsOrthogonal()));

		mM.yy = 1.0f;
		CHECK(mM.IsOrthogonal());
	}

	// IsRotationMatrix()
	TEST_FIXTURE(ConstructTest, IsRotationMatrix) {
		mM = Matrix3x3::Identity;
		CHECK(mM.IsRotationMatrix());

		mM.xx = 4.0f;
		CHECK(!(mM.IsRotationMatrix()));
	}

	// GetTrace()
	TEST_FIXTURE(ConstructTest, GetTrace) {
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.GetTrace());
		CHECK_EQUAL(3.0f, Matrix3x3::Identity.GetTrace());

		mM = Matrix3x3::Identity;
		mM.xy = 1.0f;
		CHECK_EQUAL(3.0f, mM.GetTrace());
	}

	// GetDeterminant()
	TEST_FIXTURE(ConstructTest, GetDeterminant) {
		CHECK_EQUAL(0.0f, Matrix3x3::Zero.GetDeterminant());
		CHECK_EQUAL(1.0f, Matrix3x3::Identity.GetDeterminant());
	}

	// Transpose()
	TEST_FIXTURE(ConstructTest, Transpose) {
		mM = Matrix3x3::Identity;
		mM.Transpose();
		CHECK(Matrix3x3::Identity == mM);
	}

	// GetTransposed()
	TEST_FIXTURE(ConstructTest, GetTransposed) {
		CHECK(Matrix3x3::Identity == Matrix3x3::Identity.GetTransposed());
	}

	// Invert()
	TEST_FIXTURE(ConstructTest, Invert) {
		mM = Matrix3x3::Identity;
		mM.Invert();
		CHECK(Matrix3x3::Identity== mM);
	}

	// GetInverted()
	TEST_FIXTURE(ConstructTest, GetInverted) {
		CHECK(Matrix3x3::Identity == Matrix3x3::Identity.GetInverted());
	}

	// Scale
	// GetScale(float fX, float fY, float fZ)
	TEST_FIXTURE(ConstructTest, GetScale_float_fX__float_fY__float_fZ_) {
		// Identity matrix
		vV3 = Vector3::Zero;
		Matrix3x3::Identity.GetScale(vV3.x, vV3.y, vV3.z);
		CHECK_EQUAL(1.0f, vV3.x);
		CHECK_EQUAL(1.0f, vV3.y);
		CHECK_EQUAL(1.0f, vV3.z);

		// Null matrix
		vV3 = Vector3::Zero;
		Matrix3x3::Zero.GetScale(vV3.x, vV3.y, vV3.z);
		CHECK_EQUAL(0.0f, vV3.x);
		CHECK_EQUAL(0.0f, vV3.y);
		CHECK_EQUAL(0.0f, vV3.z);

		// Any matrix
		vV3 = Vector3::Zero;
		EulerAngles::ToMatrix(1.0f, 2.0f, 3.0f, mM);
		mMT.SetScaleMatrix(1.0f, 2.0f, 3.0f);
		mM *= mMT;
		mM.GetScale(vV3.x, vV3.y, vV3.z);
		CHECK_CLOSE(1.0f, vV3.x, 0.000001f);
		CHECK_CLOSE(2.0f, vV3.y, 0.000001f);
		CHECK_CLOSE(3.0f, vV3.z, 0.000001f);
	}

	// GetXAxis()
	TEST_FIXTURE(ConstructTest, GetXAxis) {
		vV3 = Matrix3x3::Identity.GetXAxis();
		CHECK_EQUAL(1.0f, vV3.x);
		CHECK_EQUAL(0.0f, vV3.y);
		CHECK_EQUAL(0.0f, vV3.z);
	}

	// GetYAxis()
	TEST_FIXTURE(ConstructTest, GetYAxis) {
		vV3 = Matrix3x3::Identity.GetYAxis();
		CHECK_EQUAL(0.0f, vV3.x);
		CHECK_EQUAL(1.0f, vV3.y);
		CHECK_EQUAL(0.0f, vV3.z);
	}

	// GetZAxis()
	TEST_FIXTURE(ConstructTest, GetZAxis) {
		vV3 = Matrix3x3::Identity.GetZAxis();
		CHECK_EQUAL(0.0f, vV3.x);
		CHECK_EQUAL(0.0f, vV3.y);
		CHECK_EQUAL(1.0f, vV3.z);
	}

	// Rotation
	// ToAxisAngle() and FromAxisAngle()
	TEST_FIXTURE(ConstructTest, ToAxisAngle_and_FromAxisAngle) {
		// Check only x rotation
		// [TODO] Some tests will fail...
		//	if (!CheckAxisAngle(1.0f, 0.0f, 0.0f,   0.0f,   0.0f)) bResult = false;
		CHECK(CheckAxisAngle(1.0f, 0.0f, 0.0f,  45.0f,  45.0f));
		//	if (!CheckAxisAngle(1.0f, 0.0f, 0.0f, -45.0f,  45.0f)) bResult = false;
		CHECK(CheckAxisAngle(1.0f, 0.0f, 0.0f,  90.0f,  90.0f));
		CHECK(CheckAxisAngle(1.0f, 0.0f, 0.0f, 135.0f, 135.0f));
		CHECK(CheckAxisAngle(1.0f, 0.0f, 0.0f, 180.0f, 180.0f));
		//	if (!CheckAxisAngle(1.0f, 0.0f, 0.0f, 225.0f, 225.0f)) bResult = false;
		//	if (!CheckAxisAngle(1.0f, 0.0f, 0.0f, 270.0f, 270.0f)) bResult = false;
		//	if (!CheckAxisAngle(1.0f, 0.0f, 0.0f, 315.0f, 315.0f)) bResult = false;
		//	if (!CheckAxisAngle(1.0f, 0.0f, 0.0f, 360.0f, 360.0f)) bResult = false;

		// Check only y rotation
		//	if (!CheckAxisAngle(0.0f, 1.0f, 0.0f,   0.0f,   0.0f)) bResult = false;
		CHECK(CheckAxisAngle(0.0f, 1.0f, 0.0f,  45.0f,  45.0f));
		//	if (!CheckAxisAngle(0.0f, 1.0f, 0.0f, -45.0f,  45.0f)) bResult = false;
		CHECK(CheckAxisAngle(0.0f, 1.0f, 0.0f,  90.0f,  90.0f));
		CHECK(CheckAxisAngle(0.0f, 1.0f, 0.0f, 135.0f, 135.0f));
		CHECK(CheckAxisAngle(0.0f, 1.0f, 0.0f, 180.0f, 180.0f));
		//	if (!CheckAxisAngle(0.0f, 1.0f, 0.0f, 225.0f, 225.0f)) bResult = false;
		//	if (!CheckAxisAngle(0.0f, 1.0f, 0.0f, 270.0f, 270.0f)) bResult = false;
		//	if (!CheckAxisAngle(0.0f, 1.0f, 0.0f, 315.0f, 315.0f)) bResult = false;
		//	if (!CheckAxisAngle(0.0f, 1.0f, 0.0f, 360.0f, 360.0f)) bResult = false;

		// Check only z rotation
		//	if (!CheckAxisAngle(0.0f, 0.0f, 1.0f,   0.0f,   0.0f)) bResult = false;
		CHECK(CheckAxisAngle(0.0f, 0.0f, 1.0f,  45.0f,  45.0f));
		//	if (!CheckAxisAngle(0.0f, 0.0f, 1.0f, -45.0f,  45.0f)) bResult = false;
		CHECK(CheckAxisAngle(0.0f, 0.0f, 1.0f,  90.0f,  90.0f));
		CHECK(CheckAxisAngle(0.0f, 0.0f, 1.0f, 135.0f, 135.0f));
		CHECK(CheckAxisAngle(0.0f, 0.0f, 1.0f, 180.0f, 180.0f));
		//	if (!CheckAxisAngle(0.0f, 0.0f, 1.0f, 225.0f, 225.0f)) bResult = false;
		//	if (!CheckAxisAngle(0.0f, 0.0f, 1.0f, 270.0f, 270.0f)) bResult = false;
		//	if (!CheckAxisAngle(0.0f, 0.0f, 1.0f, 315.0f, 315.0f)) bResult = false;
		//	if (!CheckAxisAngle(0.0f, 0.0f, 1.0f, 360.0f, 360.0f)) bResult = false;
	}
}