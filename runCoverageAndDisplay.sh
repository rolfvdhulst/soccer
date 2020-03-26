./runCoverage.sh || exit 1
rm -rf coverage
mkdir coverage
genhtml build/total.info --output-directory coverage
firefox ./coverage/index.html && disown
