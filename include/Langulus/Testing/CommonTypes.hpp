///                                                                           
/// Langulus::Anyness                                                         
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Testing.hpp>
#include <Langulus/Typenav.hpp>
#include <Langulus/IntentOf.hpp>
#include <Langulus/Utils/Literal.hpp>
#include <Langulus/Utils/Values.hpp>
#include <Langulus/Utils/Types.hpp>
#include <Langulus/CT/Number.hpp>
#include <Langulus/CT/ReflectAs.hpp>
#include <Langulus/CT/Versioned.hpp>
#include <Langulus/CT/Deep.hpp>
#include <Langulus/CT/DefineVerb.hpp>
#include <Langulus/CT/DefineTag.hpp>
#include <Langulus/CT/Members.hpp>
#include <Langulus/CT/POD.hpp>
#include <Langulus/CT/Pooled.hpp>
#include <Langulus/CT/Convertible.hpp>
#include <Langulus/CT/Describable.hpp>
#include <Langulus/CT/Able.hpp>
//#include "ReferencedType.hpp"
//#include "PackedPointers.hpp"
#include <string>

using namespace Langulus;


///                                                                           
/// The types in this file act as a comprehensive list for testing with       
/// each and every function, container and pattern the framework provides.    
///                                                                           

/// MARK: Metas                                                               
/*using RTTI::DMeta;
using RTTI::TMeta;
using RTTI::CMeta;
using RTTI::VMeta;*/

namespace Langulus::Annies
{
   struct Many;
   struct Any;
   template<class> struct TVerb;
   using Verb = TVerb<void>;
}

/// MARK: Incomplete                                                          
class IncompleteType;

/// MARK: Enum                                                                
enum class Pi {
   Number = 314,
   ConflictingNumber = 666
};

/// MARK: Reflectable                                                         
struct NotReflectable       { using CTTI_ReflectAs = void; };
struct NotReflectableIntern { using CTTI_ReflectAs = No;   };
struct NotReflectableExtern {};
struct ReflectableIntern    { using CTTI_ReflectAs = char; };
struct ReflectableExtern    {};
struct ReflectableAsSelf    { using CTTI_ReflectAs = ReflectableAsSelf; };

/// MARK: Sheddable                                                           
template<class T>
struct SheddableType {
   using CTTI_Sheddable = T;

   T& instance;

   SheddableType(T& t) : instance {t} {}
};

template<>
struct SheddableType<IncompleteType> {
   using CTTI_Sheddable = IncompleteType;
};

template<class T>
struct SheddableTypeCastableExplicit : SheddableType<T> {
   using SheddableType<T>::SheddableType;
   using SheddableType<T>::instance;
   explicit operator T& () noexcept { return LglsFwd(instance); }
   explicit operator T& () const noexcept { return LglsFwd(const_cast<SheddableTypeCastableExplicit<T>*>(this)->instance); }
};

template<class T>
struct SheddableTypeCastableImplicit : SheddableType<T> {
   using SheddableType<T>::SheddableType;
   using SheddableType<T>::instance;
   operator T& () noexcept { return LglsFwd(instance); }
   operator T& () const noexcept { return LglsFwd(const_cast<SheddableTypeCastableImplicit<T>*>(this)->instance); }
};

struct SheddableTypeDerived
   : SheddableType<int> {};
struct NonSheddableTypeDerived
   : SheddableType<int> { using CTTI_Sheddable = void; };
struct SheddableTypeDerivedOverride
   : SheddableType<int> { using CTTI_Sheddable = bool; };

/// MARK: Typed                                                               
template<class T>
struct TypedType {
   using CTTI_Typed = T;

   T instance;

   TypedType(Devq<T> t) : instance {LglsFwd(t)} {}
};

template<class T>
struct TypedTypeCastableExplicit : TypedType<T> {
   using TypedType<T>::TypedType;
   using TypedType<T>::instance;
   explicit operator T () noexcept { return LglsFwd(instance); }
   explicit operator T () const noexcept { return LglsFwd(const_cast<TypedTypeCastableExplicit<T>*>(this)->instance); }
};

template<class T>
struct TypedTypeCastableImplicit : TypedType<T> {
   using TypedType<T>::TypedType;
   using TypedType<T>::instance;
   operator T () noexcept { return LglsFwd(instance); }
   operator T () const noexcept { return LglsFwd(const_cast<TypedTypeCastableImplicit<T>*>(this)->instance); }
};

template<class T>
struct TypedTypeCastableUsingMethod : TypedType<T> {
   using TypedType<T>::TypedType;
   using TypedType<T>::instance;
   auto TypedCast()       noexcept -> T&       { return instance; }
   auto TypedCast() const noexcept -> T const& { return instance; }
};

/*struct SheddableTypeDerived //TODO typed equivalents?
   : SheddableType<int> {};
struct NonSheddableTypeDerived
   : SheddableType<int> { using CTTI_Sheddable = void; };
struct SheddableTypeDerivedOverride
   : SheddableType<int> { using CTTI_Sheddable = bool; };*/

