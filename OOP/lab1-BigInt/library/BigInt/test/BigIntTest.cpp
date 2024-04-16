#include <gtest/gtest.h>
#include "../include/BigInt.h"
#include <limits.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(constructor, right_constructors){
BigInt test;
test = BigInt(-1234567);
EXPECT_EQ(static_cast<int>(test),-1234567);

test = BigInt("+000000000001");
EXPECT_EQ(static_cast<int>(test),1);

test = BigInt("-000000000001");
EXPECT_EQ(static_cast<int>(test),-1);

test = BigInt();
EXPECT_EQ(static_cast<std::string>(test),"0");

test = BigInt("123456789987654321");
EXPECT_EQ(static_cast<std::string>(test),"123456789987654321");

test = BigInt("-0");
EXPECT_EQ(static_cast<std::string>(test),"0");

BigInt original, copy;
original = BigInt("9999999999999999999999999");
copy = BigInt(original);
EXPECT_EQ(static_cast<std::string>(original),static_cast<std::string>(copy));
}

TEST(constructors, ConstructMistake){
EXPECT_THROW(BigInt(""), std::invalid_argument);
EXPECT_THROW(BigInt("-+"), std::invalid_argument);
EXPECT_THROW(BigInt("-"), std::invalid_argument);
EXPECT_THROW(BigInt("-5g"), std::invalid_argument);
EXPECT_THROW(BigInt("+"), std::invalid_argument);
EXPECT_THROW(BigInt("+4g"), std::invalid_argument);
EXPECT_THROW(BigInt("3g"), std::invalid_argument);
EXPECT_THROW(BigInt("as"), std::invalid_argument);
}

TEST(MathOperators, UnaryPlusUnaryMinus){
BigInt test;
test = BigInt("+5840355839045098");
EXPECT_FALSE(test++ == BigInt("5840355839045099"));
EXPECT_EQ(test, BigInt("5840355839045099"));

test = BigInt(9999);
EXPECT_FALSE(test++ == BigInt("10000"));
EXPECT_EQ(test, BigInt("10000"));

test = BigInt("-23424575");
EXPECT_FALSE(test++ == BigInt("23424574"));
EXPECT_EQ(test, BigInt("-23424574"));

test = BigInt(23);
EXPECT_FALSE(static_cast<int>(test++) == 24);
EXPECT_EQ(static_cast<int>(test),24);

test = BigInt(-1);
EXPECT_EQ(++test, BigInt("-0"));
EXPECT_EQ(test, BigInt("-0")
);

test = BigInt(-100);
EXPECT_EQ(++test, BigInt(-99));
EXPECT_EQ(test, BigInt(-99)
);

test = BigInt(23);
EXPECT_TRUE(static_cast<int>(++test) == 24);
EXPECT_EQ(static_cast<int>(test),24);

test = BigInt("5840355839045098");
EXPECT_FALSE(test-- == BigInt("5840355839045097"));
EXPECT_EQ(test, BigInt("5840355839045097"));

test = BigInt("-23424575");
EXPECT_FALSE(test-- == BigInt("-23424576"));
EXPECT_EQ(test, BigInt("-23424576"));

test = BigInt("100");
EXPECT_FALSE(test-- == BigInt("99"));
EXPECT_EQ(test, BigInt("99"));

test = BigInt("-199");
EXPECT_FALSE(test-- == BigInt("-200"));
EXPECT_EQ(test, BigInt("-200"));

test = BigInt(23);
EXPECT_FALSE(static_cast<int>(test--) == 22);
EXPECT_EQ(static_cast<int>(test),22);

test = BigInt(-1);
EXPECT_EQ(--test,BigInt("-2"));
EXPECT_EQ(test, BigInt("-2"));

test = BigInt(100);
EXPECT_EQ(--test,BigInt("99"));
EXPECT_EQ(test, BigInt("99"));

test = BigInt(0);
EXPECT_EQ(--test,BigInt(-1));
EXPECT_EQ(test, BigInt(-1));

test = BigInt(23);
EXPECT_TRUE(static_cast<int>(--test) == 22);
EXPECT_EQ(static_cast<int>(test),22);
}

TEST(MathOperators, OperatorPlus){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(234);
SecondTest = BigInt(4234);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(4468));

