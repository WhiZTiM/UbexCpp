/*
 * Copyright(C):    WhiZTiM, 2015
 *
 * This file is part of the TIML::UBEX C++14 library
 *
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 * Author: Ibrahim Timothy Onogu
 * Email:  ionogu@acm.org
 */

#include <chrono>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include "include/value.hpp"
#include "include/stream_reader.hpp"
#include "include/stream_writer.hpp"
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

using namespace std;
using namespace timl;

void tst();

int main()
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();

    cout << "Test time:" << (duration_cast<milliseconds>(high_resolution_clock::now() - start ).count()) << " milliseconds\n";

    extern int weird_cppunit_extern_bug_value_construction_test;    weird_cppunit_extern_bug_value_construction_test = 1;
    extern int weird_cppunit_extern_bug_value_conversion_test;      weird_cppunit_extern_bug_value_conversion_test = 1;
    extern int weird_cppunit_extern_bug_value_map_and_array_test;   weird_cppunit_extern_bug_value_map_and_array_test = 1;
    extern int weird_cppunit_extern_bug_value_iterator_test;        weird_cppunit_extern_bug_value_iterator_test = 1;

    std::ifstream ff;
    ff.open("sample1.ubex", ios::binary);
    if(ff.is_open())
        cout << "Opened" << endl;
    OstreamReader reader(ff);
    auto val = reader.getNextValue();
    cout <<"\tValue: " << val["int8"].asString() << endl;

    tst();
    return 0;
}

void tst()
{
    Value v1;
    v1["name"] = "Ibrahim";
    v1["surname"] = "Onogu";
    v1["country"] = "NG";
    v1["faves"] = {453, -34, '@', true, v1, "So damn funny"};
    v1["arrays"] = {v1, v1, v1};

    cout << to_ostream(v1) << endl;

    std::ofstream file;
    file.open("tst.ubex", ios::binary);

    StreamWriter<decltype(file)> writer(file);
    auto result = writer.writeValue(v1);

    if(result.second)
        cout << "Successfully ";
    cout << "wrote " << result.first << " bytes" << endl;
}
