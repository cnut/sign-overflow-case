#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>

long sim_seek(long long offset, int whence);

// main function emulates the test case below.
// TEST(Simulate, SimLseekWithOverflow) {
//   off_t ret = sim_seek(LONG_MAX, SEEK_SET);
//   ASSERT_EQ(ret, LONG_MAX);
//   ret = sim_seek(1, SEEK_CUR);
//   ASSERT_EQ(ret, -EOVERFLOW);
// }
int main() {
  printf("%s: try to seek to LLONG_MAX(%lld)\n", __func__, LLONG_MAX);
  long long ret = sim_seek(LLONG_MAX, SEEK_SET);
  if (ret != LLONG_MAX) {
    printf("%s: fail to sim_lseek to LLONG_MAX\n", __func__);
    return -1;
  }

  printf("\n");

  printf(
      "%s: try to seek to LLONG_MAX + 1 (which will trigger signed overflow)\n",
      __func__);
  ret = sim_seek(1, SEEK_CUR);

  printf("\n");
  if (ret != -EOVERFLOW) {
    printf("%s: oops!!!! sim_lseek not overflow, ret: %lld\n", __func__, ret);
    return -1;
  } else {
    printf("%s: sim_lseek overflow\n", __func__);
  }
  return 0;
}