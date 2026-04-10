#!/usr/bin/env python3
"""
Build System Test Suite
Tests CMake configuration, compilation, and test infrastructure
"""

import os
import sys
import shutil
import subprocess
import platform
from pathlib import Path

# Colors for terminal output
class Colors:
    GREEN = '\033[0;32m'
    RED = '\033[0;31m'
    YELLOW = '\033[1;33m'
    NC = '\033[0m'  # No Color

class BuildSystemTester:
    def __init__(self):
        self.script_dir = Path(__file__).parent
        self.project_root = self.script_dir.parent.parent
        self.test_build_dir = self.project_root / "build_test_temp"
        
        self.tests_run = 0
        self.tests_passed = 0
        self.tests_failed = 0
        
    def cleanup(self):
        """Remove test build directory"""
        if self.test_build_dir.exists():
            shutil.rmtree(self.test_build_dir)
    
    def pass_test(self, message):
        """Record passing test"""
        print(f"{Colors.GREEN}✓ PASS{Colors.NC}: {message}")
        self.tests_passed += 1
        self.tests_run += 1
    
    def fail_test(self, message, reason=""):
        """Record failing test"""
        print(f"{Colors.RED}✗ FAIL{Colors.NC}: {message}")
        if reason:
            print(f"  {Colors.RED}Reason: {reason}{Colors.NC}")
        self.tests_failed += 1
        self.tests_run += 1
    
    def test_header(self, message):
        """Print test section header"""
        print(f"\n{Colors.YELLOW}{'='*50}{Colors.NC}")
        print(f"{Colors.YELLOW}TEST: {message}{Colors.NC}")
        print(f"{Colors.YELLOW}{'='*50}{Colors.NC}")
    
    def run_command(self, cmd, cwd=None, capture_output=True):
        """Run a shell command and return success status"""
        try:
            result = subprocess.run(
                cmd,
                cwd=cwd or self.project_root,
                capture_output=capture_output,
                text=True,
                shell=False
            )
            return result.returncode == 0, result.stdout, result.stderr
        except Exception as e:
            return False, "", str(e)
    
    def test_cmake_configuration(self):
        """Test CMake configuration"""
        self.test_header("CMake Configuration")
        
        cmd = ["cmake", "-B", str(self.test_build_dir), "-S", str(self.project_root)]
        success, stdout, stderr = self.run_command(cmd)
        
        if success:
            self.pass_test("CMake configuration succeeds")
        else:
            self.fail_test("CMake configuration", f"CMake failed: {stderr}")
    
    def test_cmakelists_exists(self):
        """Test CMakeLists.txt file exists"""
        self.test_header("CMakeLists.txt Presence")
        
        cmake_file = self.project_root / "CMakeLists.txt"
        if cmake_file.exists():
            self.pass_test("CMakeLists.txt exists")
        else:
            self.fail_test("CMakeLists.txt", "File not found")
    
    def test_build_directory_structure(self):
        """Test build directory is created correctly"""
        self.test_header("Build Directory Structure")
        
        if self.test_build_dir.exists():
            self.pass_test("Build directory created")
        else:
            self.fail_test("Build directory", "Not created")
        
        cache_file = self.test_build_dir / "CMakeCache.txt"
        if cache_file.exists():
            self.pass_test("CMakeCache.txt generated")
        else:
            self.fail_test("CMakeCache.txt", "Not generated")
    
    def test_compilation(self):
        """Test project compilation"""
        self.test_header("Project Compilation")
        
        cmd = ["cmake", "--build", str(self.test_build_dir), "--config", "Debug"]
        success, stdout, stderr = self.run_command(cmd)
        
        if success:
            self.pass_test("Project compiles successfully")
        else:
            self.fail_test("Compilation", f"Build failed: {stderr}")
    
    def test_executable_generation(self):
        """Test that test executable is generated"""
        self.test_header("Test Executable Generation")
        
        # Check multiple possible locations
        exe_name = "HybridRenderer_unit_tests"
        if platform.system() == "Windows":
            exe_name += ".exe"
        
        possible_locations = [
            self.test_build_dir / "bin" / "Debug" / exe_name,
            self.test_build_dir / "bin" / exe_name,
            self.test_build_dir / "Debug" / exe_name,
            self.test_build_dir / exe_name,
        ]
        
        exe_found = False
        exe_path = None
        for location in possible_locations:
            if location.exists():
                exe_found = True
                exe_path = location
                break
        
        if exe_found:
            self.pass_test(f"Test executable generated at: {exe_path}")
            self.test_exe_path = exe_path
        else:
            self.fail_test("Test executable", "Not found in expected locations")
            self.test_exe_path = None
    
    def test_asset_deployment(self):
        """Test that test assets are deployed"""
        self.test_header("Test Assets Deployment")
        
        if not hasattr(self, 'test_exe_path') or not self.test_exe_path:
            self.fail_test("Asset deployment", "Cannot check - executable not found")
            return
        
        exe_dir = self.test_exe_path.parent
        assets_dir = exe_dir / "test_assets"
        
        if assets_dir.exists():
            test_file = assets_dir / "test_256x256.png"
            if test_file.exists():
                self.pass_test("Test assets deployed correctly")
            else:
                self.fail_test("Asset deployment", "test_assets exists but files missing")
        else:
            self.fail_test("Asset deployment", f"test_assets not found in {exe_dir}")
    
    def test_execution(self):
        """Test that unit tests execute"""
        self.test_header("Test Suite Execution")
        
        if not hasattr(self, 'test_exe_path') or not self.test_exe_path:
            self.fail_test("Test execution", "Cannot execute - executable not found")
            return
        
        cmd = [str(self.test_exe_path), "--gtest_brief=1"]
        success, stdout, stderr = self.run_command(cmd)
        
        if success:
            self.pass_test("All unit tests pass")
        else:
            self.fail_test("Test execution", "Some tests failed")
    
    def test_clean_build(self):
        """Test clean build process"""
        self.test_header("Clean Build Test")
        
        # Clean up
        self.cleanup()
        
        # Configure
        cmd1 = ["cmake", "-B", str(self.test_build_dir), "-S", str(self.project_root)]
        success1, _, _ = self.run_command(cmd1)
        
        # Build
        cmd2 = ["cmake", "--build", str(self.test_build_dir)]
        success2, _, _ = self.run_command(cmd2)
        
        if success1 and success2:
            self.pass_test("Clean build succeeds")
        else:
            self.fail_test("Clean build", "Failed to build from clean state")
    
    def test_required_files(self):
        """Test that required build system files exist"""
        self.test_header("Required Build Files")
        
        required_files = [
            "CMakeLists.txt",
            ".github/workflows/unit-tests.yml",
        ]
        
        for file in required_files:
            file_path = self.project_root / file
            if file_path.exists():
                self.pass_test(f"Required file exists: {file}")
            else:
                self.fail_test(f"Required file: {file}", "Not found")
    
    def run_all_tests(self):
        """Run all build system tests"""
        print("Build System Test Suite")
        print(f"Project Root: {self.project_root}\n")
        
        # Cleanup before starting
        self.cleanup()
        
        # Run tests in order
        self.test_cmakelists_exists()
        self.test_cmake_configuration()
        self.test_build_directory_structure()
        self.test_compilation()
        self.test_executable_generation()
        self.test_asset_deployment()
        self.test_execution()
        self.test_clean_build()
        self.test_required_files()
        
        # Print summary
        print(f"\n{Colors.YELLOW}{'='*50}{Colors.NC}")
        print(f"{Colors.YELLOW}BUILD SYSTEM TEST SUMMARY{Colors.NC}")
        print(f"{Colors.YELLOW}{'='*50}{Colors.NC}")
        print(f"Tests Run:    {self.tests_run}")
        print(f"{Colors.GREEN}Passed:       {self.tests_passed}{Colors.NC}")
        
        if self.tests_failed > 0:
            print(f"{Colors.RED}Failed:       {self.tests_failed}{Colors.NC}")
        else:
            print(f"Failed:       {self.tests_failed}")
        
        print()
        
        # Cleanup
        self.cleanup()
        
        # Return exit code
        if self.tests_failed > 0:
            print(f"{Colors.RED}Build system tests FAILED{Colors.NC}")
            return 1
        else:
            print(f"{Colors.GREEN}All build system tests PASSED{Colors.NC}")
            return 0

if __name__ == "__main__":
    tester = BuildSystemTester()
    exit_code = tester.run_all_tests()
    sys.exit(exit_code)