struct CustomTypedType { using CTTI_Typed = int; };
struct CustomTypedTypeDerived : CustomTypedType { };
struct CustomUntypedType : CustomTypedType { using CTTI_Typed = void; };
enum TypedEnum : int64_t {one1, two2};
enum class TypedEnumClass : int64_t {one1, two2};

/// MARK: Deep                                                                
struct ForcedDeepExternally {};
struct ForcedDeepInternally {
   using CTTI_Deep = Yup;
};

struct InheritedDeep1 : ForcedDeepInternally {};
struct InheritedDeep1Disabled : ForcedDeepInternally { using CTTI_Deep = No; };
struct InheritedDeep1ButPrivate : private ForcedDeepInternally {};
struct InheritedDeepExternally : ForcedDeepExternally {};

/// Arrays                                                                    
using ArrayType = int[50];
using ArrayType2 = int[50][2];
using ArrayTypeRef = int(&)[50];
using ArrayTypeRef2 = int(&)[50][2];
using PointerType = int*;
using PointerType2 = int**;
struct CustomArrayType {
   using CTTI_Typed = int;
   using CTTI_Array = Yes<56>;
};
struct CustomNonArrayTypeDerived : CustomArrayType { using CTTI_Array = No; };
struct CustomNonArrayType {};

/// Custom pointers                                                           
struct CustomPointerType {
   using CTTI_Sparse = Yup;
   
   int operator * () const noexcept {
      return 666;
   }
};
struct CustomNonPointerType {};

namespace Langulus::CTTI
{
   template<>
   struct ReflectAs<NotReflectableExtern> {
      using Type = void;
   };

   template<>
   struct ReflectAs<ReflectableExtern> {
      using Type = char;
   };

   template<>
   struct Deep<ForcedDeepExternally> {};
}

/// MARK: Tags                                                                
namespace Langulus::Tags
{
   struct Name {
      using CTTI_Versioned = Version<7, 10>;
      using CTTI_DefineTag = NamedTag<"Name">;
      using CTTI_Info      = Yes<"Used for tagging names">;
   };
   struct ConflictingName1 {
      using CTTI_DefineTag = NamedTag<"Name">;
   };
   struct ConflictingName2 {
      using CTTI_DefineTag = NamedTag<"int">;
   };
   struct ConflictingName3 {
      using CTTI_DefineTag = NamedTag<"create">;
   };
   struct ConflictingName4 {
      using CTTI_DefineTag = NamedTag<"Pi::Number">;
   };
}

namespace Langulus::Flow
{
   class Recipe {};
   class Recipeconst {};
   class constRecipeconst {};
   class constconst {};
}

/// MARK: Verbs                                                               
namespace Langulus::Verbs
{
   /// Defines a verb                                                         
   struct Create {
      using CTTI_Versioned    = Version<6, 10>;
      using CTTI_DefineVerb   = NamedVerb<"create", "destroy", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" + ", " - ">;
      using CTTI_Info = Yes<
         "Used for allocating new elements. "
         "If the type you're creating has	a producer, "
         "you need to execute the verb in a matching producer, "
         "or that producer will be created automatically for you, if possible"
      >;
   };

   struct ConflictingByPosToken1 {
      using CTTI_DefineVerb = NamedVerb<"create", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " -- ">;
   };
   struct ConflictingByPosToken2 {
      using CTTI_DefineVerb = NamedVerb<"int", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " -- ">;
   };
   struct ConflictingByPosToken3 {
      using CTTI_DefineVerb = NamedVerb<"name", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " -- ">;
   };

   struct ConflictingByNegToken1 {
      using CTTI_DefineVerb = NamedVerb<"create_alt", "destroy", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " -- ">;
   };
   struct ConflictingByNegToken2 {
      using CTTI_DefineVerb = NamedVerb<"create_alt2", "int", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" +++ ", " --- ">;
   };
   struct ConflictingByNegToken3 {
      using CTTI_DefineVerb = NamedVerb<"create_alt", "name", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " -- ">;
   };

   struct ConflictingByPosOp1 {
      using CTTI_DefineVerb = NamedVerb<"create_alt", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" + ", " -- ">;
   };
   struct ConflictingByPosOp2 {
      using CTTI_DefineVerb = NamedVerb<"create_alt3", "destroy_alt3", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" int ", " ---- ">;
   };
   struct ConflictingByPosOp3 {
      using CTTI_DefineVerb = NamedVerb<"create_alt", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" name ", " -- ">;
   };

   struct ConflictingByNegOp1 {
      using CTTI_DefineVerb = NamedVerb<"create_alt", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " - ">;
   };
   struct ConflictingByNegOp2 {
      using CTTI_DefineVerb = NamedVerb<"create_alt4", "destroy_alt4", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++++ ", " int ">;
   };
   struct ConflictingByNegOp3 {
      using CTTI_DefineVerb = NamedVerb<"create_alt", "destroy_alt", 5.f>;
      using CTTI_DefineVerbOp = NamedOperator<" ++ ", " name ">;
   };
}

