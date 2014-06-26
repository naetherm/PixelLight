//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLMath/Vector3.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Quaternion.h>

using namespace PLMath;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Vector3) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() :
	fX(0.0f),
		fY(0.0f),
		fZ(0.0f)
	{
		/* some setup */
		fT3[0] = 1.0f;
		fT3[1] = 2.0f;
		fT3[2] = 3.0f;
	}
	~ConstructTest() {
		/* some teardown */
	}

	// Variables for testing
	Vector3 vV;
	float fT3[3];
	Matrix3x3 m3x3;
	Matrix4x4 m4x4;
	float fX, fY, fZ;
	Quaternion qQ;
	};

	TEST_FIXTURE(ConstructTest, Constructors) {
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);

		vV = Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);

		vV = Vector3(fT3);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);

		vV = Vector3(vV);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// Static variables
	TEST_FIXTURE(ConstructTest, Static_Variables) {
		CHECK_EQUAL(0.0f, Vector3::Zero.x);
		CHECK_EQUAL(0.0f, Vector3::Zero.y);
		CHECK_EQUAL(0.0f, Vector3::Zero.z);

		CHECK_EQUAL(1.0f, Vector3::One.x);
		CHECK_EQUAL(1.0f, Vector3::One.y);
		CHECK_EQUAL(1.0f, Vector3::One.z);

		CHECK_EQUAL(1.0f, Vector3::UnitX.x);
		CHECK_EQUAL(0.0f, Vector3::UnitX.y);
		CHECK_EQUAL(0.0f, Vector3::UnitX.z);
		CHECK_EQUAL(0.0f, Vector3::UnitY.x);
		CHECK_EQUAL(1.0f, Vector3::UnitY.y);
		CHECK_EQUAL(0.0f, Vector3::UnitY.z);
		CHECK_EQUAL(0.0f, Vector3::UnitZ.x);
		CHECK_EQUAL(0.0f, Vector3::UnitZ.y);
		CHECK_EQUAL(1.0f, Vector3::UnitZ.z);

		CHECK_EQUAL(-1.0f, Vector3::NegativeUnitX.x);
		CHECK_EQUAL( 0.0f, Vector3::NegativeUnitX.y);
		CHECK_EQUAL( 0.0f, Vector3::NegativeUnitX.z);
		CHECK_EQUAL( 0.0f, Vector3::NegativeUnitY.x);
		CHECK_EQUAL(-1.0f, Vector3::NegativeUnitY.y);
		CHECK_EQUAL( 0.0f, Vector3::NegativeUnitY.z);
		CHECK_EQUAL( 0.0f, Vector3::NegativeUnitZ.x);
		CHECK_EQUAL( 0.0f, Vector3::NegativeUnitZ.y);
		CHECK_EQUAL(-1.0f, Vector3::NegativeUnitZ.z);
	}

	// Comparison
	// ==
	TEST_FIXTURE(ConstructTest, Operator_Equals) {
		CHECK(Vector3(1.0f, 2.0f, 3.0f) == Vector3(1.0f, 2.0f, 3.0f));
		CHECK(!(Vector3(1.0f, 2.0f, 3.0f) == Vector3(1.0f, 2.0001f, 3.0f)));
	}
	// !=
	TEST_FIXTURE(ConstructTest, Operator_Equals_Not) {
		CHECK(!(Vector3(1.0f, 2.0f, 3.0f) != Vector3(1.0f, 2.0f, 3.0f)));
		CHECK(Vector3(1.0f, 2.0001f, 3.0f) != Vector3(1.0f, 2.0f, 3.0f));
	}
	// >
	TEST_FIXTURE(ConstructTest, Operator_Greater) {
		CHECK(!(Vector3(1.0f, 2.0f, 3.0f) > Vector3(2.0f, 3.0f, 4.0f)));
		CHECK(Vector3(2.0f, 3.0f, 4.0f) > Vector3(1.0f, 2.0f, 3.0f));
		CHECK(!(Vector3(1.0f, 2.0f, 3.0f) > Vector3(1.0f, 2.0, 3.0f)));
	}
	// <
	TEST_FIXTURE(ConstructTest, Operator_Lesser) {
		CHECK(!(Vector3(2.0f, 3.0f, 4.0f) < Vector3(1.0f, 2.0f, 3.0f)));
		CHECK((Vector3(1.0f, 2.0f, 3.0f) < Vector3(2.0f, 3.0f, 4.0f)));
		CHECK(!(Vector3(1.0f, 2.0, 3.0f) < Vector3(1.0f, 2.0f, 3.0f)));
	}
	// >=
	TEST_FIXTURE(ConstructTest, Operator_Greater_Equals) {
		CHECK(!(Vector3(1.0f, 2.0f, 3.0f) >= Vector3(2.0f, 3.0f, 4.0f)));
		CHECK((Vector3(2.0f, 3.0f, 4.0f) >= Vector3(1.0f, 2.0f, 3.0f)));
		CHECK((Vector3(1.0f, 2.0f, 3.0f) >= Vector3(1.0f, 2.0f, 3.0f)));
	}
	// <=
	TEST_FIXTURE(ConstructTest, Operator_Lesser_Equals) {
		CHECK(!(Vector3(2.0f, 3.0f, 4.0f) <= Vector3(1.0f, 2.0f, 3.0f)));
		CHECK((Vector3(1.0f, 2.0f, 3.0f) <= Vector3(2.0f, 3.0f, 4.0f)));
		CHECK((Vector3(1.0f, 2.0, 3.0f) <= Vector3(1.0f, 2.0f, 3.0f)));
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f)+Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
	}
	// + float
	TEST_FIXTURE(ConstructTest, Operator_Add_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f)+2.0f;
		vV = Vector3(1.0f, 2.0f, 3.0f)+2.0f;
		CHECK_EQUAL(3.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
	}
	// += Vector3
	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV += Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
	}
	// += float
	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV += 2.0f;
		CHECK_EQUAL(3.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
	}

	// Subtraction
	// -
	TEST_FIXTURE(ConstructTest, Operator_Minus) {
		vV = -Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(-1.0f, vV.x);
		CHECK_EQUAL(-2.0f, vV.y);
		CHECK_EQUAL(-3.0f, vV.z);
	}
	// - Vector3
	TEST_FIXTURE(ConstructTest, Operator_Minus_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f)-Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
	}
	// - float
	TEST_FIXTURE(ConstructTest, Operator_Minus_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f)-2.0f;
		CHECK_EQUAL(-1.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
	}
	// -= Vector3
	TEST_FIXTURE(ConstructTest, Operator_Minus_Equals_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV -= Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
	}
	// -= float
	TEST_FIXTURE(ConstructTest, Operator_Minus_Equals_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV -= 2.0f;
		CHECK_EQUAL(-1.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
	}
	// Vector3()-(-Vector3())
	TEST_FIXTURE(ConstructTest, Operator_Minus_Minus_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f)-(-Vector3(1.0f, 2.0f, 3.0f));
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
	}

	// Multiplication
	// * Vector3
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f)*Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(9.0f, vV.z);
	}
	// * float
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f)*2.0f;
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
	}
	// *= Vector3
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV *= Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(9.0f, vV.z);
	}
	// *= float
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV *= 2.0f;
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
	}

	// *= Matrix3x3
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Matrix3x3) {
		// 90°
		vV = Vector3(1.0f, 0.0f, 0.0f);
		m3x3.FromEulerAngleY(static_cast<float>(90.0f*Math::DegToRad));
		vV *= m3x3;
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);

		// -90°
		vV = Vector3(1.0f, 0.0f, 0.0f);
		m3x3.FromEulerAngleY(-static_cast<float>(90.0f*Math::DegToRad));
		vV *= m3x3;
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);
	}

	// *= Matrix4x4
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Matrix4x4) {
		// 90°
		vV = Vector3(1.0f, 0.0f, 0.0f);
		m4x4.FromEulerAngleY(static_cast<float>(90.0f*Math::DegToRad));
		vV *= m4x4;
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);

		// -90°
		vV = Vector3(1.0f, 0.0f, 0.0f);
		m4x4.FromEulerAngleY(-static_cast<float>(90.0f*Math::DegToRad));
		vV *= m4x4;
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);

		// -90° + translation (2, 3, 4)
		vV = Vector3(1.0f, 0.0f, 0.0f);
		m4x4.FromEulerAngleY(-static_cast<float>(90.0f*Math::DegToRad));
		m4x4.SetTranslation(2.0f, 3.0f, 4.0f);
		vV *= m4x4;
		CHECK_CLOSE(2.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(3.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(5.0f, vV.z, Math::Epsilon);
	}

	// [TODO] At the moment, Quaternion*Vector3 and Vector3*=Quaternion will return different results
	/*	// *= Quaternion
	StartTask("*= Quaternion");
	bResult = true;
	// 90°
	vV = Vector3(1.0f, 0.0f, 0.0f);
	qQ.FromEulerAnglesXYZ(0.0f, static_cast<float>(90.0f*Math::DegToRad), 0.0f);
	vV *= qQ;
	if (!Math::AreEqual(vV.x, 0.0f) || !Math::AreEqual(vV.y, 0.0f) || !Math::AreEqual(vV.z, -1.0f)) bResult = false;
	// -90°
	vV = Vector3(1.0f, 0.0f, 0.0f);
	qQ.FromEulerAnglesXYZ(0.0f, -static_cast<float>(90.0f*Math::DegToRad), 0.0f);
	vV *= qQ;
	if (!Math::AreEqual(vV.x, 0.0f) || !Math::AreEqual(vV.y, 0.0f) || !Math::AreEqual(vV.z, 1.0f)) bResult = false;
	EndTask(bResult);
	*/

	// Division
	// / Vector3
	TEST_FIXTURE(ConstructTest, Operator_Division_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f)/Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
	}
	// / float
	TEST_FIXTURE(ConstructTest, Operator_Division_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f)/2.0f;
		CHECK_EQUAL(0.5f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.5f, vV.z);
	}
	// /= Vector3
	TEST_FIXTURE(ConstructTest, Operator_Division_Equals_Vector3) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV /= Vector3(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
	}
	// /= float
	TEST_FIXTURE(ConstructTest, Operator_Division_Equals_Float) {
		vV = Vector3(1.0f, 2.0f, 3.0f);
		vV /= 2.0f;
		CHECK_EQUAL(0.5f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.5f, vV.z);
	}

	TEST_FIXTURE(ConstructTest, To_FloatPtr) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		float *pf = vV;
		CHECK_EQUAL(0.0f, pf[0]);
		CHECK_EQUAL(1.0f, pf[1]);
		CHECK_EQUAL(2.0f, pf[2]);
	}

	TEST_FIXTURE(ConstructTest, To_const_FloatPtr) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		const float *pf = vV;
		CHECK_EQUAL(0.0f, pf[0]);
		CHECK_EQUAL(1.0f, pf[1]);
		CHECK_EQUAL(2.0f, pf[2]);
	}

	// float &operator [](int)
	TEST_FIXTURE(ConstructTest, Operator_Index) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		CHECK_EQUAL(0.0f, vV[0]);
		CHECK_EQUAL(1.0f, vV[1]);
		CHECK_EQUAL(2.0f, vV[2]);
	}

	// GetXYZ()
	TEST_FIXTURE(ConstructTest, GetXYZ) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		vV.GetXYZ(fX, fY, fZ);
		CHECK_EQUAL(0.0f, fX);
		CHECK_EQUAL(1.0f, fY);
		CHECK_EQUAL(2.0f, fZ);
	}

	// GetX()
	TEST_FIXTURE(ConstructTest, GetX) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		CHECK_EQUAL(0.0f, vV.GetX());
	}

	// GetY()
	TEST_FIXTURE(ConstructTest, GetY) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		CHECK_EQUAL(1.0f, vV.GetY());
	}

	// GetZ()
	TEST_FIXTURE(ConstructTest, GetZ) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		CHECK_EQUAL(2.0f, vV.GetZ());
	}

	// SetXYZ(float, float, float)
	TEST_FIXTURE(ConstructTest, SetXYZ_Float) {
		vV.SetXYZ(0.0f, 1.0f, 2.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(2.0f, vV.z);
	}

	// SetXYZ(const float[])
	TEST_FIXTURE(ConstructTest, SetXYZ_FloatArray) {
		vV.SetXYZ(fT3);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// SetX()
	TEST_FIXTURE(ConstructTest, SetX) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		vV.SetX(0.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// SetY()
	TEST_FIXTURE(ConstructTest, SetY) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		vV.SetY(0.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// SetZ()
	TEST_FIXTURE(ConstructTest, SetZ) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		vV.SetZ(0.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
	}

	// IncXYZ(float, float, float)
	TEST_FIXTURE(ConstructTest, IncXYZ_Float) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		vV.IncXYZ(1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(3.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
	}

	// IncXYZ(const float[])
	TEST_FIXTURE(ConstructTest, IncXYZ_FloatArray) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f;
		vV.IncXYZ(fT3);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(3.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
	}

	// IncX()
	TEST_FIXTURE(ConstructTest, IncX) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		vV.IncX(2.0f);
		CHECK_EQUAL(3.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// IncY()
	TEST_FIXTURE(ConstructTest, IncY) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		vV.IncY(2.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// IncZ()
	TEST_FIXTURE(ConstructTest, IncZ) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		vV.IncZ(2.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
	}

	// ToString()
	TEST_FIXTURE(ConstructTest, ToString) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		CHECK_EQUAL("1 2 3", vV.ToString().GetASCII());
	}

	// FromString()
	TEST_FIXTURE(ConstructTest, FromString) {
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV.FromString("1 2 3");
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
	}

	// Misc
	// IsNull()
	TEST_FIXTURE(ConstructTest, IsNull) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 5.0f;
		CHECK(!(vV.IsNull()));

		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		CHECK(vV.IsNull());
	}

	// IsPacked()
	TEST_FIXTURE(ConstructTest, IsPacked) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 5.0f;
		CHECK(!(vV.IsPacked()));

		vV.x = 0.3f; vV.y = 0.8f; vV.z = 0.0f;
		CHECK(vV.IsPacked());
	}

	// PackTo01()
	TEST_FIXTURE(ConstructTest, PackTo01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f;
		vV.PackTo01();
		CHECK_CLOSE(0.72360682f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.94721359f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.5f, vV.z, Math::Epsilon);

		//
		vV.x = -5.0f; vV.y = 2.0f; vV.z = 1.0f;
		vV.PackTo01();
		CHECK_CLOSE(0.04356455f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.6825741f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.591287f, vV.z, Math::Epsilon);
	}

	// GetPackedTo01()
	TEST_FIXTURE(ConstructTest, GetPackedTo01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f;
		vV = vV.GetPackedTo01();
		CHECK_CLOSE(0.72360682f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.94721359f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.5f, vV.z, Math::Epsilon);

		//
		vV.x = -5.0f; vV.y = 2.0f; vV.z = 1.0f;
		vV = vV.GetPackedTo01();
		CHECK_CLOSE(0.04356455f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.6825741f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.591287f, vV.z, Math::Epsilon);
	}

	// UnpackFrom01()
	TEST_FIXTURE(ConstructTest, UnpackFrom01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f;
		vV.UnpackFrom01();
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);

		//
		vV.x = 0.2f; vV.y = 0.6f; vV.z = 0.0f;
		vV.UnpackFrom01();
		CHECK_CLOSE(-0.6f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.2f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
	}

	// GetUnpackedFrom01()
	TEST_FIXTURE(ConstructTest, GetUnpackedFrom01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f;
		vV = vV.GetUnpackedFrom01();
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);

		//
		vV.x = 0.2f; vV.y = 0.6f; vV.z = 0.0f;
		vV = vV.GetUnpackedFrom01();
		CHECK_CLOSE(-0.6f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.2f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
	}

	// GetSmallestComponent()
	TEST_FIXTURE(ConstructTest, GetSmallestComponent) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		CHECK_EQUAL(Vector3::X, vV.GetSmallestComponent());
	}

	// GetSmallestValue()
	TEST_FIXTURE(ConstructTest, GetSmallestValue) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		CHECK_EQUAL(1.0f, vV.GetSmallestValue());
	}

	// GetGreatestComponent()
	TEST_FIXTURE(ConstructTest, GetGreatestComponent) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		CHECK_EQUAL(Vector3::Z, vV.GetGreatestComponent());
	}

	// GetGreatestValue()
	TEST_FIXTURE(ConstructTest, GetGreatestValue) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f;
		CHECK_EQUAL(3.0f, vV.GetGreatestValue());
	}

	// Invert()
	TEST_FIXTURE(ConstructTest, Invert) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		vV.Invert();
		CHECK_EQUAL(-0.2f, vV.x);
		CHECK_EQUAL(0.6f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
	}

	// GetInverted()
	TEST_FIXTURE(ConstructTest, GetInverted) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		vV = vV.GetInverted();
		CHECK_EQUAL(-0.2f, vV.x);
		CHECK_EQUAL(0.6f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
	}

	// GetLength()
	TEST_FIXTURE(ConstructTest, GetLength) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		CHECK_CLOSE(0.6324555f, vV.GetLength(), Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f;
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
	}

	// GetSquaredLength()
	TEST_FIXTURE(ConstructTest, GetSquaredLength) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		CHECK_CLOSE(0.4f, vV.GetSquaredLength(), Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		CHECK_CLOSE(0.0f, vV.GetSquaredLength(), Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f;
		CHECK_CLOSE(1.0f, vV.GetSquaredLength(), Math::Epsilon);
	}

	// SetLength()
	TEST_FIXTURE(ConstructTest, SetLength) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		vV.SetLength(1.0f);
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.3162277f, vV.x, Math::Epsilon);
		CHECK_CLOSE(-0.9486832f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV.SetLength(1.0f);
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV.SetLength(1.0f);
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
	}

	// Normalize()
	TEST_FIXTURE(ConstructTest, Normalize) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		vV.Normalize();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.3162277f, vV.x, Math::Epsilon);
		CHECK_CLOSE(-0.9486832f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV.Normalize();
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV.Normalize();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
	}

	// GetNormalized()
	TEST_FIXTURE(ConstructTest, GetNormalized) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		vV = vV.GetNormalized();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.3162277f, vV.x, Math::Epsilon);
		CHECK_CLOSE(-0.9486832f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV = vV.GetNormalized();
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV = vV.GetNormalized();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
	}

	// GetDistance()
	TEST_FIXTURE(ConstructTest, GetDistance) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		CHECK_CLOSE(0.6324555f, vV.GetDistance(Vector3(0.0f, 0.0f, 0.0f)), Math::Epsilon);
	}

	// GetSquaredDistance()
	TEST_FIXTURE(ConstructTest, GetSquaredDistance) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		CHECK_CLOSE(0.4f, vV.GetSquaredDistance(Vector3(0.0f, 0.0f, 0.0f)), Math::Epsilon);
	}

	// DotProduct()
	TEST_FIXTURE(ConstructTest, DotProduct) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f;
		CHECK_CLOSE(0.0f, vV.DotProduct(Vector3(0.0f, 0.0f, 0.0f)), Math::Epsilon);
		CHECK_CLOSE(0.4f, vV.DotProduct(Vector3(0.2f, -0.6f, 0.0f)), Math::Epsilon);
	}

	// GetAngle()
	TEST_FIXTURE(ConstructTest, GetAngle) {
		// ~69.5°
		vV.x = 1.0f; vV.y = 4.0f; vV.z = -2.0f;
		CHECK_CLOSE(1.2127572f, vV.GetAngle(Vector3(-3.0f, 3.0f, 1.0f)), Math::Epsilon);

		// 45°
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 0.0f;
		CHECK_CLOSE(0.7853981f, vV.GetAngle(Vector3(1.0f, 1.0f, 0.0f)), Math::Epsilon);

		// Now use normalized vectors and use the dot product... (~69.5°)
		vV.x = 1.0f; vV.y = 4.0f; vV.z = -2.0f;
		vV.Normalize();
		CHECK_CLOSE(1.2127573f, Math::ACos(vV.DotProduct(Vector3(-3.0f, 3.0f, 1.0f).Normalize())), Math::Epsilon);
	}

	// CrossProduct(Vector3)
	TEST_FIXTURE(ConstructTest, CrossProduct_Vector3_) {
		//
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV = vV.CrossProduct(Vector3(0.0f, 1.0f, 0.0f));
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);

		//
		vV.x = -1.0f; vV.y = 0.0f; vV.z = 0.0f;
		vV = vV.CrossProduct(Vector3(0.0f, -1.0f, 0.0f));
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);
	}

	// CrossProduct(Vector3, Vector3)
	TEST_FIXTURE(ConstructTest, CrossProduct_Vector3__Vector3_) {
		//
		vV.CrossProduct(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);

		//
		vV.CrossProduct(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);
	}


	/* [TODO]
	PLMATH_API void GetRightUp(Vector3 &vRight, Vector3 &vUp) const;
	PLMATH_API void GetRightUp(float fRight[], float fUp[]) const;
	PLMATH_API void GetFaceNormal(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);
	PLMATH_API Vector3 &RotateAxis(Vector3 &vV, float fAngle);
	PLMATH_API Quaternion GetRotationTo(const Vector3 &vDest) const;
	PLMATH_API Vector3 ProjectVector(const Vector3 &vA, const Vector3 &vB) const;
	PLMATH_API Vector3 &GetProjection(const Vector3 &vX, const Vector3 &vN);
	PLMATH_API Vector3 &ProjectPlane(const Vector3 &vV1, const Vector3 &vV2);
	PLMATH_API Vector3 &Reflect(const Vector3 &vIncidentNormal, const Vector3 &vNormal);
	PLMATH_API Vector3 &Refract(const Vector3 &vIncidentNormal, const Vector3 &vNormal, float fEtaRatio);
	PLMATH_API Vector3 ClosestPointOnLine(const Vector3 &vV1, const Vector3 &vV2);
	PLMATH_API bool IsPointInTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);
	PLMATH_API Vector3 ClosestPointOnTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);
	PLMATH_API bool ClosestPointOnTriangle(const Vector3 &vV1, const Vector3 &vV2,
	const Vector3 &vV3, float fRadius, Vector3 &vClosest);
	//
	StartTask("");
	EndTask();
	*/
}