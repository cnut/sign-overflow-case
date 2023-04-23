#include <errno.h>
#include <fcntl.h>
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