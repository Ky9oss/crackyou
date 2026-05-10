#include <errno.h>
#include <gtest/gtest.h>
#define errno_t int

// implemented in a C source file
extern "C" char *get_error(errno_t errnum);

namespace {
TEST(GetErrorTest, KnownError) {
  EXPECT_STREQ(get_error(ENOMEM), "Cannot allocate memory");
  EXPECT_STREQ(get_error(ENOTSOCK), "Socket operation on non-socket");
  EXPECT_STREQ(get_error(EPIPE), "Broken pipe");
}

TEST(GetErrorTest, UnknownError) {
  EXPECT_STREQ(get_error(-2), "Unknown error -1");
}
} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
