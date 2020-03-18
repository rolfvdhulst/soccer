mkdir build
cd build
cmake -DCODE_COVERAGE=ON ..
make
./test/allTests
lcov --directory . --capture --rc lcov_branch_coverage=1 --output-file coverage.info
lcov --remove coverage.info '**/soccer/test/*' '/usr/*'  '**/soccer/build/src/protobuf/*' "${HOME}" --output-file coverage.info --rc lcov_branch_coverage=1
cd ..
rm -rf coverage
mkdir coverage
genhtml build/coverage.info --output-directory coverage
firefox ./coverage/index.html && disown
# This script builds all tests, does coverage on them and opens a webbrowser page with the coverage results in it
