///                                                                           
/// Langulus::Tester                                                          
/// Copyright (c) 2025 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/Core.hpp>
#include <Langulus/Except.hpp>

/// Doctest begins its life here                                              
#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#include <doctest/doctest.h>

REGISTER_EXCEPTION_TRANSLATOR(::Langulus::Exception& e) {
   #if LANGULUS(DEBUG)
      return e.mMessage;
   #else
      (void)e;
      return ::Langulus::Exception::DefaultMessage;
   #endif
}

/*namespace doctest
{
   template<>
   struct StringMaker<::Langulus::RTTI::DMeta> {
      static String convert(::Langulus::RTTI::DMeta const& value) {
         return toString(static_cast<::std::string>(value.GetName()));
      }
   };

   template<>
   struct StringMaker<::Langulus::pot_t> {
      static String convert(::Langulus::pot_t const& value) {
         return toString(static_cast<size_t>(value));
      }
   };
}*/

#if LANGULUS(BENCHMARK)
   #include <Langulus/Profiler.hpp>

   constexpr int BenchmarkWarmupCycles  =  100;
   constexpr int BenchmarkMeasureCycles = 1000;
#endif

#if LANGULUS(BENCHMARK)
   #define prevent_optimization(a)
#else
   #define prevent_optimization(a) volatile auto a##_optimization_prevention = a; (void) a##_optimization_prevention;
#endif