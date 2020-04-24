//
// Created by rolf on 21-04-20.
//

#include <containers/circular_buffer.h>
#include <array>
#include <gtest/gtest.h>
#include <numeric>
void access(){
  circular_buffer<double,1> s;
  double x = s[2];
  exit(0);
}
TEST(CircularBufferTest,aTest){
  //TODO: fix more tests
  //ASSERT_EXIT(access(),::testing::KilledBySignal(SIGSEGV),".*");
}

TEST(CircularBufferTest,construct){
  circular_buffer<int,4> test;
  for (int kI = 0; kI < 5; ++kI) {
    test.push_back(kI*kI);
  }
  EXPECT_EQ(test.size(),4);
  EXPECT_EQ(test.back(),16);
  EXPECT_EQ(test.front(),1);

  EXPECT_EQ(test.pop_front(),1);
  EXPECT_EQ(test.size(),3);
  EXPECT_EQ(test.max_size(),4);
  EXPECT_EQ(test.front(),4);
  EXPECT_EQ(test.back(),16);
}
void addVal(int &val){
  val+=4;
}
TEST(CircularBufferTest,iteratorBasics){
  circular_buffer<int,4> test;
  for (int kI = 0; kI < 5; ++kI) {
    test.push_back(kI*kI);
  }
  int index = 1;
  for (const auto& elem : test) {
    EXPECT_EQ(elem,index*index);
    index++;
  }
  std::for_each(test.begin(),test.end(),addVal);
  index = 1;
  for (const auto& elem : test) {
    EXPECT_EQ(elem,index*index+4);
    index++;
  }
  int sum = std::accumulate(test.begin(),test.end(),0);
  int check = 4*4 + 1 + 4 + 9 + 16;
  EXPECT_EQ(sum,check);

  sum = std::accumulate(test.rbegin(),test.rend(),0);
  check = 4*4 + 1 + 4 + 9 + 16;
  EXPECT_EQ(sum,check);

  sum = std::accumulate(test.cbegin(),test.cend(),0);
  check = 4*4 + 1 + 4 + 9 + 16;
  EXPECT_EQ(sum,check);

  sum = std::accumulate(test.crbegin(),test.crend(),0);
  check = 4*4 + 1 + 4 + 9 + 16;
  EXPECT_EQ(sum,check);

  index = 4;
  for (circular_buffer<int,4>::reverse_iterator it = test.rbegin(); it != test.rend(); it++) {
    EXPECT_EQ(*it,index*index+4);
    index--;
  }
  index = 4;
  for (circular_buffer<int,4>::const_reverse_iterator it = test.crbegin(); it != test.crend(); it++) {
    EXPECT_EQ(*it,index*index+4);
    index--;
  }
  //TODO: fix below
  //circular_buffer<int,4>::const_reverse_iterator it = test.rbegin();
//  index = 4;
//  for (circular_buffer<int,4>::const_reverse_iterator it = test.rbegin(); it != test.rend(); it++) {
//    EXPECT_EQ(*it,index*index+4);
//    index--;
//  }
}