cmake_minimum_required(VERSION 3.29)
project(spell_corrector)

set(CMAKE_CXX_STANDARD 20)

add_executable(spell_corrector main.cpp
        SpellingCorrector.cpp
        SpellingCorrector.h
        Pinyin.cpp
        Pinyin.h
        SpellingCorrectorForZhCN.cpp
        SpellingCorrectorForZhCN.h
)

target_compile_options(spell_corrector PRIVATE -O3)
