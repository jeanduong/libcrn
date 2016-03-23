/* Copyright 2006-2016 Yann LEYDIER, CoReNum, INSA-Lyon, ENS-Lyon
 * 
 * This file is part of libcrn.
 * 
 * libcrn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libcrn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libcrn.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * file: CRNObject.h
 * \author Yann LEYDIER
 */

#ifndef CRNOBJECT_HEADER
#define CRNOBJECT_HEADER

#include <CRN.h>

namespace crn
{
	class Object;
	CRN_ALIAS_SMART_PTR(Object)
}
namespace crn
{
	/*! \defgroup base Base classes */
	class Object
	{
		public:
			virtual ~Object() = default;
		protected:
	};

	UObject Clone(const Object &obj);
	template<typename T> inline std::unique_ptr<T> CloneAs(const Object &obj) { return std::unique_ptr<T>(dynamic_cast<T*>(Clone(obj).release())); }

	namespace xml
	{
		class Element;
	}
	void Deserialize(Object &obj, xml::Element &el);
	xml::Element Serialize(const Object &obj, xml::Element &parent);
	double Distance(const Object &o1, const Object &o2);


	namespace protocol
	{
		// Partially ordered objects
		struct DummyType {};
		template<typename T> DummyType operator<(const T &, const T &) { return DummyType{}; }
		template<typename T> DummyType operator>(const T &, const T &) { return DummyType{}; }
		template<typename T> DummyType operator<=(const T &, const T &) { return DummyType{}; }
		template<typename T> DummyType operator>=(const T &, const T &) { return DummyType{}; }

