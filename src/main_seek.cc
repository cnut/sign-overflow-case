#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>

// use a global variable to emulate file pos.
long long g_pos = 0;

// to show the sign overflow undefined behavior, we simplify the seek logic.
long long sim_seek(long long offset, int whence) {
  printf("%s: input for seek, g_pos: %lld offset: %lld whence: %d\n", __func__,
         g_pos, offset, whence);

  long long old_pos = g_pos;
  long long new_pos = -1;

  switch (whence) {
    case SEEK_SET: {
      new_pos = offset;
      break;
    }
    case SEEK_CUR: {
      new_pos = old_pos + offset;
      if (offset > 0 && new_pos < old_pos) {
        printf(
            "%s: overflow by adding, "
            "(offset(%lld) + old_pos)=new_pos(%lld) < old_pos(%lld)\n",
            __func__, offset, new_pos, old_pos);
        return -EOVERFLOW;
      }

      break;
    }
    case SEEK_END:  // omit the logic for SEEK_END
    default:
      return -EINVAL;
  }

  if (new_pos < 0) {
    printf("%s: new_pos(%lld) is negative\n", __func__, new_pos);
    return -EINVAL;
  }

  g_pos = new_pos;
  printf("%s: end of lseek, g_pos: %lld \n", __func__, g_pos);
  return g_pos;
}

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