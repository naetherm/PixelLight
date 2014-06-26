//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Vector4.h>

using namespace PLMath;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Vector4) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest()
		{
			/* some setup */
			fT4[0] = 1.0f;
			fT4[1] = 2.0f;
			fT4[2] = 3.0f;
			fT4[3] = 4.0f;
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Container for testing
		PLMath::Vector4 vec;

		float fT4[4];
		Matrix4x4 m4x4;
		float fX, fY, fZ, fW;
		Vector3 vV3;
		Vector4 vV;
	};

	// Vector constructors & copy
	TEST_FIXTURE(ConstructTest, Vector_constructors___copy) {
		CHECK_EQUAL(vV.x, 0.0f);
		CHECK_EQUAL(vV.y, 0.0f);
		CHECK_EQUAL(vV.z, 0.0f);
		CHECK_EQUAL(vV.w, 1.0f);

		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(vV.x, 1.0f);
		CHECK_EQUAL(vV.y, 2.0f);
		CHECK_EQUAL(vV.z, 3.0f);
		CHECK_EQUAL(vV.w, 4.0f);

		vV = Vector4(fT4);
		CHECK_EQUAL(vV.x, 1.0f);
		CHECK_EQUAL(vV.y, 2.0f);
		CHECK_EQUAL(vV.z, 3.0f);
		CHECK_EQUAL(vV.w, 4.0f);

		vV = Vector4(vV);
		CHECK_EQUAL(vV.x, 1.0f);
		CHECK_EQUAL(vV.y, 2.0f);
		CHECK_EQUAL(vV.z, 3.0f);
		CHECK_EQUAL(vV.w, 4.0f);
	}

	// Static variables
	TEST_FIXTURE(ConstructTest, Static_variables) {
		CHECK_EQUAL(Vector4::Zero.x, 0.0f);
		CHECK_EQUAL(Vector4::Zero.y, 0.0f);
		CHECK_EQUAL(Vector4::Zero.z, 0.0f);
		CHECK_EQUAL(Vector4::Zero.w, 0.0f);

		CHECK_EQUAL(Vector4::One.x, 1.0f);
		CHECK_EQUAL(Vector4::One.y, 1.0f);
		CHECK_EQUAL(Vector4::One.z, 1.0f);
		CHECK_EQUAL(Vector4::One.w, 1.0f);

		CHECK_EQUAL(Vector4::UnitX.x, 1.0f);
		CHECK_EQUAL(Vector4::UnitX.y, 0.0f);
		CHECK_EQUAL(Vector4::UnitX.z, 0.0f);
		CHECK_EQUAL(Vector4::UnitX.w, 1.0f);
		CHECK_EQUAL(Vector4::UnitY.x, 0.0f);
		CHECK_EQUAL(Vector4::UnitY.y, 1.0f);
		CHECK_EQUAL(Vector4::UnitY.z, 0.0f);
		CHECK_EQUAL(Vector4::UnitY.w, 1.0f);
		CHECK_EQUAL(Vector4::UnitZ.x, 0.0f);
		CHECK_EQUAL(Vector4::UnitZ.y, 0.0f);
		CHECK_EQUAL(Vector4::UnitZ.z, 1.0f);
		CHECK_EQUAL(Vector4::UnitZ.w, 1.0f);
		CHECK_EQUAL(Vector4::UnitW.x, 0.0f);
		CHECK_EQUAL(Vector4::UnitW.y, 0.0f);
		CHECK_EQUAL(Vector4::UnitW.z, 0.0f);
		CHECK_EQUAL(Vector4::UnitW.w, 1.0f);

		CHECK_EQUAL(Vector4::NegativeUnitX.x, -1.0f);
		CHECK_EQUAL(Vector4::NegativeUnitX.y, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitX.z, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitX.w, 1.0f);
		CHECK_EQUAL(Vector4::NegativeUnitY.x, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitY.y, -1.0f);
		CHECK_EQUAL(Vector4::NegativeUnitY.z, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitY.w, 1.0f);
		CHECK_EQUAL(Vector4::NegativeUnitZ.x, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitZ.y, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitZ.z, -1.0f);
		CHECK_EQUAL(Vector4::NegativeUnitZ.w, 1.0f);
		CHECK_EQUAL(Vector4::NegativeUnitW.x, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitW.y, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitW.z, 0.0f);
		CHECK_EQUAL(Vector4::NegativeUnitW.w, -1.0f);
	}

	// Comparison
	// ==
	TEST_FIXTURE(ConstructTest, Operator_Equals) {
		CHECK(Vector4(1.0f, 2.0f, 3.0f, 4.0f) == Vector4(1.0f, 2.0f, 3.0f, 4.0f));
		CHECK(!(Vector4(1.0f, 2.0f, 3.0f, 4.0f) == Vector4(1.0f, 2.0001f, 3.0f, 4.0f)));
	}
	// !=
	TEST_FIXTURE(ConstructTest, Operator_Equals_Not) {
		CHECK(!(Vector4(1.0f, 2.0f, 3.0f, 4.0f) != Vector4(1.0f, 2.0f, 3.0f, 4.0f)));
		CHECK(Vector4(1.0f, 2.0f, 3.0f, 4.0f) != Vector4(1.0f, 2.0001f, 3.0f, 4.0f));
	}
	// >
	TEST_FIXTURE(ConstructTest, Operator_Greater) {
		CHECK(!(Vector4(1.0f, 2.0f, 3.0f, 4.0f) > Vector4(2.0f, 3.0f, 4.0f, 5.0f)));
		CHECK(Vector4(2.0f, 3.0f, 4.0f, 5.0f) > Vector4(1.0f, 2.0f, 3.0f, 4.0f));
		CHECK(!(Vector4(1.0f, 2.0f, 3.0f, 4.0f) > Vector4(1.0f, 2.0, 3.0f, 4.0f)));
	}
	// <
	TEST_FIXTURE(ConstructTest, Operator_Lesser) {
		CHECK(!(Vector4(2.0f, 3.0f, 4.0f, 5.0f) < Vector4(1.0f, 2.0f, 3.0f, 4.0f)));
		CHECK(Vector4(1.0f, 2.0f, 3.0f, 4.0f) < Vector4(2.0f, 3.0f, 4.0f, 5.0f));
		CHECK(!(Vector4(1.0f, 2.0, 3.0f, 4.0f) < Vector4(1.0f, 2.0f, 3.0f, 4.0f)));
	}
	// >=
	TEST_FIXTURE(ConstructTest, Operator_Greater_Equals) {
		CHECK(!(Vector4(1.0f, 2.0f, 3.0f, 4.0f) >= Vector4(2.0f, 3.0f, 4.0f, 5.0f)));
		CHECK(Vector4(2.0f, 3.0f, 4.0f, 5.0f) >= Vector4(1.0f, 2.0f, 3.0f, 4.0f));
		CHECK(Vector4(1.0f, 2.0f, 3.0f, 4.0f) >= Vector4(1.0f, 2.0f, 3.0f, 4.0f));
	}
	// <=
	TEST_FIXTURE(ConstructTest, Operator_Lesser_Equals) {
		CHECK(!(Vector4(2.0f, 3.0f, 4.0f, 5.0f) <= Vector4(1.0f, 2.0f, 3.0f, 4.0f)));
		CHECK(Vector4(1.0f, 2.0f, 3.0f, 4.0f) <= Vector4(2.0f, 3.0f, 4.0f, 5.0f));
		CHECK(Vector4(1.0f, 2.0, 3.0f, 4.0f) <= Vector4(1.0f, 2.0f, 3.0f, 4.0f));
	}

	// Addition
	// + Vector4
	TEST_FIXTURE(ConstructTest, Operator_Add_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)+Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
		CHECK_EQUAL(8.0f, vV.w);
	}
	// + float
	TEST_FIXTURE(ConstructTest, Operator_Add_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)+2.0f;
		CHECK_EQUAL(3.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
		CHECK_EQUAL(6.0f, vV.w);
	}
	// += Vector4
	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV += Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
		CHECK_EQUAL(8.0f, vV.w);
	}
	// += float
	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV += 2.0f;
		CHECK_EQUAL(3.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
		CHECK_EQUAL(6.0f, vV.w);
	}

	// Subtraction
	// -
	TEST_FIXTURE(ConstructTest, Operator_Minus) {
		vV = -Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(-1.0f, vV.x);
		CHECK_EQUAL(-2.0f, vV.y);
		CHECK_EQUAL(-3.0f, vV.z);
		CHECK_EQUAL(-4.0f, vV.w);
	}
	// - Vector4
	TEST_FIXTURE(ConstructTest, Operator_Minus_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)-Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
		CHECK_EQUAL(0.0f, vV.w);
	}
	// - float
	TEST_FIXTURE(ConstructTest, Operator_Minus_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)-2.0f;
		CHECK_EQUAL(-1.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
		CHECK_EQUAL(2.0f, vV.w);
	}
	// -= Vector4
	TEST_FIXTURE(ConstructTest, Operator_Minus_Equals_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV -= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
		CHECK_EQUAL(0.0f, vV.w);
	}
	// -= float
	TEST_FIXTURE(ConstructTest, Operator_Minus_Equals_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV -= 2.0f;
		CHECK_EQUAL(-1.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
		CHECK_EQUAL(2.0f, vV.w);
	}
	// Vector4()-(-Vector4())
	TEST_FIXTURE(ConstructTest, Operator_Minus_Minus_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)-(-Vector4(1.0f, 2.0f, 3.0f, 4.0f));
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
		CHECK_EQUAL(8.0f, vV.w);
	}

	// Multiplication
	// * Vector4
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)*Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(9.0f, vV.z);
		CHECK_EQUAL(16.0f, vV.w);
	}
	// * float
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)*2.0f;
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
		CHECK_EQUAL(8.0f, vV.w);
	}
	// *= Vector4
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV *= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(9.0f, vV.z);
		CHECK_EQUAL(16.0f, vV.w);
	}
	// *= float
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV *= 2.0f;
		CHECK_EQUAL(2.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(6.0f, vV.z);
		CHECK_EQUAL(8.0f, vV.w);
	}

	// *= Matrix4x4
	TEST_FIXTURE(ConstructTest, Operator_Multiplcation_Equals_Matrix4x4) {
		// 90°
		vV = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		m4x4.FromEulerAngleY(static_cast<float>(90.0f*Math::DegToRad));
		vV *= m4x4;
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.w, Math::Epsilon);

		// -90°
		vV = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		m4x4.FromEulerAngleY(-static_cast<float>(90.0f*Math::DegToRad));
		vV *= m4x4;
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.w, Math::Epsilon);

		// -90° + translation (2, 3, 4)
		vV = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		m4x4.FromEulerAngleY(-static_cast<float>(90.0f*Math::DegToRad));
		m4x4.SetTranslation(2.0f, 3.0f, 4.0f);
		vV *= m4x4;
		CHECK_CLOSE(2.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(3.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(5.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.w, Math::Epsilon);
	}


	// Division
	// / Vector4
	TEST_FIXTURE(ConstructTest, Operator_Division_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)/Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
		CHECK_EQUAL(1.0f, vV.w);
	}
	// / float
	TEST_FIXTURE(ConstructTest, Operator_Division_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f)/2.0f;
		CHECK_EQUAL(0.5f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.5f, vV.z);
		CHECK_EQUAL(2.0f, vV.w);
	}
	// /= Vector4
	TEST_FIXTURE(ConstructTest, Operator_Division_Equals_Vector4) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV /= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.0f, vV.z);
		CHECK_EQUAL(1.0f, vV.w);
	}
	// /= float
	TEST_FIXTURE(ConstructTest, Operator_Division_Equals_Float) {
		vV = Vector4(1.0f, 2.0f, 3.0f, 4.0f);
		vV /= 2.0f;
		CHECK_EQUAL(0.5f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(1.5f, vV.z);
		CHECK_EQUAL(2.0f, vV.w);
	}

	TEST_FIXTURE(ConstructTest, To_FloatPtr) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		float *pf = vV;
		CHECK_EQUAL(0.0f, pf[0]);
		CHECK_EQUAL(1.0f, pf[1]);
		CHECK_EQUAL(2.0f, pf[2]);
		CHECK_EQUAL(3.0f, pf[3]);
	}

	TEST_FIXTURE(ConstructTest, To_const_FloatPtr) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		const float *pf = vV;
		CHECK_EQUAL(0.0f, pf[0]);
		CHECK_EQUAL(1.0f, pf[1]);
		CHECK_EQUAL(2.0f, pf[2]);
		CHECK_EQUAL(3.0f, pf[3]);
	}

	// float &operator [](int)
	TEST_FIXTURE(ConstructTest, Operator_Index) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		CHECK_EQUAL(0.0f, vV[0]);
		CHECK_EQUAL(1.0f, vV[1]);
		CHECK_EQUAL(2.0f, vV[2]);
		CHECK_EQUAL(3.0f, vV[3]);
	}

	// GetXYZW()
	TEST_FIXTURE(ConstructTest, GetXYZW) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		vV.GetXYZW(fX, fY, fZ, fW);
		CHECK_EQUAL(0.0f, fX);
		CHECK_EQUAL(1.0f, fY);
		CHECK_EQUAL(2.0f, fZ);
		CHECK_EQUAL(3.0f, fW);
	}

	// GetX()
	TEST_FIXTURE(ConstructTest, GetX) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		CHECK_EQUAL(0.0f, vV.GetX());
	}

	// GetY()
	TEST_FIXTURE(ConstructTest, GetY) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		CHECK_EQUAL(1.0f, vV.GetY());
	}

	// GetZ()
	TEST_FIXTURE(ConstructTest, GetZ) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		CHECK_EQUAL(2.0f, vV.GetZ());
	}

	// GetW()
	TEST_FIXTURE(ConstructTest, GetW) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		CHECK_EQUAL(3.0f, vV.GetW());
	}

	// SetXYZ(float, float, float)
	TEST_FIXTURE(ConstructTest, SetXYZ_Float) {
		vV.w = 99.0f;
		vV.SetXYZ(0.0f, 1.0f, 2.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(2.0f, vV.z);
		CHECK_EQUAL(99.0f, vV.w);
	}

	// SetXYZ(const float[])
	TEST_FIXTURE(ConstructTest, SetXYZ_FloatArray) {
		vV.w = 99.0f;
		vV.SetXYZ(fT4);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(99.0f, vV.w);
	}

	// SetXYZ(const Vector3 &)
	TEST_FIXTURE(ConstructTest, SetXYZ_Vector3) {
		vV3.x = 0.0f; vV3.y = 1.0f; vV3.z = 2.0f;
		vV.SetXYZ(vV3);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(2.0f, vV.z);
	}

	// SetXYZW(float, float, float, float)
	TEST_FIXTURE(ConstructTest, SetXYZW_Float) {
		vV.SetXYZW(0.0f, 1.0f, 2.0f, 3.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(2.0f, vV.z);
		CHECK_EQUAL(3.0f, vV.w);
	}

	// SetXYZW(const float[])
	TEST_FIXTURE(ConstructTest, SetXYZW_FloatArray) {
		vV.SetXYZW(fT4);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// SetXYZW(const Vector3 &, float)
	TEST_FIXTURE(ConstructTest, SetXYZW_Vector3_and_Float) {
		vV3.x = 0.0f; vV3.y = 1.0f; vV3.z = 2.0f;
		vV.SetXYZW(vV3, 1.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(1.0f, vV.y);
		CHECK_EQUAL(2.0f, vV.z);
		CHECK_EQUAL(1.0f, vV.w);
	}

	// SetX()
	TEST_FIXTURE(ConstructTest, SetX) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.SetX(0.0f);
		CHECK_EQUAL(0.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// SetY()
	TEST_FIXTURE(ConstructTest, SetY) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.SetY(0.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(0.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// SetZ()
	TEST_FIXTURE(ConstructTest, SetZ) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.SetZ(0.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// SetW()
	TEST_FIXTURE(ConstructTest, SetW) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.SetW(0.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(0.0f, vV.w);
	}

	// IncXYZW(float, float, float, float)
	TEST_FIXTURE(ConstructTest, IncXYZW_Float) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		vV.IncXYZW(1.0f, 2.0f, 3.0f, 4.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(3.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
		CHECK_EQUAL(7.0f, vV.w);
	}

	// IncXYZW(const float[])
	TEST_FIXTURE(ConstructTest, IncXYZW_FloatArray) {
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 2.0f; vV.w = 3.0f;
		vV.IncXYZW(fT4);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(3.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
		CHECK_EQUAL(7.0f, vV.w);
	}

	// IncX()
	TEST_FIXTURE(ConstructTest, IncX) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.IncX(2.0f);
		CHECK_EQUAL(3.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// IncY()
	TEST_FIXTURE(ConstructTest, IncY) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.IncY(2.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(4.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// IncZ()
	TEST_FIXTURE(ConstructTest, IncZ) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.IncZ(2.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(5.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// IncW()
	TEST_FIXTURE(ConstructTest, IncW) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		vV.IncW(2.0f);
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(6.0f, vV.w);
	}

	// ToString()
	TEST_FIXTURE(ConstructTest, ToString) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		CHECK_EQUAL("1 2 3 4", vV.ToString().GetASCII());
	}

	// FromString()
	TEST_FIXTURE(ConstructTest, FromString) {
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV.FromString("1 2 3 4");
		CHECK_EQUAL(1.0f, vV.x);
		CHECK_EQUAL(2.0f, vV.y);
		CHECK_EQUAL(3.0f, vV.z);
		CHECK_EQUAL(4.0f, vV.w);
	}

	// Misc
	// IsNull()
	TEST_FIXTURE(ConstructTest, IsNull) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 5.0f; vV.w = 0.0f;
		CHECK(!(vV.IsNull()));
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		CHECK(vV.IsNull());
	}

	// IsPacked()
	TEST_FIXTURE(ConstructTest, IsPacked) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 5.0f; vV.w = 0.0f;
		CHECK(!(vV.IsPacked()));
		vV.x = 0.3f; vV.y = 0.8f; vV.z = 0.0f; vV.w = 1.0f;
		CHECK(vV.IsPacked());
	}

	// PackTo01()
	TEST_FIXTURE(ConstructTest, PackTo01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f; vV.w = 0.1f;
		vV.PackTo01();
		CHECK_CLOSE(0.7227177f, vV.x, Math::Epsilon);
		CHECK_CLOSE(vV.y, 0.9454354f, Math::Epsilon);
		CHECK_CLOSE(vV.z, 0.5f, Math::Epsilon);
		CHECK_CLOSE(vV.w, 0.5445435f, Math::Epsilon);

		//
		vV.x = -5.0f; vV.y = 2.0f; vV.z = 1.0f; vV.w = 0.1f;
		vV.PackTo01();
		CHECK_CLOSE(0.04364058f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.6825437f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.5912718f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.5091272f, vV.w, Math::Epsilon);
	}

	// GetPackedTo01()
	TEST_FIXTURE(ConstructTest, GetPackedTo01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f; vV.w = 0.1f;
		vV = vV.GetPackedTo01();
		CHECK_CLOSE(0.7227177f, vV.x, Math::Epsilon);
		CHECK_CLOSE(vV.y, 0.9454354f, Math::Epsilon);
		CHECK_CLOSE(vV.z, 0.5f, Math::Epsilon);
		CHECK_CLOSE(vV.w, 0.5445435f, Math::Epsilon);

		//
		vV.x = -5.0f; vV.y = 2.0f; vV.z = 1.0f; vV.w = 0.1f;
		vV = vV.GetPackedTo01();
		CHECK_CLOSE(0.04364058f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.6825437f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.5912718f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.5091272f, vV.w, Math::Epsilon);
	}

	// UnpackFrom01()
	TEST_FIXTURE(ConstructTest, UnpackFrom01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f; vV.w = 0.1f;
		vV.UnpackFrom01();
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(-0.8f, vV.w, Math::Epsilon);

		//
		vV.x = 0.2f; vV.y = 0.6f; vV.z = 0.0f; vV.w = 0.1f;
		vV.UnpackFrom01();
		CHECK_CLOSE(-0.6f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.2f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(-0.8f, vV.w, Math::Epsilon);
	}

	// GetUnpackedFrom01()
	TEST_FIXTURE(ConstructTest, GetUnpackedFrom01) {
		//
		vV.x = 0.5f; vV.y = 1.0f; vV.z = 0.0f; vV.w = 0.1f;
		vV = vV.GetUnpackedFrom01();
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(-0.8f, vV.w, Math::Epsilon);

		//
		vV.x = 0.2f; vV.y = 0.6f; vV.z = 0.0f; vV.w = 0.1f;
		vV = vV.GetUnpackedFrom01();
		CHECK_CLOSE(-0.6f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.2f, vV.y, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(-0.8f, vV.w, Math::Epsilon);
	}

	// GetSmallestComponent()
	TEST_FIXTURE(ConstructTest, GetSmallestComponent) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		CHECK_EQUAL(Vector4::X, vV.GetSmallestComponent());
	}

	// GetSmallestValue()
	TEST_FIXTURE(ConstructTest, GetSmallestValue) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		CHECK_EQUAL(1.0f, vV.GetSmallestValue());
	}

	// GetGreatestComponent()
	TEST_FIXTURE(ConstructTest, GetGreatestComponent) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		CHECK_EQUAL(Vector4::W, vV.GetGreatestComponent());
	}

	// GetGreatestValue()
	TEST_FIXTURE(ConstructTest, GetGreatestValue) {
		vV.x = 1.0f; vV.y = 2.0f; vV.z = 3.0f; vV.w = 4.0f;
		CHECK_EQUAL(4.0f, vV.GetGreatestValue());
	}

	// Invert()
	TEST_FIXTURE(ConstructTest, Invert) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		vV.Invert();
		CHECK_EQUAL(-0.2f, vV.x);
		CHECK_EQUAL(0.6f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
		CHECK_EQUAL(-1.0f, vV.w);
	}

	// GetInverted()
	TEST_FIXTURE(ConstructTest, GetInverted) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		vV = vV.GetInverted();
		CHECK_EQUAL(-0.2f, vV.x);
		CHECK_EQUAL(0.6f, vV.y);
		CHECK_EQUAL(0.0f, vV.z);
		CHECK_EQUAL(-1.0f, vV.w);
	}

	// GetLength()
	TEST_FIXTURE(ConstructTest, GetLength) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		CHECK_CLOSE(1.183216f, vV.GetLength(), Math::Epsilon);
		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
	}

	// GetSquaredLength()
	TEST_FIXTURE(ConstructTest, GetSquaredLength) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		CHECK_CLOSE(1.4f, vV.GetSquaredLength(), Math::Epsilon);
		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		CHECK_CLOSE(0.0f, vV.GetSquaredLength(), Math::Epsilon);
		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		CHECK_CLOSE(1.0f, vV.GetSquaredLength(), Math::Epsilon);
	}

	// SetLength()
	TEST_FIXTURE(ConstructTest, SetLength) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		vV.SetLength(1.0f);
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.1690308f, vV.x, Math::Epsilon);
		CHECK_CLOSE(-0.5070926f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.8451542f, vV.w, Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV.SetLength(1.0f);
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.w, Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV.SetLength(1.0f);
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.w, Math::Epsilon);
	}

	// Normalize()
	TEST_FIXTURE(ConstructTest, Normalize) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		vV.Normalize();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.1690308f, vV.x, Math::Epsilon);
		CHECK_CLOSE(-0.5070926f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.8451542f, vV.w, Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV.Normalize();
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.w, Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV.Normalize();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.w, Math::Epsilon);
	}

	// GetNormalized()
	TEST_FIXTURE(ConstructTest, GetNormalized) {
		// Any vector
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		vV = vV.GetNormalized();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.1690308f, vV.x, Math::Epsilon);
		CHECK_CLOSE(-0.5070926f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.8451542f, vV.w, Math::Epsilon);

		// Null vector
		vV.x = 0.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV = vV.GetNormalized();
		CHECK_CLOSE(0.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.w, Math::Epsilon);

		// Unit vector
		vV.x = 1.0f; vV.y = 0.0f; vV.z = 0.0f; vV.w = 0.0f;
		vV = vV.GetNormalized();
		CHECK_CLOSE(1.0f, vV.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vV.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.y, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.z, Math::Epsilon);
		CHECK_CLOSE(0.0f, vV.w, Math::Epsilon);
	}

	// GetDistance()
	TEST_FIXTURE(ConstructTest, GetDistance) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		CHECK_CLOSE(1.183216f, vV.GetDistance(Vector4(0.0f, 0.0f, 0.0f, 0.0f)), Math::Epsilon);
	}

	// GetSquaredDistance()
	TEST_FIXTURE(ConstructTest, GetSquaredDistance) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		CHECK_CLOSE(1.4f, vV.GetSquaredDistance(Vector4(0.0f, 0.0f, 0.0f, 0.0f)), Math::Epsilon);
	}

	// DotProduct()
	TEST_FIXTURE(ConstructTest, DotProduct) {
		vV.x = 0.2f; vV.y = -0.6f; vV.z = 0.0f; vV.w = 1.0f;
		CHECK_CLOSE(0.0f, vV.DotProduct(Vector4(0.0f, 0.0f, 0.0f, 0.0f)), Math::Epsilon);
		CHECK_CLOSE(1.4f, vV.DotProduct(Vector4(0.2f, -0.6f, 0.0f, 1.0f)), Math::Epsilon);
	}

	// GetAngle()
	TEST_FIXTURE(ConstructTest, GetAngle) {
		// ~69.5°
		vV.x = 1.0f; vV.y = 4.0f; vV.z = -2.0f; vV.w = 0.0f;
		CHECK_CLOSE(1.2127572f, vV.GetAngle(Vector4(-3.0f, 3.0f, 1.0f, 0.0f)), Math::Epsilon);
		// 45°
		vV.x = 0.0f; vV.y = 1.0f; vV.z = 0.0f; vV.w = 0.0f;
		CHECK_CLOSE(0.7853981f, vV.GetAngle(Vector4(1.0f, 1.0f, 0.0f, 0.0f)), Math::Epsilon);
		// Now use normalized vectors and use the dot product... (~69.5°)
		vV.x = 1.0f; vV.y = 4.0f; vV.z = -2.0f; vV.w = 0.0f;
		vV.Normalize();
		CHECK_CLOSE(1.2127573f, Math::ACos(vV.DotProduct(Vector4(-3.0f, 3.0f, 1.0f, 0.0f).Normalize())), Math::Epsilon);
	}

	/* [TODO]
		PLMATH_API Vector4 ProjectVector(const Vector4 &vA, const Vector4 &vB) const;
		PLMATH_API Vector4 &GetProjection(const Vector4 &vX, const Vector4 &vN);
		PLMATH_API Vector4 &ProjectPlane(const Vector4 &vV1, const Vector4 &vV2);
	*/
}