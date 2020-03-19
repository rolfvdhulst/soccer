mkdir build
cd build
cmake -DCODE_COVERAGE=ON ..
make
rm -rf base.info coverage.info total.info
lcov --directory src/ --capture -i --output-file base.info
./test/allTests
lcov --directory . --capture --rc lcov_branch_coverage=1 --output-file coverage.info
lcov -a base.info -a coverage.info --output-file total.info
lcov --remove total.info '**/soccer/test/*' '/usr/*'  '**/soccer/build/src/protobuf/*' "${HOME}" --output-file total.info --rc lcov_branch_coverage=1
cd ..
rm -rf coverage
mkdir coverage
genhtml build/total.info --output-directory coverage
firefox ./coverage/index.html && disown
# This script builds all tests, does coverage on them and opens a webbrowser page with the coverage results in it
