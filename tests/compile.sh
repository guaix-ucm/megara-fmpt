#g++ test1.cpp gtest.cpp -I include -L lib -lcppunit -o test1
g++ main_testScalars.cpp testScalars.cpp -I include -L lib -lcppunit -o main_testScalars
g++ main_testFileMethods.cpp testFileMethods.cpp -I include -L lib -lcppunit -o main_testFileMethods