FirstTest = BigInt(-234);
SecondTest = BigInt(4234);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(4000));

FirstTest = BigInt(34);
SecondTest = BigInt(197);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(231));

FirstTest = BigInt(34);
SecondTest = BigInt(197);
expect = SecondTest + FirstTest;
EXPECT_EQ(expect, BigInt(231));

FirstTest = BigInt(32);
SecondTest = BigInt(-64);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(-32));

FirstTest = BigInt(-38);
SecondTest = BigInt(64);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(26));

FirstTest = BigInt(-2343);
SecondTest = BigInt(2000);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt("-343"));

FirstTest = BigInt(3);
SecondTest = BigInt(-3);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(0));

FirstTest = BigInt(234);
SecondTest = BigInt(-4234);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(-4000));

FirstTest = BigInt(-234);
SecondTest = BigInt(-4234);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(-4468));

FirstTest = BigInt(99999);
SecondTest = BigInt(1234);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(101233));

FirstTest = BigInt(1234);
SecondTest = BigInt(99999);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(101233));

FirstTest = BigInt(-100000);
SecondTest = BigInt(111);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(-99889));

FirstTest = BigInt(111);
SecondTest = BigInt(-100000);
expect = FirstTest + SecondTest;
EXPECT_EQ(expect, BigInt(-99889));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest + FirstTest;
EXPECT_EQ(static_cast<int>(expect),781);
}

TEST(MathOperators, OperatorMul){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(234);
SecondTest = BigInt(434);
expect = FirstTest * SecondTest;
EXPECT_EQ(expect, BigInt(101556));

FirstTest = BigInt(-234);
SecondTest = BigInt(434);
expect = FirstTest * SecondTest;
EXPECT_EQ(expect, BigInt(-101556));

FirstTest = BigInt(234);
SecondTest = BigInt("+0");
expect = FirstTest * SecondTest;
EXPECT_EQ(expect, BigInt(0));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest * FirstTest;
EXPECT_EQ(static_cast<int>(expect),9984);
}

TEST(MathOperators, OperatorMinus){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(234);
SecondTest = BigInt(4234);
expect = FirstTest - SecondTest;
EXPECT_EQ(expect, BigInt(-4000));

FirstTest = BigInt(-234);
SecondTest = BigInt(-4234);
expect = FirstTest - SecondTest;
EXPECT_EQ(expect, BigInt(4000));

FirstTest = BigInt(-234);
SecondTest = BigInt(0);
expect = FirstTest - SecondTest;
EXPECT_EQ(expect, BigInt(-234));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest - FirstTest;
EXPECT_EQ(static_cast<int>(expect),755);
}

TEST(MathOperators, OperatorDiv){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(234);
SecondTest = BigInt(0);
EXPECT_THROW(FirstTest/SecondTest,std::invalid_argument);

FirstTest = BigInt(234);
SecondTest = BigInt(4234);
expect = FirstTest / SecondTest;
EXPECT_EQ(expect, BigInt(0));

FirstTest = BigInt(4234);
SecondTest = BigInt(200);
expect = FirstTest / SecondTest;
EXPECT_EQ(expect, BigInt(21));

FirstTest = BigInt(-234);
SecondTest = BigInt(30);
expect = FirstTest / SecondTest;
EXPECT_EQ(expect, BigInt(-7));

FirstTest = BigInt(234);
SecondTest = BigInt(-50);
expect = FirstTest / SecondTest;
EXPECT_EQ(expect, BigInt(-4));

FirstTest = BigInt(-8234);
SecondTest = BigInt(-4234);
expect = FirstTest / SecondTest;
EXPECT_EQ(expect, BigInt(1));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest / FirstTest;
EXPECT_EQ(static_cast<int>(expect),59);
}

TEST(BitwiseOperators, ExclusiveOrOperator){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(646);
SecondTest = BigInt(347);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(989));

FirstTest = BigInt(-646);
SecondTest = BigInt(347);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(-991));

FirstTest = BigInt(-647);
SecondTest = BigInt(347);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(-990));

FirstTest = BigInt(-745);
SecondTest = BigInt(-5365);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(5660));