namespace Langulus::CTTI
{
   LglsImplementAbilitiesFor(int) {
      using Can = Verbs::Create;

      /// Binary creator                                                      
      static bool Default(int&, Verbs::Create&) {
         //Logger::Special("Verbs::Create binary executed in int");
         return true;
      }

      static bool Default(int const&, Verbs::Create&) {
         //Logger::Special("Verbs::Create binary executed in int");
         return true;
      }

      /// Unary creator                                                       
      static bool Default(Verbs::Create&) {
         //Logger::Special("Verbs::Create unary executed in int");
         return true;
      }
   };

   template<class T>
   concept AnythingWithCreateMethod = requires (T c, Annies::Verb& v) { c.Create(v); };

   LglsImplementAbilitiesForConcept(AnythingWithCreateMethod, T) {
      using Can = Verbs::Create;

      /// Binary creator                                                      
      static bool Default(T&, Verbs::Create&) {
         //instance.Create(verb);
         return true;
      }

      /*static bool Default(T const&, Verbs::Create&) { // const verb disabled, T has to be mutable to have the ability!
         //instance.Create(verb);
         return true;
      }*/
   };
}

struct ImplicitlyReflectedData {
   enum Named { One, Two, Three };

   using CTTI_POD    = Yes<>;
   using CTTI_Files  = Yes<"ase">;
   using CTTI_Typed  = Named;
   using CTTI_Values = Values<One, Two, Three>;

   Named v = One;

   inline bool operator == (const ImplicitlyReflectedData&) const noexcept = default;
};

class alignas(128) ImplicitlyReflectedDataWithTraits : public ImplicitlyReflectedData {
public:
   int member {664};
   Tags::Name anotherMember {};
   int anotherMemberArray [12] {};
   int* sparseMember {};

   inline operator int() const noexcept {
      return member;
   }

   void Create(Langulus::Annies::Verb&) const {} // Should never be reflected because LglsImplementAbilitiesForConcept(AnythingWithCreateMethod, T) requires mutable only

   void Create(Langulus::Annies::Verb&) {
      ++member;
   }

   ImplicitlyReflectedDataWithTraits() = default;
   explicit ImplicitlyReflectedDataWithTraits(Pi)
      : member {314} {}

   using CTTI_Named     = Yes<"MyType">;
   using CTTI_Info      = Yes<"Info about MyType">;
   using CTTI_Files     = Yes<"txt, pdf">;
   using CTTI_Versioned = Version<2, 1>;
   //using CTTI_Deep      = Yes<>;
   using CTTI_Nullable  = Yes<>;
   using CTTI_POD       = No;
   using CTTI_Pooled    = PooledBySize;
   using CTTI_Concrete  = ImplicitlyReflectedData;
   using CTTI_Abstract  = Yes<>;
   using CTTI_Bases     = ImplicitlyReflectedData;
   using CTTI_Ability   = Verbs::Create;
   //using CTTI_MapsTo    = int;
   using CTTI_Values    = No;
   using CTTI_MinAlloc  = Yes<1024>;

   using Self = ImplicitlyReflectedDataWithTraits;
   using CTTI_Members = Members<
      &Self::member,
      &Self::anotherMember,
      &Self::anotherMemberArray,
      &Self::sparseMember
   >;
};
static_assert(not CT::POD<ImplicitlyReflectedDataWithTraits>);

struct ConvertibleData : ImplicitlyReflectedData {
   int member {664};

   inline operator int() const noexcept {
      return member;
   }

   ConvertibleData() = default;
   explicit ConvertibleData(Pi)
      : member {314} {}

   using CTTI_Bases     = ImplicitlyReflectedData;
   using CTTI_Values    = No;
};
static_assert(CT::POD<ConvertibleData>);

struct CheckingWhatConverterGetsInherited : ConvertibleData {
   using CTTI_Named = Yes<"CheckingWhatConverterGetsInherited">;

   using ConvertibleData::ConvertibleData;
};

struct ConflictingName { using CTTI_Named = Yes<"MyType">;   };
struct InvalidName1    { using CTTI_Named = Yes<"1MyType">;  };
struct InvalidName2    { using CTTI_Named = Yes<"MyType{}">; };
struct InvalidName3    { using CTTI_Named = Yes<"My.Type">;  };
struct InvalidName4    { using CTTI_Named = Yes<"MyType[]">; };
struct InvalidName5    { using CTTI_Named = Yes<"MyType,">;  };
struct InvalidName6    { using CTTI_Named = Yes<"My Type">;  };

struct ReservedName1   { using CTTI_Named = Yes<"const">;    };
struct ReservedName2   { using CTTI_Named = Yes<"null">;     };
struct ReservedName3   { using CTTI_Named = Yes<"noverb">;   };
struct ReservedName4   { using CTTI_Named = Yes<"nodata">;   };
struct ReservedName5   { using CTTI_Named = Yes<"novalue">;  };
struct ReservedName6   { using CTTI_Named = Yes<"notag">;    };

