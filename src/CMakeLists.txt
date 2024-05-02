cmake_minimum_required(VERSION 3.25)
project(TailoredSketch_Code)

set(CMAKE_CXX_STANDARD 17)

add_executable(TailoredSketch_Code main.cpp Sketch.h CM_Sketch.h MurmurHash.h params.h CU_Sketch.h CountLess.h StingyC.h StingyCM.h StingyCM_Base.h StingyCM_Sample.h StingyCU.h StingyCU_Base.h Tailored.h O_Tailored.h Choose_Ske.h C_Sketch.h)