FirstTest = BigInt(-246);
SecondTest = BigInt(-8366);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(8280));

FirstTest = BigInt(0);
SecondTest = BigInt(1);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(1));

FirstTest = BigInt(-1);
SecondTest = BigInt(0);
expect = FirstTest ^ SecondTest;
EXPECT_EQ(expect, BigInt(-1));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest ^ FirstTest;
EXPECT_EQ(static_cast<int>(expect),781);
}

TEST(MathOperators, RemainderOperator){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(234);
SecondTest = BigInt(0);
EXPECT_THROW(FirstTest%SecondTest,std::invalid_argument);

FirstTest = BigInt(646);
SecondTest = BigInt(347);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(299));

FirstTest = BigInt(-646);
SecondTest = BigInt(347);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(-299));

FirstTest = BigInt(646);
SecondTest = BigInt(-347);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(299));

FirstTest = BigInt(10000000);
SecondTest = BigInt(423);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(280));

FirstTest = BigInt(4234);
SecondTest = BigInt(200);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(34));

FirstTest = BigInt(4);
SecondTest = BigInt(42345);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(4));

FirstTest = BigInt("112001185434");
SecondTest = BigInt(16000);
expect = FirstTest % SecondTest;
EXPECT_EQ(expect, BigInt(1434));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest % FirstTest;
EXPECT_EQ(static_cast<int>(expect),1);
}

TEST(BitwiseOperators, BitwiseAndOperator){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(646);
SecondTest = BigInt(347);
expect = FirstTest & SecondTest;
EXPECT_EQ(expect, BigInt(2));

FirstTest = BigInt(-646);
SecondTest = BigInt(347);
expect = FirstTest & SecondTest;
EXPECT_EQ(expect, BigInt(346));

FirstTest = BigInt(646);
SecondTest = BigInt(-347);
expect = FirstTest & SecondTest;
EXPECT_EQ(expect, BigInt(644));

FirstTest = BigInt(-246);
SecondTest = BigInt(-8366);
expect = FirstTest & SecondTest;
EXPECT_EQ(expect, BigInt(-8446));

FirstTest = BigInt(0);
SecondTest = BigInt(6);
expect = FirstTest & SecondTest;
EXPECT_EQ(expect, BigInt(0));

FirstTest = BigInt(65);
SecondTest = BigInt(0);
expect = FirstTest & SecondTest;
EXPECT_EQ(expect, BigInt(0));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest & FirstTest;
EXPECT_EQ(static_cast<int>(expect),0);
}

TEST(BitwiseOperators, BitwiseOrOperator){
BigInt FirstTest;
BigInt SecondTest;
BigInt expect;

FirstTest = BigInt(646);
SecondTest = BigInt(347);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(991));

FirstTest = BigInt(-646);
SecondTest = BigInt(347);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(-645));

FirstTest = BigInt(646);
SecondTest = BigInt(-347);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(-345));

FirstTest = BigInt(646);
SecondTest = BigInt(-348);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(-346));

FirstTest = BigInt(111);
SecondTest = BigInt(0);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(111));

FirstTest = BigInt(0);
SecondTest = BigInt(111);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(111));

FirstTest = BigInt(-745);
SecondTest = BigInt(-5365);
expect = FirstTest | SecondTest;
EXPECT_EQ(expect, BigInt(-225));

SecondTest = BigInt(768);
FirstTest = BigInt(13);
expect = SecondTest | FirstTest;
EXPECT_EQ(static_cast<int>(expect),781);
}

TEST(BitwiseOperators, OperatorTilda){
BigInt test;
test = BigInt(0);
EXPECT_EQ(~test, BigInt("-1"));

test = BigInt(543);
EXPECT_EQ(~test, BigInt("-544"));

test = BigInt(-111);
EXPECT_EQ(~test, BigInt("110"));

test = BigInt(-110);
EXPECT_EQ(~test, BigInt("109"));

test = BigInt(45);
EXPECT_EQ(static_cast<int>(~test),-46);
}

TEST(MathOperators, UnaryPlus){
BigInt test;
test = BigInt(3);
test = +test;
EXPECT_EQ(test, BigInt(3));

test = BigInt(42);
test = +test;
EXPECT_EQ(static_cast<int>(test),42);
}

