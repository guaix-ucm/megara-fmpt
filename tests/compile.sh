#g++ test1.cpp gtest.cpp -I include -L lib -lcppunit -o test1
#g++ main_test_vclemu.cpp test_vclemu.cpp -I include -L lib -lcppunit -o main_test_vclemu
g++ main_testScalars.cpp testScalars.cpp -I include -L lib -lcppunit -o main_testScalars
g++ main_testSlideArray.cpp testSlideArray.cpp -I include -L lib -lcppunit -o main_testSlideArray
g++ main_testFileMethods.cpp testFileMethods.cpp -I include -L lib -lcppunit -o main_testFileMethods

