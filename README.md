This repository an implementation of an AI for an ssl-soccer team to play soccer.

## Build status

###### Master    
[![Build Status](https://travis-ci.com/rolfvdhulst/soccer.svg?branch=master)](https://travis-ci.com/rolfvdhulst/soccer)
[![codecov](https://codecov.io/gh/RoboTeamTwente/roboteam_ai/branch/master/graph/badge.svg)](https://codecov.io/gh/rolfvdhulst/soccer)

###### Development    
[![Build Status](https://travis-ci.com/rolfvdhulst/soccer.svg?branch=development)](https://travis-ci.com/rolfvdhulst/soccer)
[![codecov](https://codecov.io/gh/rolfvdhulst/soccer/branch/development/graph/badge.svg)](https://codecov.io/gh/rolfvdhulst/soccer)


## Code Style Formatting
To be consistent in the code style, we use clang-format. You can use clang-format to automatically format your code during every commit and you can use clang-format in CLion to format all code files in a given folder.

### Commit Formatting
1. Open a terminal and run 'sudo apt-get install clang-format'
2. Run install.sh in the cloned 'formatting' folder with as working directory the cloned soccer folder. 
This will make sure that the code is formatted during every commit from then on. 

### Dependencies
Eigen3,

google protobuf,

Google Test,

Google Benchmark (libbenchmark-dev)

Qt5

### CLion Formatting
1. Open CLion and go to File->Settings->Tools->External Tools
2. Click on Add and fill in without quotes: 
- Name: 'clang-format'
- Program: The location on your computer to the formatting/format-all.sh file of this repository.
- Working directory: '$FilePath$'
3. Click on OK, OK
4. If you now right click a folder in your Project Tree and go to External Tools then you can click on clang-format which will format the entire folder (not this unfortunately does not yet work on single files).