TEST(MathOperators, UnaryMinus){
BigInt test;
test = BigInt(3);
test = -test;
EXPECT_EQ(test, BigInt(-3));

test = BigInt(6);
test = -test;
EXPECT_EQ(static_cast<int>(test),-6);

test = BigInt(-3);
test = -test;
EXPECT_EQ(test, BigInt(3));
}

TEST(CompareOperators, EqualOperator){
EXPECT_FALSE(BigInt(0)== BigInt(1));
EXPECT_FALSE (BigInt(INT_MAX)==BigInt(CHAR_MAX));
EXPECT_FALSE(BigInt(-257)== BigInt(256));
EXPECT_FALSE(BigInt("111111111111111")== BigInt("111111111111"));
EXPECT_FALSE(static_cast<int>(BigInt(2))==3);

EXPECT_TRUE(BigInt("123")== BigInt("0000123"));
EXPECT_TRUE(BigInt(0)== BigInt(0));
EXPECT_TRUE(BigInt("123456789987654321")== BigInt("123456789987654321"));
EXPECT_TRUE(BigInt("11117")== BigInt(11117));
EXPECT_TRUE(BigInt("-76547654")== BigInt("-76547654"));
EXPECT_TRUE(static_cast<int>(BigInt(2))==2);

EXPECT_FALSE(BigInt(0)!= BigInt(0));
EXPECT_FALSE(BigInt("123456789987654321")!= BigInt("123456789987654321"));
EXPECT_FALSE(BigInt("11117")!= BigInt(11117));
EXPECT_FALSE(BigInt("-76547654")!= BigInt("-76547654"));
EXPECT_FALSE(BigInt("-1")!= BigInt("-1"));
EXPECT_FALSE(static_cast<int>(BigInt(2))!=2);

EXPECT_TRUE(BigInt(0)!= BigInt(1));
EXPECT_TRUE (BigInt(INT_MAX)!=BigInt(CHAR_MAX));
EXPECT_TRUE(BigInt(-257)!= BigInt(256));
EXPECT_TRUE(BigInt("111111111111111")!= BigInt("111111111111"));
EXPECT_TRUE(static_cast<int>(BigInt(2))!=3);
}

TEST(CompareOperators, LesssAndEqLess){
EXPECT_FALSE(BigInt(1)< BigInt(0));
EXPECT_FALSE (BigInt(INT_MAX)<BigInt(CHAR_MAX));
EXPECT_FALSE(BigInt(257)< BigInt(-256));
EXPECT_FALSE(BigInt("111111111111111")< BigInt("111111111111"));
EXPECT_FALSE(BigInt("-123456789123456789")< BigInt("-123456789123456789"));
EXPECT_FALSE(BigInt(-4)<BigInt(-50));
EXPECT_FALSE(BigInt(-113)<BigInt(-123));
EXPECT_FALSE(static_cast<int>(BigInt(123))<2);

EXPECT_FALSE(BigInt(1)<= BigInt(0));
EXPECT_FALSE (BigInt(INT_MAX)<=BigInt(CHAR_MAX));
EXPECT_FALSE(BigInt(257)<= BigInt(-256));
EXPECT_FALSE(BigInt("111111111111111")<= BigInt("111111111111"));
EXPECT_FALSE(BigInt("-123456789123456780")<= BigInt("-123456789123456789"));
EXPECT_FALSE(BigInt(54)<=BigInt(3));
EXPECT_FALSE(BigInt(-5)<=BigInt(-45));
EXPECT_FALSE(static_cast<int>(BigInt(123))<=2);

EXPECT_TRUE(BigInt(-1)< BigInt(678));
EXPECT_TRUE(BigInt(0)< BigInt(1));
EXPECT_TRUE(BigInt(CHAR_MAX)<BigInt(INT_MAX));
EXPECT_TRUE(BigInt(-256)< BigInt(257));
EXPECT_TRUE(BigInt("11111111111")< BigInt("111111111111"));
EXPECT_TRUE(BigInt("-123456789123456789000")< BigInt("-123456789123456789"));
EXPECT_TRUE(BigInt(-133)<BigInt(-123));
EXPECT_TRUE(static_cast<int>(BigInt(123))<244);

EXPECT_TRUE(BigInt(0)<= BigInt(1));
EXPECT_TRUE(BigInt(CHAR_MAX)<=BigInt(CHAR_MAX));
EXPECT_TRUE(BigInt(-257)<= BigInt(-256));
EXPECT_TRUE(BigInt("111111111111")<= BigInt("111111111111"));
EXPECT_TRUE(BigInt("-123456789123456789")<= BigInt("-123456789123456789"));
EXPECT_TRUE(BigInt(3)<= BigInt(54));
EXPECT_TRUE(BigInt(-100)<= BigInt(3));
EXPECT_TRUE(BigInt(-111)<= BigInt(-1));
EXPECT_TRUE(static_cast<int>(BigInt(123))<=244);
}

