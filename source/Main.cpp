///                                                                           
/// Langulus::Tester                                                          
/// Copyright (c) 2025 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "Main.hpp"
#include <Langulus/Logger.hpp>
#include <filesystem>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>


int main(int argc, char* argv[]) {
   // Duplicate any logging messages to an external HTML file           
   std::filesystem::path filename = argv[0]; filename += ".htm";
   Logger::ToHTML logFile {filename.filename().string()};
   Logger::AttachDuplicator(&logFile);

   Catch::Session session;
   return session.run(argc, argv);
}
