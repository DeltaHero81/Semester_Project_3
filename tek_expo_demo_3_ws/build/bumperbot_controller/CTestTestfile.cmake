# CMake generated Testfile for 
# Source directory: /home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller
# Build directory: /home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_load_mecanum_drive_controller "/usr/bin/python3" "-u" "/opt/ros/jazzy/share/ament_cmake_test/cmake/run_test.py" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_load_mecanum_drive_controller.gtest.xml" "--package-name" "bumperbot_controller" "--output-file" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/ament_cmake_gmock/test_load_mecanum_drive_controller.txt" "--command" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_load_mecanum_drive_controller" "--gtest_output=xml:/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_load_mecanum_drive_controller.gtest.xml")
set_tests_properties(test_load_mecanum_drive_controller PROPERTIES  LABELS "gmock" REQUIRED_FILES "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_load_mecanum_drive_controller" TIMEOUT "60" WORKING_DIRECTORY "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller" _BACKTRACE_TRIPLES "/opt/ros/jazzy/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/jazzy/share/ament_cmake_gmock/cmake/ament_add_gmock_test.cmake;98;ament_add_test;/opt/ros/jazzy/share/ament_cmake_gmock/cmake/ament_add_gmock.cmake;90;ament_add_gmock_test;/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/CMakeLists.txt;93;ament_add_gmock;/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/CMakeLists.txt;0;")
add_test(test_mecanum_drive_controller "/usr/bin/python3" "-u" "/opt/ros/jazzy/share/ament_cmake_test/cmake/run_test.py" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_mecanum_drive_controller.gtest.xml" "--package-name" "bumperbot_controller" "--output-file" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_mecanum_drive_controller.txt" "--command" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_mecanum_drive_controller" "--ros-args" "--params-file" "/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/test/mecanum_drive_controller_params.yaml" "--" "--gtest_output=xml:/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_mecanum_drive_controller.gtest.xml")
set_tests_properties(test_mecanum_drive_controller PROPERTIES  TIMEOUT "60" WORKING_DIRECTORY "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller" _BACKTRACE_TRIPLES "/opt/ros/jazzy/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/jazzy/share/generate_parameter_library/cmake/generate_parameter_library.cmake;161;ament_add_test;/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/CMakeLists.txt;100;add_rostest_with_parameters_gmock;/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/CMakeLists.txt;0;")
add_test(test_mecanum_drive_controller_preceeding "/usr/bin/python3" "-u" "/opt/ros/jazzy/share/ament_cmake_test/cmake/run_test.py" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_mecanum_drive_controller_preceeding.gtest.xml" "--package-name" "bumperbot_controller" "--output-file" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_mecanum_drive_controller_preceeding.txt" "--command" "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_mecanum_drive_controller_preceeding" "--ros-args" "--params-file" "/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/test/mecanum_drive_controller_preceeding_params.yaml" "--" "--gtest_output=xml:/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller/test_results/bumperbot_controller/test_mecanum_drive_controller_preceeding.gtest.xml")
set_tests_properties(test_mecanum_drive_controller_preceeding PROPERTIES  TIMEOUT "60" WORKING_DIRECTORY "/home/papalist/tek_expo_demo_3_ws/build/bumperbot_controller" _BACKTRACE_TRIPLES "/opt/ros/jazzy/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/jazzy/share/generate_parameter_library/cmake/generate_parameter_library.cmake;161;ament_add_test;/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/CMakeLists.txt;111;add_rostest_with_parameters_gmock;/home/papalist/tek_expo_demo_3_ws/src/bumperbot_controller/CMakeLists.txt;0;")
subdirs("gmock")
subdirs("gtest")