struct CheckingWhatGetsInherited : ImplicitlyReflectedDataWithTraits {
   using CTTI_Named = Yes<"CheckingWhatGetsInherited">;
   using ImplicitlyReflectedDataWithTraits::ImplicitlyReflectedDataWithTraits;
};

/*void FunctionForTesting(void*) {
   Logger::Verbose("Executed FunctionForTesting");
}*/

/// MARK: Abstract                                                            
/// Built-in abstract type via a pure virtual function                        
struct PureAbstract {
   PureAbstract() = delete;
   virtual ~PureAbstract() {}
   PureAbstract(void*) {}
   [[maybe_unused]] virtual auto PureVirtualMethod() -> size_t = 0;
};

/// Proper type, reflected as abstract                                        
struct ForcedAbstractExternally {};
struct ForcedAbstractInternally {
   using CTTI_Abstract = Yes<>;
};

/// Types that can inherit abstractness                                       
struct InheritedAbstract1 : ForcedAbstractInternally { };
struct InheritedAbstract1Disabled : ForcedAbstractInternally { using CTTI_Abstract = No; };
struct InheritedAbstract2 : PureAbstract { };

/// Types that can inherit abstractness privately                             
struct ImpureVirtual {
   virtual ~ImpureVirtual() {}
};
struct InheritedAbstract1ButPrivate : private ForcedAbstractInternally {};
struct InheritedAbstract2ButPrivate : private PureAbstract {};
struct InheritedAbstractExternally  : ForcedAbstractExternally {};

/// MARK: Virtual base                                                        
/// Type that has a virtual base                                              
struct VirtuallyDerived : virtual ImpureVirtual {
   using CTTI_Bases = Types<ImpureVirtual, int>;
};

/// Type that has a private non-virtual base                                  
struct PrivatelyDerived : private ImpureVirtual {
   using CTTI_Bases = Types<ImpureVirtual, int, float>;
};

/// MARK: Convertible                                                         
struct ConvertibleToInt {
   ConvertibleToInt(int inner = 666)
      : member{inner} {}

   explicit operator int() const noexcept {
      return member;
   }

private:
   int member;
};

struct BuiltinConvertibleFromIntViaConstructor {
   int inner = 0;
   BuiltinConvertibleFromIntViaConstructor(int x) : inner {x} {}
};

struct BuiltinConvertibleFromIntViaExplicitConstructor {
   int inner = 0;
   explicit BuiltinConvertibleFromIntViaExplicitConstructor(int x) : inner {x} {}
};

struct ConvertibleFromIntExternallyMissingConverter {};
class  ConvertibleFromIntExternally {
   int inner = 0;
public:
   static ConvertibleFromIntExternally Init(int i) { 
      ConvertibleFromIntExternally temp;
      temp.inner = i;
      return temp;
   }
};

/// Types that inherit convertible properties                                 
struct InheritedConvertibleFromInt2
   : ConvertibleFromIntExternally {};
struct InheritedConvertibleFromInt3
   : BuiltinConvertibleFromIntViaConstructor {};
struct InheritedConvertibleFromInt4
   : BuiltinConvertibleFromIntViaExplicitConstructor {};

/// Types that inherit convertible properties privately                       
struct InheritedConvertibleFromInt2ButPrivate : private BuiltinConvertibleFromIntViaConstructor {};
struct InheritedConvertibleFromInt3ButPrivate : private BuiltinConvertibleFromIntViaExplicitConstructor {};
struct InheritedConvertibleFromIntExternally : ConvertibleFromIntExternally {};


///                                                                           
/// Convertible to int                                                        
class BuiltinConvertibleToIntViaOperator {
   int inner = 0;
public:
   operator int() const noexcept {
      return inner;
   }
};

class BuiltinConvertibleToIntViaOperatorMutable {
   int inner = 0;
public:
   operator int() noexcept {
      return inner;
   }
};

class BuiltinConvertibleToIntViaExplicitOperator {
   int inner = 0;
public:
   explicit operator int() const noexcept {
      return inner;
   }
};

class BuiltinConvertibleToIntViaExplicitOperatorMutable {
   int inner = 0;
public:
   explicit operator int() noexcept {
      return inner;
   }
};

struct BuiltinConvertibleToIntBecauseAggregate {
   int inner = 0;
};

struct ConvertibleToIntExternallyMissingConverter {};
static_assert(not std::is_convertible_v<ConvertibleToIntExternallyMissingConverter, int>);

struct ConvertibleToIntExternally {
   ::std::string inner;
};

/// Types that inherit convertible properties                                 
struct InheritedConvertibleToInt2
   : ConvertibleToIntExternally {};
struct InheritedConvertibleToInt3
   : BuiltinConvertibleToIntViaOperator {};
