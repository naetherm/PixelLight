//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/String/String.h>
#include <PLMath/Vector2.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Matrix4x4.h>

using namespace PLMath;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Vector2) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() :
			fX(0.0f),
			fY(0.0f)
		{
			/* some setup */
			T2[0] = 1.0f;
			T2[1] = 2.0f;

			vec2 = Vector2(T2);
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Objects for testing
		Vector2 vec2;

		Matrix3x3 m3x3;
		Matrix4x4 m4x4;
		float fX, fY, T2[2];
	};

	TEST(Vector2_Vars) {
		CHECK_EQUAL(0.0f, Vector2::Zero.x);
		CHECK_EQUAL(0.0f, Vector2::Zero.y);

		CHECK_EQUAL(1.0f, Vector2::One.x);
		CHECK_EQUAL(1.0f, Vector2::One.y);

		CHECK_EQUAL(1.0f, Vector2::UnitX.x);
		CHECK_EQUAL(0.0f, Vector2::UnitX.y);

		CHECK_EQUAL(0.0f, Vector2::UnitY.x);
		CHECK_EQUAL(1.0f, Vector2::UnitY.y);

		CHECK_EQUAL(-1.0f, Vector2::NegativeUnitX.x);
		CHECK_EQUAL(0.0f, Vector2::NegativeUnitX.y);

		CHECK_EQUAL(0.0f, Vector2::NegativeUnitY.x);
		CHECK_EQUAL(-1.0f, Vector2::NegativeUnitY.y);
	}

	TEST_FIXTURE(ConstructTest, Constructors) {
		Vector2 vVec2;
		CHECK_EQUAL(0.0f, vVec2.x);
		CHECK_EQUAL(0.0f, vVec2.y);

		vVec2 = Vector2(1.0f, 2.0f);
		CHECK_EQUAL(1.0f, vVec2.x);
		CHECK_EQUAL(2.0f, vVec2.y);

		vVec2 = Vector2(T2);
		CHECK_EQUAL(1.0f, vVec2.x);
		CHECK_EQUAL(2.0f, vVec2.y);

		vVec2 = Vector2(Vector2(1.0f, 2.0f));
		CHECK_EQUAL(1.0f, vVec2.x);
		CHECK_EQUAL(2.0f, vVec2.y);

		vVec2 = Vector2(PLCore::String("1.0f 2.0f"));
		CHECK_EQUAL(1.0f, vVec2.x);
		CHECK_EQUAL(2.0f, vVec2.y);

		vVec2 = Vector2(Vector3(1.0f, 2.0f, 3.0f));
		CHECK_EQUAL(1.0f, vVec2.x);
		CHECK_EQUAL(2.0f, vVec2.y);

		vVec2 = Vector2(Vector4(1.0f, 2.0f, 3.0f, 4.0f));
		CHECK_EQUAL(1.0f, vVec2.x);
		CHECK_EQUAL(2.0f, vVec2.y);
	}

	// Comparison
	TEST(Compare_Equal) {
		CHECK(Vector2(1.0f, 2.0f) == Vector2(1.0f, 2.0f));
		CHECK(!(Vector2(1.0f, 2.0f) == Vector2(1.0f, 2.0001f)));
	}

	TEST(Compare_NotEqual) {
		CHECK(!(Vector2(1.0f, 2.0f) != Vector2(1.0f, 2.0f)));
		CHECK(Vector2(1.0f, 2.0f) != Vector2(1.0f, 2.0001f));
	}

	TEST(Compare_Bigger) {
		CHECK(!(Vector2(1.0f, 2.0f) > Vector2(2.0f, 3.0f)));
		CHECK(Vector2(2.0f, 3.0f) > Vector2(1.0f, 2.0f));
		CHECK(!(Vector2(1.0f, 2.0f) > Vector2(1.0f, 2.0f)));
	}

	TEST(Compare_Smaller) {
		CHECK(Vector2(1.0f, 2.0f) < Vector2(2.0f, 3.0f));
		CHECK(!(Vector2(2.0f, 3.0f) < Vector2(1.0f, 2.0f)));
		CHECK(!(Vector2(1.0f, 2.0f) < Vector2(1.0f, 2.0f)));
	}

	TEST(Compare_BiggerEqual) {
		CHECK(!(Vector2(1.0f, 2.0f) >= Vector2(2.0f, 3.0f)));
		CHECK(Vector2(2.0f, 3.0f) >= Vector2(1.0f, 2.0f));
		CHECK(Vector2(1.0f, 2.0f) >= Vector2(1.0f, 2.0f));
	}

	TEST(Compare_SmallerEqual) {
		CHECK(Vector2(1.0f, 2.0f) <= Vector2(2.0f, 3.0f));
		CHECK(!(Vector2(2.0f, 3.0f) <= Vector2(1.0f, 2.0f)));
		CHECK(Vector2(1.0f, 2.0f) <= Vector2(1.0f, 2.0f));
	}

	// Addition
	TEST_FIXTURE(ConstructTest, Operator_AddVector2) {
		vec2 = Vector2(1.0f, 2.0f)+Vector2(1.0f, 2.0f);
		CHECK_EQUAL(2.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);

		vec2 += Vector2(1.0f, 2.0f);
		CHECK_EQUAL(3.0f, vec2.x);
		CHECK_EQUAL(6.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, Operator_AddFloat) {
		vec2 = Vector2(1.0f, 2.0f)+2.0f;
		CHECK_EQUAL(3.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);

		vec2 += 2.0f;
		CHECK_EQUAL(5.0f, vec2.x);
		CHECK_EQUAL(6.0f, vec2.y);
	}

	// Subtraction
	TEST_FIXTURE(ConstructTest, Operator_negative) {
		vec2 = -Vector2(1.0f, 2.0f);
		CHECK_EQUAL(-1.0f, vec2.x);
		CHECK_EQUAL(-2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, Operator_SubtractVector2) {
		vec2 = vec2-Vector2(1.0f, 2.0f);
		CHECK_EQUAL(0.0f, vec2.x);
		CHECK_EQUAL(0.0f, vec2.y);

		vec2 -= Vector2(1.0f, 2.0f);
		CHECK_EQUAL(-1.0f, vec2.x);
		CHECK_EQUAL(-2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, Operator_SubtractFloat) {
		vec2 = vec2-1.0f;
		CHECK_EQUAL(0.0f, vec2.x);
		CHECK_EQUAL(1.0f, vec2.y);

		vec2 -= 1.0f;
		CHECK_EQUAL(-1.0f, vec2.x);
		CHECK_EQUAL(0.0f, vec2.y);
	}

	// Multiplication
	TEST_FIXTURE(ConstructTest, Operator_MultiplicationVector2) {
		vec2 = Vector2(1.0f, 2.0f)*Vector2(1.0f, 2.0f);
		CHECK_EQUAL(1.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);

		vec2 *= Vector2(2.0f, 1.0f);
		CHECK_EQUAL(2.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, Operator_MultiplicationFloat) {
		vec2 = Vector2(1.0f, 2.0f)*2.0f;
		CHECK_EQUAL(2.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);

		vec2 *= 2.0f;
		CHECK_EQUAL(4.0f, vec2.x);
		CHECK_EQUAL(8.0f, vec2.y);
	}

	// Multiplication 3x3 Matrix
	TEST_FIXTURE(ConstructTest, Operator_MultiplicationMatrix3x3) {
		// 90°
		vec2 = Vector2(1.0f, 0.0f);
		m3x3.FromEulerAngleZ(static_cast<float>(90.0f*Math::DegToRad));
		vec2 *= m3x3;

		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vec2.y, Math::Epsilon);

		// -90°
		vec2 = Vector2(1.0f, 0.0f);
		m3x3.FromEulerAngleZ(-static_cast<float>(90.0f*Math::DegToRad));
		vec2 *= m3x3;

		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vec2.y, Math::Epsilon);
	}

	// Multiplication 4x4 Matrix
	TEST_FIXTURE(ConstructTest, Operator_MultiplicationMatrix4x4) {
		// 90°
		vec2 = Vector2(1.0f, 0.0f);
		m4x4.FromEulerAngleZ(static_cast<float>(90.0f*Math::DegToRad));
		vec2 *= m4x4;

		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vec2.y, Math::Epsilon);

		// -90°
		vec2 = Vector2(1.0f, 0.0f);
		m4x4.FromEulerAngleZ(-static_cast<float>(90.0f*Math::DegToRad));
		vec2 *= m4x4;

		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(-1.0f, vec2.y, Math::Epsilon);

		// -90° + translation (2, 3, 4)
		vec2 = Vector2(1.0f, 0.0f);
		m4x4.FromEulerAngleZ(-static_cast<float>(90.0f*Math::DegToRad));
		m4x4.SetTranslation(2.0f, 3.0f, 4.0f);
		vec2 *= m4x4;

		CHECK_CLOSE(2.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(2.0f, vec2.y, Math::Epsilon);
	}

	// Division
	TEST_FIXTURE(ConstructTest, Operator_DivisionVector2) {
		vec2 = Vector2(1.0f, 2.0f)/Vector2(1.0f, 2.0f);
		CHECK_EQUAL(1.0f, vec2.x);
		CHECK_EQUAL(1.0f, vec2.y);

		vec2 /= Vector2(0.5f, 0.5f);
		CHECK_EQUAL(2.0f, vec2.x);
		CHECK_EQUAL(2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, Operator_DivisionFloat) {
		vec2 = Vector2(1.0f, 2.0f)/2.0f;
		CHECK_EQUAL(0.5f, vec2.x);
		CHECK_EQUAL(1.0f, vec2.y);

		vec2 /= 2.0f;
		CHECK_EQUAL(0.25f, vec2.x);
		CHECK_EQUAL(0.5f, vec2.y);
	}

	// Get and Set
	TEST_FIXTURE(ConstructTest, Operator_Index) {
		CHECK_EQUAL(1.0f, vec2[0]);
		CHECK_EQUAL(2.0f, vec2[1]);
	}

	TEST_FIXTURE(ConstructTest, GetXY) {
		float x, y;
		vec2.GetXY(x,y);
		CHECK_EQUAL(1.0f, x);
		CHECK_EQUAL(2.0f, y);
	}

	TEST_FIXTURE(ConstructTest, GetX) {
		CHECK_EQUAL(1.0f, vec2.GetX());
	}

	TEST_FIXTURE(ConstructTest, GetY) {
		CHECK_EQUAL(2.0f, vec2.GetY());
	}

	TEST_FIXTURE(ConstructTest, SetXY_Float) {
		vec2.SetXY(5.0f, 6.0f);
		CHECK_EQUAL(5.0f, vec2.x);
		CHECK_EQUAL(6.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, SetXY_FloatArray) {
		vec2.SetXY(T2);
		CHECK_EQUAL(1.0f, vec2.x);
		CHECK_EQUAL(2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, SetX) {
		vec2.SetX(4.0f);
		CHECK_EQUAL(4.0f, vec2.x);
		CHECK_EQUAL(2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, SetY) {
		vec2.SetY(4.0f);
		CHECK_EQUAL(1.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, IncXY_Float) {
		vec2.IncXY(2.0f, 1.0f);
		CHECK_EQUAL(3.0f, vec2.x);
		CHECK_EQUAL(3.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, IncXY_FloatArray) {
		vec2.IncXY(T2);
		CHECK_EQUAL(2.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, IncX) {
		vec2.IncX(2.0f);
		CHECK_EQUAL(3.0f, vec2.x);
		CHECK_EQUAL(2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, IncY) {
		vec2.IncY(2.0f);
		CHECK_EQUAL(1.0f, vec2.x);
		CHECK_EQUAL(4.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, ToString) {
		CHECK_EQUAL("1 2", vec2.ToString().GetASCII());
	}

	TEST_FIXTURE(ConstructTest, FromString) {
		vec2.FromString("2.0f 1");
		CHECK_EQUAL(2.0f, vec2.x);
		CHECK_EQUAL(1.0f, vec2.y);
	}

	// Misc tests
	TEST_FIXTURE(ConstructTest, IsNull) {
		CHECK(!vec2.IsNull());

		vec2 = Vector2(0.0f, 0.0f);
		CHECK(vec2.IsNull());
	}

	TEST_FIXTURE(ConstructTest, IsPacked) {
		CHECK(!vec2.IsPacked());

		vec2 = Vector2(0.3f, 0.8f);
		CHECK(vec2.IsPacked());
	}

	TEST_FIXTURE(ConstructTest, PackTo01) {
		vec2 = Vector2(0.5f, 1.0f);
		vec2.PackTo01();

		CHECK_CLOSE(0.72360682f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(0.94721359f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetPackedTo01) {
		vec2 = Vector2(0.5f, 1.0f);
		vec2 = vec2.GetPackedTo01();

		CHECK_CLOSE(0.72360682f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(0.94721359f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, UnpackFrom01) {
		vec2 = Vector2(0.5f, 1.0f);
		vec2.UnpackFrom01();

		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetUnpackedFrom01) {
		vec2 = Vector2(0.5f, 1.0f);
		vec2 = vec2.GetUnpackedFrom01();

		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(1.0f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetSmallestComponent) {
		CHECK(Vector2::X == vec2.GetSmallestComponent());
	}

	TEST_FIXTURE(ConstructTest, GetSmallestValue) {
		CHECK_EQUAL(1.0f, vec2.GetSmallestValue());
	}

	TEST_FIXTURE(ConstructTest, GetGreatestComponent) {
		CHECK(Vector2::Y == vec2.GetGreatestComponent());
	}

	TEST_FIXTURE(ConstructTest, GetGreatestValue) {
		CHECK_EQUAL(2.0f, vec2.GetGreatestValue());
	}

	TEST_FIXTURE(ConstructTest, Invert) {
		vec2 = Vector2(1.0f, -2.0f);
		vec2.Invert();

		CHECK_EQUAL(-1.0f, vec2.x);
		CHECK_EQUAL(2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, GetInverted) {
		vec2 = Vector2(1.0f, -2.0f);
		vec2 = vec2.GetInverted();

		CHECK_EQUAL(-1.0f, vec2.x);
		CHECK_EQUAL(2.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, GetLength_NullVec) {
		vec2 = Vector2(0.0f, 0.0f);

		CHECK_CLOSE(0.0f, vec2.GetLength(), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetLength_NormalVec) {
		vec2 = Vector2(0.2f, -0.6f);

		CHECK_CLOSE(0.6324555f, vec2.GetLength(), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetLength_UnitVec) {
		vec2 = Vector2(1.0f, 0.0f);

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetSquaredLength_NullVec) {
		vec2 = Vector2(0.0f, 0.0f);

		CHECK_CLOSE(0.0f, vec2.GetSquaredLength(), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetSquaredLength_NormalVec) {
		vec2 = Vector2(0.2f, -0.6f);

		CHECK_CLOSE(0.4f, vec2.GetSquaredLength(), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetSquaredLength_UnitVec) {
		vec2 = Vector2(1.0f, 0.0f);

		CHECK_CLOSE(1.0f, vec2.GetSquaredLength(), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, SetLength_NullVec) {
		vec2 = Vector2(0.0f, 0.0f);
		vec2.SetLength(1.0f);

		CHECK_CLOSE(0.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_EQUAL(0.0f, vec2.x);
		CHECK_EQUAL(0.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, SetLength_NormalVec) {
		vec2 = Vector2(0.2f, -0.6f);
		vec2.SetLength(1.0f);

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.3162277f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(-0.9486832f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, SetLength_UnitVec) {
		vec2 = Vector2(1.0f, 0.0f);
		vec2.SetLength(1.0f);

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_EQUAL(1.0f, vec2.x);
		CHECK_EQUAL(0.0f, vec2.y);
	}

	TEST_FIXTURE(ConstructTest, Normalize_NullVec) {
		vec2 = Vector2(0.0f, 0.0f);
		vec2.Normalize();

		CHECK_CLOSE(0.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, Normalize_NormalVec) {
		vec2 = Vector2(0.2f, -0.6f);
		vec2.Normalize();

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.3162277f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(-0.9486832f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, Normalize_UnitVec) {
		vec2 = Vector2(1.0f, 0.0f);
		vec2.Normalize();

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetNormalized_NullVec) {
		vec2 = Vector2(0.0f, 0.0f);
		vec2 = vec2.GetNormalized();

		CHECK_CLOSE(0.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetNormalized_NormalVec) {
		vec2 = Vector2(0.2f, -0.6f);
		vec2 = vec2.GetNormalized();

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(0.3162277f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(-0.9486832f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetNormalized_UnitVec) {
		vec2 = Vector2(1.0f, 0.0f);
		vec2 = vec2.GetNormalized();

		CHECK_CLOSE(1.0f, vec2.GetLength(), Math::Epsilon);
		CHECK_CLOSE(1.0f, vec2.x, Math::Epsilon);
		CHECK_CLOSE(0.0f, vec2.y, Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetDistance) {
		vec2 = Vector2(0.2f, -0.6f);

		CHECK_CLOSE(0.6324555f, vec2.GetDistance(Vector2(0.0f, 0.0f)), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetSquaredDistance) {
		vec2 = Vector2(0.2f, -0.6f);

		CHECK_CLOSE(0.4f, vec2.GetSquaredDistance(Vector2(0.0f, 0.0f)), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, DotProduct) {
		vec2 = Vector2(0.2f, -0.6f);

		CHECK_CLOSE(0.0f, vec2.DotProduct(Vector2(0.0f, 0.0f)), Math::Epsilon);
		CHECK_CLOSE(0.4f, vec2.DotProduct(Vector2(0.2f, -0.6f)), Math::Epsilon);
	}

	TEST_FIXTURE(ConstructTest, GetAngle) {
		// ~74,7°
		vec2 = Vector2(1.0f, 4.0f);
		CHECK_CLOSE(1.0303768f, vec2.GetAngle(Vector2(-3.0f, 3.0f)), Math::Epsilon);

		// 45°
		vec2 = Vector2(0.0f, 1.0f);
		CHECK_CLOSE(0.7853981f, vec2.GetAngle(Vector2(1.0f, 1.0f)), Math::Epsilon);

		// Now use normalized vectors and use the dot product... (~74.7°)
		vec2 = Vector2(1.0f, 4.0f);
		vec2.Normalize();
		CHECK_CLOSE(1.0303768f, Math::ACos(vec2.DotProduct(Vector2(-3.0f, 3.0f).Normalize())), Math::Epsilon);
	}
}