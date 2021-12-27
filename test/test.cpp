#include "my_stack.h"
#include <utility>
#include <iostream>
#include "gtest/gtest.h"
class StackTest : public ::testing::Test{
	public:
	my_stack<int> stack;
	protected:
		void SetUp() override
		{
			stack.Push(1);
			stack.Push(2);
			stack.Push(3);
		}
};
TEST_F(StackTest, proper_size){
	EXPECT_EQ(stack.Capacity(), 3);
}
TEST_F(StackTest, proper_capacity){
	EXPECT_EQ(stack.Size(), 4);
}
TEST_F(StackTest, proper_top){
	EXPECT_EQ(stack.Top(), 3);
}

TEST(Stack_base_func, push_and_pop)
{
	my_stack<int> stack;
		stack.Push(42);
		EXPECT_EQ(stack.Capacity(), 1);
		EXPECT_EQ(stack.Size(), 1);
}

TEST(Stack_base_func, push_and_second_pop)
{
	my_stack<int> stack;
		stack.Push(42);
		EXPECT_EQ(stack.Capacity(), 1);
		EXPECT_EQ(stack.Size(), 1);
		stack.Push(43);
		stack.Pop();
		int i = stack.pop();
		EXPECT_EQ(i, 42);
		EXPECT_EQ(stack.Capacity(), 0);
}

TEST(AssignmentTest, equal_on_copy_ctor)
{
	my_stack<int> stack;
	stack.Push(42);
	stack.Push(42);
	my_stack<int> stack2(stack);
	EXPECT_EQ(stack, stack2);
}

TEST(AssignmentTest, equal_on_copy_assignment)
{
	my_stack<int> stack;
	stack.Push(42);
	stack.Push(42);
	my_stack<int> stack2;
	stack2 = stack;
	EXPECT_EQ(stack, stack2);
}

TEST(Exception, empty_pop)
{
	my_stack<int> stack;
	EXPECT_THROW(stack.Pop(), std::invalid_argument);
}

TEST(Exception, empty_top)
{
	my_stack<int> stack;
	EXPECT_THROW(stack.Top(), std::invalid_argument);
}

TEST(AssignmentTest, not_equal_after_pop)
{
	my_stack<int> stack;
	stack.Push(42);
	stack.Push(42);
	my_stack<int> stack2(stack);
	stack2.Push(10);
	EXPECT_NE(stack, stack2);
	stack2.Pop();
	EXPECT_EQ(stack, stack2);
}

TEST(EqualTest, diff_types)
{
	my_stack<int> stack;
	stack.Push(42);
	stack.Push(42);
	my_stack<float> stack2;
	stack2.Push(42);
	stack2.Push(42);
	EXPECT_NE(stack, stack2);
}

TEST(Move_test, move_ctor)
{
	my_stack<int> stack;
	stack.Push(42);
	stack.Push(84);
	my_stack<int> stack2(stack);
	EXPECT_EQ(stack, stack2);
	my_stack<int> stack3(std::move(stack));
	EXPECT_EQ(stack2, stack3);
}

TEST(Move_test, moved_from_and_move_ctor)
{
	my_stack<int> stack;
	stack.Push(42);
	stack.Push(84);
	my_stack<int> stack2(stack);
	EXPECT_EQ(stack, stack2);
	my_stack<int> stack3(std::move(stack));
	EXPECT_EQ(stack2, stack3);
	EXPECT_EQ(stack.Size(), 0);
	EXPECT_EQ(stack.Capacity(), 0);
}
int main()
{
::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