struct InheritedConvertibleToInt4
   : BuiltinConvertibleToIntViaOperatorMutable {};
struct InheritedConvertibleToInt5
   : BuiltinConvertibleToIntViaExplicitOperator {};
struct InheritedConvertibleToInt6
   : BuiltinConvertibleToIntViaExplicitOperatorMutable {};

/// Types that inherit convertible properties privately                       
struct InheritedConvertibleToInt2ButPrivate : private BuiltinConvertibleToIntViaOperator {};
struct InheritedConvertibleToInt3ButPrivate : private BuiltinConvertibleToIntViaExplicitOperator {};
struct InheritedConvertibleToIntExternally : ConvertibleToIntExternally {};

/// MARK: Container                                                           
/// For testing container related concepts                                    
struct CustomContainer {
   using CTTI_Container = Yes<>;
};

/// These customizations need to appear as early as possible, in order        
/// to be consistently reflected in all tests:                                
LANGULUS_MORPHISM_CUSTOM(std::string, { return from == "the devil" ? 666 : -1; }, int);
LANGULUS_MORPHISM_CUSTOM(int, { return TO::Init(from); }, ConvertibleFromIntExternally);
LANGULUS_MORPHISM(Pi, ImplicitlyReflectedDataWithTraits, ConvertibleData);
LANGULUS_MORPHISM(ImplicitlyReflectedDataWithTraits, int);
LANGULUS_MORPHISM(ConvertibleData, int);
LANGULUS_MORPHISM(ConvertibleToInt, int);
LANGULUS_MORPHISM(ConvertibleToIntExternallyMissingConverter, int);
LANGULUS_MORPHISM_CUSTOM(ConvertibleToIntExternally, { return from.inner.size(); }, int);

namespace Langulus::CTTI
{
   template<>
   struct Named<::std::string> : Yes<"string"> {};

   /// Extend bool, by associating couple of constants with it                
   template<>
   struct DefineConstant<Pi> : Types<
      NamedValue<Pi::Number, "Pi::Number", "The number Pi">,
      NamedValue<Pi::ConflictingNumber, "Pi::Number", "Conflicting number">
   > {};

   template<>
   struct Abstract<ForcedAbstractExternally> {};
}

struct AnotherTypeWithSimilarilyNamedValues {
   enum Named {One = 501, Two, Three};
   using CTTI_Values = Values<One, Two, Three>;
   using CTTI_Named  = Yes<"YetAnotherNamedType">;

   int v = One;

   inline bool operator == (const AnotherTypeWithSimilarilyNamedValues&) const noexcept = default;
};

/// MARK: Empty                                                               
struct EmptyType {};
static_assert(::std::is_copy_constructible_v<EmptyType>);
static_assert(::std::is_move_constructible_v<EmptyType>);
static_assert(::std::is_copy_assignable_v<EmptyType>);
static_assert(::std::is_move_assignable_v<EmptyType>);

/// MARK: Aggregate                                                           
struct ActualAggregateType {
   int i;
   float f;
};
static_assert(::std::is_copy_constructible_v<ActualAggregateType>);
static_assert(::std::is_move_constructible_v<ActualAggregateType>);
static_assert(::std::is_copy_assignable_v<ActualAggregateType>);
static_assert(::std::is_move_assignable_v<ActualAggregateType>);

struct CustomAggregateType {
   using CTTI_Aggregate = Yes<>;
   int force_not_aggregate;

   CustomAggregateType()
      : force_not_aggregate(666) {
      --force_not_aggregate;
   }
};
struct AggregateTypeDerived : CustomAggregateType {};
struct NonAggregateTypeDerived : CustomAggregateType {
   using CTTI_Aggregate = No;

   NonAggregateTypeDerived()
      : CustomAggregateType() {
      --force_not_aggregate;
   }
};

/// MARK: Destructible                                                        
/// Explicitly deleted destructor                                             
/// @attention this hits a nasty compiler bug on MSVC v143 when intents       
///   are implicitly cast to built-in move/copy semantics                     
///   They are disabled because of this, as well as other compiler bugs       
///   https://stackoverflow.com/questions/79665049                            
struct NonDestructible {
   ~NonDestructible() = delete;
};
static_assert(    ::std::is_standard_layout_v<NonDestructible>);
static_assert(not ::std::is_copy_constructible_v<NonDestructible>);
static_assert(not ::std::is_move_constructible_v<NonDestructible>);
static_assert(    ::std::is_copy_assignable_v<NonDestructible>);
static_assert(    ::std::is_move_assignable_v<NonDestructible>);

struct DestructibleType {
   char* p {};

   ~DestructibleType() { if (p) delete p; }
};
static_assert(::std::is_copy_constructible_v<DestructibleType>);
static_assert(::std::is_move_constructible_v<DestructibleType>);
static_assert(::std::is_copy_assignable_v<DestructibleType>);
static_assert(::std::is_move_assignable_v<DestructibleType>);