		template<typename T> struct HasLT :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() < std::declval<T const&>())
			>::value
			>
		{};
		template<typename T> struct HasGT :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() > std::declval<T const&>())
			>::value
			>
		{};
		template<typename T> struct HasLE :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() <= std::declval<T const&>())
			>::value
			>
		{};
		template<typename T> struct HasGE :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() >= std::declval<T const&>())
			>::value
			>
		{};

		// Addable objects
		template<typename T> DummyType operator+(const T &, const T &) { return DummyType{}; }
		template<typename T> DummyType operator==(const T &, const T &) { return DummyType{}; }

		template<typename T> struct HasPlus :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() + std::declval<T const&>())
			>::value
			>
		{};
		template<typename T> struct HasEquals :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() == std::declval<T const&>())
			>::value
			>
		{};

		// Addable and subtractable objects
		template<typename T> DummyType operator-(const T &, const T &) { return DummyType{}; }

		template<typename T> struct HasMinus :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() - std::declval<T const&>())
			>::value
			>
		{};

		// Addable, subtractable and inner-multipliable objects
		template<typename T> DummyType operator*(const T &, const T &) { return DummyType{}; }

		template<typename T> struct HasInnerMult :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() * std::declval<T const&>())
			>::value
			>
		{};

		// Addable, subtractable and outer-multipliable objects
		struct doubleWrapper { doubleWrapper(double) {} };
		template<typename T> DummyType operator*(const T &, doubleWrapper) { return DummyType{}; }
		template<typename T> DummyType operator*(doubleWrapper, const T &) { return DummyType{}; }

		template<typename T> struct HasRightOuterMult :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() * 0.0)
			>::value
			>
		{};
		template<typename T> struct HasLeftOuterMult :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(0.0 * std::declval<T const&>())
			>::value
			>
		{};

		// Addable, subtractable, inner-multipliable and outer-multipliable objects

		// Addable, subtractable, inner-multipliable, outer-multipliable and dividable objects
		template<typename T> DummyType operator/(const T &, const T &) { return DummyType{}; }

		template<typename T> struct HasDivide :
			public std::integral_constant<
			bool,
			!std::is_same<
			DummyType,
			decltype(std::declval<T const&>() / std::declval<T const&>())
			>::value
			>
		{};


	}

	/*! Has:
	 * - Prop3 operator<(T, T)
	 * - Prop3 operator<=(T, T)
	 * - Prop3 operator>(T, T)
	 * - Prop3 operator>=(T, T)
	 */
	template<typename T> struct IsPOSet: public std::integral_constant<bool, protocol::HasLT<T>::value && protocol::HasGT<T>::value && protocol::HasLE<T>::value && protocol::HasGE<T>::value> {};


	// Metric objects
	/*! Has:
	 * - Distance(T, T)
	 */
	template<typename T> struct IsMetric: public std::integral_constant<bool, std::is_arithmetic<T>::value>{};

	/*! Has:
	 * - operator+(T, T)
	 * - operator==(T, T)
	 */
	template<typename T> struct IsMagma: public std::integral_constant<bool, protocol::HasEquals<T>::value && protocol::HasPlus<T>::value> {};

	/*! Has:
	 * - operator+(T, T)
	 * - operator-(T, T)
	 */
	template<typename T> struct IsGroup: public std::integral_constant<bool, IsMagma<T>::value && protocol::HasMinus<T>::value> {};

	/*! Has:
	 * - operator+(T, T)
	 * - operator-(T, T)
	 * - operator*(T, T)
	 */
	template<typename T> struct IsRing: public std::integral_constant<bool, IsGroup<T>::value && protocol::HasInnerMult<T>::value> {};

	/*! Has:
	 * - operator+(T, T)
	 * - operator-(T, T)
	 * - operator*(T, double)
	 * - operator*(double, T)
	 */
	template<typename T> struct IsVectorOverR: public std::integral_constant<bool, IsGroup<T>::value && protocol::HasRightOuterMult<T>::value && protocol::HasLeftOuterMult<T>::value> {};


	/*! Has:
	 * - operator+(T, T)
	 * - operator-(T, T)
	 * - operator*(T, T)
	 * - operator*(T, double)
	 * - operator*(double, T)
	 */
	template<typename T> struct IsAlgebra: public std::integral_constant<bool, IsRing<T>::value && IsVectorOverR<T>::value> {};
	/*! Has:
	 * - operator+(T, T)
	 * - operator-(T, T)
	 * - operator*(T, T)
	 * - operator*(T, double)
	 * - operator*(double, T)
	 * - operator/(T, T)
	 */
	template<typename T> struct IsField: public std::integral_constant<bool, IsAlgebra<T>::value && protocol::HasDivide<T>::value> {};

	// Serializable objects

	/*! Has:
	 * - T::T(xml::Element &)
	 * - Serialize(const T &, xml::Element &parent)
	 * - Deserialize(T &, xml::Element &)
	 */
	template<typename T> struct IsSerializable: public std::false_type {};
	// Clonable objects

	/*! Has:
	 * - T::Clone()
	 */
	template<typename T> struct IsClonable: public std::false_type {};

	// Savable objects

	/*! Has:
	 * - T::T(const Path &)
	 * - Save(const T &, const Path &)
	 * - Load(T &, const Path &)
	 */
	template<typename T> struct IsSavable: public std::false_type {};

}

/*! \brief	Defines a default constructor from xml element 
 * \param[in]	classname	the class in which the constructor is added
 * \ingroup	base
 */
#define CRN_SERIALIZATION_CONSTRUCTOR(classname) public: classname(crn::xml::Element &el) { Deserialize(el); }

/*! \brief Declares a class constructor
 *
 * Declares a class constructor. Add this to a class declaration.
 * \param[in]	classname	the name of the current class
 * \ingroup	base
 */
#define CRN_DECLARE_CLASS_CONSTRUCTOR(classname) public:\
	static void Initialize();\
private:\
struct Init { Init() { classname::Initialize(); } };\
static Init init;

/*! \brief Defines a class constructor
 *
 * Defines a class constructor. Add this to a class definition file.
 * \param[in]	classname	the name of the current class
 * \ingroup	base
 */
#define CRN_BEGIN_CLASS_CONSTRUCTOR(classname) void classname::Initialize()\
{\
	static bool init_done = false;\
	if (!init_done)\
	{\
		init_done = true;

		/*! \brief Defines a class constructor
		 *
		 * Defines a class constructor. Add this to a class definition file.
		 * \param[in]	classname	the name of the current class
		 * \ingroup	base
		 */
#define CRN_END_CLASS_CONSTRUCTOR(classname) }\
}\
classname::Init classname::init;

#endif