TEST(CompareOperators, GreaterOrEqmore){
EXPECT_TRUE(BigInt(1)> BigInt(0));
EXPECT_TRUE(BigInt(INT_MAX)>BigInt(CHAR_MAX));
EXPECT_TRUE(BigInt(257)> BigInt(-256));
EXPECT_TRUE(BigInt("111111111111111")> BigInt("111111111111"));
EXPECT_TRUE(BigInt("-123456789123456780")> BigInt("-123456789123456789"));
EXPECT_TRUE(static_cast<int>(BigInt(123))>2);

EXPECT_TRUE(BigInt(1)>= BigInt(0));
EXPECT_TRUE(BigInt(INT_MAX)>=BigInt(CHAR_MAX));
EXPECT_TRUE(BigInt(257)>= BigInt(-256));
EXPECT_TRUE(BigInt("111111111111111")>= BigInt("111111111111"));
EXPECT_TRUE(BigInt("-123456789123456780")>= BigInt("-123456789123456789"));
EXPECT_TRUE(BigInt("-10")>= BigInt("-100"));
EXPECT_TRUE(BigInt(2)>= BigInt(2));
EXPECT_TRUE(static_cast<int>(BigInt(123))>=2);

EXPECT_FALSE(BigInt(0)> BigInt(1));
EXPECT_FALSE(BigInt(CHAR_MAX)>BigInt(INT_MAX));
EXPECT_FALSE(BigInt(-256)> BigInt(257));
EXPECT_FALSE(BigInt("11111111111")> BigInt("111111111111"));
EXPECT_FALSE(BigInt("-123456789123456789")> BigInt("-123456789123456789"));
EXPECT_FALSE(static_cast<int>(BigInt(123))>244);

EXPECT_FALSE(BigInt(0)>= BigInt(1));
EXPECT_FALSE (BigInt(CHAR_MIN)>=BigInt(CHAR_MAX));
EXPECT_FALSE(BigInt(-257)>= BigInt(-256));
EXPECT_FALSE(BigInt("111111111")>= BigInt("111111111111"));
EXPECT_FALSE(BigInt("-1234567891234567800")>= BigInt("-123456789123456789"));
EXPECT_FALSE(BigInt(-234234)>BigInt(-2));
EXPECT_FALSE(static_cast<int>(BigInt(123))>=244);
}

TEST(ConverionOperators, ToInt){
BigInt test;
test = BigInt(0);
EXPECT_EQ(static_cast<int>(test),0);

test = BigInt(-1);
EXPECT_EQ(static_cast<int>(test),-1);

test = BigInt(123456);
EXPECT_EQ(static_cast<int>(test), BigInt(123456));

test = BigInt(CHAR_MAX);
EXPECT_EQ(static_cast<int>(test), BigInt(CHAR_MAX));

test = BigInt(CHAR_MIN);
EXPECT_EQ(static_cast<int>(test), BigInt(CHAR_MIN));

test = BigInt(CHAR_MAX);
EXPECT_EQ(static_cast<int>(test), BigInt(CHAR_MAX));
}

TEST(StringOperators, GetString){
BigInt test;
std::string str;
test = BigInt(2346543);
str = test.operator std::string();
EXPECT_EQ(str,"2346543");
}

TEST(StreamOperators, PrintOperator){
BigInt test = BigInt("-234423423443245");
std::ostringstream o;
o<<test;
EXPECT_STREQ(o.str().data(),"-234423423443245");
}