/// MARK: Constructible                                                       
/// Has no explicit intent constructors and assigners                         
struct NonIntentConstructible {
   NonIntentConstructible(CT::NoIntent auto&&) {}
};
static_assert(::std::is_copy_constructible_v<NonIntentConstructible>);
static_assert(::std::is_move_constructible_v<NonIntentConstructible>);
static_assert(::std::is_copy_assignable_v<NonIntentConstructible>);
static_assert(::std::is_move_assignable_v<NonIntentConstructible>);

/// Default-constructible, but only privately                                 
class PrivatelyConstructible {
   PrivatelyConstructible() = default;
   PrivatelyConstructible(PrivatelyConstructible const&) = default;
   PrivatelyConstructible(PrivatelyConstructible&&) = default;
};
static_assert(not ::std::is_copy_constructible_v<PrivatelyConstructible>);
static_assert(not ::std::is_move_constructible_v<PrivatelyConstructible>);
static_assert(not ::std::is_copy_assignable_v<PrivatelyConstructible>);
static_assert(not ::std::is_move_assignable_v<PrivatelyConstructible>);

/// Has explicit copy, move, refer, clone, abandon, disown constructors       
/// Because they're explicit, there are no implicit intent-assigners          
struct PartiallyIntentConstructible {
   template<template<class> class S, class T>
   explicit PartiallyIntentConstructible(S<T>&&) requires CT::Intent<S<T>> {}
};
static_assert(::std::is_copy_constructible_v<PartiallyIntentConstructible>);
static_assert(::std::is_move_constructible_v<PartiallyIntentConstructible>);
static_assert(::std::is_copy_assignable_v<PartiallyIntentConstructible>);
static_assert(::std::is_move_assignable_v<PartiallyIntentConstructible>);

/// Has implicit copy, move, refer, clone, abandon, disown constructors       
/// Because they're implicit, the type should also have all intent-assigs     
///   @attention this hits a lot of compiler bugs on different compilers:     
///   - it causes ambiguity on Clang 19.1 for refer intents, because          
///     the compiler can't decide whether to implicit-cast to && or           
///     const&                                                                
///   - it causes ambiguity on GCC 14.2 for move/abandon intents, because     
///     the compiler can't decide how to implicit-cast to && or               
///     const&                                                                
///   @note implicit coversion of intents has been disabled to cope           
struct PartiallyIntentConstructibleButImplicitly {
   template<template<class> class S, class T>
   PartiallyIntentConstructibleButImplicitly(S<T>&&) requires CT::Intent<S<T>> {}
};
static_assert(::std::is_copy_constructible_v<PartiallyIntentConstructibleButImplicitly>);
static_assert(::std::is_move_constructible_v<PartiallyIntentConstructibleButImplicitly>);
static_assert(::std::is_copy_assignable_v<PartiallyIntentConstructibleButImplicitly>);
static_assert(::std::is_move_assignable_v<PartiallyIntentConstructibleButImplicitly>);

/// Has all intent constructors                                               
/// Making constructor explicit makes sure, that no implicit intent assign    
/// happens                                                                   
struct AllIntentConstructible {
   explicit AllIntentConstructible(CT::Intent auto&&) {}
};
   
/// Has all intent constructors                                               
/// Making constructor implicit also allows for implicit intent assignments   
struct AllIntentConstructibleImplicit {
   AllIntentConstructibleImplicit(CT::Intent auto&&) {}
};

/// Has all intent constructors and assigners                                 
struct AllIntentConstructibleAndAssignable {
   AllIntentConstructibleAndAssignable(CT::Intent auto&&) {}
   AllIntentConstructibleAndAssignable& operator = (CT::Intent auto&&) { return *this; }
};

/// MARK: Complex                                                             
/// Implicit assignment is disabled due to custom copy/move constructors      
struct alignas(128) Complex {
   int  member;
   bool anotherMember {};
   int  anotherMemberArray [12] {};
   int* sparseMember {};

   Complex(const Complex& s) : member(s.member) {}
   Complex(Complex&& s) : member(s.member) {}
   Complex(int stuff) : member(stuff) {}

   ~Complex() {
      if (sparseMember) delete sparseMember;
   }
};
static_assert(    ::std::is_copy_constructible_v<Complex>);
static_assert(    ::std::is_move_constructible_v<Complex>);
static_assert(not ::std::is_copy_assignable_v<Complex>);
static_assert(not ::std::is_move_assignable_v<Complex>);

/// MARK: DescribeConstructible                                               
struct DescribeConstructible {
   int  member;
   bool anotherMember {};
   int  anotherMemberArray [12] {};
   int* sparseMember {};

   DescribeConstructible(Describe&&) {}
};


class ContainsComplex {
   Complex mData;
};

/// A complex aggregate type                                                  
struct AggregateTypeComplex {
   int m1, m2, m3, m4;
   bool m5;
   Complex mData;
};
   
