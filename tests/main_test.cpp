#include <gtest/gtest.h>
#include <iostream>

// Custom event listener for cleaner output
class CleanTestListener : public ::testing::EmptyTestEventListener {
private:
    int passedTests = 0;
    int failedTests = 0;
    
public:
    void OnTestProgramStart(const ::testing::UnitTest& unit_test) override {
        std::cout << "\n";
        std::cout << "+========================================================+\n";
        std::cout << "||          Hybrid Renderer - Test Suite                  \n";
        std::cout << "+========================================================+\n";
        std::cout << "+========================================================+\n";
        std::cout << "\n";
    }
    
    void OnTestStart(const ::testing::TestInfo& test_info) override {
        std::cout << "[ RUN      ] " << test_info.test_suite_name() 
                  << "." << test_info.name() << std::endl;
    }
    
    void OnTestEnd(const ::testing::TestInfo& test_info) override {
        if (test_info.result()->Passed()) {
            std::cout << "\033[92m[       OK ]\033[0m " 
                      << test_info.test_suite_name() << "." << test_info.name()
                      << " (" << test_info.result()->elapsed_time() << " ms)\n";
            passedTests++;
        } else {
            std::cout << "\033[91m[  FAILED  ]\033[0m " 
                      << test_info.test_suite_name() << "." << test_info.name()
                      << " (" << test_info.result()->elapsed_time() << " ms)\n";
            failedTests++;
        }
    }
    
    void OnTestProgramEnd(const ::testing::UnitTest& unit_test) override {
        std::cout << "\n";
        std::cout << "+========================================================+\n";
        std::cout << "||                    Test Summary                        \n";
        std::cout << "+========================================================+\n";
        
        if (failedTests == 0) {
            std::cout << "||  \033[92mStatus: ALL TESTS PASSED\033[0m                            \n";
        } else {
            std::cout << "||  \033[91mStatus: SOME TESTS FAILED\033[0m                          \n";
        }
        
        std::cout << "||                                                        ||\n";
        std::cout << "||  Total:   " << (passedTests + failedTests) << " tests" 
                  << std::string(40 - std::to_string(passedTests + failedTests).length(), ' ') << "||\n";
        std::cout << "||  \033[92mPassed:  " << passedTests << " tests\033[0m" 
                  << std::string(39 - std::to_string(passedTests).length(), ' ') << "||\n";
        
        if (failedTests > 0) {
            std::cout << "||  \033[91mFailed:  " << failedTests << " tests\033[0m" 
                      << std::string(39 - std::to_string(failedTests).length(), ' ') << "||\n";
        }
        
        std::cout << "||                                                        ||\n";
        std::cout << "||  Time:    " << unit_test.elapsed_time() << " ms" 
                  << std::string(38 - std::to_string(unit_test.elapsed_time()).length(), ' ') << "||\n";
        std::cout << "+========================================================+\n";
        std::cout << "\n";
    }
};

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    // Remove default listener
    ::testing::TestEventListeners& listeners = 
        ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    
    // Add custom listener
    listeners.Append(new CleanTestListener);
    
    return RUN_ALL_TESTS();
}