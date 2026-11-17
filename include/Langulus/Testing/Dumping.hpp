///                                                                           
/// Langulus::Tester                                                          
/// Copyright (c) 2025 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include <Langulus/Logger.hpp>

/// Dump parse results and requirements                                       
template<class INPUT, class OUTPUT, class REQUIRED>
void DumpResults(const INPUT& in, const OUTPUT& out, const REQUIRED& required) {
   Langulus::Logger::Special("-------------");
   Langulus::Logger::Special("Script:   ", in);
   Langulus::Logger::Special("Parsed:   ", out);
   Langulus::Logger::Special("Required: ", required);
   Langulus::Logger::Special("-------------");
}