/// Constructible but not assignable                                          
struct ReferConstructibleButNotAssignable {
   int m;
   explicit ReferConstructibleButNotAssignable(const ReferConstructibleButNotAssignable& a) : m {a.m} {}
   ReferConstructibleButNotAssignable(Refer<ReferConstructibleButNotAssignable>&& a) : m {a->m} {}
   ReferConstructibleButNotAssignable& operator = (ReferConstructibleButNotAssignable const&) = delete;
   ReferConstructibleButNotAssignable& operator = (ReferConstructibleButNotAssignable&&) = delete;
};
static_assert(not ::std::is_trivially_copy_constructible_v<ReferConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_constructible_v<ReferConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_copy_assignable_v<ReferConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_assignable_v<ReferConstructibleButNotAssignable>);

struct CopyConstructibleButNotAssignable {
   int m;
   CopyConstructibleButNotAssignable(Copy<CopyConstructibleButNotAssignable>&& a) : m {a->m} {}
   CopyConstructibleButNotAssignable& operator = (CopyConstructibleButNotAssignable const&) = delete;
   CopyConstructibleButNotAssignable& operator = (CopyConstructibleButNotAssignable&&) = delete;
};
static_assert(not ::std::is_trivially_copy_constructible_v<CopyConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_constructible_v<CopyConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_copy_assignable_v<CopyConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_assignable_v<CopyConstructibleButNotAssignable>);

struct MoveConstructibleButNotAssignable {
   int m;
   explicit MoveConstructibleButNotAssignable(MoveConstructibleButNotAssignable&& a) : m {a.m} {}
   MoveConstructibleButNotAssignable(Move<MoveConstructibleButNotAssignable>&& a) : m {a->m} {}
   MoveConstructibleButNotAssignable& operator = (MoveConstructibleButNotAssignable const&) = delete;
   MoveConstructibleButNotAssignable& operator = (MoveConstructibleButNotAssignable&&) = delete;
};
static_assert(not ::std::is_trivially_copy_constructible_v<MoveConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_constructible_v<MoveConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_copy_assignable_v<MoveConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_assignable_v<MoveConstructibleButNotAssignable>);

struct AbandonConstructibleButNotAssignable {
   int m;
   AbandonConstructibleButNotAssignable(Abandon<AbandonConstructibleButNotAssignable>&& a) : m {a->m} {}
   AbandonConstructibleButNotAssignable& operator = (AbandonConstructibleButNotAssignable const&) = delete;
   AbandonConstructibleButNotAssignable& operator = (AbandonConstructibleButNotAssignable&&) = delete;
};
static_assert(not ::std::is_trivially_copy_constructible_v<AbandonConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_constructible_v<AbandonConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_copy_assignable_v<AbandonConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_assignable_v<AbandonConstructibleButNotAssignable>);

struct DisownConstructibleButNotAssignable {
   int m;
   DisownConstructibleButNotAssignable(Disown<DisownConstructibleButNotAssignable>&& a) : m {a->m} {}
   DisownConstructibleButNotAssignable& operator = (DisownConstructibleButNotAssignable const&) = delete;
   DisownConstructibleButNotAssignable& operator = (DisownConstructibleButNotAssignable&&) = delete;
};
static_assert(not ::std::is_trivially_copy_constructible_v<DisownConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_constructible_v<DisownConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_copy_assignable_v<DisownConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_assignable_v<DisownConstructibleButNotAssignable>);

struct CloneConstructibleButNotAssignable {
   int m;
   CloneConstructibleButNotAssignable(Clone<CloneConstructibleButNotAssignable>&& a) : m {a->m} {}
   CloneConstructibleButNotAssignable& operator = (CloneConstructibleButNotAssignable const&) = delete;
   CloneConstructibleButNotAssignable& operator = (CloneConstructibleButNotAssignable&&) = delete;
};
static_assert(not ::std::is_trivially_copy_constructible_v<CloneConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_constructible_v<CloneConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_copy_assignable_v<CloneConstructibleButNotAssignable>);
static_assert(not ::std::is_trivially_move_assignable_v<CloneConstructibleButNotAssignable>);

/// MARK: Assignable                                                          
/// Assignable but not constructible                                          
struct ReferAssignableButNotConstructible {
   int m;
   ReferAssignableButNotConstructible& operator = (Refer<ReferAssignableButNotConstructible>&& a) {
      m = a->m;
      return *this;
   }
};
static_assert(::std::is_assignable_v<ReferAssignableButNotConstructible, Refer<ReferAssignableButNotConstructible>>);

struct CopyAssignableButNotConstructible {
   int m;
   CopyAssignableButNotConstructible& operator = (Copy<CopyAssignableButNotConstructible>&& a) {
      m = a->m;
      return *this;
   }
};
static_assert(::std::is_assignable_v<CopyAssignableButNotConstructible, Copy<CopyAssignableButNotConstructible>>);

struct MoveAssignableButNotConstructible {
   int m;
   MoveAssignableButNotConstructible& operator = (Move<MoveAssignableButNotConstructible>&& a) {
      m = a->m;
      return *this;
   }
};
static_assert(::std::is_assignable_v<MoveAssignableButNotConstructible, Move<MoveAssignableButNotConstructible>>);

struct AbandonAssignableButNotConstructible {
   int m;
   AbandonAssignableButNotConstructible& operator = (Abandon<AbandonAssignableButNotConstructible>&& a) {
      m = a->m;
      return *this;
   }
};
static_assert(::std::is_assignable_v<AbandonAssignableButNotConstructible, Abandon<AbandonAssignableButNotConstructible>>);

struct DisownAssignableButNotConstructible {
   int m;
   DisownAssignableButNotConstructible& operator = (Disown<DisownAssignableButNotConstructible>&& a) {
      m = a->m;
      return *this;
   }
};
static_assert(::std::is_assignable_v<DisownAssignableButNotConstructible, Disown<DisownAssignableButNotConstructible>>);

struct CloneAssignableButNotConstructible {
   int m;
   CloneAssignableButNotConstructible& operator = (Clone<CloneAssignableButNotConstructible>&& a) {
      m = a->m;
      return *this;
   }
};
static_assert(::std::is_assignable_v<CloneAssignableButNotConstructible, Clone<CloneAssignableButNotConstructible>>);

/// MARK: POD                                                                 
struct ForcefullyPod {
   using CTTI_POD = Yes<>;
   Complex mData;
};
static_assert(    ::std::is_copy_constructible_v<ForcefullyPod>);
static_assert(    ::std::is_move_constructible_v<ForcefullyPod>);
static_assert(not ::std::is_copy_assignable_v<ForcefullyPod>); // not available due to missing in mData (implicitly deleted because of custom constructor)
static_assert(not ::std::is_move_assignable_v<ForcefullyPod>); // not available due to missing in mData (implicitly deleted because of custom constructor)


namespace One::Two::Three
{
   struct TypeDeepIntoNamespaces;

   template<class T>
   struct TemplatedTypeDeepIntoNamespaces {
      enum VeryDeeplyTemplatedEnum { YesYouGotThatRight };

      template<class MORE>
      struct Nested;
   };

   template<class T>
   struct VeryComplexTemplate;
}

struct NamedUsingMember {
   using CTTI_Named = Yes<"NameOverrideUsingMember">;
};
struct NamedBySpecialization {};

struct Nаsty {
   int this_type_name_contains_a_cyrillic_letter;
};

enum PiNonClass {
   Number = 314
};

enum {
   AnonymousNumber = 314
};

enum class PiButNamed {
   Number = 314
};

enum PiNonClassButNamed {
   NumberButNamed = 314
};

enum {
   AnonymousNumberButNamed = 314,
   AnonymousNumberButNotNamed = 315
};

using TypeDeepAlias             = One::Two::Three::TypeDeepIntoNamespaces;
using TemplatedAlias            = One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>;
using VeryComplexTemplatedAlias = One::Two::Three::VeryComplexTemplate<
   One::Two::Three::TemplatedTypeDeepIntoNamespaces<uint16_t>
>;

class A; class B; class C; class D; class E; class F; class G; class H; class I;
class J; class K; class L; class M; class N; class O; class P; class Q; class R;
class S; class T; class U; class V; class W; class X; class Y; class Z; class _;

class a; class b; class c; class d; class e; class f; class g; class h; class i;
class j; class k; class l; class m; class n; class o; class p; class q; class r;
class s; class t; class u; class v; class w; class x; class y; class z;

struct s_struct; struct t_struct; struct u_struct; struct v_struct; struct w_struct; struct x_struct; struct y_struct; struct z_struct;

using Signature = void(*)(void*);

namespace Langulus::CTTI
{
   template<>
   struct Named<NamedBySpecialization>
      : Yes<"NameOverridedBySpecializing_CTTI_Named"> {};

   template<>
   struct Named<NamedBySpecialization*>
      : Yes<"NameOverridedBySpecializing_CTTI_Named_Ptr"> {};

   template<>
   struct Named<NamedBySpecialization const*> 
      : Yes<"NameOverridedBySpecializing_CTTI_Named_ConstPtr"> {};

   template<>
   struct DefineConstant<PiNonClass> : Values<PiNonClass::Number> {};

   template<>
   struct DefineConstant<PiButNamed>
      : NamedValue<PiButNamed::Number, "PiButNamedByCTTI"> {};

   template<>
   struct DefineConstant<PiNonClassButNamed>
      : NamedValue<NumberButNamed, "NumberButNamedByCTTI"> {};

   template<>
   struct DefineConstant<decltype(AnonymousNumberButNamed)>
      : NamedValue<AnonymousNumberButNamed, "AnonymousNumberButNamedByCTTI"> {};
}

namespace N1 {
   struct EmptyType {};
   struct Create {};
}

namespace N2 {
   struct EmptyType {};
}

namespace N3 {
   struct emptytype {};
}
