#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int last_message;

u_char * __add_md5(u_char *filename,int *len,u_char **result)

{
  int iVar1;
  int iVar2;
  char *__s;
  size_t sVar3;
  u_char *puVar4;
  long lVar5;
  
  __s = MD5File((char *)filename,(char *)0x0);
  sVar3 = strlen(__s);
  puVar4 = *result;
  iVar2 = (int)sVar3;
  if (puVar4 == (u_char *)0x0) {
    iVar1 = *len;
    *result = (u_char *)0xffffffffffffffff;
    sVar3 = SEXT48(iVar1 + 1 + iVar2);
LAB_00109faa:
    lVar5 = (long)iVar1;
    puVar4 = (u_char *)0xffffffffffffffff;
    if (sVar3 == 0) goto LAB_00109f60;
    puVar4 = (u_char *)malloc(sVar3);
    if (puVar4 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      lVar5 = (long)*len;
      goto LAB_00109f60;
    }
  }
  else {
    iVar1 = *len;
    sVar3 = SEXT48(iVar1 + 1 + iVar2);
    if (puVar4 == (u_char *)0xffffffffffffffff) goto LAB_00109faa;
    if (sVar3 == 0) {
      free(puVar4);
      puVar4 = (u_char *)0xffffffffffffffff;
      lVar5 = (long)*len;
      goto LAB_00109f60;
    }
    puVar4 = (u_char *)realloc(puVar4,sVar3);
    if (puVar4 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
    }
  }
  lVar5 = (long)*len;
LAB_00109f60:
  *result = puVar4;
  memcpy(puVar4 + lVar5,__s,(long)(iVar2 + 1));
  if (__s != (char *)0xffffffffffffffff) {
    free(__s);
  }
  *len = *len + iVar2;
  return *result;
}


void add_message(char *message)

{
  // ulonglong uVar1;
  size_t sVar2;
  char *pcVar3;
  int iVar4;
  long lVar5;
  
  if (last_message == 3) {
    scroll_messages();
  }
  uVar1 = get_time();
  iVar4 = last_message + 1;
  lVar5 = (long)iVar4;
  msg_line[lVar5].time = uVar1 + 10000000;
  sVar2 = strlen(message);
  pcVar3 = (char *)malloc(sVar2 + 1);
  if (pcVar3 == (char *)0x0) {
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    iVar4 = last_message + 1;
  }
  msg_line[lVar5].msg = pcVar3;
  pcVar3 = msg_line[iVar4].msg;
  if (pcVar3 != (char *)0x0) {
    sVar2 = strlen(message);
    memcpy(pcVar3,message,sVar2 + 1);
    last_message = last_message + 1;
  }
  return;
}


void add_to_table(object_list *pointer)

{
  size_t __size;
  byte bVar1;
  object_list **ppoVar2;
  ulong uVar3;
  
  uVar3 = (ulong)(*(uint *)&(pointer->member).type >> 8 & 0x7fff);
  bVar1 = table[uVar3].count;
  ppoVar2 = table[uVar3].pointer;
  __size = (ulong)bVar1 * 8 + 8;
  if (ppoVar2 == (object_list **)0xffffffffffffffff) {
    ppoVar2 = (object_list **)malloc(__size);
    if (ppoVar2 != (object_list **)0x0) {
LAB_0010df9a:
      table[uVar3].pointer = ppoVar2;
      ppoVar2[bVar1] = pointer;
      table[uVar3].count = table[uVar3].count + '\x01';
      return;
    }
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    table[uVar3].pointer = (object_list **)0x0;
  }
  else {
    if (ppoVar2 == (object_list **)0x0) {
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)");
      table[uVar3].pointer = (object_list **)0x0;
    }
    else {
      ppoVar2 = (object_list **)realloc(ppoVar2,__size);
      if (ppoVar2 != (object_list **)0x0) goto LAB_0010df9a;
      error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
      table[uVar3].pointer = (object_list **)0x0;
    }
  }
  fwrite("Not enough memory.\n",1,0x13,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void blit_screen(u_char ignore_bg)

{
  bool bVar1;
  byte fg;
  long lVar2;
  int y;
  uint uVar3;
  ulong uVar4;
  byte bVar5;
  ulong uVar6;
  u_char *__src;
  long lVar7;
  uint uVar8;
  byte *local_58;
  u_char *local_50;
  u_char *local_48;
  
  if ((TRI_D_ON == 0) || (tri_d == 0)) {
    local_48 = screen_old;
    local_58 = screen_a;
    local_50 = screen_a_old;
    __src = screen;
  }
  else {
    local_48 = screen2_old;
    local_58 = screen2_a;
    local_50 = screen2_a_old;
    __src = screen2;
  }
  fg = *local_58;
  if (ignore_bg != '\0') {
    fg = *local_58 & 0xf;
  }
  c_setcolor_bg(fg,fg >> 4);
  if (0 < SCREEN_Y) {
    bVar1 = true;
    y = 0;
    do {
      c_goto(0,y);
      if (0 < SCREEN_X) {
        lVar2 = 0;
        lVar7 = (long)(SCREEN_X * y);
        do {
          uVar8 = (uint)local_58[lVar2 + lVar7];
          bVar5 = local_50[lVar2 + lVar7];
          if (ignore_bg != '\0') {
            uVar8 = local_58[lVar2 + lVar7] & 0xf;
            bVar5 = bVar5 & 0xf;
          }
          if ((SCREEN_X + -1 == (int)lVar2) && (SCREEN_Y + -1 == y)) break;
          if ((__src[lVar2 + lVar7] != local_48[lVar2 + lVar7]) || ((byte)uVar8 != bVar5)) {
            if (!bVar1) {
              c_goto((int)lVar2,y);
            }
            uVar8 = fg ^ uVar8;
            uVar3 = (uVar8 >> 3) * 2 & 2 | (uint)((uVar8 & 7) != 0);
            uVar6 = (ulong)(uVar3 | 4);
            uVar4 = (ulong)uVar3;
            if ((uVar8 & 0x70) != 0) {
              uVar4 = uVar6;
            }
                    /* WARNING: Could not recover jumptable at 0x0010c9bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&DAT_00112a6c + *(int *)(&DAT_00112a6c + uVar4 * 4)))
                      (uVar6,uVar4,&DAT_00112a6c + *(int *)(&DAT_00112a6c + uVar4 * 4));
            return;
          }
          bVar1 = false;
          lVar2 = lVar2 + 1;
        } while ((int)lVar2 < SCREEN_X);
      }
      y = y + 1;
    } while (y < SCREEN_Y);
  }
  memcpy(local_48,__src,(long)(SCREEN_X * SCREEN_Y));
  memcpy(local_50,local_58,(long)(SCREEN_X * SCREEN_Y));
  c_refresh();
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

my_double can_go_x(my_double old_x,my_double new_x,int yh,int yl,u_char *flag)

{
  int iVar1;
  int iVar2;
  byte *pbVar3;
  int iVar4;
  byte *pbVar5;
  
  if ((double)old_x == (double)new_x + _DAT_001129b0) {
    if (flag != (u_char *)0x0) {
      *flag = '\0';
    }
    return new_x;
  }
  if (flag != (u_char *)0x0) {
    *flag = '\x01';
  }
  if (old_x < new_x) {
    iVar1 = (old_x >> 10) + 1;
    iVar2 = new_x + 0x3ff >> 10;
    if (iVar1 <= iVar2) {
      if (iVar1 < 1000) {
        pbVar5 = area_a + (long)(yh * 1000) + (long)iVar1;
        do {
          pbVar3 = pbVar5;
          iVar4 = yh;
          if (yh <= yl) {
            do {
              if ((*pbVar3 & 0xf0) == 0x10) {
                return (iVar1 + -1) * 0x400;
              }
              iVar4 = iVar4 + 1;
              pbVar3 = pbVar3 + 1000;
            } while (iVar4 <= yl);
          }
          iVar1 = iVar1 + 1;
          if (iVar2 < iVar1) goto LAB_00109b6d;
          pbVar5 = pbVar5 + 1;
        } while (iVar1 != 1000);
      }
      return 0xf9c00;
    }
  }
  else {
    iVar1 = (old_x + 0x3ff >> 10) + -1;
    if (new_x >> 10 <= iVar1) {
      if (-1 < iVar1) {
        pbVar5 = area_a + (long)(yh * 1000) + (long)iVar1;
        do {
          pbVar3 = pbVar5;
          iVar2 = yh;
          if (yh <= yl) {
            do {
              if ((*pbVar3 & 0xf0) == 0x10) {
                return (my_double)((iVar1 + 1) * 0x400);
              }
              iVar2 = iVar2 + 1;
              pbVar3 = pbVar3 + 1000;
            } while (iVar2 <= yl);
          }
          iVar1 = iVar1 + -1;
          if (iVar1 < new_x >> 10) goto LAB_00109b6d;
          pbVar5 = pbVar5 + -1;
        } while (iVar1 != -1);
      }
      return 0;
    }
  }
LAB_00109b6d:
  if (flag == (u_char *)0x0) {
    return new_x;
  }
  *flag = '\0';
  return new_x;
}


my_double can_go_y(my_double old_y,my_double new_y,int xl,int xr,u_char *flag,u_char down_ladder)

{
  int iVar1;
  int iVar2;
  int iVar3;
  byte bVar4;
  long lVar5;
  
  if (old_y == new_y) {
    if (flag != (u_char *)0x0) {
      *flag = '\0';
    }
    return new_y;
  }
  iVar1 = old_y >> 10;
  iVar2 = new_y >> 10;
  if (flag == (u_char *)0x0) {
    if (new_y <= old_y) {
      iVar1 = (old_y + 0x3ff >> 10) + -1;
      if (iVar1 < iVar2) {
        return new_y;
      }
LAB_00109c3f:
      if (-1 < iVar1) {
        iVar3 = iVar1 * 1000;
        do {
          if (xl <= xr) {
            lVar5 = (long)xl;
            do {
              if ((area_a[lVar5 + iVar3] & 0xf0) == 0x10) {
                return (iVar1 + 1) * 0x400;
              }
              lVar5 = lVar5 + 1;
            } while ((int)lVar5 <= xr);
          }
          iVar1 = iVar1 + -1;
          if (iVar1 < iVar2) goto LAB_00109d6b;
          iVar3 = iVar3 + -1000;
        } while (iVar1 != -1);
      }
      return 0;
    }
    iVar2 = new_y + 0x3ff >> 10;
    if (iVar2 < iVar1 + 1) {
      return new_y;
    }
LAB_00109bb8:
    iVar1 = iVar1 + 1;
    if (iVar1 < 500) {
      iVar3 = iVar1 * 1000;
      do {
        if (xl <= xr) {
          lVar5 = (long)xl;
          if (down_ladder == '\0') {
            do {
              bVar4 = (area_a + iVar3)[lVar5] & 0xf0;
              if ((bVar4 == 0x10) || ((bVar4 - 0x20 & 0xd0) == 0)) goto LAB_00109c13;
              lVar5 = lVar5 + 1;
            } while ((int)lVar5 <= xr);
          }
          else {
            do {
              if (((area_a + iVar3)[lVar5] & 0xf0) == 0x10) {
LAB_00109c13:
                return (my_double)((iVar1 + -1) * 0x400);
              }
              lVar5 = lVar5 + 1;
            } while ((int)lVar5 <= xr);
          }
        }
        iVar1 = iVar1 + 1;
        if (iVar2 < iVar1) goto LAB_00109d6b;
        iVar3 = iVar3 + 1000;
      } while (iVar1 != 500);
    }
    return 0x7cc00;
  }
  *flag = '\x01';
  if (old_y < new_y) {
    iVar2 = new_y + 0x3ff >> 10;
    if (iVar1 + 1 <= iVar2) goto LAB_00109bb8;
  }
  else {
    *flag = '\x02';
    iVar1 = (old_y + 0x3ff >> 10) + -1;
    if (iVar2 <= iVar1) goto LAB_00109c3f;
  }
  goto LAB_00109d70;
LAB_00109d6b:
  if (flag == (u_char *)0x0) {
    return new_y;
  }
LAB_00109d70:
  *flag = '\0';
  return new_y;
}


void c_bell(void)

{
  my_print("\a");
  return;
}


void c_clear(int x1,int y1,int x2,int y2)

{
  long lVar1;
  int iVar2;
  char *str;
  size_t __size;
  char *pcVar3;
  long in_FS_OFFSET;
  char txt [16];
  
  iVar2 = x2 - x1;
  __size = SEXT48(iVar2 + 1);
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  if (__size == 0) {
    DAT_ffffffff = 0;
    str = (char *)0xffffffffffffffff;
    if (y1 <= y2) {
LAB_0010d8a0:
      do {
        iVar2 = x1;
        if ((TRI_D_ON != 0) && (tri_d != 0)) {
          iVar2 = x1 + screen_width;
        }
        y1 = y1 + 1;
        __snprintf_chk(txt,0x10,1,0x10,&DAT_00112aa6,y1,iVar2 + 1);
        my_print(txt);
        my_print(str);
      } while (y1 <= y2);
      if (str != (char *)0xffffffffffffffff) goto LAB_0010d908;
    }
    if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
      return;
    }
  }
  else {
    str = (char *)malloc(__size);
    if (str == (char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      fwrite("Error: Not enough memory.\n",1,0x1a,stderr);
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    pcVar3 = str;
    if (-1 < iVar2) {
      memset(str,0x20,(long)iVar2 + 1);
      pcVar3 = str + __size;
    }
    *pcVar3 = '\0';
    if (y1 <= y2) goto LAB_0010d8a0;
LAB_0010d908:
    if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
      free(str);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_cls(void)

{
  my_print("\x1b[2J");
  return;
}


void c_cursor(int type)

{
  if (type == 0) {
    my_print("\x1b[?25h");
    return;
  }
  if (type == 1) {
    my_print("\x1b[?25l");
    return;
  }
  return;
}


void c_get_size(int *x,int *y)

{
  long lVar1;
  int iVar2;
  long in_FS_OFFSET;
  winsize ws;
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  iVar2 = ioctl(1,0x5413,&ws);
  if (iVar2 == -1) {
    *x = 0x50;
    *y = 0x18;
  }
  else {
    *x = (uint)ws.ws_col;
    *y = (uint)ws.ws_row;
  }
  screen_width = *x;
  if (TRI_D_ON != 0) {
    screen_width = screen_width >> 1;
    *x = screen_width;
  }
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_goto(int x,int y)

{
  long lVar1;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  if ((TRI_D_ON != 0) && (tri_d != 0)) {
    x = x + screen_width;
  }
  __snprintf_chk(txt,0x10,1,0x10,&DAT_00112aa6,y + 1,x + 1);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void change_level(void)

{
  long lVar1;
  object_list *poVar2;
  char *__ptr;
  object_list *poVar3;
  long lVar4;
  u_char *__ptr_00;
  msgline_type *pmVar5;
  int iVar6;
  uint uVar7;
  long in_FS_OFFSET;
  u_char txt [256];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  poVar2 = &objects;
  while (poVar3 = poVar2, poVar2 = poVar3->next, poVar2 != (object_list *)0x0) {
    uVar7 = *(uint *)&(poVar2->member).type >> 8;
    if (*(uint *)&hero->type >> 8 != uVar7) {
      delete_obj((ulong)uVar7);
      poVar2 = poVar3;
    }
  }
  lVar4 = (long)last_message;
  if (-1 < last_message) {
    pmVar5 = msg_line;
    iVar6 = 0;
    do {
      __ptr = pmVar5->msg;
      if (__ptr != (char *)0xffffffffffffffff) {
        if (__ptr == (char *)0x0) {
          errfile = "error.h";
          errline = 0x4c;
          int_error((u_char *)"mem_free(NULL)");
          lVar4 = (long)last_message;
        }
        else {
          free(__ptr);
          lVar4 = (long)last_message;
        }
      }
      iVar6 = iVar6 + 1;
      pmVar5 = pmVar5 + 1;
    } while (iVar6 <= (int)lVar4);
  }
  iVar6 = level_sprites_start;
  last_message = -1;
  msg_line[lVar4].msg = (char *)0x0;
  free_sprites(iVar6);
  reinit_area();
  __ptr_00 = load_level(level_number);
  __snprintf_chk(txt,0x100,1,0x100,"%s%s%s","data/",__ptr_00,&DAT_001120da);
  load_sprites(txt);
  __snprintf_chk(txt,0x100,1,0x100,"%s%s%s","data/",__ptr_00,&DAT_001120de);
  load_data(txt);
  if (__ptr_00 != (u_char *)0xffffffffffffffff) {
    if (__ptr_00 == (u_char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
    }
    else {
      free(__ptr_00);
    }
  }
  if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


void check_memory_leaks(void)

{
  return;
}


/* WARNING: Could not reconcile some variable overlaps */

void c_init(int w,int h)

{
  long lVar1;
  char *pcVar2;
  long in_FS_OFFSET;
  termios t;
  
  screen_buffer_size = w * h;
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  console_ok = 0;
  if ((long)screen_buffer_size == 0) {
    screen_buffer = (char *)0xffffffffffffffff;
    pcVar2 = screen_buffer;
  }
  else {
    pcVar2 = (char *)malloc((long)screen_buffer_size);
    if (pcVar2 == (char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen_buffer = (char *)0x0;
      fwrite("Error: Not enough memory for screen buffer.\n",1,0x2c,stderr);
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
  }
  screen_buffer = pcVar2;
  tcgetattr(0,(termios *)&term_setting);
  t._48_8_ = term_setting._48_8_;
  t._0_8_ = CONCAT44(term_setting.c_oflag,term_setting.c_iflag);
  t.c_ospeed = term_setting.c_ospeed;
  t._0_8_ = t._0_8_ & 0xfffffffefffffa14;
  t._16_4_ = term_setting._16_4_;
  t.c_cc._3_4_ = term_setting.c_cc._3_4_;
  t.c_cc._7_4_ = term_setting.c_cc._7_4_;
  t.c_cc._11_4_ = term_setting.c_cc._11_4_;
  t.c_cc._15_4_ = term_setting.c_cc._15_4_;
  t.c_cc._19_4_ = term_setting.c_cc._19_4_;
  t.c_cc._23_4_ = term_setting.c_cc._23_4_;
  t.c_cc._27_4_ = term_setting.c_cc._27_4_;
  t.c_cflag = term_setting.c_cflag & 0xfffffecf | 0x30;
  t.c_lflag = term_setting.c_lflag & 0xffff7fb4;
  tcsetattr(0,0,(termios *)&t);
  kbd_init();
  my_print("\x1b[2J");
  my_print("\x1b[;H");
  write(1,screen_buffer,(long)screen_buffer_pos);
  screen_buffer_pos = 0;
  if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  screen_buffer_pos = 0;
  return;
}


void clean_memory(void)

{
  object_list *poVar1;
  char *__ptr;
  object_list *poVar2;
  long lVar3;
  int iVar4;
  uint uVar5;
  msgline_type *pmVar6;
  
  poVar1 = &objects;
  while (poVar2 = poVar1, poVar1 = poVar2->next, poVar1 != (object_list *)0x0) {
    uVar5 = *(uint *)&(poVar1->member).type >> 8;
    if (*(uint *)&hero->type >> 8 != uVar5) {
      delete_obj((ulong)uVar5);
      poVar1 = poVar2;
    }
  }
  lVar3 = (long)last_message;
  if (-1 < last_message) {
    pmVar6 = msg_line;
    iVar4 = 0;
    do {
      __ptr = pmVar6->msg;
      if (__ptr != (char *)0xffffffffffffffff) {
        if (__ptr == (char *)0x0) {
          errfile = "error.h";
          errline = 0x4c;
          int_error((u_char *)"mem_free(NULL)");
          lVar3 = (long)last_message;
        }
        else {
          free(__ptr);
          lVar3 = (long)last_message;
        }
      }
      iVar4 = iVar4 + 1;
      pmVar6 = pmVar6 + 1;
    } while (iVar4 <= (int)lVar3);
  }
  last_message = -1;
  msg_line[lVar3].msg = (char *)0x0;
  return;
}


void clear_screen(void)

{
  memset(screen_a,0,(long)(SCREEN_X * SCREEN_Y));
  memset(screen,0,(long)(SCREEN_X * SCREEN_Y));
  if (TRI_D_ON == 0) {
    return;
  }
  memset(screen2_a,0,(long)(SCREEN_X * SCREEN_Y));
  memset(screen2,0,(long)(SCREEN_X * SCREEN_Y));
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char * contact_server(int color,u_char *name)

{
  byte bVar1;
  int iVar2;
  my_double yspeed;
  my_double xspeed;
  my_double y;
  my_double x;
  int sprite;
  uint id;
  size_t sVar3;
  char *pcVar4;
  int *piVar5;
  ulonglong uVar6;
  long lVar7;
  fd_set *pfVar8;
  long in_FS_OFFSET;
  timeval tv;
  fd_set fds;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  sVar3 = strlen((char *)name);
  lVar7 = 0x10;
  tv.tv_sec = _DAT_00112810;
  tv.tv_usec = _UNK_00112818;
  pfVar8 = (fd_set *)&fds;
  while (lVar7 != 0) {
    lVar7 = lVar7 + -1;
    pfVar8->fds_bits[0] = 0;
    pfVar8 = (fd_set *)(pfVar8->fds_bits + 1);
  }
  lVar7 = __fdelt_chk(_DAT_00112810,(long)fd);
  contact_server::packet._0_4_ = 0x10000000;
  contact_server::packet[4] = (u_char)color;
  bVar1 = (byte)(fd >> 0x37);
  fds.__fds_bits[lVar7] =
       fds.__fds_bits[lVar7] | 1 << (((char)fd + (bVar1 >> 2) & 0x3f) - (bVar1 >> 2) & 0x3f);
  __memcpy_chk(contact_server::packet + 5,name,(long)((int)sVar3 + 1));
  send_packet(contact_server::packet,(int)sVar3 + 6,(sockaddr *)&server,my_id,0);
  iVar2 = select(fd + 1,(fd_set *)&fds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv);
  pcVar4 = "No reply within 4 seconds. Press ENTER.";
  if (iVar2 != 0) {
    iVar2 = recv_packet(contact_server::packet,0x100,(sockaddr *)0x0,(int *)0x0,1,0,(int *)0x0);
    if (iVar2 < 0) {
      piVar5 = __errno_location();
      pcVar4 = "Server hung up. Press ENTER.";
      if (*piVar5 != 4) {
        pcVar4 = "Connection error. Press ENTER.";
      }
    }
    else {
      if ((char)contact_server::packet._0_4_ == '\x01') {
        my_id = get_int(contact_server::packet + 0x21);
        if (((iVar2 < 0x27) || (contact_server::packet[37] != '\0')) ||
           (contact_server::packet[38] < 0x10)) {
          send_quit();
          pcVar4 = "Incompatible server version. Givin\' up. Press Enter.";
        }
        else {
          game_start_offset = get_time();
          uVar6 = get_long_long(contact_server::packet + 0x19);
          game_start_offset = game_start_offset - uVar6;
          ammo[0] = (ushort)weapon[0].basic_ammo;
          health = 'd';
          armor = '\0';
          ammo._2_8_ = 0;
          current_weapon = '\0';
          weapons = '\x11';
          yspeed = get_float(contact_server::packet + 0x13);
          xspeed = get_float(contact_server::packet + 0xf);
          y = get_float(contact_server::packet + 0xb);
          x = get_float(contact_server::packet + 7);
          iVar2 = get_int16(contact_server::packet + 0x17);
          sprite = get_int16(contact_server::packet + 5);
          id = get_int(contact_server::packet + 1);
          hero = new_obj(id,'\0',0,sprite,'\0',iVar2,x,y,xspeed,yspeed,(void *)0x0);
          pcVar4 = (char *)0x0;
        }
      }
      else {
        pcVar4 = "Connection error. Press ENTER.";
        if ((char)contact_server::packet._0_4_ == '\x02') {
          pcVar4 = "Incompatible client version. Connection refused. Press Enter.";
          if (contact_server::packet[1] != '\0') {
            pcVar4 = "Connection refused. Press ENTER.";
          }
        }
      }
    }
  }
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return pcVar4;
}


int _conv_color(int c)

{
  uint uVar1;
  __int32_t **pp_Var2;
  
  if (c + 0x80U < 0x180) {
    pp_Var2 = __ctype_tolower_loc();
    c = (*pp_Var2)[c];
  }
  if (5 < c - 0x61U) {
    uVar1 = 0;
    if (c - 0x30U < 10) {
      uVar1 = c - 0x30U;
    }
    return (int)uVar1;
  }
  return c + -0x57;
}


int _convert_type(u_char c)

{
  int iVar1;
  
  iVar1 = -1;
  if ((byte)(c - 0x31) < 0x47) {
    iVar1 = (int)*(short *)(CSWTCH_138 + (ulong)(byte)(c - 0x31) * 2);
  }
  return iVar1;
}


int c_pressed(int key)

{
  int iVar1;
  
  iVar1 = kbd_is_pressed(key);
  return iVar1;
}


void c_print(char *str)

{
  my_print(str);
  return;
}


void c_putc(char c)

{
  char *pcVar1;
  long lVar2;
  size_t __size;
  
  lVar2 = (long)screen_buffer_pos;
  pcVar1 = screen_buffer;
  if (screen_buffer_size <= screen_buffer_pos) {
    screen_buffer_size = screen_buffer_size * 2;
    __size = SEXT48(screen_buffer_size);
    if (screen_buffer == (char *)0xffffffffffffffff) {
      if ((__size != 0) && (pcVar1 = (char *)malloc(__size), pcVar1 == (char *)0x0)) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
LAB_0010d7d2:
        do {
          screen_buffer = (char *)0x0;
          my_putc();
LAB_0010d7e0:
          errline = 0x56;
          errfile = "error.h";
          int_error((u_char *)"mem_realloc(NULL, %d)");
        } while( true );
      }
    }
    else {
      if (screen_buffer == (char *)0x0) goto LAB_0010d7e0;
      if (__size == 0) {
        free(screen_buffer);
        lVar2 = (long)screen_buffer_pos;
        screen_buffer = (char *)0xffffffffffffffff;
        pcVar1 = screen_buffer;
      }
      else {
        pcVar1 = (char *)realloc(screen_buffer,__size);
        if (pcVar1 == (char *)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          goto LAB_0010d7d2;
        }
        lVar2 = (long)screen_buffer_pos;
      }
    }
  }
  screen_buffer = pcVar1;
  screen_buffer[lVar2] = c;
  screen_buffer_pos = screen_buffer_pos + 1;
  return;
}


void c_refresh(void)

{
  write(1,screen_buffer,(long)screen_buffer_pos);
  screen_buffer_pos = 0;
  return;
}


void c_setbgcolor(u_char a)

{
  long lVar1;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __snprintf_chk(txt,0x10,1,0x10,&DAT_00112abb,(a & 7) + 0x28);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_setcolor(u_char a)

{
  long lVar1;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __snprintf_chk(txt,0x10,1,0x10,&DAT_00112a93,(a >> 3 & 1) + 0x30,(a & 7) + 0x1e);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_setcolor_3b(u_char a)

{
  long lVar1;
  long in_FS_OFFSET;
  char txt [8];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __snprintf_chk(txt,0x10,1,8,&DAT_00112abb,(a & 7) + 0x1e);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_setcolor_3b_bg(u_char fg,u_char bg)

{
  long lVar1;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __snprintf_chk(txt,0x10,1,0x10,&DAT_00112acb,(fg & 7) + 0x1e,(bg & 7) + 0x28);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_setcolor_bg(u_char fg,u_char bg)

{
  long lVar1;
  byte bVar2;
  char cVar3;
  undefined *puVar4;
  char cVar5;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  cVar3 = (fg & 7) + 0x1e;
  bVar2 = fg >> 3 & 1;
  if ((bVar2 | bg) == 0) {
    cVar5 = '0';
    puVar4 = &DAT_00112a93;
  }
  else {
    puVar4 = &DAT_00112aaf;
    cVar3 = (bg & 7) + 0x28;
    cVar5 = bVar2 + 0x30;
  }
  __snprintf_chk(txt,0x10,1,0x10,puVar4,cVar5,cVar3);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_sethlt(u_char a)

{
  char *str;
  
  str = "\x1b[1m";
  if (a == '\0') {
    str = "\x1b[0m";
  }
  my_print(str);
  return;
}


void c_sethlt_bg(u_char hlt,u_char bg)

{
  long lVar1;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __snprintf_chk(txt,0x10,1,0x10,&DAT_00112acb,hlt & 1,(bg & 7) + 0x28);
  my_print(txt);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void c_shutdown(void)

{
  long lVar1;
  long in_FS_OFFSET;
  undefined8 uVar2;
  char txt [16];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  if (screen_buffer != (char *)0xffffffffffffffff) {
    if (screen_buffer == (char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
    }
    else {
      free(screen_buffer);
    }
  }
  kbd_close();
  tcsetattr(0,0,(termios *)&term_setting);
  uVar2 = 0x10ce58;
  my_print("\x1b[?25h");
  __snprintf_chk(txt,0x10,1,0x10,&DAT_00112a93,0x30,0x25,uVar2);
  my_print(txt);
  my_print("\x1b[2J");
  my_print("\x1b[;H");
  write(1,screen_buffer,(long)screen_buffer_pos);
  screen_buffer_pos = 0;
  console_ok = 1;
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    console_ok = 1;
    screen_buffer_pos = 0;
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


int c_update_kbd(void)

{
  int iVar1;
  
  iVar1 = kbd_update();
  return iVar1;
}


void c_wait_for_key(void)

{
  kbd_wait_for_key();
  return;
}


int c_was_pressed(int key)

{
  int iVar1;
  
  iVar1 = kbd_was_pressed(key);
  return iVar1;
}


void debug_msg(u_char *m,...)

{
  long lVar1;
  char in_AL;
  undefined8 in_RCX;
  undefined8 in_RDX;
  undefined8 in_RSI;
  undefined8 in_R8;
  undefined8 in_R9;
  long in_FS_OFFSET;
  undefined4 in_XMM0_Da;
  undefined4 in_XMM1_Da;
  undefined4 in_XMM2_Da;
  undefined4 in_XMM3_Da;
  undefined4 in_XMM4_Da;
  undefined4 in_XMM5_Da;
  undefined4 in_XMM6_Da;
  undefined4 in_XMM7_Da;
  va_list l;
  undefined local_b8 [8];
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined4 local_88;
  undefined4 local_78;
  undefined4 local_68;
  undefined4 local_58;
  undefined4 local_48;
  undefined4 local_38;
  undefined4 local_28;
  undefined4 local_18;
  
  if (in_AL != '\0') {
    local_88 = in_XMM0_Da;
    local_78 = in_XMM1_Da;
    local_68 = in_XMM2_Da;
    local_58 = in_XMM3_Da;
    local_48 = in_XMM4_Da;
    local_38 = in_XMM5_Da;
    local_28 = in_XMM6_Da;
    local_18 = in_XMM7_Da;
  }
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  l[0].overflow_arg_area = &stack0x00000008;
  l[0].reg_save_area = local_b8;
  l[0].gp_offset = 8;
  l[0].fp_offset = 0x30;
  local_b0 = in_RSI;
  local_a8 = in_RDX;
  local_a0 = in_RCX;
  local_98 = in_R8;
  local_90 = in_R9;
  __sprintf_chk(errbuf,1,0x1000,"DEBUG MESSAGE at %s:%d: ",errfile,errline);
  __strcat_chk(errbuf,m,0x1000);
  __vfprintf_chk(stderr,1,errbuf,l);
  fputc(10,stderr);
  sleep(1);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void delete_obj(ulong id)

{
  object_list *poVar1;
  object_list *poVar2;
  object_list *__ptr;
  
  __ptr = remove_from_table((uint)id);
  if (__ptr != (object_list *)0x0) {
    poVar1 = __ptr->prev;
    poVar2 = __ptr->next;
    poVar1->next = poVar2;
    if (poVar2 != (object_list *)0x0) {
      poVar2->prev = poVar1;
      poVar1 = last_obj;
    }
    last_obj = poVar1;
    if (__ptr != (object_list *)0xffffffffffffffff) {
      free(__ptr);
      return;
    }
  }
  return;
}


void do_not_optimize_here(void *p)

{
  return;
}


void draw_board(void)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  u_char uVar6;
  long lVar7;
  int iVar8;
  long in_FS_OFFSET;
  char txt [16];
  
  lVar3 = *(long *)(in_FS_OFFSET + 0x28);
  iVar4 = (SCREEN_X + -0x3c) / 5;
  lVar7 = (long)((SCREEN_Y + -2) * SCREEN_X);
  memset(screen_a + lVar7,4,(long)SCREEN_X);
  memset(screen + lVar7,0x2d,(long)SCREEN_X);
  memset(screen + lVar7 + SCREEN_X,0,(long)SCREEN_X);
  print2screen(0,SCREEN_Y + -1,'\a',"HEALTH");
  __sprintf_chk(txt,1,0x10,"% 3d%%",health);
  uVar6 = '\n';
  if (health < 0x32) {
    uVar6 = (0x18 < health) * '\x02' + '\t';
  }
  iVar8 = iVar4 >> 1;
  print2screen(6,SCREEN_Y + -1,uVar6,txt);
  print2screen(iVar8 + 0xb,SCREEN_Y + -2,'\x04',",");
  iVar1 = iVar8 + 0x15 + iVar4;
  print2screen(iVar8 + 0xb,SCREEN_Y + -1,'\x04',"|");
  print2screen(iVar4 + 0xb,SCREEN_Y + -1,'\a',"FRAGS");
  __sprintf_chk(txt,1,0x10,&DAT_0011210b,frags);
  print2screen(iVar4 + 0x11,SCREEN_Y + -1,'\v',txt);
  print2screen(iVar1,SCREEN_Y + -2,'\x04',",");
  iVar5 = iVar4 * 2;
  iVar2 = iVar8 + 0x1f + iVar5;
  print2screen(iVar1,SCREEN_Y + -1,'\x04',"|");
  print2screen(iVar5 + 0x15,SCREEN_Y + -1,'\a',"DEATHS");
  __sprintf_chk(txt,1,0x10,&DAT_0011210b,deaths);
  print2screen(iVar5 + 0x1c,SCREEN_Y + -1,'\v',txt);
  print2screen(iVar2,SCREEN_Y + -2,'\x04',",");
  print2screen(iVar2,SCREEN_Y + -1,'\x04',"|");
  __sprintf_chk(txt,1,0x10,&DAT_00112110,weapon[current_weapon].name);
  iVar2 = iVar8 + 0x29 + iVar4 * 3;
  print2screen(iVar4 * 3 + 0x1f,SCREEN_Y + -1,'\v',txt);
  print2screen(iVar2,SCREEN_Y + -2,'\x04',",");
  iVar1 = iVar4 * 4;
  print2screen(iVar2,SCREEN_Y + -1,'\x04',"|");
  print2screen(iVar1 + 0x29,SCREEN_Y + -1,'\a',"AMMO");
  __sprintf_chk(txt,1,0x10,&DAT_0011210b,ammo[current_weapon]);
  uVar6 = '\n';
  if ((uint)ammo[current_weapon] < (uint)((int)(uint)weapon[current_weapon].max_ammo >> 1)) {
    uVar6 = ((uint)((int)(uint)weapon[current_weapon].max_ammo >> 2) <= (uint)ammo[current_weapon])
            * '\x02' + '\t';
  }
  iVar2 = iVar8 + 0x31 + iVar1;
  print2screen(iVar1 + 0x2e,SCREEN_Y + -1,uVar6,txt);
  print2screen(iVar2,SCREEN_Y + -2,'\x04',",");
  print2screen(iVar2,SCREEN_Y + -1,'\x04',"|");
  print2screen(iVar4 * 5 + 0x31,SCREEN_Y + -1,'\a',"ARMOR");
  __sprintf_chk(txt,1,0x10,"% 4d%%",armor);
  uVar6 = '\n';
  if (armor < 0x32) {
    uVar6 = (0x18 < armor) * '\x02' + '\t';
  }
  print2screen(iVar4 * 5 + 0x36,SCREEN_Y + -1,uVar6,txt);
  if ((*(byte *)&hero->field_0x1a & 0x40) != 0) {
    print2screen(SCREEN_X + -0xe,SCREEN_Y + -2,'\v',"INVISIBILITY");
  }
  if (autorun != '\0') {
    print2screen(2,SCREEN_Y + -2,'\x0f',"AUTORUN");
  }
  if (autocreep != '\0') {
    print2screen(10,SCREEN_Y + -2,'\x0f',"AUTOCREEP");
  }
  if (lVar3 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


void draw_frame(int x,int y,int w,int h,u_char color)

{
  int iVar1;
  int iVar2;
  long lVar3;
  size_t sVar4;
  u_char color_00;
  uint __c;
  int iVar5;
  int iVar6;
  int iVar7;
  char *message;
  
  message = (char *)0xffffffffffffffff;
  __c = color & 0xf;
  if ((long)(w + 3) != 0) {
    message = (char *)malloc((long)(w + 3));
    if (message == (char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      return;
    }
  }
  lVar3 = (long)w;
  message[lVar3 + 2] = '\0';
  message[lVar3] = '\0';
  message[lVar3 + -1] = '\0';
  color_00 = (u_char)__c;
  print2screen(x,y,color_00,"+");
  iVar7 = x + 1 + w;
  print2screen(iVar7,y,color_00,"+");
  iVar1 = y + 1 + h;
  print2screen(iVar7,iVar1,color_00,"+");
  print2screen(x,iVar1,color_00,"+");
  if (0 < w) {
    memset(message,0x2d,(ulong)(w - 1) + 1);
  }
  print2screen(x + 1,y,color_00,message);
  print2screen(x + 1,iVar1,color_00,message);
  memset(message,0x20,(long)(w + 1));
  *message = '|';
  message[lVar3 + 1] = '|';
  if (0 < h) {
    iVar1 = y + 1 + h;
    iVar7 = y + 1;
LAB_0010ccb6:
    do {
      iVar2 = SCREEN_X;
      iVar5 = SCREEN_Y;
      if ((x < SCREEN_X) && (iVar7 < SCREEN_Y)) {
        sVar4 = strlen(message);
        iVar5 = iVar2 * iVar5;
        iVar6 = iVar7 * iVar2 + x;
        lVar3 = (long)iVar6;
        iVar2 = (int)sVar4;
        if (iVar5 < iVar6 + (int)sVar4) {
          iVar2 = iVar5 - iVar6;
        }
        sVar4 = SEXT48(iVar2);
        memset(screen_a + lVar3,__c,sVar4);
        memcpy(screen + lVar3,message,sVar4);
        if (TRI_D_ON != 0) {
          iVar7 = iVar7 + 1;
          memset(screen2_a + lVar3,__c,sVar4);
          memcpy(screen2 + lVar3,message,sVar4);
          if (iVar7 == iVar1) break;
          goto LAB_0010ccb6;
        }
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 != iVar1);
  }
  if (message != (char *)0xffffffffffffffff) {
    free(message);
    return;
  }
  return;
}


void draw_scene(void)

{
  byte bVar1;
  ushort uVar2;
  void *pvVar3;
  it *piVar4;
  uint uVar5;
  uint uVar6;
  object_list *poVar7;
  long lVar8;
  pos *ppVar9;
  object_list *poVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  
  iVar11 = (hero->y >> 10) - SCREEN_YOFFSET;
  iVar13 = (hero->x >> 10) - SCREEN_XOFFSET;
  if (TRI_D_ON != 0) {
    tri_d = 1;
    show_window(iVar13,iVar11);
    tri_d = 0;
    iVar11 = (hero->y >> 10) - SCREEN_YOFFSET;
    iVar13 = (hero->x >> 10) - SCREEN_XOFFSET;
  }
  show_window(iVar13,iVar11);
  iVar11 = 0;
  do {
    poVar10 = &objects;
    poVar7 = objects.next;
    while (poVar7 != (object_list *)0x0) {
      while ((bVar1 = (poVar7->member).type, obj_attr[bVar1].foreground == (u_char)iVar11 &&
             (&poVar7->member != hero))) {
        if (bVar1 == 0) {
          update_anim(&poVar7->member);
          poVar7 = poVar10->next;
        }
        else {
          *(short *)&(poVar7->member).field_0x18 =
               (short)((long)(ulong)(ushort)(*(short *)&(poVar7->member).field_0x18 + 1) %
                      (long)sprites[*(ushort *)&(poVar7->member).field_0x16].n_steps);
        }
        if ((*(ushort *)&(poVar7->member).field_0x1a & 0x440) == 0) {
          ppVar9 = sprites[*(ushort *)&(poVar7->member).field_0x16].positions +
                   sprites[*(ushort *)&(poVar7->member).field_0x16].steps
                   [*(ushort *)&(poVar7->member).field_0x18];
          iVar13 = ((poVar7->member).y - hero->y >> 10) + SCREEN_YOFFSET;
          iVar12 = (poVar7->member).x - hero->x >> 10;
          if (TRI_D_ON != 0) {
            tri_d = 1;
            put_sprite(iVar12 + iVar11 + SCREEN_XOFFSET,iVar13,ppVar9,'\x01');
            poVar7 = poVar10->next;
            tri_d = 0;
            ppVar9 = sprites[*(ushort *)&(poVar7->member).field_0x16].positions +
                     sprites[*(ushort *)&(poVar7->member).field_0x16].steps
                     [*(ushort *)&(poVar7->member).field_0x18];
            iVar13 = ((poVar7->member).y - hero->y >> 10) + SCREEN_YOFFSET;
            iVar12 = (poVar7->member).x - hero->x >> 10;
          }
          put_sprite(iVar12 + SCREEN_XOFFSET,iVar13,ppVar9,'\x01');
          poVar7 = poVar10->next;
        }
        if ((((poVar7->member).type != '\0') ||
            (uVar2 = *(ushort *)&(poVar7->member).field_0x1a, (uVar2 & 0x80) == 0)) ||
           (*(ushort *)&(poVar7->member).field_0x1a = uVar2 & 0xff7f, (uVar2 & 0x400) != 0)) break;
        pvVar3 = (poVar7->member).data;
        uVar6 = (uint)pvVar3;
        ppVar9 = sprites[hit_sprite].positions + sprites[hit_sprite].steps[(int)uVar6 >> 0x10];
        iVar13 = (poVar7->member).y;
        uVar5 = (uint)(byte)((ulong)pvVar3 >> 8);
        iVar12 = (poVar7->member).x;
        uVar6 = uVar6 & 0xff;
        if (TRI_D_ON != 0) {
          tri_d = 1;
          put_sprite(SCREEN_XOFFSET + (iVar12 - hero->x >> 10) + iVar11 + uVar6,
                     SCREEN_YOFFSET + (iVar13 - hero->y >> 10) + uVar5,ppVar9,'\x01');
          poVar7 = poVar10->next;
          tri_d = 0;
          pvVar3 = (poVar7->member).data;
          uVar6 = (uint)pvVar3;
          ppVar9 = sprites[hit_sprite].positions + sprites[hit_sprite].steps[(int)uVar6 >> 0x10];
          iVar13 = (poVar7->member).y;
          uVar6 = uVar6 & 0xff;
          uVar5 = (uint)(byte)((ulong)pvVar3 >> 8);
          iVar12 = (poVar7->member).x;
        }
        put_sprite(SCREEN_XOFFSET + (iVar12 - hero->x >> 10) + uVar6,
                   SCREEN_YOFFSET + (iVar13 - hero->y >> 10) + uVar5,ppVar9,'\x01');
        poVar10 = poVar10->next;
        poVar7 = poVar10->next;
        if (poVar7 == (object_list *)0x0) goto LAB_00104de1;
      }
      poVar10 = poVar7;
      poVar7 = poVar7->next;
    }
LAB_00104de1:
    if ((obj_attr[0].foreground == (u_char)iVar11) &&
       (update_anim(hero), (*(byte *)&hero->field_0x1b & 4) == 0)) {
      ppVar9 = sprites[*(ushort *)&hero->field_0x16].positions +
               sprites[*(ushort *)&hero->field_0x16].steps[*(ushort *)&hero->field_0x18];
      if (TRI_D_ON != 0) {
        tri_d = 1;
        put_sprite(SCREEN_XOFFSET,SCREEN_YOFFSET,ppVar9,'\x01');
        tri_d = 0;
        ppVar9 = sprites[*(ushort *)&hero->field_0x16].positions +
                 sprites[*(ushort *)&hero->field_0x16].steps[*(ushort *)&hero->field_0x18];
      }
      put_sprite(SCREEN_XOFFSET,SCREEN_YOFFSET,ppVar9,'\x01');
      piVar4 = hero;
      if ((*(ushort *)&hero->field_0x1a & 0x80) != 0) {
        lVar8 = (long)hit_sprite;
        *(ushort *)&hero->field_0x1a = *(ushort *)&hero->field_0x1a & 0xff7f;
        uVar6 = (uint)piVar4->data;
        iVar13 = (uVar6 & 0xff) + SCREEN_XOFFSET;
        iVar12 = (uint)(byte)((ulong)piVar4->data >> 8) + SCREEN_YOFFSET;
        ppVar9 = sprites[lVar8].positions + sprites[lVar8].steps[(int)uVar6 >> 0x10];
        if (TRI_D_ON != 0) {
          tri_d = 1;
          put_sprite(iVar13,iVar12,ppVar9,'\x01');
          tri_d = 0;
          uVar6 = (uint)hero->data;
          iVar13 = (uVar6 & 0xff) + SCREEN_XOFFSET;
          iVar12 = (uint)(byte)((ulong)hero->data >> 8) + SCREEN_YOFFSET;
          ppVar9 = sprites[hit_sprite].positions + sprites[hit_sprite].steps[(int)uVar6 >> 0x10];
        }
        put_sprite(iVar13,iVar12,ppVar9,'\x01');
      }
    }
    if (iVar11 == 1) {
      return;
    }
    iVar11 = 1;
  } while( true );
}


void end_game(void)

{
  long in_FS_OFFSET;
  u_char packet;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  packet = '\x03';
  send_packet(&packet,1,(sockaddr *)&server,my_id,0);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void er(int b,u_char *m,__va_list_tag *l)

{
  if (b != 0) {
    fputc(7,stderr);
  }
  __vfprintf_chk(stderr,1,m,l);
  fputc(10,stderr);
  sleep(1);
  return;
}


void error(u_char *m,...)

{
  long lVar1;
  char in_AL;
  undefined8 in_RCX;
  undefined8 in_RDX;
  undefined8 in_RSI;
  undefined8 in_R8;
  undefined8 in_R9;
  long in_FS_OFFSET;
  undefined4 in_XMM0_Da;
  undefined4 in_XMM1_Da;
  undefined4 in_XMM2_Da;
  undefined4 in_XMM3_Da;
  undefined4 in_XMM4_Da;
  undefined4 in_XMM5_Da;
  undefined4 in_XMM6_Da;
  undefined4 in_XMM7_Da;
  va_list l;
  undefined local_b8 [8];
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined4 local_88;
  undefined4 local_78;
  undefined4 local_68;
  undefined4 local_58;
  undefined4 local_48;
  undefined4 local_38;
  undefined4 local_28;
  undefined4 local_18;
  
  if (in_AL != '\0') {
    local_88 = in_XMM0_Da;
    local_78 = in_XMM1_Da;
    local_68 = in_XMM2_Da;
    local_58 = in_XMM3_Da;
    local_48 = in_XMM4_Da;
    local_38 = in_XMM5_Da;
    local_28 = in_XMM6_Da;
    local_18 = in_XMM7_Da;
  }
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  l[0].overflow_arg_area = &stack0x00000008;
  l[0].reg_save_area = local_b8;
  l[0].gp_offset = 8;
  l[0].fp_offset = 0x30;
  local_b0 = in_RSI;
  local_a8 = in_RDX;
  local_a0 = in_RCX;
  local_98 = in_R8;
  local_90 = in_R9;
  fputc(7,stderr);
  __vfprintf_chk(stderr,1,m,l);
  fputc(10,stderr);
  sleep(1);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


object_list * find_in_table(uint id)

{
  object_list **ppoVar1;
  object_list **ppoVar2;
  
  if (table[id & 0x7fff].count != 0) {
    ppoVar2 = table[id & 0x7fff].pointer;
    ppoVar1 = ppoVar2 + (ulong)(table[id & 0x7fff].count - 1) + 1;
    do {
      if (*(uint *)&((*ppoVar2)->member).type >> 8 == id) {
        return *ppoVar2;
      }
      ppoVar2 = ppoVar2 + 1;
    } while (ppoVar2 != ppoVar1);
  }
  return (object_list *)0x0;
}


char * find_server(char *name,ushort port)

{
  hostent *phVar1;
  char *pcVar2;
  
  phVar1 = gethostbyname(name);
  pcVar2 = "Error: Can\'t resolve server address.";
  if (phVar1 != (hostent *)0x0) {
    pcVar2 = (char *)0x0;
    server.sin_port = port << 8 | port >> 8;
    server.sin_family = 2;
    server.sin_addr = *(in_addr_t *)*phVar1->h_addr_list;
  }
  return pcVar2;
}


int find_sprite(u_char *name,int *num)

{
  u_char **ppuVar1;
  int iVar2;
  long lVar3;
  
  *num = 0;
  ppuVar1 = sprite_names;
  if (n_sprites < 1) {
    return 1;
  }
  lVar3 = 0;
  do {
    iVar2 = strcmp((char *)ppuVar1[lVar3],(char *)name);
    if (iVar2 == 0) {
      return 0;
    }
    iVar2 = (int)lVar3;
    lVar3 = lVar3 + 1;
    *num = iVar2 + 1;
  } while ((int)lVar3 < n_sprites);
  return 1;
}


void free_area(void)

{
  if (area != (u_char *)0xffffffffffffffff) {
    if (area == (u_char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
    }
    else {
      free(area);
    }
  }
  if (area_a != (u_char *)0xffffffffffffffff) {
    if (area_a != (u_char *)0x0) {
      free(area_a);
      return;
    }
    errline = 0x4c;
    errfile = "error.h";
    int_error((u_char *)"mem_free(NULL)");
    return;
  }
  return;
}


void free_hash_table(void)

{
  object_list *__ptr;
  byte bVar1;
  long lVar2;
  table_type *ptVar3;
  object_list *__ptr_00;
  
  ptVar3 = table;
  do {
    bVar1 = ptVar3->count;
    __ptr_00 = (object_list *)ptVar3->pointer;
    lVar2 = 0;
    if (bVar1 != 0) {
      do {
        while (__ptr = *(object_list **)((long)__ptr_00 + lVar2 * 8),
              __ptr == (object_list *)0xffffffffffffffff) {
LAB_0010e29d:
          lVar2 = lVar2 + 1;
          if ((int)(uint)bVar1 <= (int)lVar2) goto LAB_0010e2f0;
        }
        if (__ptr != (object_list *)0x0) {
          free(__ptr);
          bVar1 = ptVar3->count;
          __ptr_00 = (object_list *)ptVar3->pointer;
          goto LAB_0010e29d;
        }
        lVar2 = lVar2 + 1;
        errfile = "error.h";
        errline = 0x4c;
        int_error((u_char *)"mem_free(NULL)");
        bVar1 = ptVar3->count;
        __ptr_00 = (object_list *)ptVar3->pointer;
      } while ((int)lVar2 < (int)(uint)bVar1);
    }
LAB_0010e2f0:
    if (__ptr_00 != (object_list *)0xffffffffffffffff) {
      if (__ptr_00 == (object_list *)0x0) {
        errfile = "error.h";
        errline = 0x4c;
        int_error((u_char *)"mem_free(NULL)");
      }
      else {
        free(__ptr_00);
      }
    }
    ptVar3 = ptVar3 + 1;
    if (ptVar3 == (table_type *)&keyboard_type) {
      return;
    }
  } while( true );
}


void free_sprite(sprite *sp)

{
  line *plVar1;
  u_char *puVar2;
  ushort *__ptr;
  pos *ppVar3;
  int iVar4;
  long lVar5;
  line *__ptr_00;
  long lVar6;
  
  lVar6 = 0;
  if (0 < sp->n_positions) {
    do {
      ppVar3 = sp->positions + lVar6;
      iVar4 = ppVar3->n;
      __ptr_00 = ppVar3->lines;
      if (0 < iVar4) {
        lVar5 = 0;
        do {
          while( true ) {
            plVar1 = __ptr_00 + lVar5;
            if (plVar1->len == 0) break;
            puVar2 = plVar1->attr;
            if (puVar2 != (u_char *)0xffffffffffffffff) {
              if (puVar2 == (u_char *)0x0) {
                errfile = "error.h";
                errline = 0x4c;
                int_error((u_char *)"mem_free(NULL)");
              }
              else {
                free(puVar2);
              }
            }
            puVar2 = plVar1->bmp;
            if (puVar2 != (u_char *)0xffffffffffffffff) {
              if (puVar2 == (u_char *)0x0) {
                errfile = "error.h";
                errline = 0x4c;
                int_error((u_char *)"mem_free(NULL)");
                iVar4 = ppVar3->n;
                __ptr_00 = ppVar3->lines;
                break;
              }
              free(puVar2);
            }
            iVar4 = ppVar3->n;
            lVar5 = lVar5 + 1;
            __ptr_00 = ppVar3->lines;
            if (iVar4 <= (int)lVar5) goto LAB_0010b60f;
          }
          lVar5 = lVar5 + 1;
        } while ((int)lVar5 < iVar4);
      }
LAB_0010b60f:
      if (__ptr_00 != (line *)0xffffffffffffffff) {
        if (__ptr_00 == (line *)0x0) {
          errfile = "error.h";
          errline = 0x4c;
          int_error((u_char *)"mem_free(NULL)");
        }
        else {
          free(__ptr_00);
        }
      }
      lVar6 = lVar6 + 1;
    } while (sp->n_positions != (int)lVar6 && (int)lVar6 <= sp->n_positions);
  }
  if (sp->n_steps != 0) {
    __ptr = sp->steps;
    if (__ptr != (ushort *)0xffffffffffffffff) {
      if (__ptr == (ushort *)0x0) {
        errline = 0x4c;
        errfile = "error.h";
        int_error((u_char *)"mem_free(NULL)");
      }
      else {
        free(__ptr);
      }
    }
    ppVar3 = sp->positions;
    if (ppVar3 != (pos *)0xffffffffffffffff) {
      if (ppVar3 != (pos *)0x0) {
        free(ppVar3);
        return;
      }
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
      return;
    }
  }
  return;
}


void free_sprites(int start_num)

{
  void *__ptr;
  u_char **ppuVar1;
  sprite *psVar2;
  long lVar3;
  uint uVar4;
  ulong uVar5;
  ulong uVar6;
  size_t sVar7;
  
  uVar6 = SEXT48(start_num);
  if (start_num < n_sprites) {
    lVar3 = uVar6 * 8;
    uVar5 = uVar6 & 0xffffffff;
    do {
      __ptr = *(void **)((long)sprite_names + lVar3);
      if (__ptr != (void *)0xffffffffffffffff) {
        if (__ptr == (void *)0x0) {
          errfile = "error.h";
          errline = 0x4c;
          int_error((u_char *)"mem_free(NULL)");
        }
        else {
          free(__ptr);
        }
      }
      uVar4 = (int)uVar5 + 1;
      uVar5 = (ulong)uVar4;
      psVar2 = (sprite *)(&sprites->n_positions + lVar3);
      lVar3 = lVar3 + 8;
      free_sprite(psVar2);
    } while ((int)uVar4 < n_sprites);
  }
  sVar7 = uVar6 * 8;
  n_sprites = start_num;
  if (sprite_names == (u_char **)0xffffffffffffffff) {
    ppuVar1 = sprite_names;
    if ((sVar7 != 0) && (ppuVar1 = (u_char **)malloc(sVar7), ppuVar1 == (u_char **)0x0)) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      sprite_names = (u_char **)0x0;
      goto LAB_00109461;
    }
  }
  else {
    if (sprite_names == (u_char **)0x0) {
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)");
      sprite_names = (u_char **)0x0;
      goto LAB_00109461;
    }
    if (sVar7 == 0) {
      free(sprite_names);
      uVar6 = SEXT48(n_sprites);
      sprite_names = (u_char **)0xffffffffffffffff;
      ppuVar1 = sprite_names;
    }
    else {
      ppuVar1 = (u_char **)realloc(sprite_names,sVar7);
      if (ppuVar1 == (u_char **)0x0) {
        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
        sprite_names = (u_char **)0x0;
        goto LAB_00109461;
      }
      uVar6 = SEXT48(n_sprites);
    }
  }
  sprite_names = ppuVar1;
  sVar7 = uVar6 << 5;
  if (sprites == (sprite *)0xffffffffffffffff) {
    psVar2 = sprites;
    if ((sVar7 == 0) || (psVar2 = (sprite *)malloc(sVar7), psVar2 != (sprite *)0x0)) {
      sprites = psVar2;
      return;
    }
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    sprites = (sprite *)0x0;
  }
  else {
    if (sprites == (sprite *)0x0) {
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)",sVar7);
      sprites = (sprite *)0x0;
    }
    else {
      if (sVar7 == 0) {
        free(sprites);
        sprites = (sprite *)0xffffffffffffffff;
        return;
      }
      psVar2 = (sprite *)realloc(sprites,sVar7);
      if (psVar2 != (sprite *)0x0) {
        sprites = psVar2;
        return;
      }
      error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
      sprites = (sprite *)0x0;
    }
  }
LAB_00109461:
  fwrite("Memory allocation error!\n",1,0x19,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void get_dimensions(int type,int status,pos *s,int *w,int *h)

{
  int iVar1;
  
  if (type == 4) {
    *w = 0x16;
    *h = 3;
    return;
  }
  if (type == 5) {
    *w = 0xc;
    *h = 3;
    return;
  }
  if (type == 0) {
    *w = 0xf;
    if ((status & 0x100U) != 0) {
      *h = 5;
      return;
    }
    *h = 0xd;
    return;
  }
  *w = 0;
  if (s != (pos *)0x0) {
    iVar1 = s->n;
    *h = iVar1;
    if (iVar1 != 0) {
      *w = s->lines->len;
    }
    return;
  }
  *h = 0;
  return;
}


int get_float(byte *param_1)

{
  return (uint)param_1[3] * 0x1000000 +
         (uint)*param_1 + (uint)param_1[1] * 0x100 + (uint)param_1[2] * 0x10000;
}


int get_int(u_char *p)

{
  return (int)((uint)p[3] * 0x1000000 + (uint)*p + (uint)p[1] * 0x100 + (uint)p[2] * 0x10000);
}


int get_int16(u_char *p)

{
  return (int)((uint)p[1] * 0x100 + (uint)*p);
}


int getkey(void)

{
  int iVar1;
  __int32_t **pp_Var2;
  long lVar3;
  fd_set *pfVar4;
  long in_FS_OFFSET;
  u_char a;
  fd_set fds;
  long local_20;
  
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  read(0,&a,1);
  shift_pressed = 0;
  switch(a) {
  case '\0':
    read(0,&a,1);
    iVar1 = 0;
    if (a - 0x48 < 9) {
      iVar1 = *(int *)(CSWTCH_26 + (ulong)(a - 0x48) * 4);
    }
    break;
  case '\b':
    iVar1 = 0x12d;
    break;
  case '\t':
    iVar1 = 0x12e;
    break;
  case '\r':
    iVar1 = 0x12f;
    break;
  case '\x1b':
    lVar3 = 0x10;
    pfVar4 = (fd_set *)&fds;
    while (lVar3 != 0) {
      lVar3 = lVar3 + -1;
      pfVar4->fds_bits[0] = 0;
      pfVar4 = (fd_set *)(pfVar4->fds_bits + 1);
    }
    fds.__fds_bits[0] = fds.__fds_bits[0] | 1;
    _a = (undefined  [16])0x0;
    iVar1 = select(1,(fd_set *)&fds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&a);
    if (iVar1 != 0) {
      read(0,&a,1);
      if (a == '[') {
        read(0,&a,1);
        if (a - 0x41 < 4) {
          iVar1 = *(int *)(CSWTCH_27 + (ulong)(a - 0x41) * 4);
          break;
        }
      }
    }
    iVar1 = 300;
    break;
  default:
    iVar1 = 0x12d;
    if (a == '\x7f') break;
    switch(a) {
    case '!':
    case '\"':
    case '#':
    case '$':
    case '%':
    case '&':
    case '(':
    case ')':
    case '*':
    case '+':
    case ':':
    case '<':
    case '>':
    case '?':
    case '@':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '^':
    case '_':
    case '{':
    case '|':
    case '}':
    case '~':
      shift_pressed = 1;
    }
  case '\x01':
  case '\x02':
  case '\x03':
  case '\x04':
  case '\x05':
  case '\x06':
  case '\a':
  case '\n':
  case '\v':
  case '\f':
  case '\x0e':
  case '\x0f':
  case '\x10':
  case '\x11':
  case '\x12':
  case '\x13':
  case '\x14':
  case '\x15':
  case '\x16':
  case '\x17':
  case '\x18':
  case '\x19':
  case '\x1a':
    pp_Var2 = __ctype_tolower_loc();
    iVar1 = (*pp_Var2)[a];
  }
  if (local_20 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar1;
}


ulonglong get_long_long(u_char *p)

{
  return ((ulong)p[7] << 0x38) +
         (ulong)*p + (ulong)p[2] * 0x10000 + (ulong)p[1] * 0x100 + (ulong)p[3] * 0x1000000 +
         ((ulong)p[4] << 0x20) + ((ulong)p[5] << 0x28) + ((ulong)p[6] << 0x30);
}


ulonglong get_time(void)

{
  long lVar1;
  long in_FS_OFFSET;
  timezone tz;
  timeval tv;
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  tz = (timezone)0x0;
  gettimeofday((timeval *)&tv,(__timezone_ptr_t)&tz);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return tv.tv_sec * 1000000 + tv.tv_usec;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void hash_table_init(void)

{
  table_type *ptVar1;
  table_type *ptVar2;
  
  ptVar1 = table;
  do {
    ptVar1->count = '\0';
    ptVar2 = ptVar1 + 1;
    ptVar1->pointer = (object_list **)0xffffffffffffffff;
    ptVar1 = ptVar2;
  } while (ptVar2 != (table_type *)&keyboard_type);
  return;
}


void init_area(void)

{
  u_char *__s;
  u_char *puVar1;
  
  __s = (u_char *)malloc(500000);
  if (__s == (u_char *)0x0) {
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    area = (u_char *)0x0;
  }
  else {
    area = __s;
    puVar1 = (u_char *)calloc(500000,1);
    if (puVar1 != (u_char *)0x0) {
      area_a = puVar1;
      memset(__s,0x20,500000);
      return;
    }
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    area_a = (u_char *)0x0;
  }
  fwrite("Error: Not enough memory!\n",1,0x1a,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void init_blit(void)

{
  u_char *puVar1;
  size_t sVar2;
  
  sVar2 = SEXT48(SCREEN_X * SCREEN_Y);
  if (sVar2 == 0) {
    screen = (u_char *)0xffffffffffffffff;
    screen_a = (u_char *)0xffffffffffffffff;
    screen_old = (u_char *)0xffffffffffffffff;
    screen_a_old = (u_char *)0xffffffffffffffff;
    puVar1 = screen_a_old;
  }
  else {
    puVar1 = (u_char *)malloc(sVar2);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen = (u_char *)0x0;
      goto LAB_0010b97f;
    }
    screen = puVar1;
    puVar1 = (u_char *)malloc(sVar2);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen_a = (u_char *)0x0;
      goto LAB_0010b97f;
    }
    screen_a = puVar1;
    puVar1 = (u_char *)malloc(sVar2);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen_old = (u_char *)0x0;
      goto LAB_0010b97f;
    }
    screen_old = puVar1;
    puVar1 = (u_char *)malloc(sVar2);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen_a_old = (u_char *)0x0;
      goto LAB_0010b97f;
    }
  }
  screen_a_old = puVar1;
  memset(screen_a_old,0,sVar2);
  memset(screen_old,0,(long)(SCREEN_X * SCREEN_Y));
  if (TRI_D_ON != 0) {
    sVar2 = SEXT48(SCREEN_X * SCREEN_Y);
    if (sVar2 == 0) {
      screen2 = (u_char *)0xffffffffffffffff;
      screen2_a = (u_char *)0xffffffffffffffff;
      screen2_old = (u_char *)0xffffffffffffffff;
      screen2_a_old = (u_char *)0xffffffffffffffff;
      puVar1 = screen2_a_old;
    }
    else {
      puVar1 = (u_char *)malloc(sVar2);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2 = (u_char *)0x0;
LAB_0010b97f:
        fwrite("Not enough memory!\n",1,0x13,stderr);
                    /* WARNING: Subroutine does not return */
        exit(1);
      }
      screen2 = puVar1;
      puVar1 = (u_char *)malloc(sVar2);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2_a = (u_char *)0x0;
        goto LAB_0010b97f;
      }
      screen2_a = puVar1;
      puVar1 = (u_char *)malloc(sVar2);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2_old = (u_char *)0x0;
        goto LAB_0010b97f;
      }
      screen2_old = puVar1;
      puVar1 = (u_char *)malloc(sVar2);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2_a_old = (u_char *)0x0;
        goto LAB_0010b97f;
      }
    }
    screen2_a_old = puVar1;
    memset(screen2_a_old,0,sVar2);
    memset(screen2_old,0,(long)(SCREEN_X * SCREEN_Y));
  }
  SCREEN_XOFFSET = SCREEN_X + -0xf >> 1;
  SCREEN_YOFFSET = SCREEN_Y + -0xd >> 1;
  return;
}


char * init_socket(void)

{
  char *pcVar1;
  
  fd = socket(2,2,0x11);
  pcVar1 = "Can\'t get socket.\n";
  if (-1 < fd) {
    pcVar1 = (char *)0x0;
  }
  return pcVar1;
}


void init_sprites(void)

{
  init_blit();
  return;
}


void int_error(u_char *m,...)

{
  long lVar1;
  char in_AL;
  undefined8 in_RCX;
  undefined8 in_RDX;
  undefined8 in_RSI;
  undefined8 in_R8;
  undefined8 in_R9;
  long in_FS_OFFSET;
  undefined4 in_XMM0_Da;
  undefined4 in_XMM1_Da;
  undefined4 in_XMM2_Da;
  undefined4 in_XMM3_Da;
  undefined4 in_XMM4_Da;
  undefined4 in_XMM5_Da;
  undefined4 in_XMM6_Da;
  undefined4 in_XMM7_Da;
  va_list l;
  undefined local_b8 [8];
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined4 local_88;
  undefined4 local_78;
  undefined4 local_68;
  undefined4 local_58;
  undefined4 local_48;
  undefined4 local_38;
  undefined4 local_28;
  undefined4 local_18;
  
  if (in_AL != '\0') {
    local_88 = in_XMM0_Da;
    local_78 = in_XMM1_Da;
    local_68 = in_XMM2_Da;
    local_58 = in_XMM3_Da;
    local_48 = in_XMM4_Da;
    local_38 = in_XMM5_Da;
    local_28 = in_XMM6_Da;
    local_18 = in_XMM7_Da;
  }
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  l[0].overflow_arg_area = &stack0x00000008;
  l[0].reg_save_area = local_b8;
  l[0].gp_offset = 8;
  l[0].fp_offset = 0x30;
  local_b0 = in_RSI;
  local_a8 = in_RDX;
  local_a0 = in_RCX;
  local_98 = in_R8;
  local_90 = in_R9;
  __sprintf_chk(errbuf,1,0x1000,&DAT_00112b50,errfile,errline);
  __strcat_chk(errbuf,m,0x1000);
  fputc(7,stderr);
  __vfprintf_chk(stderr,1,errbuf,l);
  fputc(10,stderr);
  sleep(1);
  __fprintf_chk(stderr,1,&DAT_00112b84,"Forcing core dump");
  fflush(stderr);
  raise(0xb);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void kbd_close(void)

{
  if (keyboard_type != 0) {
    return;
  }
  ioctl(0,0x4b45,(ulong)(uint)oldkbmode);
  return;
}


void kbd_init(void)

{
  int iVar1;
  uint uVar2;
  long lVar3;
  undefined8 *puVar4;
  byte bVar5;
  
  bVar5 = 0;
  keyboard_type = 0;
  iVar1 = ioctl(0,0x4b44,&oldkbmode);
  if (iVar1 == 0) {
    iVar1 = ioctl(0,0x4b45,2);
    if (iVar1 == 0) {
      uVar2 = fcntl(0,3);
      fcntl(0,4,(ulong)(uVar2 | 0x800));
      lVar3 = 0x10;
      puVar4 = (undefined8 *)keyboard;
      while (lVar3 != 0) {
        lVar3 = lVar3 + -1;
        *puVar4 = 0;
        puVar4 = puVar4 + (ulong)bVar5 * -2 + 1;
      }
      lVar3 = 0x10;
      puVar4 = (undefined8 *)old_keyboard;
      while (lVar3 != 0) {
        lVar3 = lVar3 + -1;
        *puVar4 = 0;
        puVar4 = puVar4 + (ulong)bVar5 * -2 + 1;
      }
      return;
    }
  }
  keyboard_type = 1;
  current_key = 0;
  return;
}


int kbhit(void)

{
  int iVar1;
  long lVar2;
  fd_set *pfVar3;
  long in_FS_OFFSET;
  timeval tv;
  fd_set fds;
  long local_10;
  
  lVar2 = 0x10;
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  pfVar3 = (fd_set *)&fds;
  while (lVar2 != 0) {
    lVar2 = lVar2 + -1;
    pfVar3->fds_bits[0] = 0;
    pfVar3 = (fd_set *)(pfVar3->fds_bits + 1);
  }
  fds.__fds_bits[0] = fds.__fds_bits[0] | 1;
  tv = (timeval)0x0;
  iVar1 = select(1,(fd_set *)&fds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return (int)(uint)(iVar1 != 0);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


/* WARNING: Could not reconcile some variable overlaps */

void load_banner(char **banner)

{
  long lVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  FILE *__stream;
  char *pcVar5;
  size_t sVar6;
  ulong uVar7;
  ulong uVar8;
  uint *puVar9;
  uint *puVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  size_t __size;
  int iVar13;
  long in_FS_OFFSET;
  bool bVar14;
  byte bVar15;
  undefined8 local_450;
  char line [1025];
  long local_40;
  
  bVar15 = 0;
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  __stream = fopen("data/banner.dat","r");
  if (__stream == (FILE *)0x0) {
    shut_down();
    fwrite("Error: Can\'t load file \"data/banner.dat\".\n",1,0x2a,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  pcVar5 = (char *)malloc(1);
  if (pcVar5 == (char *)0x0) {
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    *banner = (char *)0x0;
    do {
      invalidInstructionException();
    } while( true );
  }
  *banner = pcVar5;
  *pcVar5 = '\0';
  while (pcVar5 = fgets(line,0x400,__stream), puVar10 = (uint *)line, pcVar5 != (char *)0x0) {
    do {
      puVar9 = puVar10;
      uVar3 = *puVar9 + 0xfefefeff & ~*puVar9;
      uVar4 = uVar3 & 0x80808080;
      puVar10 = puVar9 + 1;
    } while (uVar4 == 0);
    bVar14 = (uVar3 & 0x8080) == 0;
    if (bVar14) {
      uVar4 = uVar4 >> 0x10;
    }
    if (bVar14) {
      puVar10 = (uint *)((long)puVar9 + 6);
    }
    uVar8 = (long)puVar10 + ((-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)) - (long)line);
    lVar1 = uVar8 - 1;
    if (*(char *)((long)&local_450 + uVar8 + 7) == '\n') {
      line[uVar8 - 1] = '\0';
      puVar10 = (uint *)line;
      do {
        puVar9 = puVar10;
        uVar3 = *puVar9 + 0xfefefeff & ~*puVar9;
        uVar4 = uVar3 & 0x80808080;
        puVar10 = puVar9 + 1;
      } while (uVar4 == 0);
      bVar14 = (uVar3 & 0x8080) == 0;
      if (bVar14) {
        uVar4 = uVar4 >> 0x10;
      }
      if (bVar14) {
        puVar10 = (uint *)((long)puVar9 + 6);
      }
      uVar8 = (long)puVar10 + ((-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)) - (long)line);
      lVar1 = uVar8 - 1;
      cVar2 = line[uVar8 - 1];
    }
    else {
      cVar2 = line[lVar1];
    }
    if (cVar2 == '\r') {
      line[lVar1] = '\0';
      puVar10 = (uint *)line;
      do {
        puVar9 = puVar10;
        uVar3 = *puVar9 + 0xfefefeff & ~*puVar9;
        uVar4 = uVar3 & 0x80808080;
        puVar10 = puVar9 + 1;
      } while (uVar4 == 0);
      bVar14 = (uVar3 & 0x8080) == 0;
      if (bVar14) {
        uVar4 = uVar4 >> 0x10;
      }
      if (bVar14) {
        puVar10 = (uint *)((long)puVar9 + 6);
      }
      uVar8 = (long)puVar10 + ((-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)) - (long)line);
    }
    pcVar5 = *banner;
    sVar6 = strlen(pcVar5);
    uVar4 = (uint)uVar8;
    iVar13 = (int)sVar6 + uVar4;
    __size = SEXT48(SCREEN_X + iVar13 + 1);
    if (pcVar5 == (char *)0xffffffffffffffff) {
      if ((__size != 0) &&
         (local_450 = sVar6, pcVar5 = (char *)malloc(__size), sVar6 = local_450,
         pcVar5 == (char *)0x0)) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        sVar6 = local_450;
      }
    }
    else {
      if (__size == 0) {
        local_450 = sVar6;
        free(pcVar5);
        pcVar5 = (char *)0xffffffffffffffff;
        sVar6 = local_450;
      }
      else {
        local_450 = sVar6;
        pcVar5 = (char *)realloc(pcVar5,__size);
        sVar6 = local_450;
        if (pcVar5 == (char *)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          sVar6 = local_450;
        }
      }
    }
    *banner = pcVar5;
    uVar7 = uVar8 & 0xffffffff;
    puVar11 = (undefined8 *)(pcVar5 + (int)sVar6);
    if (uVar4 < 8) {
      if ((uVar8 & 4) == 0) {
        if ((int)uVar7 != 0) {
          *(char *)puVar11 = line[0];
          if ((uVar8 & 2) != 0) {
            *(undefined2 *)((long)puVar11 + (uVar7 - 2)) = *(undefined2 *)(line + (uVar7 - 2));
          }
        }
      }
      else {
        *(uint *)puVar11 = line._0_4_;
        *(undefined4 *)((long)puVar11 + (uVar7 - 4)) = *(undefined4 *)(line + (uVar7 - 4));
      }
    }
    else {
      *puVar11 = CONCAT44(line._4_4_,line._0_4_);
      *(undefined8 *)((long)puVar11 + ((uVar8 & 0xffffffff) - 8)) =
           *(undefined8 *)(line + ((uVar8 & 0xffffffff) - 8));
      puVar12 = (undefined8 *)
                ((long)puVar11 - (long)(undefined8 *)((ulong)(puVar11 + 1) & 0xfffffffffffffff8));
      uVar7 = (ulong)(uVar4 + (int)puVar12 >> 3);
      puVar12 = (undefined8 *)((long)line - (long)puVar12);
      puVar11 = (undefined8 *)((ulong)(puVar11 + 1) & 0xfffffffffffffff8);
      while (uVar7 != 0) {
        uVar7 = uVar7 - 1;
        *puVar11 = *puVar12;
        puVar12 = puVar12 + (ulong)bVar15 * -2 + 1;
        puVar11 = puVar11 + (ulong)bVar15 * -2 + 1;
      }
    }
    memset(*banner + uVar8 + (long)(int)sVar6,0x20,(long)SCREEN_X);
    (*banner)[iVar13 + SCREEN_X] = '\0';
  }
  fclose(__stream);
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


/* WARNING: Could not reconcile some variable overlaps */

void load_cfg(char *host,char *name,int *color)

{
  char *pcVar1;
  FILE *__stream;
  long lVar2;
  ulong uVar3;
  uint *puVar4;
  uint *puVar5;
  uint uVar6;
  uint uVar7;
  long lVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  long in_FS_OFFSET;
  bool bVar11;
  byte bVar12;
  undefined8 uStack320;
  u_char txt [256];
  long local_30;
  
  bVar12 = 0;
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  uStack320 = (undefined *)0x1034ce;
  pcVar1 = getenv("HOME");
  uStack320 = (undefined *)0x1034f3;
  __sprintf_chk(txt,1,0x100,"%s/%s",pcVar1,".0verkill");
  uStack320 = (undefined *)0x103502;
  __stream = fopen((char *)txt,"r");
  if (__stream != (FILE *)0x0) {
    uStack320 = (undefined *)0x10351e;
    pcVar1 = fgets((char *)txt,0x42,__stream);
    puVar5 = (uint *)txt;
    if (pcVar1 != (char *)0x0) {
      do {
        puVar4 = puVar5;
        uVar6 = *puVar4 + 0xfefefeff & ~*puVar4;
        uVar7 = uVar6 & 0x80808080;
        puVar5 = puVar4 + 1;
      } while (uVar7 == 0);
      bVar11 = (uVar6 & 0x8080) == 0;
      if (bVar11) {
        uVar7 = uVar7 >> 0x10;
      }
      if (bVar11) {
        puVar5 = (uint *)((long)puVar4 + 6);
      }
      lVar2 = (long)puVar5 + ((-3 - (ulong)CARRY1((byte)uVar7,(byte)uVar7)) - (long)txt);
      lVar8 = (long)((int)lVar2 + -1);
      if (txt[lVar8] == '\n') {
        txt[lVar8] = '\0';
        puVar5 = (uint *)txt;
        do {
          puVar4 = puVar5;
          uVar6 = *puVar4 + 0xfefefeff & ~*puVar4;
          uVar7 = uVar6 & 0x80808080;
          puVar5 = puVar4 + 1;
        } while (uVar7 == 0);
        bVar11 = (uVar6 & 0x8080) == 0;
        if (bVar11) {
          uVar7 = uVar7 >> 0x10;
        }
        if (bVar11) {
          puVar5 = (uint *)((long)puVar4 + 6);
        }
        lVar2 = (long)puVar5 + ((-3 - (ulong)CARRY1((byte)uVar7,(byte)uVar7)) - (long)txt);
      }
      uVar3 = lVar2 + 1;
      uVar7 = (uint)uVar3;
      if (uVar7 < 8) {
        if ((uVar3 & 4) == 0) {
          if (uVar7 != 0) {
            *host = txt[0];
            if ((uVar3 & 2) != 0) {
              *(undefined2 *)(host + ((uVar3 & 0xffffffff) - 2)) =
                   *(undefined2 *)(txt + ((uVar3 & 0xffffffff) - 2));
            }
          }
        }
        else {
          *(uint *)host = txt._0_4_;
          *(undefined4 *)(host + ((uVar3 & 0xffffffff) - 4)) =
               *(undefined4 *)(txt + ((uVar3 & 0xffffffff) - 4));
        }
      }
      else {
        *(ulong *)host = CONCAT44(txt._4_4_,txt._0_4_);
        *(undefined8 *)(host + ((uVar3 & 0xffffffff) - 8)) =
             *(undefined8 *)(txt + ((uVar3 & 0xffffffff) - 8));
        uVar3 = (ulong)(uVar7 + (int)(host + -(long)(undefined8 *)
                                                    ((ulong)(host + 8) & 0xfffffffffffffff8)) >> 3);
        puVar9 = (undefined8 *)
                 (txt + -(long)(host + -(long)(undefined8 *)((ulong)(host + 8) & 0xfffffffffffffff8)
                               ));
        puVar10 = (undefined8 *)((ulong)(host + 8) & 0xfffffffffffffff8);
        while (uVar3 != 0) {
          uVar3 = uVar3 - 1;
          *puVar10 = *puVar9;
          puVar9 = puVar9 + (ulong)bVar12 * -2 + 1;
          puVar10 = puVar10 + (ulong)bVar12 * -2 + 1;
        }
      }
      uStack320 = (undefined *)0x1035c1;
      pcVar1 = fgets((char *)txt,0x1a,__stream);
      puVar5 = (uint *)txt;
      if (pcVar1 != (char *)0x0) {
        do {
          puVar4 = puVar5;
          uVar6 = *puVar4 + 0xfefefeff & ~*puVar4;
          uVar7 = uVar6 & 0x80808080;
          puVar5 = puVar4 + 1;
        } while (uVar7 == 0);
        bVar11 = (uVar6 & 0x8080) == 0;
        if (bVar11) {
          uVar7 = uVar7 >> 0x10;
        }
        if (bVar11) {
          puVar5 = (uint *)((long)puVar4 + 6);
        }
        lVar2 = (long)puVar5 + ((-3 - (ulong)CARRY1((byte)uVar7,(byte)uVar7)) - (long)txt);
        lVar8 = (long)((int)lVar2 + -1);
        if (txt[lVar8] == '\n') {
          txt[lVar8] = '\0';
          puVar5 = (uint *)txt;
          do {
            puVar4 = puVar5;
            uVar6 = *puVar4 + 0xfefefeff & ~*puVar4;
            uVar7 = uVar6 & 0x80808080;
            puVar5 = puVar4 + 1;
          } while (uVar7 == 0);
          bVar11 = (uVar6 & 0x8080) == 0;
          if (bVar11) {
            uVar7 = uVar7 >> 0x10;
          }
          if (bVar11) {
            puVar5 = (uint *)((long)puVar4 + 6);
          }
          lVar2 = (long)puVar5 + ((-3 - (ulong)CARRY1((byte)uVar7,(byte)uVar7)) - (long)txt);
        }
        uVar3 = lVar2 + 1;
        uVar7 = (uint)uVar3;
        if (uVar7 < 8) {
          if ((uVar3 & 4) == 0) {
            if (uVar7 != 0) {
              *name = txt[0];
              if ((uVar3 & 2) != 0) {
                *(undefined2 *)(name + ((uVar3 & 0xffffffff) - 2)) =
                     *(undefined2 *)(txt + ((uVar3 & 0xffffffff) - 2));
              }
            }
          }
          else {
            *(uint *)name = txt._0_4_;
            *(undefined4 *)(name + ((uVar3 & 0xffffffff) - 4)) =
                 *(undefined4 *)(txt + ((uVar3 & 0xffffffff) - 4));
          }
        }
        else {
          *(ulong *)name = CONCAT44(txt._4_4_,txt._0_4_);
          *(undefined8 *)(name + ((uVar3 & 0xffffffff) - 8)) =
               *(undefined8 *)(txt + ((uVar3 & 0xffffffff) - 8));
          uVar3 = (ulong)(uVar7 + (int)(name + -(long)(undefined8 *)
                                                      ((ulong)(name + 8) & 0xfffffffffffffff8)) >> 3
                         );
          puVar9 = (undefined8 *)
                   (txt + -(long)(name + -(long)(undefined8 *)
                                                ((ulong)(name + 8) & 0xfffffffffffffff8)));
          puVar10 = (undefined8 *)((ulong)(name + 8) & 0xfffffffffffffff8);
          while (uVar3 != 0) {
            uVar3 = uVar3 - 1;
            *puVar10 = *puVar9;
            puVar9 = puVar9 + (ulong)bVar12 * -2 + 1;
            puVar10 = puVar10 + (ulong)bVar12 * -2 + 1;
          }
        }
        uStack320 = (undefined *)0x103644;
        pcVar1 = fgets((char *)txt,0x100,__stream);
        if (pcVar1 != (char *)0x0) {
          uStack320 = (undefined *)0x103658;
          lVar2 = strtol((char *)txt,(char **)0x0,10);
          *color = (int)lVar2;
          uStack320 = (undefined *)0x103663;
          fclose(__stream);
        }
      }
    }
  }
  if (local_30 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    uStack320 = &UNK_001037fb;
    __stack_chk_fail();
  }
  return;
}


void load_data(u_char *filename)

{
  uint uVar1;
  char cVar2;
  short sVar3;
  long lVar4;
  u_char **ppuVar5;
  char *pcVar6;
  u_char *puVar7;
  u_char uVar8;
  int iVar9;
  FILE *__stream;
  char *pcVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  long in_FS_OFFSET;
  bool bVar14;
  int local_170;
  int local_168;
  u_char local_15a;
  char *p;
  char *q;
  char msg [256];
  
  lVar4 = *(long *)(in_FS_OFFSET + 0x28);
  __stream = fopen((char *)filename,"rb");
  if (__stream == (FILE *)0x0) {
    pcVar10 = "Can\'t open file \"%s\"!\n";
LAB_00108d7e:
    __snprintf_chk(msg,0x100,1,0x100,pcVar10,filename);
LAB_00108d1d:
    __fprintf_chk(stderr,1,msg);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
LAB_00108abe:
  do {
    do {
      pcVar10 = fgets(load_data::line,0x400,__stream);
      if (pcVar10 == (char *)0x0) {
        fclose(__stream);
        if (lVar4 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        return;
      }
      p = load_data::line;
      if (((byte)(load_data::line[0] - 9U) < 2) ||
         (uVar8 = load_data::line[0], filename = (u_char *)p, load_data::line[0] == ' ')) {
        puVar7 = (u_char *)(load_data::line + 1);
        do {
          do {
            p = (char *)puVar7;
            uVar8 = *p;
            puVar7 = (u_char *)p + 1;
          } while ((byte)(uVar8 - 9) < 2);
          filename = (u_char *)p;
        } while (uVar8 == ' ');
      }
      do {
        if (uVar8 == '\n') goto LAB_00108b44;
        if (uVar8 == '\0') goto LAB_00108abe;
        p = p + 1;
        uVar8 = *p;
      } while ((uVar8 != ' ') && (uVar8 != '\t'));
    } while (uVar8 == '\0');
LAB_00108b44:
    *p = '\0';
    pcVar10 = p + 1;
    cVar2 = p[1];
    if (((byte)(cVar2 - 9U) < 2) || (cVar2 == ' ')) {
      pcVar6 = p + 2;
      do {
        do {
          pcVar10 = pcVar6;
          cVar2 = *pcVar10;
          pcVar6 = pcVar10 + 1;
        } while ((byte)(cVar2 - 9U) < 2);
      } while (cVar2 == ' ');
    }
    if ((0x46 < (byte)(cVar2 - 0x31U)) ||
       (sVar3 = *(short *)(CSWTCH_138 + (ulong)(byte)(cVar2 - 0x31U) * 2), sVar3 < 0)) {
      p = pcVar10;
      __snprintf_chk(msg,0x100,1,0x100,"Unknown object type \'%c\'.\n");
      goto LAB_00108d1d;
    }
    p = pcVar10 + 1;
    if (((byte)(pcVar10[1] - 9U) < 2) || (pcVar10[1] == ' ')) {
      pcVar10 = pcVar10 + 2;
      do {
        do {
          p = pcVar10;
          pcVar10 = p + 1;
        } while ((byte)(*p - 9U) < 2);
      } while (*p == ' ');
    }
    lVar11 = strtol(p,&q,0);
    if (((byte)(*q - 9U) < 2) || (*q == ' ')) {
      pcVar10 = q + 1;
      do {
        do {
          q = pcVar10;
          pcVar10 = q + 1;
        } while ((byte)(*q - 9U) < 2);
      } while (*q == ' ');
    }
    lVar12 = strtol(q,&p,0);
    ppuVar5 = sprite_names;
    if (n_sprites < 1) {
LAB_00108d99:
      pcVar10 = "Unknown bitmap name \"%s\"!\n";
      goto LAB_00108d7e;
    }
    uVar1 = n_sprites - 1;
    uVar13 = 0;
    while (iVar9 = strcmp((char *)ppuVar5[uVar13],(char *)filename), iVar9 != 0) {
      bVar14 = uVar1 == uVar13;
      uVar13 = uVar13 + 1;
      if (bVar14) goto LAB_00108d99;
    }
    local_15a = (u_char)sVar3;
    local_168 = (int)lVar12;
    local_170 = (int)lVar11;
    _put_sprite(1000,500,area,area_a,local_170,local_168,sprites[uVar13].positions,local_15a,'\0');
  } while( true );
}


u_char * load_level(int level_num)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  FILE *__stream;
  char *pcVar4;
  uint *puVar5;
  uint *puVar6;
  long lVar7;
  undefined8 *__ptr;
  ulong uVar8;
  long lVar9;
  ulong uVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  int iVar13;
  long in_FS_OFFSET;
  bool bVar14;
  byte bVar15;
  undefined8 uStack1088;
  u_char txt [1024];
  long local_30;
  
  bVar15 = 0;
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  uStack1088 = (code *)0x109539;
  __stream = fopen("data/level.dat","r");
  if (__stream != (FILE *)0x0) {
    iVar13 = 0;
    if (-1 < level_num) {
      do {
        uStack1088 = (code *)0x109583;
        pcVar4 = fgets((char *)txt,0x400,__stream);
        puVar6 = (uint *)txt;
        if (pcVar4 == (char *)0x0) goto LAB_00109640;
        do {
          puVar5 = puVar6;
          uVar2 = *puVar5 + 0xfefefeff & ~*puVar5;
          uVar3 = uVar2 & 0x80808080;
          puVar6 = puVar5 + 1;
        } while (uVar3 == 0);
        bVar14 = (uVar2 & 0x8080) == 0;
        if (bVar14) {
          uVar3 = uVar3 >> 0x10;
        }
        if (bVar14) {
          puVar6 = (uint *)((long)puVar5 + 6);
        }
        lVar9 = (long)puVar6 + ((-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)) - (long)txt);
        lVar7 = lVar9 + -1;
        if (txt[lVar9 + -1] == '\n') {
          txt[lVar7] = '\0';
          puVar6 = (uint *)txt;
          do {
            puVar5 = puVar6;
            uVar2 = *puVar5 + 0xfefefeff & ~*puVar5;
            uVar3 = uVar2 & 0x80808080;
            puVar6 = puVar5 + 1;
          } while (uVar3 == 0);
          bVar14 = (uVar2 & 0x8080) == 0;
          if (bVar14) {
            uVar3 = uVar3 >> 0x10;
          }
          if (bVar14) {
            puVar6 = (uint *)((long)puVar5 + 6);
          }
          lVar7 = (long)puVar6 + (-(long)txt - (ulong)CARRY1((byte)uVar3,(byte)uVar3)) + -4;
        }
        if (txt[lVar7] == '\r') {
          txt[lVar7] = '\0';
        }
        iVar13 = (iVar13 + 1) - (uint)(txt[0] == '\0');
      } while (iVar13 <= level_num);
    }
    uStack1088 = (code *)0x10967b;
    fclose(__stream);
    puVar6 = (uint *)txt;
    do {
      puVar5 = puVar6;
      uVar2 = *puVar5 + 0xfefefeff & ~*puVar5;
      uVar3 = uVar2 & 0x80808080;
      puVar6 = puVar5 + 1;
    } while (uVar3 == 0);
    bVar14 = (uVar2 & 0x8080) == 0;
    if (bVar14) {
      uVar3 = uVar3 >> 0x10;
    }
    if (bVar14) {
      puVar6 = (uint *)((long)puVar5 + 6);
    }
    uVar10 = (long)puVar6 + ((-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)) - (long)txt);
    uStack1088 = (code *)0x1096bc;
    __ptr = (undefined8 *)malloc(uVar10 + 1);
    if (__ptr == (undefined8 *)0x0) {
      uStack1088 = (code *)0x109736;
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      goto LAB_00109643;
    }
    uVar3 = (int)uVar10 + 1;
    uVar8 = (ulong)uVar3;
    if (uVar3 < 8) {
      if ((uVar3 & 4) == 0) {
        if (uVar3 != 0) {
          *(u_char *)__ptr = txt[0];
          if ((uVar3 & 2) != 0) {
            *(undefined2 *)((long)__ptr + (uVar8 - 2)) = *(undefined2 *)(txt + (uVar8 - 2));
          }
        }
      }
      else {
        *(uint *)__ptr = txt._0_4_;
        *(undefined4 *)((long)__ptr + (uVar8 - 4)) = *(undefined4 *)(txt + (uVar8 - 4));
      }
      cVar1 = *(char *)__ptr;
    }
    else {
      uVar10 = uVar10 >> 3 & 0x1fffffff;
      *(undefined8 *)((long)__ptr + (uVar8 - 8)) = *(undefined8 *)(txt + (uVar8 - 8));
      puVar11 = (undefined8 *)txt;
      puVar12 = __ptr;
      while (uVar10 != 0) {
        uVar10 = uVar10 - 1;
        *puVar12 = *puVar11;
        puVar11 = puVar11 + (ulong)bVar15 * -2 + 1;
        puVar12 = puVar12 + (ulong)bVar15 * -2 + 1;
      }
      cVar1 = *(char *)__ptr;
    }
    if (cVar1 != '\0') goto LAB_00109643;
    uStack1088 = (code *)0x10963f;
    free(__ptr);
  }
LAB_00109640:
  __ptr = (undefined8 *)0x0;
LAB_00109643:
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return (u_char *)__ptr;
  }
                    /* WARNING: Subroutine does not return */
  uStack1088 = new_obj;
  __stack_chk_fail();
}


void load_sprite(u_char *filename,sprite *s)

{
  long lVar1;
  u_char uVar2;
  char cVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  FILE *__stream;
  char *pcVar7;
  u_char *puVar8;
  size_t sVar9;
  __int32_t **pp_Var10;
  pos *ppVar11;
  ushort *__ptr;
  line *plVar12;
  line *plVar13;
  uint *puVar14;
  uint *puVar15;
  long lVar16;
  uint uVar17;
  int iVar18;
  ulong uVar19;
  pos *ppVar20;
  long in_FS_OFFSET;
  bool bVar21;
  undefined8 uVar22;
  char *p;
  char *q;
  u_char msg [256];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  s->positions = (pos *)0xffffffffffffffff;
  s->n_positions = 0;
  __stream = fopen((char *)filename,"rb");
  if (__stream != (FILE *)0x0) {
    uVar17 = 0;
LAB_0010ac70:
    pcVar7 = fgets((char *)load_sprite::buffer,0x1fff,__stream);
    if (pcVar7 == (char *)0x0) {
      if (uVar17 == 4) {
        fclose(__stream);
        if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        return;
      }
      pcVar7 = "Unexpected end of file in \"%s\".\n";
      goto LAB_0010ae81;
    }
    puVar15 = (uint *)load_sprite::buffer;
    do {
      puVar14 = puVar15;
      uVar4 = *puVar14 + 0xfefefeff & ~*puVar14;
      uVar5 = uVar4 & 0x80808080;
      puVar15 = puVar14 + 1;
    } while (uVar5 == 0);
    bVar21 = (uVar4 & 0x8080) == 0;
    if (bVar21) {
      uVar5 = uVar5 >> 0x10;
    }
    if (bVar21) {
      puVar15 = (uint *)((long)puVar14 + 6);
    }
    lVar16 = (long)(int)(((int)puVar15 - (uint)CARRY1((byte)uVar5,(byte)uVar5)) + -0x118dc4);
    if (load_sprite::buffer[lVar16] == '\n') {
      load_sprite::buffer[lVar16] = '\0';
      bVar21 = load_sprite::buffer[0] == 'l';
      if (!bVar21) goto LAB_0010acf0;
LAB_0010b0e3:
      if ((uVar17 & 0xfffffffd) != 1) goto LAB_0010b4d8;
      ppVar11 = s->positions + (long)s->n_positions + -1;
      iVar6 = ppVar11->n + 1;
      sVar9 = (long)iVar6 * 0x18;
      ppVar20 = ppVar11;
      if (ppVar11->n == 0) {
        ppVar11->lines = (line *)0xffffffffffffffff;
        ppVar11->n = 1;
LAB_0010b127:
        plVar12 = (line *)malloc(sVar9);
        if (plVar12 == (line *)0x0) {
          error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
          ppVar20 = s->positions + (long)s->n_positions + -1;
        }
      }
      else {
        plVar12 = ppVar11->lines;
        ppVar11->n = iVar6;
        if (plVar12 == (line *)0xffffffffffffffff) {
          if (sVar9 != 0) goto LAB_0010b127;
        }
        else {
          if (plVar12 == (line *)0x0) {
            errline = 0x56;
            errfile = "error.h";
            int_error((u_char *)"mem_realloc(NULL, %d)");
            ppVar20 = s->positions + (long)s->n_positions + -1;
          }
          else {
            if (sVar9 == 0) {
              free(plVar12);
              plVar12 = (line *)0xffffffffffffffff;
              ppVar20 = s->positions + (long)s->n_positions + -1;
            }
            else {
              plVar12 = (line *)realloc(plVar12,sVar9);
              if (plVar12 == (line *)0x0) {
                error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
              }
              ppVar20 = s->positions + (long)s->n_positions + -1;
            }
          }
        }
      }
      ppVar11->lines = plVar12;
      plVar12 = ppVar20->lines;
      if (plVar12 != (line *)0x0) {
        sVar9 = strlen((char *)(load_sprite::buffer + 1));
        plVar12 = plVar12 + (long)ppVar20->n + -1;
        uVar17 = 2;
        iVar6 = (int)sVar9;
        plVar12->len = iVar6;
        if (iVar6 == 0) goto LAB_0010ac70;
        puVar8 = (u_char *)malloc((long)iVar6);
        plVar13 = plVar12;
        if (puVar8 == (u_char *)0x0) {
          error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
          plVar13 = s->positions[(long)s->n_positions + -1].lines +
                    (long)s->positions[(long)s->n_positions + -1].n + -1;
        }
        plVar12->bmp = puVar8;
        if (plVar13->bmp != (u_char *)0x0) {
          uVar17 = 2;
          memcpy(plVar13->bmp,load_sprite::buffer + 1,(long)plVar13->len);
          goto LAB_0010ac70;
        }
      }
    }
    else {
      bVar21 = load_sprite::buffer[0] == 'l';
      if (bVar21) goto LAB_0010b0e3;
LAB_0010acf0:
      if (0x6b < load_sprite::buffer[0] && !bVar21) {
        if (load_sprite::buffer[0] != 'p') {
          if (load_sprite::buffer[0] == 's') {
            if (uVar17 == 3) {
              q = (char *)load_sprite::buffer;
              do {
                p = q + 1;
                cVar3 = *q;
                do {
                  if ((cVar3 == '\0') || (cVar3 == '\n')) {
                    uVar17 = 4;
                    goto LAB_0010ac70;
                  }
                  uVar22 = 0x10b039;
                  lVar16 = strtol(p,&q,0);
                  iVar6 = (int)lVar16;
                  if ((iVar6 < 0) || (s->n_positions == iVar6 || s->n_positions < iVar6)) {
                    __snprintf_chk(msg,0x100,1,0x100,
                                   "Error loading sprite \"%s\". Undefined position %d.\n",filename,
                                   lVar16,uVar22);
                    __fprintf_chk(stderr,1,msg);
                    /* WARNING: Subroutine does not return */
                    exit(1);
                  }
                  iVar6 = s->n_steps + 1;
                  sVar9 = (long)iVar6 * 2;
                  if (s->n_steps == 0) {
                    s->steps = (ushort *)0xffffffffffffffff;
                    s->n_steps = 1;
LAB_0010afe0:
                    __ptr = (ushort *)malloc(sVar9);
                    if (__ptr == (ushort *)0x0) {
                      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
                      s->steps = (ushort *)0x0;
                      goto LAB_0010b1ee;
                    }
LAB_0010b000:
                    s->steps = __ptr;
                  }
                  else {
                    __ptr = s->steps;
                    s->n_steps = iVar6;
                    if (__ptr != (ushort *)0xffffffffffffffff) {
                      if (__ptr == (ushort *)0x0) {
                        errline = 0x56;
                        errfile = "error.h";
                        int_error((u_char *)"mem_realloc(NULL, %d)");
                        s->steps = (ushort *)0x0;
                      }
                      else {
                        if (sVar9 == 0) {
                          free(__ptr);
                          s->steps = (ushort *)0xffffffffffffffff;
                          __ptr = (ushort *)0xffffffffffffffff;
                          sVar9 = (long)s->n_steps * 2;
                          goto LAB_0010b004;
                        }
                        __ptr = (ushort *)realloc(__ptr,sVar9);
                        if (__ptr != (ushort *)0x0) {
                          sVar9 = (long)s->n_steps * 2;
                          goto LAB_0010b000;
                        }
                        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
                        s->steps = (ushort *)0x0;
                      }
                      goto LAB_0010b1ee;
                    }
                    if (sVar9 != 0) goto LAB_0010afe0;
                  }
LAB_0010b004:
                  *(short *)((long)__ptr + (sVar9 - 2)) = (short)lVar16;
                  cVar3 = *q;
                } while (cVar3 != ',');
              } while( true );
            }
            goto LAB_0010b4d8;
          }
          goto LAB_0010ae5e;
        }
        if ((uVar17 == 0) || (uVar17 == 3)) {
          ppVar11 = s->positions;
          iVar6 = s->n_positions + 1;
          s->n_positions = iVar6;
          sVar9 = (long)iVar6 * 0x18;
          if (ppVar11 == (pos *)0xffffffffffffffff) {
            if (sVar9 != 0) {
              ppVar11 = (pos *)malloc(sVar9);
              if (ppVar11 == (pos *)0x0) {
                error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
                s->positions = (pos *)0x0;
                goto LAB_0010b1ee;
              }
LAB_0010af17:
              s->positions = ppVar11;
            }
          }
          else {
            if (ppVar11 == (pos *)0x0) {
              errline = 0x56;
              errfile = "error.h";
              int_error((u_char *)"mem_realloc(NULL, %d)");
              s->positions = (pos *)0x0;
              goto LAB_0010b1ee;
            }
            if (sVar9 != 0) {
              ppVar11 = (pos *)realloc(ppVar11,sVar9);
              if (ppVar11 == (pos *)0x0) {
                error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
                s->positions = (pos *)0x0;
                goto LAB_0010b1ee;
              }
              sVar9 = (long)s->n_positions * 0x18;
              goto LAB_0010af17;
            }
            free(ppVar11);
            s->positions = (pos *)0xffffffffffffffff;
            ppVar11 = (pos *)0xffffffffffffffff;
            sVar9 = (long)s->n_positions * 0x18;
          }
          lVar16 = (long)&ppVar11[-1].xo + sVar9;
          *(undefined4 *)(lVar16 + 8) = 0;
          *(undefined8 *)(lVar16 + 0x10) = 0;
          s->n_steps = 0;
          s->steps = (ushort *)0x0;
          lVar16 = strtol((char *)(load_sprite::buffer + 1),&p,0);
          s->positions[(long)s->n_positions + -1].xo = (int)lVar16;
          lVar16 = strtol(p + 1,&q,0);
          uVar17 = 1;
          s->positions[(long)s->n_positions + -1].yo = (int)lVar16;
          goto LAB_0010ac70;
        }
LAB_0010b4d8:
        pcVar7 = "Syntax error in file \"%s\".\n";
        goto LAB_0010ae81;
      }
      if (load_sprite::buffer[0] == '#') goto LAB_0010ac70;
      if (load_sprite::buffer[0] != 'a') {
        if (load_sprite::buffer[0] != '\0') goto LAB_0010ae5e;
        goto LAB_0010ac70;
      }
      if (uVar17 != 2) goto LAB_0010b4d8;
      plVar12 = s->positions[(long)s->n_positions + -1].lines;
      if (plVar12 == (line *)0x0) goto LAB_0010b1ee;
      uVar17 = 3;
      plVar12 = plVar12 + (long)s->positions[(long)s->n_positions + -1].n + -1;
      if (plVar12->len == 0) goto LAB_0010ac70;
      puVar8 = (u_char *)malloc((long)plVar12->len);
      plVar13 = plVar12;
      if (puVar8 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        plVar13 = s->positions[(long)s->n_positions + -1].lines +
                  (long)s->positions[(long)s->n_positions + -1].n + -1;
      }
      plVar12->attr = puVar8;
      puVar8 = plVar13->attr;
      if (puVar8 != (u_char *)0x0) {
        sVar9 = strlen((char *)(load_sprite::buffer + 1));
        iVar6 = plVar13->len;
        iVar18 = (int)sVar9;
        if (iVar6 <= (int)sVar9) {
          iVar18 = iVar6;
        }
        if (0 < iVar18) {
          pp_Var10 = __ctype_tolower_loc();
          uVar19 = 0;
          while( true ) {
            iVar6 = (*pp_Var10)[load_sprite::buffer[uVar19 + 1]];
            if (iVar6 - 0x61U < 6) {
              puVar8[uVar19] = (char)iVar6 + -0x57;
              iVar6 = s->n_positions;
            }
            else {
              uVar2 = (u_char)(iVar6 - 0x30U);
              if (9 < iVar6 - 0x30U) {
                uVar2 = '\0';
              }
              puVar8[uVar19] = uVar2;
              iVar6 = s->n_positions;
            }
            if (uVar19 == iVar18 - 1) break;
            uVar19 = uVar19 + 1;
            puVar8 = s->positions[(long)iVar6 + -1].lines
                     [(long)s->positions[(long)iVar6 + -1].n + -1].attr;
          }
          plVar12 = s->positions[(long)iVar6 + -1].lines +
                    (long)s->positions[(long)iVar6 + -1].n + -1;
          iVar6 = plVar12->len;
          puVar8 = plVar12->attr;
        }
        uVar17 = 3;
        memset(puVar8 + iVar18,0,(long)(iVar6 - iVar18));
        goto LAB_0010ac70;
      }
    }
LAB_0010b1ee:
    fwrite("Memory allocation error!\n",1,0x19,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  pcVar7 = "Error opening file \"%s\"!\n";
  goto LAB_0010ae81;
LAB_0010ae5e:
  pcVar7 = "Syntax error in file \"%s\"!\n";
LAB_0010ae81:
  __snprintf_chk(msg,0x100,1,0x100,pcVar7,filename);
  __fprintf_chk(stderr,1,msg);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void load_sprites(u_char *filename)

{
  long lVar1;
  char cVar2;
  byte bVar3;
  FILE *__stream;
  char *pcVar4;
  size_t sVar5;
  u_char **ppuVar6;
  sprite *psVar7;
  u_char *puVar8;
  u_char **ppuVar9;
  long lVar10;
  char *pcVar11;
  size_t sVar12;
  u_char *filename_00;
  long in_FS_OFFSET;
  char msg [256];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __stream = fopen((char *)filename,"rb");
  if (__stream == (FILE *)0x0) {
    __snprintf_chk(msg,0x100,1,0x100,"Can\'t open file \"%s\"!\n",filename);
    __fprintf_chk(stderr,1,msg);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
LAB_00108e00:
  do {
    do {
      pcVar4 = fgets(load_sprites::line,0x400,__stream);
      if (pcVar4 == (char *)0x0) {
        fclose(__stream);
        if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        return;
      }
      pcVar4 = load_sprites::line;
      cVar2 = load_sprites::line[0];
      if (1 < (byte)(load_sprites::line[0] - 9U)) goto LAB_00108e40;
      do {
        do {
          cVar2 = pcVar4[1];
          pcVar4 = pcVar4 + 1;
        } while ((byte)(cVar2 - 9U) < 2);
LAB_00108e40:
        pcVar11 = pcVar4;
      } while (cVar2 == ' ');
      do {
        if (cVar2 == '\n') goto LAB_00108e70;
        if (cVar2 == '\0') goto LAB_00108e00;
        cVar2 = pcVar11[1];
        pcVar11 = pcVar11 + 1;
      } while ((cVar2 != ' ') && (cVar2 != '\t'));
    } while (cVar2 == '\0');
LAB_00108e70:
    *pcVar11 = '\0';
    filename_00 = (u_char *)(pcVar11 + 1);
    sVar5 = strlen(pcVar4);
    n_sprites = n_sprites + 1;
    lVar10 = (long)n_sprites;
    sVar12 = lVar10 * 8;
    if (sprite_names == (u_char **)0xffffffffffffffff) {
      ppuVar6 = sprite_names;
      if ((sVar12 != 0) && (ppuVar6 = (u_char **)malloc(sVar12), ppuVar6 == (u_char **)0x0)) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        sprite_names = (u_char **)0x0;
        goto LAB_00109142;
      }
    }
    else {
      if (sprite_names == (u_char **)0x0) {
        errline = 0x56;
        errfile = "error.h";
        int_error((u_char *)"mem_realloc(NULL, %d)");
        sprite_names = (u_char **)0x0;
        goto LAB_00109142;
      }
      if (sVar12 == 0) {
        free(sprite_names);
        lVar10 = (long)n_sprites;
        sprite_names = (u_char **)0xffffffffffffffff;
        ppuVar6 = sprite_names;
      }
      else {
        ppuVar6 = (u_char **)realloc(sprite_names,sVar12);
        if (ppuVar6 == (u_char **)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          sprite_names = (u_char **)0x0;
          goto LAB_00109142;
        }
        lVar10 = (long)n_sprites;
      }
    }
    sprite_names = ppuVar6;
    ppuVar6 = sprite_names;
    sVar12 = lVar10 << 5;
    if (sprites == (sprite *)0xffffffffffffffff) {
      psVar7 = sprites;
      if ((sVar12 != 0) && (psVar7 = (sprite *)malloc(sVar12), psVar7 == (sprite *)0x0)) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        sprites = (sprite *)0x0;
        goto LAB_00109142;
      }
    }
    else {
      if (sprites == (sprite *)0x0) {
        errline = 0x56;
        errfile = "error.h";
        int_error((u_char *)"mem_realloc(NULL, %d)");
        sprites = (sprite *)0x0;
        goto LAB_00109142;
      }
      if (sVar12 == 0) {
        free(sprites);
        lVar10 = (long)n_sprites;
        sprites = (sprite *)0xffffffffffffffff;
        ppuVar6 = sprite_names;
        psVar7 = sprites;
      }
      else {
        psVar7 = (sprite *)realloc(sprites,sVar12);
        if (psVar7 == (sprite *)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          sprites = (sprite *)0x0;
          goto LAB_00109142;
        }
        lVar10 = (long)n_sprites;
        ppuVar6 = sprite_names;
      }
    }
    sprites = psVar7;
    ppuVar6 = ppuVar6 + lVar10 + -1;
    sVar12 = SEXT48((int)sVar5 + 1);
    ppuVar9 = ppuVar6;
    if (sVar12 == 0) {
      puVar8 = (u_char *)0xffffffffffffffff;
    }
    else {
      puVar8 = (u_char *)malloc(sVar12);
      if (puVar8 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        ppuVar9 = sprite_names + (long)n_sprites + -1;
      }
    }
    *ppuVar6 = puVar8;
    if (*ppuVar9 == (u_char *)0x0) {
LAB_00109142:
      fwrite("Memory allocation error!\n",1,0x19,stderr);
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    memcpy(*ppuVar9,pcVar4,sVar12);
    bVar3 = pcVar11[1];
    if (1 < (byte)(bVar3 - 9)) goto LAB_00108fa1;
    do {
      do {
        bVar3 = filename_00[1];
        filename_00 = filename_00 + 1;
      } while ((byte)(bVar3 - 9) < 2);
LAB_00108fa1:
      puVar8 = filename_00;
    } while (bVar3 == 0x20);
    do {
      if ((bVar3 & 0xdf) == 0) break;
      bVar3 = puVar8[1];
      puVar8 = puVar8 + 1;
    } while (1 < (byte)(bVar3 - 9));
    *puVar8 = '\0';
    load_sprite(filename_00,sprites + (long)n_sprites + -1);
  } while( true );
}


/* WARNING: Could not reconcile some variable overlaps */

int main(int argc,char **argv)

{
  object_list *poVar1;
  char *__ptr;
  int iVar2;
  int iVar3;
  object_list *poVar4;
  long lVar5;
  uint uVar6;
  msgline_type *pmVar7;
  int *num;
  ushort port;
  int color;
  char name [25];
  char host [65];
  u_char txt [256];
  
  host._0_16_ = (undefined  [16])0x0;
  host._16_16_ = (undefined  [16])0x0;
  host._32_16_ = (undefined  [16])0x0;
  host._48_16_ = (undefined  [16])0x0;
  name._0_16_ = (undefined  [16])0x0;
  port = 0x1a0a;
  color = 1;
  last_message = -1;
  autorun = '\0';
  autocreep = '\0';
  host[64] = '\0';
  name._16_8_ = 0;
  name[24] = '\0';
  load_cfg(host,name,&color);
  last_obj = &objects;
  error_message[0] = '\0';
  parse_command_line(argc,argv);
  hash_table_init();
  if (set_size == '\0') {
    c_get_size(&SCREEN_X,&SCREEN_Y);
  }
  init_sprites();
  init_area();
  load_sprites((u_char *)"data/sprites.dat");
  level_sprites_start = n_sprites;
  iVar2 = find_sprite("hit",&hit_sprite);
  if (iVar2 != 0) {
    fwrite("Error: Can\'t find sprite \"hit\".\n",1,0x20,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  iVar2 = find_sprite((u_char *)"title",&title_sprite);
  if (iVar2 != 0) {
    fwrite("Error: Can\'t find sprite \"title\".\n",1,0x22,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  iVar2 = find_sprite((u_char *)"bulge",&bulge_sprite);
  num = shrapnel_sprite;
  if (iVar2 == 0) {
    do {
      iVar2 = iVar2 + 1;
      __sprintf_chk(txt,1,0x100,"shrapnel%d",iVar2);
      iVar3 = find_sprite(txt,num);
      if (iVar3 != 0) {
        __fprintf_chk(stderr,1,"Can\'t find sprite \"%s\".\n",txt);
                    /* WARNING: Subroutine does not return */
        exit(1);
      }
      num = num + 1;
    } while (iVar2 != 10);
    signal(2,signal_handler);
    signal(0xf,signal_handler);
    signal(4,signal_handler);
    signal(6,signal_handler);
    signal(8,signal_handler);
    signal(0xb,signal_handler);
    signal(3,signal_handler);
    signal(7,signal_handler);
    signal(0x1c,sigwinch_handler);
    c_init(SCREEN_X,SCREEN_Y);
    c_cls();
    c_cursor(1);
    c_refresh();
    do {
      level_number = -1;
      connected = 0;
      menu_screen(host,name,&port,&color);
      connected = 1;
      play();
      poVar1 = &objects;
      while (poVar4 = poVar1, poVar1 = poVar4->next, poVar1 != (object_list *)0x0) {
        uVar6 = *(uint *)&(poVar1->member).type >> 8;
        if (*(uint *)&hero->type >> 8 != uVar6) {
          delete_obj((ulong)uVar6);
          poVar1 = poVar4;
        }
      }
      lVar5 = (long)last_message;
      if (-1 < last_message) {
        pmVar7 = msg_line;
        iVar2 = 0;
        do {
          __ptr = pmVar7->msg;
          if (__ptr != (char *)0xffffffffffffffff) {
            if (__ptr == (char *)0x0) {
              errline = 0x4c;
              errfile = "error.h";
              int_error((u_char *)"mem_free(NULL)");
              lVar5 = (long)last_message;
            }
            else {
              free(__ptr);
              lVar5 = (long)last_message;
            }
          }
          iVar2 = iVar2 + 1;
          pmVar7 = pmVar7 + 1;
        } while (iVar2 <= (int)lVar5);
      }
      uVar6 = *(uint *)&hero->type;
      last_message = -1;
      msg_line[lVar5].msg = (char *)0x0;
      delete_obj((ulong)(uVar6 >> 8));
    } while( true );
  }
  fwrite("Error: Can\'t find sprite \"bulge\".\n",1,0x22,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


char * MD5Data(u_char *data,uint len,char *buf)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  char cVar8;
  char cVar9;
  char cVar10;
  char cVar11;
  char cVar12;
  char cVar13;
  char cVar14;
  long lVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  long in_FS_OFFSET;
  undefined4 local_108;
  undefined4 uStack256;
  MD5_CTX ctx;
  u_char digest [16];
  
  lVar15 = *(long *)(in_FS_OFFSET + 0x28);
  MD5Init(&ctx);
  MD5Update(&ctx,data,len);
  if (buf == (char *)0x0) {
    buf = (char *)malloc(0x21);
    if ((undefined8 *)buf == (undefined8 *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      buf = (char *)0x0;
      goto LAB_0010fc24;
    }
  }
  MD5Final(digest,&ctx);
  uVar16 = CONCAT31(CONCAT21(CONCAT11("0123456789abcdef"[digest[11] & 0xf],
                                      "0123456789abcdef"[digest[11] >> 4]),
                             "0123456789abcdef"[digest[10] & 0xf]),
                    "0123456789abcdef"[digest[10] >> 4]);
  uVar17 = CONCAT31(CONCAT21(CONCAT11("0123456789abcdef"[digest[15] & 0xf],
                                      "0123456789abcdef"[digest[15] >> 4]),
                             "0123456789abcdef"[digest[14] & 0xf]),
                    "0123456789abcdef"[digest[14] >> 4]);
  cVar1 = "0123456789abcdef"[digest[3] >> 4];
  cVar2 = "0123456789abcdef"[digest[3] & 0xf];
  cVar3 = "0123456789abcdef"[digest[2] & 0xf];
  cVar4 = "0123456789abcdef"[digest[2] >> 4];
  cVar5 = "0123456789abcdef"[digest[1] >> 4];
  cVar6 = "0123456789abcdef"[digest[6] >> 4];
  cVar7 = "0123456789abcdef"[digest[5] >> 4];
  local_108 = (undefined4)
              CONCAT71(CONCAT61(CONCAT51(CONCAT41(uVar16,"0123456789abcdef"[digest[9] & 0xf]),
                                         "0123456789abcdef"[digest[9] >> 4]),
                                "0123456789abcdef"[digest[8] & 0xf]),
                       "0123456789abcdef"[digest[8] >> 4]);
  uStack256 = (undefined4)
              CONCAT71(CONCAT61(CONCAT51(CONCAT41(uVar17,"0123456789abcdef"[digest[13] & 0xf]),
                                         "0123456789abcdef"[digest[13] >> 4]),
                                "0123456789abcdef"[digest[12] & 0xf]),
                       "0123456789abcdef"[digest[12] >> 4]);
  cVar8 = "0123456789abcdef"[digest[7] >> 4];
  *(char *)((long)buf + 0x20) = '\0';
  cVar9 = "0123456789abcdef"[digest[1] & 0xf];
  *(undefined4 *)((long)buf + 0x10) = local_108;
  *(undefined4 *)((long)buf + 0x14) = uVar16;
  *(undefined4 *)((long)buf + 0x18) = uStack256;
  *(undefined4 *)((long)buf + 0x1c) = uVar17;
  cVar10 = "0123456789abcdef"[digest[7] & 0xf];
  cVar11 = "0123456789abcdef"[digest[6] & 0xf];
  cVar12 = "0123456789abcdef"[digest[5] & 0xf];
  cVar13 = "0123456789abcdef"[digest[4] & 0xf];
  cVar14 = "0123456789abcdef"[digest[4] >> 4];
  *(ulong *)buf =
       CONCAT71(CONCAT61(CONCAT51(CONCAT41(CONCAT31(CONCAT21(CONCAT11(cVar2,cVar1),cVar3),cVar4),
                                           cVar9),cVar5),"0123456789abcdef"[digest[0] & 0xf]),
                "0123456789abcdef"[digest[0] >> 4]);
  *(ulong *)((long)buf + 8) =
       CONCAT71(CONCAT61(CONCAT51(CONCAT41(CONCAT31(CONCAT21(CONCAT11(cVar10,cVar8),cVar11),cVar6),
                                           cVar12),cVar7),cVar13),cVar14);
LAB_0010fc24:
  if (lVar15 == *(long *)(in_FS_OFFSET + 0x28)) {
    return buf;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


char * MD5End(MD5_CTX *ctx,char *buf)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  char cVar8;
  char cVar9;
  char cVar10;
  char cVar11;
  char cVar12;
  char cVar13;
  char cVar14;
  char cVar15;
  char cVar16;
  char cVar17;
  char cVar18;
  char cVar19;
  long lVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  long in_FS_OFFSET;
  undefined4 local_a8;
  undefined4 uStack160;
  u_char digest [16];
  
  lVar20 = *(long *)(in_FS_OFFSET + 0x28);
  if (buf == (char *)0x0) {
    buf = (char *)malloc(0x21);
    if ((undefined8 *)buf == (undefined8 *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      buf = (char *)0x0;
      goto LAB_0010f30c;
    }
  }
  MD5Final(digest,ctx);
  cVar1 = "0123456789abcdef"[digest[9] & 0xf];
  uVar21 = CONCAT31(CONCAT21(CONCAT11("0123456789abcdef"[digest[11] & 0xf],
                                      "0123456789abcdef"[digest[11] >> 4]),
                             "0123456789abcdef"[digest[10] & 0xf]),
                    "0123456789abcdef"[digest[10] >> 4]);
  cVar2 = "0123456789abcdef"[digest[9] >> 4];
  cVar3 = "0123456789abcdef"[digest[8] & 0xf];
  cVar4 = "0123456789abcdef"[digest[13] & 0xf];
  cVar5 = "0123456789abcdef"[digest[8] >> 4];
  uVar22 = CONCAT31(CONCAT21(CONCAT11("0123456789abcdef"[digest[15] & 0xf],
                                      "0123456789abcdef"[digest[15] >> 4]),
                             "0123456789abcdef"[digest[14] & 0xf]),
                    "0123456789abcdef"[digest[14] >> 4]);
  cVar6 = "0123456789abcdef"[digest[13] >> 4];
  cVar7 = "0123456789abcdef"[digest[12] & 0xf];
  cVar8 = "0123456789abcdef"[digest[12] >> 4];
  cVar9 = "0123456789abcdef"[digest[3] >> 4];
  cVar10 = "0123456789abcdef"[digest[3] & 0xf];
  cVar11 = "0123456789abcdef"[digest[2] & 0xf];
  cVar12 = "0123456789abcdef"[digest[2] >> 4];
  cVar13 = "0123456789abcdef"[digest[0] & 0xf];
  cVar14 = "0123456789abcdef"[digest[6] >> 4];
  *(char *)((long)buf + 0x20) = '\0';
  cVar15 = "0123456789abcdef"[digest[1] >> 4];
  cVar16 = "0123456789abcdef"[digest[7] >> 4];
  cVar17 = "0123456789abcdef"[digest[5] >> 4];
  cVar18 = "0123456789abcdef"[digest[0] >> 4];
  cVar19 = "0123456789abcdef"[digest[1] & 0xf];
  local_a8 = (undefined4)CONCAT71(CONCAT61(CONCAT51(CONCAT41(uVar21,cVar1),cVar2),cVar3),cVar5);
  uStack160 = (undefined4)CONCAT71(CONCAT61(CONCAT51(CONCAT41(uVar22,cVar4),cVar6),cVar7),cVar8);
  *(undefined4 *)((long)buf + 0x10) = local_a8;
  *(undefined4 *)((long)buf + 0x14) = uVar21;
  *(undefined4 *)((long)buf + 0x18) = uStack160;
  *(undefined4 *)((long)buf + 0x1c) = uVar22;
  cVar1 = "0123456789abcdef"[digest[7] & 0xf];
  cVar2 = "0123456789abcdef"[digest[6] & 0xf];
  cVar3 = "0123456789abcdef"[digest[5] & 0xf];
  cVar4 = "0123456789abcdef"[digest[4] & 0xf];
  cVar5 = "0123456789abcdef"[digest[4] >> 4];
  *(ulong *)buf =
       CONCAT71(CONCAT61(CONCAT51(CONCAT41(CONCAT31(CONCAT21(CONCAT11(cVar10,cVar9),cVar11),cVar12),
                                           cVar19),cVar15),cVar13),cVar18);
  *(ulong *)((long)buf + 8) =
       CONCAT71(CONCAT61(CONCAT51(CONCAT41(CONCAT31(CONCAT21(CONCAT11(cVar1,cVar16),cVar2),cVar14),
                                           cVar3),cVar17),cVar4),cVar5);
LAB_0010f30c:
  if (lVar20 == *(long *)(in_FS_OFFSET + 0x28)) {
    return buf;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


char * MD5File(char *filename,char *buf)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  char cVar8;
  char cVar9;
  char cVar10;
  char cVar11;
  char cVar12;
  char cVar13;
  char cVar14;
  char cVar15;
  char cVar16;
  char cVar17;
  char cVar18;
  char cVar19;
  int iVar20;
  long lVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  int __fd;
  uint len;
  ssize_t sVar24;
  int *piVar25;
  long in_FS_OFFSET;
  undefined4 local_2108;
  undefined4 uStack8448;
  MD5_CTX ctx;
  u_char digest [16];
  u_char buffer [8192];
  
  lVar21 = *(long *)(in_FS_OFFSET + 0x28);
  MD5Init(&ctx);
  __fd = open(filename,0);
  if (-1 < __fd) {
    while( true ) {
      sVar24 = read(__fd,buffer,0x2000);
      len = (uint)sVar24;
      if ((int)len < 1) break;
      MD5Update(&ctx,buffer,len);
    }
    piVar25 = __errno_location();
    iVar20 = *piVar25;
    close(__fd);
    *piVar25 = iVar20;
    if (len == 0) {
      if ((buf != (char *)0x0) ||
         (buf = (char *)malloc(0x21), (undefined8 *)buf != (undefined8 *)0x0)) {
        MD5Final(digest,&ctx);
        cVar1 = "0123456789abcdef"[digest[9] & 0xf];
        uVar22 = CONCAT31(CONCAT21(CONCAT11("0123456789abcdef"[digest[11] & 0xf],
                                            "0123456789abcdef"[digest[11] >> 4]),
                                   "0123456789abcdef"[digest[10] & 0xf]),
                          "0123456789abcdef"[digest[10] >> 4]);
        cVar2 = "0123456789abcdef"[digest[9] >> 4];
        cVar3 = "0123456789abcdef"[digest[8] & 0xf];
        cVar4 = "0123456789abcdef"[digest[13] & 0xf];
        cVar5 = "0123456789abcdef"[digest[8] >> 4];
        uVar23 = CONCAT31(CONCAT21(CONCAT11("0123456789abcdef"[digest[15] & 0xf],
                                            "0123456789abcdef"[digest[15] >> 4]),
                                   "0123456789abcdef"[digest[14] & 0xf]),
                          "0123456789abcdef"[digest[14] >> 4]);
        cVar6 = "0123456789abcdef"[digest[13] >> 4];
        cVar7 = "0123456789abcdef"[digest[12] & 0xf];
        cVar8 = "0123456789abcdef"[digest[12] >> 4];
        cVar9 = "0123456789abcdef"[digest[3] & 0xf];
        cVar10 = "0123456789abcdef"[digest[3] >> 4];
        cVar11 = "0123456789abcdef"[digest[2] & 0xf];
        cVar12 = "0123456789abcdef"[digest[2] >> 4];
        cVar13 = "0123456789abcdef"[digest[0] & 0xf];
        cVar14 = "0123456789abcdef"[digest[6] >> 4];
        *(char *)((long)buf + 0x20) = '\0';
        cVar15 = "0123456789abcdef"[digest[1] >> 4];
        cVar16 = "0123456789abcdef"[digest[7] >> 4];
        cVar17 = "0123456789abcdef"[digest[5] >> 4];
        cVar18 = "0123456789abcdef"[digest[0] >> 4];
        cVar19 = "0123456789abcdef"[digest[1] & 0xf];
        local_2108 = (undefined4)
                     CONCAT71(CONCAT61(CONCAT51(CONCAT41(uVar22,cVar1),cVar2),cVar3),cVar5);
        uStack8448 = (undefined4)
                     CONCAT71(CONCAT61(CONCAT51(CONCAT41(uVar23,cVar4),cVar6),cVar7),cVar8);
        *(undefined4 *)((long)buf + 0x10) = local_2108;
        *(undefined4 *)((long)buf + 0x14) = uVar22;
        *(undefined4 *)((long)buf + 0x18) = uStack8448;
        *(undefined4 *)((long)buf + 0x1c) = uVar23;
        cVar1 = "0123456789abcdef"[digest[7] & 0xf];
        cVar2 = "0123456789abcdef"[digest[6] & 0xf];
        cVar3 = "0123456789abcdef"[digest[5] & 0xf];
        cVar4 = "0123456789abcdef"[digest[4] & 0xf];
        cVar5 = "0123456789abcdef"[digest[4] >> 4];
        *(ulong *)buf =
             CONCAT71(CONCAT61(CONCAT51(CONCAT41(CONCAT31(CONCAT21(CONCAT11(cVar9,cVar10),cVar11),
                                                          cVar12),cVar19),cVar15),cVar13),cVar18);
        *(ulong *)((long)buf + 8) =
             CONCAT71(CONCAT61(CONCAT51(CONCAT41(CONCAT31(CONCAT21(CONCAT11(cVar1,cVar16),cVar2),
                                                          cVar14),cVar3),cVar17),cVar4),cVar5);
        goto LAB_0010f7c3;
      }
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    }
  }
  buf = (char *)0x0;
LAB_0010f7c3:
  if (lVar21 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return buf;
}


void MD5Final(u_char *digest,MD5_CTX *ctx)

{
  u_int32_t *in;
  undefined *__s;
  u_int32_t uVar1;
  u_int32_t uVar2;
  u_int32_t uVar3;
  uint uVar4;
  ulong uVar5;
  uint uVar6;
  
  in = (u_int32_t *)ctx->in;
  uVar4 = ctx->bits[0] >> 3 & 0x3f;
  *(undefined *)((ulong)uVar4 + (long)in) = 0x80;
  __s = (undefined *)((ulong)uVar4 + (long)in) + 1;
  uVar6 = -uVar4 + 0x3f;
  if (uVar6 < 8) {
    if (-uVar4 != -0x3f) {
      uVar4 = 0;
      do {
        uVar5 = (ulong)uVar4;
        uVar4 = uVar4 + 1;
        __s[uVar5] = 0;
      } while (uVar4 < uVar6);
    }
    MD5Transform((u_int32_t *)ctx,in);
    *(undefined (*) [16])ctx->in = (undefined  [16])0x0;
    *(undefined8 *)(ctx->in + 0x30) = 0;
    *(undefined (*) [16])(ctx->in + 0x10) = (undefined  [16])0x0;
    *(undefined (*) [16])(ctx->in + 0x20) = (undefined  [16])0x0;
  }
  else {
    memset(__s,0,(ulong)(0x37 - uVar4));
  }
  *(undefined8 *)(ctx->in + 0x38) = *(undefined8 *)ctx->bits;
  MD5Transform((u_int32_t *)ctx,in);
  uVar1 = ctx->buf[1];
  uVar2 = ctx->buf[2];
  uVar3 = ctx->buf[3];
  *(u_int32_t *)digest = ctx->buf[0];
  *(u_int32_t *)(digest + 4) = uVar1;
  *(u_int32_t *)(digest + 8) = uVar2;
  *(u_int32_t *)(digest + 0xc) = uVar3;
  *(undefined8 *)ctx->buf = 0;
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void MD5Init(MD5_CTX *ctx)

{
  u_int32_t uVar1;
  u_int32_t uVar2;
  u_int32_t uVar3;
  u_int32_t uVar4;
  
  uVar4 = _UNK_00112bbc;
  uVar3 = _UNK_00112bb8;
  uVar2 = _UNK_00112bb4;
  uVar1 = _DAT_00112bb0;
  *(undefined8 *)ctx->bits = 0;
  ctx->buf[0] = uVar1;
  ctx->buf[1] = uVar2;
  ctx->buf[2] = uVar3;
  ctx->buf[3] = uVar4;
  return;
}


u_char * md5_level(int level_num)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  u_char *__ptr;
  u_char *puVar4;
  FILE *__stream;
  char *pcVar5;
  uint *puVar6;
  long lVar7;
  long lVar8;
  uint *puVar9;
  long in_FS_OFFSET;
  bool bVar10;
  int len;
  u_char *result;
  u_char p [2048];
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  len = 0;
  result = (u_char *)0x0;
  __ptr = load_level(level_num);
  if (__ptr != (u_char *)0x0) {
    puVar4 = __add_md5((u_char *)"data/level.dat",&len,&result);
    if (puVar4 == (u_char *)0x0) {
LAB_0010a2c3:
      if (result == (u_char *)0xffffffffffffffff) goto LAB_0010a3df;
      if (result != (u_char *)0x0) {
LAB_0010a2db:
        free(result);
        goto LAB_0010a2e0;
      }
    }
    else {
      __snprintf_chk(p,0x800,1,0x800,"%s%s%s","data/",__ptr,&DAT_001120da);
      puVar4 = __add_md5(p,&len,&result);
      if (puVar4 == (u_char *)0x0) goto LAB_0010a2c3;
      __snprintf_chk(p,0x800,1,0x800,"%s%s%s","data/",__ptr,&DAT_001120de);
      puVar4 = __add_md5(p,&len,&result);
      if (puVar4 == (u_char *)0x0) goto LAB_0010a2c3;
      __snprintf_chk(p,0x800,1,0x800,"%s%s%s","data/",__ptr,&DAT_001128ec);
      puVar4 = __add_md5(p,&len,&result);
      if (puVar4 == (u_char *)0x0) goto LAB_0010a2c3;
      __snprintf_chk(p,0x800,1,0x800,"%s%s%s","data/",__ptr,&DAT_001120da);
      if (__ptr != (u_char *)0xffffffffffffffff) {
        free(__ptr);
      }
      __stream = fopen((char *)p,"r");
      if (__stream != (FILE *)0x0) {
        do {
          pcVar5 = fgets((char *)p,0x800,__stream);
          puVar9 = (uint *)p;
          if (pcVar5 == (char *)0x0) {
            fclose(__stream);
            puVar4 = (u_char *)MD5Data(result,len,(char *)0x0);
            if (result == (u_char *)0xffffffffffffffff) goto LAB_0010a2e0;
            if (result != (u_char *)0x0) goto LAB_0010a2db;
            errline = 0x4c;
            errfile = "error.h";
            int_error((u_char *)"mem_free(NULL)");
            goto LAB_0010a2e0;
          }
          do {
            puVar6 = puVar9;
            uVar2 = *puVar6 + 0xfefefeff & ~*puVar6;
            uVar3 = uVar2 & 0x80808080;
            puVar9 = puVar6 + 1;
          } while (uVar3 == 0);
          bVar10 = (uVar2 & 0x8080) == 0;
          if (bVar10) {
            uVar3 = uVar3 >> 0x10;
          }
          if (bVar10) {
            puVar9 = (uint *)((long)puVar6 + 6);
          }
          lVar8 = (long)puVar9 + ((-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)) - (long)p);
          lVar7 = lVar8 + -1;
          if (*(char *)((long)&result + lVar8 + 7) == '\r') {
            p[lVar7] = '\0';
            puVar9 = (uint *)p;
            do {
              puVar6 = puVar9;
              uVar2 = *puVar6 + 0xfefefeff & ~*puVar6;
              uVar3 = uVar2 & 0x80808080;
              puVar9 = puVar6 + 1;
            } while (uVar3 == 0);
            bVar10 = (uVar2 & 0x8080) == 0;
            if (bVar10) {
              uVar3 = uVar3 >> 0x10;
            }
            if (bVar10) {
              puVar9 = (uint *)((long)puVar6 + 6);
            }
            lVar7 = (long)puVar9 + (-(long)p - (ulong)CARRY1((byte)uVar3,(byte)uVar3)) + -4;
          }
          if (p[lVar7] == '\n') {
            p[lVar7] = '\0';
          }
          puVar9 = (uint *)p;
          bVar1 = p[0];
          if (1 < (byte)(p[0] - 9)) goto LAB_0010a260;
          do {
            do {
              bVar1 = *(byte *)((long)puVar9 + 1);
              puVar9 = (uint *)((long)puVar9 + 1);
            } while ((byte)(bVar1 - 9) < 2);
LAB_0010a260:
          } while (bVar1 == 0x20);
          if ((0x20 < bVar1) || ((0x100002601U >> ((ulong)bVar1 & 0x3f) & 1) == 0)) {
            do {
              do {
                bVar1 = *(byte *)((long)puVar9 + 1);
                puVar9 = (uint *)((long)puVar9 + 1);
              } while (0x20 < bVar1);
            } while ((0x100002601U >> ((ulong)bVar1 & 0x3f) & 1) == 0);
            if (1 < (byte)(bVar1 - 9)) goto LAB_0010a2a0;
            do {
              do {
                bVar1 = *(byte *)((long)puVar9 + 1);
                puVar9 = (uint *)((long)puVar9 + 1);
              } while ((byte)(bVar1 - 9) < 2);
LAB_0010a2a0:
            } while (bVar1 == 0x20);
          }
        } while ((bVar1 == 0) ||
                (puVar4 = __add_md5((u_char *)puVar9,&len,&result), puVar4 != (u_char *)0x0));
        goto LAB_0010a2c3;
      }
      if (result == (u_char *)0xffffffffffffffff) goto LAB_0010a3df;
      if (result != (u_char *)0x0) {
        free(result);
        puVar4 = (u_char *)0x0;
        goto LAB_0010a2e0;
      }
    }
    errline = 0x4c;
    errfile = "error.h";
    int_error((u_char *)"mem_free(NULL)");
  }
LAB_0010a3df:
  puVar4 = (u_char *)0x0;
LAB_0010a2e0:
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return puVar4;
}


void MD5Transform(u_int32_t *buf,u_int32_t *in)

{
  uint uVar1;
  u_int32_t uVar2;
  u_int32_t uVar3;
  u_int32_t uVar4;
  u_int32_t uVar5;
  u_int32_t uVar6;
  u_int32_t uVar7;
  u_int32_t uVar8;
  u_int32_t uVar9;
  u_int32_t uVar10;
  u_int32_t uVar11;
  u_int32_t uVar12;
  u_int32_t uVar13;
  u_int32_t uVar14;
  u_int32_t uVar15;
  u_int32_t uVar16;
  u_int32_t uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  
  uVar1 = buf[1];
  uVar2 = *in;
  uVar3 = in[1];
  uVar4 = in[5];
  uVar18 = ((buf[2] ^ buf[3]) & uVar1 ^ buf[3]) + uVar2 + 0xd76aa478 + *buf;
  uVar5 = in[9];
  uVar18 = (uVar18 * 0x80 | uVar18 >> 0x19) + uVar1;
  uVar19 = ((buf[2] ^ uVar1) & uVar18 ^ buf[2]) + uVar3 + 0xe8c7b756 + buf[3];
  uVar6 = in[2];
  uVar20 = (uVar19 * 0x1000 | uVar19 >> 0x14) + uVar18;
  uVar7 = in[3];
  uVar19 = ((uVar1 ^ uVar18) & uVar20 ^ uVar1) + uVar6 + 0x242070db + buf[2];
  uVar8 = in[4];
  uVar19 = (uVar19 >> 0xf | uVar19 * 0x20000) + uVar20;
  uVar21 = ((uVar18 ^ uVar20) & uVar19 ^ uVar18) + uVar7 + 0xc1bdceee + uVar1;
  uVar21 = (uVar21 >> 10 | uVar21 * 0x400000) + uVar19;
  uVar18 = ((uVar20 ^ uVar19) & uVar21 ^ uVar20) + uVar8 + 0xf57c0faf + uVar18;
  uVar18 = (uVar18 * 0x80 | uVar18 >> 0x19) + uVar21;
  uVar20 = ((uVar19 ^ uVar21) & uVar18 ^ uVar19) + uVar4 + 0x4787c62a + uVar20;
  uVar9 = in[6];
  uVar20 = (uVar20 * 0x1000 | uVar20 >> 0x14) + uVar18;
  uVar10 = in[7];
  uVar19 = ((uVar21 ^ uVar18) & uVar20 ^ uVar21) + uVar9 + 0xa8304613 + uVar19;
  uVar11 = in[8];
  uVar19 = (uVar19 >> 0xf | uVar19 * 0x20000) + uVar20;
  uVar12 = in[10];
  uVar21 = ((uVar18 ^ uVar20) & uVar19 ^ uVar18) + uVar10 + 0xfd469501 + uVar21;
  uVar21 = (uVar21 >> 10 | uVar21 * 0x400000) + uVar19;
  uVar18 = ((uVar20 ^ uVar19) & uVar21 ^ uVar20) + uVar11 + 0x698098d8 + uVar18;
  uVar13 = in[0xc];
  uVar18 = (uVar18 * 0x80 | uVar18 >> 0x19) + uVar21;
  uVar20 = ((uVar19 ^ uVar21) & uVar18 ^ uVar19) + uVar5 + 0x8b44f7af + uVar20;
  uVar20 = (uVar20 * 0x1000 | uVar20 >> 0x14) + uVar18;
  uVar19 = ((uVar21 ^ uVar18) & uVar20 ^ uVar21) + (uVar12 - 0xa44f) + uVar19;
  uVar14 = in[0xb];
  uVar19 = (uVar19 >> 0xf | uVar19 * 0x20000) + uVar20;
  uVar15 = in[0xe];
  uVar21 = ((uVar18 ^ uVar20) & uVar19 ^ uVar18) + uVar14 + 0x895cd7be + uVar21;
  uVar21 = (uVar21 >> 10 | uVar21 * 0x400000) + uVar19;
  uVar18 = ((uVar20 ^ uVar19) & uVar21 ^ uVar20) + uVar13 + 0x6b901122 + uVar18;
  uVar16 = in[0xd];
  uVar18 = (uVar18 * 0x80 | uVar18 >> 0x19) + uVar21;
  uVar20 = ((uVar19 ^ uVar21) & uVar18 ^ uVar19) + uVar16 + 0xfd987193 + uVar20;
  uVar20 = (uVar20 * 0x1000 | uVar20 >> 0x14) + uVar18;
  uVar19 = ((uVar21 ^ uVar18) & uVar20 ^ uVar21) + uVar15 + 0xa679438e + uVar19;
  uVar17 = in[0xf];
  uVar19 = (uVar19 >> 0xf | uVar19 * 0x20000) + uVar20;
  uVar21 = ((uVar18 ^ uVar20) & uVar19 ^ uVar18) + uVar17 + 0x49b40821 + uVar21;
  uVar21 = (uVar21 >> 10 | uVar21 * 0x400000) + uVar19;
  uVar18 = ((uVar19 ^ uVar21) & uVar20 ^ uVar19) + uVar3 + 0xf61e2562 + uVar18;
  uVar18 = (uVar18 * 0x20 | uVar18 >> 0x1b) + uVar21;
  uVar20 = ((uVar21 ^ uVar18) & uVar19 ^ uVar21) + uVar9 + 0xc040b340 + uVar20;
  uVar20 = (uVar20 * 0x200 | uVar20 >> 0x17) + uVar18;
  uVar19 = ((uVar18 ^ uVar20) & uVar21 ^ uVar18) + uVar14 + 0x265e5a51 + uVar19;
  uVar19 = (uVar19 * 0x4000 | uVar19 >> 0x12) + uVar20;
  uVar21 = ((uVar20 ^ uVar19) & uVar18 ^ uVar20) + uVar2 + 0xe9b6c7aa + uVar21;
  uVar21 = (uVar21 >> 0xc | uVar21 * 0x100000) + uVar19;
  uVar18 = ((uVar19 ^ uVar21) & uVar20 ^ uVar19) + uVar4 + 0xd62f105d + uVar18;
  uVar18 = (uVar18 * 0x20 | uVar18 >> 0x1b) + uVar21;
  uVar20 = ((uVar21 ^ uVar18) & uVar19 ^ uVar21) + uVar12 + 0x2441453 + uVar20;
  uVar20 = (uVar20 * 0x200 | uVar20 >> 0x17) + uVar18;
  uVar19 = ((uVar18 ^ uVar20) & uVar21 ^ uVar18) + uVar17 + 0xd8a1e681 + uVar19;
  uVar19 = (uVar19 * 0x4000 | uVar19 >> 0x12) + uVar20;
  uVar21 = ((uVar20 ^ uVar19) & uVar18 ^ uVar20) + uVar8 + 0xe7d3fbc8 + uVar21;
  uVar21 = (uVar21 >> 0xc | uVar21 * 0x100000) + uVar19;
  uVar18 = ((uVar19 ^ uVar21) & uVar20 ^ uVar19) + uVar5 + 0x21e1cde6 + uVar18;
  uVar18 = (uVar18 * 0x20 | uVar18 >> 0x1b) + uVar21;
  uVar20 = ((uVar21 ^ uVar18) & uVar19 ^ uVar21) + uVar15 + 0xc33707d6 + uVar20;
  uVar20 = (uVar20 * 0x200 | uVar20 >> 0x17) + uVar18;
  uVar19 = ((uVar18 ^ uVar20) & uVar21 ^ uVar18) + uVar7 + 0xf4d50d87 + uVar19;
  uVar19 = (uVar19 * 0x4000 | uVar19 >> 0x12) + uVar20;
  uVar21 = ((uVar20 ^ uVar19) & uVar18 ^ uVar20) + uVar11 + 0x455a14ed + uVar21;
  uVar22 = (uVar21 >> 0xc | uVar21 * 0x100000) + uVar19;
  uVar18 = ((uVar19 ^ uVar22) & uVar20 ^ uVar19) + uVar16 + 0xa9e3e905 + uVar18;
  uVar18 = (uVar18 * 0x20 | uVar18 >> 0x1b) + uVar22;
  uVar20 = ((uVar22 ^ uVar18) & uVar19 ^ uVar22) + uVar6 + 0xfcefa3f8 + uVar20;
  uVar20 = (uVar20 * 0x200 | uVar20 >> 0x17) + uVar18;
  uVar19 = ((uVar18 ^ uVar20) & uVar22 ^ uVar18) + uVar10 + 0x676f02d9 + uVar19;
  uVar21 = (uVar19 * 0x4000 | uVar19 >> 0x12) + uVar20;
  uVar19 = ((uVar20 ^ uVar21) & uVar18 ^ uVar20) + uVar13 + 0x8d2a4c8a + uVar22;
  uVar22 = (uVar19 >> 0xc | uVar19 * 0x100000) + uVar21;
  uVar18 = (uVar20 ^ uVar21 ^ uVar22) + (uVar4 - 0x5c6be) + uVar18;
  uVar19 = (uVar18 * 0x10 | uVar18 >> 0x1c) + uVar22;
  uVar18 = (uVar21 ^ uVar22 ^ uVar19) + uVar11 + 0x8771f681 + uVar20;
  uVar20 = (uVar18 * 0x800 | uVar18 >> 0x15) + uVar19;
  uVar18 = (uVar22 ^ uVar19 ^ uVar20) + uVar14 + 0x6d9d6122 + uVar21;
  uVar21 = (uVar18 * 0x10000 | uVar18 >> 0x10) + uVar20;
  uVar18 = (uVar19 ^ uVar20 ^ uVar21) + uVar15 + 0xfde5380c + uVar22;
  uVar18 = (uVar18 >> 9 | uVar18 * 0x800000) + uVar21;
  uVar19 = (uVar20 ^ uVar21 ^ uVar18) + uVar3 + 0xa4beea44 + uVar19;
  uVar19 = (uVar19 * 0x10 | uVar19 >> 0x1c) + uVar18;
  uVar20 = (uVar21 ^ uVar18 ^ uVar19) + uVar8 + 0x4bdecfa9 + uVar20;
  uVar22 = (uVar20 * 0x800 | uVar20 >> 0x15) + uVar19;
  uVar20 = (uVar18 ^ uVar19 ^ uVar22) + uVar10 + 0xf6bb4b60 + uVar21;
  uVar20 = (uVar20 * 0x10000 | uVar20 >> 0x10) + uVar22;
  uVar18 = (uVar19 ^ uVar22 ^ uVar20) + uVar12 + 0xbebfbc70 + uVar18;
  uVar21 = (uVar18 >> 9 | uVar18 * 0x800000) + uVar20;
  uVar18 = (uVar22 ^ uVar20 ^ uVar21) + uVar16 + 0x289b7ec6 + uVar19;
  uVar19 = (uVar18 * 0x10 | uVar18 >> 0x1c) + uVar21;
  uVar18 = (uVar20 ^ uVar21 ^ uVar19) + uVar2 + 0xeaa127fa + uVar22;
  uVar18 = (uVar18 * 0x800 | uVar18 >> 0x15) + uVar19;
  uVar20 = (uVar21 ^ uVar19 ^ uVar18) + uVar7 + 0xd4ef3085 + uVar20;
  uVar22 = (uVar20 * 0x10000 | uVar20 >> 0x10) + uVar18;
  uVar20 = (uVar19 ^ uVar18 ^ uVar22) + uVar9 + 0x4881d05 + uVar21;
  uVar21 = (uVar20 >> 9 | uVar20 * 0x800000) + uVar22;
  uVar19 = (uVar18 ^ uVar22 ^ uVar21) + uVar5 + 0xd9d4d039 + uVar19;
  uVar20 = (uVar19 * 0x10 | uVar19 >> 0x1c) + uVar21;
  uVar18 = (uVar22 ^ uVar21 ^ uVar20) + uVar13 + 0xe6db99e5 + uVar18;
  uVar19 = (uVar18 * 0x800 | uVar18 >> 0x15) + uVar20;
  uVar18 = (uVar21 ^ uVar20 ^ uVar19) + uVar17 + 0x1fa27cf8 + uVar22;
  uVar22 = (uVar18 * 0x10000 | uVar18 >> 0x10) + uVar19;
  uVar18 = (uVar20 ^ uVar19 ^ uVar22) + uVar6 + 0xc4ac5665 + uVar21;
  uVar18 = (uVar18 >> 9 | uVar18 * 0x800000) + uVar22;
  uVar20 = ((~uVar19 | uVar18) ^ uVar22) + uVar2 + 0xf4292244 + uVar20;
  uVar20 = (uVar20 * 0x40 | uVar20 >> 0x1a) + uVar18;
  uVar19 = ((~uVar22 | uVar20) ^ uVar18) + uVar10 + 0x432aff97 + uVar19;
  uVar19 = (uVar19 * 0x400 | uVar19 >> 0x16) + uVar20;
  uVar21 = ((~uVar18 | uVar19) ^ uVar20) + uVar15 + 0xab9423a7 + uVar22;
  uVar21 = (uVar21 * 0x8000 | uVar21 >> 0x11) + uVar19;
  uVar18 = ((~uVar20 | uVar21) ^ uVar19) + uVar4 + 0xfc93a039 + uVar18;
  uVar18 = (uVar18 >> 0xb | uVar18 * 0x200000) + uVar21;
  uVar20 = ((~uVar19 | uVar18) ^ uVar21) + uVar13 + 0x655b59c3 + uVar20;
  uVar20 = (uVar20 * 0x40 | uVar20 >> 0x1a) + uVar18;
  uVar19 = ((~uVar21 | uVar20) ^ uVar18) + uVar7 + 0x8f0ccc92 + uVar19;
  uVar19 = (uVar19 * 0x400 | uVar19 >> 0x16) + uVar20;
  uVar21 = ((~uVar18 | uVar19) ^ uVar20) + (uVar12 - 0x100b83) + uVar21;
  uVar21 = (uVar21 * 0x8000 | uVar21 >> 0x11) + uVar19;
  uVar18 = ((~uVar20 | uVar21) ^ uVar19) + uVar3 + 0x85845dd1 + uVar18;
  uVar18 = (uVar18 >> 0xb | uVar18 * 0x200000) + uVar21;
  uVar20 = ((~uVar19 | uVar18) ^ uVar21) + uVar11 + 0x6fa87e4f + uVar20;
  uVar20 = (uVar20 * 0x40 | uVar20 >> 0x1a) + uVar18;
  uVar19 = ((~uVar21 | uVar20) ^ uVar18) + uVar17 + 0xfe2ce6e0 + uVar19;
  uVar19 = (uVar19 * 0x400 | uVar19 >> 0x16) + uVar20;
  uVar21 = ((~uVar18 | uVar19) ^ uVar20) + uVar9 + 0xa3014314 + uVar21;
  uVar21 = (uVar21 * 0x8000 | uVar21 >> 0x11) + uVar19;
  uVar18 = ((~uVar20 | uVar21) ^ uVar19) + uVar16 + 0x4e0811a1 + uVar18;
  uVar22 = (uVar18 >> 0xb | uVar18 * 0x200000) + uVar21;
  uVar18 = ((~uVar19 | uVar22) ^ uVar21) + uVar8 + 0xf7537e82 + uVar20;
  uVar18 = (uVar18 * 0x40 | uVar18 >> 0x1a) + uVar22;
  uVar19 = ((~uVar21 | uVar18) ^ uVar22) + uVar14 + 0xbd3af235 + uVar19;
  uVar20 = (uVar19 * 0x400 | uVar19 >> 0x16) + uVar18;
  uVar19 = ((~uVar22 | uVar20) ^ uVar18) + uVar6 + 0x2ad7d2bb + uVar21;
  *buf = uVar18 + *buf;
  uVar19 = (uVar19 * 0x8000 | uVar19 >> 0x11) + uVar20;
  uVar18 = ((~uVar18 | uVar19) ^ uVar20) + uVar5 + 0xeb86d391 + uVar22;
  buf[1] = (uVar18 >> 0xb | uVar18 * 0x200000) + uVar1 + uVar19;
  buf[2] = uVar19 + buf[2];
  buf[3] = uVar20 + buf[3];
  return;
}


void MD5Update(MD5_CTX *ctx,u_char *buf,uint len)

{
  u_int32_t uVar1;
  u_int32_t uVar2;
  u_int32_t uVar3;
  uint uVar4;
  uint uVar5;
  undefined8 *puVar6;
  ulong uVar7;
  undefined8 *puVar8;
  uint uVar9;
  ulong uVar10;
  u_int32_t *puVar11;
  MD5_CTX *buf_00;
  
  uVar10 = (ulong)len;
  puVar11 = (u_int32_t *)ctx->in;
  uVar4 = ctx->bits[0];
  uVar1 = ctx->bits[1];
  ctx->bits[0] = len * 8 + uVar4;
  ctx->bits[1] = (len >> 0x1d) + ((uVar1 + 1) - (uint)(CARRY4(len * 8,uVar4) == false));
  uVar4 = uVar4 >> 3 & 0x3f;
  buf_00 = ctx;
  if (uVar4 != 0) {
    uVar9 = 0x40 - uVar4;
    puVar6 = (undefined8 *)((ulong)uVar4 + (long)puVar11);
    if (len < uVar9) {
      if (7 < len) {
        *puVar6 = *(undefined8 *)buf;
        *(undefined8 *)((long)puVar6 + ((ulong)len - 8)) = *(undefined8 *)(buf + ((ulong)len - 8));
        puVar8 = (undefined8 *)((long)puVar6 - ((ulong)(puVar6 + 1) & 0xfffffffffffffff8));
        uVar4 = len + (int)puVar8 & 0xfffffff8;
        if (uVar4 < 8) {
          return;
        }
        uVar9 = 0;
        do {
          uVar10 = (ulong)uVar9;
          uVar9 = uVar9 + 8;
          *(undefined8 *)(((ulong)(puVar6 + 1) & 0xfffffffffffffff8) + uVar10) =
               *(undefined8 *)(buf + (uVar10 - (long)puVar8));
        } while (uVar9 < uVar4);
        return;
      }
      if ((len & 4) != 0) {
        *(undefined4 *)puVar6 = *(undefined4 *)buf;
        *(undefined4 *)((long)puVar6 + ((ulong)len - 4)) = *(undefined4 *)(buf + ((ulong)len - 4));
        return;
      }
      if (len == 0) {
        return;
      }
      *(u_char *)puVar6 = *buf;
      if ((len & 2) == 0) {
        return;
      }
      *(undefined2 *)((long)puVar6 + ((ulong)len - 2)) = *(undefined2 *)(buf + ((ulong)len - 2));
      return;
    }
    uVar10 = (ulong)uVar9;
    if (uVar9 < 8) {
      if ((uVar9 & 4) == 0) {
        if ((uVar9 != 0) && (*(u_char *)puVar6 = *buf, (uVar9 & 2) != 0)) {
          *(undefined2 *)((long)puVar6 + (uVar10 - 2)) = *(undefined2 *)(buf + (uVar10 - 2));
        }
      }
      else {
        *(undefined4 *)puVar6 = *(undefined4 *)buf;
        *(undefined4 *)((long)puVar6 + (uVar10 - 4)) = *(undefined4 *)(buf + (uVar10 - 4));
      }
    }
    else {
      *puVar6 = *(undefined8 *)buf;
      *(undefined8 *)((long)puVar6 + (uVar10 - 8)) = *(undefined8 *)(buf + (uVar10 - 8));
      puVar8 = (undefined8 *)((long)puVar6 - ((ulong)(puVar6 + 1) & 0xfffffffffffffff8));
      uVar9 = uVar9 + (int)puVar8 & 0xfffffff8;
      if (7 < uVar9) {
        uVar5 = 0;
        do {
          uVar7 = (ulong)uVar5;
          uVar5 = uVar5 + 8;
          *(undefined8 *)(((ulong)(puVar6 + 1) & 0xfffffffffffffff8) + uVar7) =
               *(undefined8 *)(buf + (uVar7 - (long)puVar8));
        } while (uVar5 < uVar9);
      }
    }
    buf = buf + uVar10;
    uVar10 = (ulong)((uVar4 - 0x40) + len);
    MD5Transform((u_int32_t *)ctx,puVar11);
  }
  uVar4 = (uint)uVar10;
  if (0x3f < uVar4) {
    puVar6 = (undefined8 *)buf;
    do {
      uVar1 = *(u_int32_t *)((long)puVar6 + 4);
      uVar2 = *(u_int32_t *)(puVar6 + 1);
      uVar3 = *(u_int32_t *)((long)puVar6 + 0xc);
      puVar8 = puVar6 + 8;
      *puVar11 = *(u_int32_t *)puVar6;
      *(u_int32_t *)(ctx->in + 4) = uVar1;
      *(u_int32_t *)(ctx->in + 8) = uVar2;
      *(u_int32_t *)(ctx->in + 0xc) = uVar3;
      uVar1 = *(u_int32_t *)((long)puVar6 + 0x14);
      uVar2 = *(u_int32_t *)(puVar6 + 3);
      uVar3 = *(u_int32_t *)((long)puVar6 + 0x1c);
      *(u_int32_t *)(ctx->in + 0x10) = *(u_int32_t *)(puVar6 + 2);
      *(u_int32_t *)(ctx->in + 0x14) = uVar1;
      *(u_int32_t *)(ctx->in + 0x18) = uVar2;
      *(u_int32_t *)(ctx->in + 0x1c) = uVar3;
      uVar1 = *(u_int32_t *)((long)puVar6 + 0x24);
      uVar2 = *(u_int32_t *)(puVar6 + 5);
      uVar3 = *(u_int32_t *)((long)puVar6 + 0x2c);
      *(u_int32_t *)(ctx->in + 0x20) = *(u_int32_t *)(puVar6 + 4);
      *(u_int32_t *)(ctx->in + 0x24) = uVar1;
      *(u_int32_t *)(ctx->in + 0x28) = uVar2;
      *(u_int32_t *)(ctx->in + 0x2c) = uVar3;
      uVar1 = *(u_int32_t *)((long)puVar6 + 0x34);
      uVar2 = *(u_int32_t *)(puVar6 + 7);
      uVar3 = *(u_int32_t *)((long)puVar6 + 0x3c);
      *(u_int32_t *)(ctx->in + 0x30) = *(u_int32_t *)(puVar6 + 6);
      *(u_int32_t *)(ctx->in + 0x34) = uVar1;
      *(u_int32_t *)(ctx->in + 0x38) = uVar2;
      *(u_int32_t *)(ctx->in + 0x3c) = uVar3;
      MD5Transform((u_int32_t *)buf_00,puVar11);
      puVar6 = puVar8;
    } while (puVar8 != (undefined8 *)((long)buf + ((ulong)(uVar4 - 0x40 >> 6) + 1) * 8 * 8));
    uVar10 = (ulong)(uVar4 & 0x3f);
    buf = (u_char *)(undefined8 *)((long)buf + ((ulong)(uVar4 - 0x40 >> 6) + 1) * 8 * 8);
  }
  uVar4 = (uint)uVar10;
  if (uVar4 < 8) {
    if ((uVar10 & 4) == 0) {
      if ((uVar4 != 0) && (buf_00->in[0] = *buf, (uVar10 & 2) != 0)) {
        *(undefined2 *)((long)ctx->bits + uVar10 + 6) = *(undefined2 *)((long)buf + (uVar10 - 2));
      }
    }
    else {
      *(u_int32_t *)buf_00->in = *(u_int32_t *)buf;
      *(undefined4 *)((long)ctx->bits + uVar10 + 4) = *(undefined4 *)((long)buf + (uVar10 - 4));
    }
  }
  else {
    uVar7 = (ulong)(ctx->in + 8) & 0xfffffffffffffff8;
    *(undefined8 *)buf_00->in = *(undefined8 *)buf;
    *(undefined8 *)((long)ctx->bits + uVar10) = *(undefined8 *)((long)buf + (uVar10 - 8));
    puVar11 = (u_int32_t *)((long)puVar11 - uVar7);
    uVar4 = (int)puVar11 + uVar4 & 0xfffffff8;
    if (7 < uVar4) {
      uVar9 = 0;
      do {
        uVar10 = (ulong)uVar9;
        uVar9 = uVar9 + 8;
        *(undefined8 *)(uVar7 + uVar10) = *(undefined8 *)((long)buf + (uVar10 - (long)puVar11));
      } while (uVar9 < uVar4);
    }
  }
  return;
}


void mem_free(void *p)

{
  if (p == (void *)0xffffffffffffffff) {
    return;
  }
  if (p != (void *)0x0) {
    free(p);
    return;
  }
  errline = 0x4c;
  errfile = "error.h";
  int_error((u_char *)"mem_free(NULL)");
  return;
}


void menu_screen(char *host,char *name,ushort *p,int *color)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  size_t sVar9;
  ulonglong uVar10;
  char *pcVar11;
  FILE *pFVar12;
  long lVar13;
  hostent *phVar14;
  size_t sVar15;
  long lVar16;
  uint *puVar17;
  uint *puVar18;
  ushort uVar19;
  msgline_type *pmVar20;
  long in_FS_OFFSET;
  bool bVar21;
  uint local_23c;
  int local_238;
  int local_234;
  int local_230;
  int sprite;
  char *banner;
  char *color_name [15];
  char txt [8];
  char port [9];
  u_char msg [256];
  
  lVar16 = *(long *)(in_FS_OFFSET + 0x28);
  color_name[0] = "red";
  color_name[1] = "green";
  color_name[2] = "brown";
  color_name[3] = "blue";
  color_name[4] = "violet";
  color_name[5] = "cyan";
  color_name[6] = "gray";
  color_name[7] = "black";
  color_name[8] = "light red";
  color_name[9] = "light green";
  color_name[10] = "yellow";
  color_name[11] = "light blue";
  color_name[14] = "white";
  color_name[12] = "magenta";
  color_name[13] = "light cyan";
  load_banner(&banner);
  sVar9 = strlen(banner);
  __sprintf_chk(port,1,9,"%d",*p);
  __sprintf_chk(txt,1,8,"hero%d",*color);
  iVar4 = find_sprite((u_char *)txt,&sprite);
  if (iVar4 != 0) {
    mem_free(banner);
    shut_down();
    __snprintf_chk(msg,0x100,1,0x100,"Error: Can\'t find sprite \"%s\".\n",txt);
    __fprintf_chk(stderr,1,msg);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  bVar3 = false;
  iVar5 = 0;
  local_230 = 0;
  local_234 = 0;
  local_238 = 0;
  local_23c = 0;
  do {
    uVar10 = get_time();
    clear_screen();
    if (TRI_D_ON != 0) {
      tri_d = 1;
      put_sprite(SCREEN_X + -0x47 >> 1,0,
                 sprites[title_sprite].positions + sprites[title_sprite].steps[local_238],'\0');
      put_sprite(SCREEN_XOFFSET + 0x10 + ((SCREEN_X - SCREEN_XOFFSET) + -0x23 >> 1),
                 (SCREEN_Y + -0x19 >> 1) + 10,
                 sprites[bulge_sprite].positions + sprites[bulge_sprite].steps[local_234],'\0');
      put_sprite(SCREEN_XOFFSET + 1,(SCREEN_Y + -0x1a >> 1) + 10,
                 sprites[sprite].positions +
                 sprites[sprite].steps[(int)((uint)(local_23c < 2) * 9 + 0x27 + (local_23c & 7))],
                 '\0');
      tri_d = 0;
    }
    put_sprite(SCREEN_X + -0x48 >> 1,0,
               sprites[title_sprite].positions + sprites[title_sprite].steps[local_238],'\0');
    put_sprite(SCREEN_XOFFSET + 0xf + ((SCREEN_X - SCREEN_XOFFSET) + -0x23 >> 1),
               (SCREEN_Y + -0x19 >> 1) + 10,
               sprites[bulge_sprite].positions + sprites[bulge_sprite].steps[local_234],'\0');
    put_sprite(SCREEN_XOFFSET,(SCREEN_Y + -0x1a >> 1) + 10,
               sprites[sprite].positions +
               sprites[sprite].steps[(int)((uint)(local_23c < 2) * 9 + 0x27 + (local_23c & 7))],'\0'
              );
    print2screen(0,0xe,'\v',"N:");
    print2screen(1,0xe,'\x02',"AME:");
    print2screen(6,0xe,'\a',name);
    print2screen(0,0x10,'\x02',"COLOR:");
    print2screen(7,0x10,'\a',color_name[(*color + -1) % 0xf]);
    print2screen(0,10,'\v',"S");
    print2screen(1,10,'\x02',"ERVER ADDRESS:");
    print2screen(0x10,10,'\a',host);
    print2screen(0,0xc,'\v',"P");
    print2screen(1,0xc,'\x02',"ORT:");
    print2screen(6,0xc,'\a',port);
    print2screen(SCREEN_X + -0x1a >> 1,SCREEN_Y + -3,'\a',"Use arrows to change color");
    print2screen(SCREEN_X + -0x34 >> 1,SCREEN_Y + -2,'\a',
                 "Press ENTER to connect, H for help, ESC or Q to quit");
    print2screen(0,SCREEN_Y + -1,'\x0f',banner + (local_230 >> 1));
    iVar1 = sprites[title_sprite].n_steps;
    iVar2 = sprites[bulge_sprite].n_steps;
    if (error_message[0] == '\0') {
      if (bVar3) {
        print_help_window();
        if (iVar5 != 2) goto LAB_0010732d;
LAB_00107987:
        iVar6 = read_num_online(SCREEN_Y + -1,port,"Enter port: ",8);
        if (iVar6 != 0) {
LAB_00107341:
          c_update_kbd();
joined_r0x00107ab8:
          if (TRI_D_ON == 0) {
            blit_screen('\x01');
            iVar5 = c_was_pressed(0x2b);
            if (iVar5 != 0) goto LAB_00107370;
LAB_00107af8:
            iVar5 = c_was_pressed(0x3d);
            if ((((iVar5 != 0) || (iVar5 = c_was_pressed(0x15a), iVar5 != 0)) ||
                (iVar5 = c_was_pressed(0x159), iVar5 != 0)) ||
               (iVar5 = c_was_pressed(0x154), iVar5 != 0)) goto LAB_00107370;
          }
          else {
            tri_d = 1;
            blit_screen('\x01');
            tri_d = 0;
            blit_screen('\x01');
            iVar5 = c_was_pressed(0x2b);
            if (iVar5 == 0) goto LAB_00107af8;
LAB_00107370:
            iVar5 = *color + 1;
            if (0x1e < iVar5) {
              iVar5 = 1;
            }
            *color = iVar5;
            __sprintf_chk(txt,1,8);
            iVar5 = find_sprite((u_char *)txt,&sprite);
            if (iVar5 != 0) {
              shut_down();
              if (banner != (char *)0xffffffffffffffff) {
                if (banner == (char *)0x0) {
                  errline = 0x4c;
                  errfile = "error.h";
                  int_error((u_char *)"mem_free(NULL)");
                }
                else {
                  free(banner);
                }
              }
              __fprintf_chk(stderr,1,"Error: Can\'t find sprite \"%s\".\n");
            }
          }
          iVar5 = c_was_pressed(0x2d);
          if (((iVar5 != 0) || (iVar5 = c_was_pressed(0x153), iVar5 != 0)) ||
             ((iVar5 = c_was_pressed(0x15b), iVar5 != 0 ||
              (iVar5 = c_was_pressed(0x158), iVar5 != 0)))) {
            iVar5 = *color + -1;
            if (iVar5 < 1) {
              iVar5 = 0x1e;
            }
            *color = iVar5;
            __sprintf_chk(txt,1,8);
            iVar5 = find_sprite((u_char *)txt,&sprite);
            if (iVar5 != 0) {
              shut_down();
              if (banner != (char *)0xffffffffffffffff) {
                if (banner == (char *)0x0) {
                  errline = 0x4c;
                  errfile = "error.h";
                  int_error((u_char *)"mem_free(NULL)");
                }
                else {
                  free(banner);
                }
              }
              __fprintf_chk(stderr,1,"Error: Can\'t find sprite \"%s\".\n");
            }
          }
          iVar5 = c_was_pressed(0x68);
          bVar21 = (bool)(bVar3 ^ 1);
          if (iVar5 == 0) {
            bVar21 = bVar3;
          }
          iVar5 = c_was_pressed(0x12f);
          bVar3 = bVar21;
          if (iVar5 != 0) {
            pcVar11 = getenv("HOME");
            __sprintf_chk(msg,1,0x100,"%s/%s",pcVar11);
            pFVar12 = fopen((char *)msg,"w");
            if (pFVar12 != (FILE *)0x0) {
              __fprintf_chk(pFVar12,1,"%s\n%s\n%d\n",host,name);
              fclose(pFVar12);
            }
            lVar13 = strtol(port,(char **)0x0,10);
            uVar19 = (ushort)lVar13;
            *p = uVar19;
            phVar14 = gethostbyname(host);
            if (phVar14 == (hostent *)0x0) {
              print2screen((int)((long)SCREEN_X - 0x24U >> 1),SCREEN_Y + -1,'\t',
                           "Error: Can\'t resolve server address.");
              if (TRI_D_ON != 0) {
                tri_d = 1;
                blit_screen('\x01');
                tri_d = 0;
              }
              blit_screen('\x01');
              c_update_kbd();
              while (iVar5 = c_was_pressed(0x12f), iVar5 == 0) {
                c_wait_for_key();
                c_update_kbd();
              }
              iVar5 = 0;
            }
            else {
              server.sin_port = uVar19 << 8 | uVar19 >> 8;
              server.sin_family = 2;
              server.sin_addr = *(in_addr_t *)*phVar14->h_addr_list;
              fd = socket(2,2,0x11);
              if (fd < 0) {
                print2screen((int)((long)SCREEN_X - 0x12U >> 1),SCREEN_Y + -1,'\t',
                             "Can\'t get socket.\n");
                if (TRI_D_ON != 0) {
                  tri_d = 1;
                  blit_screen('\x01');
                  tri_d = 0;
                }
                blit_screen('\x01');
                c_update_kbd();
                while (iVar5 = c_was_pressed(0x12f), iVar5 == 0) {
                  c_wait_for_key();
                  c_update_kbd();
                }
                iVar5 = 0;
              }
              else {
                pcVar11 = contact_server(*color,(u_char *)name);
                if (pcVar11 == (char *)0x0) {
                  if (banner != (char *)0xffffffffffffffff) {
                    if (banner == (char *)0x0) {
                      errline = 0x4c;
                      errfile = "error.h";
                      int_error((u_char *)"mem_free(NULL)");
                    }
                    else {
                      free(banner);
                    }
                  }
                  if (lVar16 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
                    __stack_chk_fail();
                  }
                  return;
                }
                sVar15 = strlen(pcVar11);
                print2screen((int)((long)SCREEN_X - sVar15 >> 1),SCREEN_Y + -1,'\t',pcVar11);
                if (TRI_D_ON != 0) {
                  tri_d = 1;
                  blit_screen('\x01');
                  tri_d = 0;
                }
                blit_screen('\x01');
                c_update_kbd();
                while (iVar5 = c_was_pressed(0x12f), iVar5 == 0) {
                  c_wait_for_key();
                  c_update_kbd();
                }
                iVar5 = 0;
              }
            }
          }
          goto cc1;
        }
LAB_001079ae:
        c_update_kbd();
      }
      else {
        if (iVar5 == 2) goto LAB_00107987;
LAB_0010732d:
        if (iVar5 != 3) {
          if (iVar5 != 1) goto LAB_00107341;
          iVar6 = read_str_online(SCREEN_Y + -1,name,"Enter your name: ",0x18);
          if (iVar6 == 0) goto LAB_001079ae;
          c_update_kbd();
          goto joined_r0x00107ab8;
        }
        iVar6 = read_str_online(SCREEN_Y + -1,host,"Enter address: ",0x40);
        if (iVar6 != 0) goto LAB_00107341;
        c_update_kbd();
      }
      if (TRI_D_ON == 0) {
        blit_screen('\x01');
      }
      else {
        tri_d = 1;
        blit_screen('\x01');
        tri_d = 0;
        blit_screen('\x01');
      }
    }
    else {
      puVar18 = (uint *)error_message;
      do {
        puVar17 = puVar18;
        uVar7 = *puVar17 + 0xfefefeff & ~*puVar17;
        uVar8 = uVar7 & 0x80808080;
        puVar18 = puVar17 + 1;
      } while (uVar8 == 0);
      bVar21 = (uVar7 & 0x8080) == 0;
      if (bVar21) {
        uVar8 = uVar8 >> 0x10;
      }
      if (bVar21) {
        puVar18 = (uint *)((long)puVar17 + 6);
      }
      print2screen((int)((long)SCREEN_X -
                         ((long)puVar18 + (-0x11b203 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))) >> 1
                        ),SCREEN_Y + -1,'\t',(char *)error_message);
      if (TRI_D_ON != 0) {
        tri_d = 1;
        blit_screen('\x01');
        tri_d = 0;
      }
      blit_screen('\x01');
      c_update_kbd();
      while (iVar6 = c_was_pressed(0x12f), iVar6 == 0) {
        c_wait_for_key();
        c_update_kbd();
      }
      error_message[0] = '\0';
cc1:
      iVar6 = c_was_pressed(0x71);
      if ((iVar6 != 0) || (iVar6 = c_was_pressed(300), iVar6 != 0)) {
        iVar5 = *color;
        pcVar11 = getenv("HOME");
        __sprintf_chk(msg,1,0x100,"%s/%s",pcVar11,".0verkill");
        pFVar12 = fopen((char *)msg,"w");
        if (pFVar12 != (FILE *)0x0) {
          __fprintf_chk(pFVar12,1,"%s\n%s\n%d\n",host,name,iVar5);
          fclose(pFVar12);
        }
        if (banner != (char *)0xffffffffffffffff) {
          if (banner == (char *)0x0) {
            errline = 0x4c;
            errfile = "error.h";
            int_error((u_char *)"mem_free(NULL)");
          }
          else {
            free(banner);
          }
        }
        c_shutdown();
        free_sprites(0);
        free_area();
        if (hero != (it *)0x0) {
          delete_obj((ulong)(*(uint *)&hero->type >> 8));
        }
        while (objects.next != (object_list *)0x0) {
          delete_obj((ulong)(*(uint *)&((objects.next)->member).type >> 8));
        }
        lVar16 = (long)last_message;
        pmVar20 = msg_line;
        if (-1 < last_message) {
          do {
            pcVar11 = pmVar20->msg;
            if (pcVar11 != (char *)0xffffffffffffffff) {
              if (pcVar11 == (char *)0x0) {
                errfile = "error.h";
                errline = 0x4c;
                int_error((u_char *)"mem_free(NULL)");
                lVar16 = (long)last_message;
              }
              else {
                free(pcVar11);
                lVar16 = (long)last_message;
              }
            }
            iVar4 = iVar4 + 1;
            pmVar20 = pmVar20 + 1;
          } while (iVar4 <= (int)lVar16);
        }
        msg_line[lVar16].msg = (char *)0x0;
        shutdown_sprites();
        check_memory_leaks();
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
      iVar6 = c_was_pressed(0x6e);
      if (iVar6 != 0) {
        iVar5 = 1;
      }
      iVar6 = c_was_pressed(0x61);
      if ((iVar6 != 0) || (iVar6 = c_was_pressed(0x73), iVar6 != 0)) {
        iVar5 = 3;
      }
      iVar6 = c_was_pressed(0x70);
      if (iVar6 != 0) {
        iVar5 = 2;
      }
    }
    local_230 = (local_230 + 1) % ((int)sVar9 * 2);
    local_23c = local_23c + 1 & 0xf;
    local_238 = (local_238 + 1) % iVar1;
    local_234 = (local_234 + 1) % iVar2;
    sleep_until(uVar10 + 50000);
  } while( true );
}


int my_getchar(void)

{
  long in_FS_OFFSET;
  u_char a;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  read(0,&a,1);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return (int)(uint)a;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void my_print(char *str)

{
  int iVar1;
  int iVar2;
  size_t sVar4;
  char *pcVar5;
  long lVar6;
  int iVar7;
  int iVar3;
  
  sVar4 = strlen(str);
  lVar6 = (long)screen_buffer_pos;
  iVar3 = (int)sVar4;
  iVar2 = iVar3 + screen_buffer_pos;
  iVar7 = screen_buffer_size;
  pcVar5 = screen_buffer;
  if (screen_buffer_size < iVar2) {
    do {
      screen_buffer_size = iVar7 * 2;
      iVar1 = iVar7 * -2;
      iVar7 = screen_buffer_size;
    } while (iVar2 != screen_buffer_size && SBORROW4(iVar2,screen_buffer_size) == iVar2 + iVar1 < 0)
    ;
    sVar4 = SEXT48(screen_buffer_size);
    if (screen_buffer == (char *)0xffffffffffffffff) {
      if ((sVar4 != 0) && (pcVar5 = (char *)malloc(sVar4), pcVar5 == (char *)0x0)) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        goto LAB_0010d071;
      }
    }
    else {
      if (screen_buffer == (char *)0x0) {
        errline = 0x56;
        errfile = "error.h";
        int_error((u_char *)"mem_realloc(NULL, %d)");
        goto LAB_0010d071;
      }
      if (sVar4 == 0) {
        free(screen_buffer);
        lVar6 = (long)screen_buffer_pos;
        screen_buffer = (char *)0xffffffffffffffff;
        pcVar5 = screen_buffer;
      }
      else {
        pcVar5 = (char *)realloc(screen_buffer,sVar4);
        if (pcVar5 == (char *)0x0) {
          do {
            error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
LAB_0010d071:
            screen_buffer = (char *)0x0;
            my_putc();
          } while( true );
        }
        lVar6 = (long)screen_buffer_pos;
      }
    }
  }
  screen_buffer = pcVar5;
  memcpy(screen_buffer + lVar6,str,(long)iVar3);
  screen_buffer_pos = screen_buffer_pos + iVar3;
  return;
}


void my_putc(void)

{
  c_shutdown();
  fwrite("Error: Can\'t reallocate screen buffer.\n",1,0x27,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void my_putc(char c)

{
  char *pcVar1;
  long lVar2;
  size_t __size;
  
  lVar2 = (long)screen_buffer_pos;
  pcVar1 = screen_buffer;
  if (screen_buffer_size <= screen_buffer_pos) {
    screen_buffer_size = screen_buffer_size * 2;
    __size = SEXT48(screen_buffer_size);
    if (screen_buffer == (char *)0xffffffffffffffff) {
      if ((__size != 0) && (pcVar1 = (char *)malloc(__size), pcVar1 == (char *)0x0)) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
LAB_0010d2f2:
        do {
          screen_buffer = (char *)0x0;
          my_putc();
LAB_0010d300:
          errline = 0x56;
          errfile = "error.h";
          int_error((u_char *)"mem_realloc(NULL, %d)");
        } while( true );
      }
    }
    else {
      if (screen_buffer == (char *)0x0) goto LAB_0010d300;
      if (__size == 0) {
        free(screen_buffer);
        lVar2 = (long)screen_buffer_pos;
        screen_buffer = (char *)0xffffffffffffffff;
        pcVar1 = screen_buffer;
      }
      else {
        pcVar1 = (char *)realloc(screen_buffer,__size);
        if (pcVar1 == (char *)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          goto LAB_0010d2f2;
        }
        lVar2 = (long)screen_buffer_pos;
      }
    }
  }
  screen_buffer = pcVar1;
  screen_buffer[lVar2] = c;
  screen_buffer_pos = screen_buffer_pos + 1;
  return;
}


void my_putc(void)

{
  c_shutdown();
  fwrite("Error: Can\'t reallocate screen buffer.\n",1,0x27,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void my_sleep(ulonglong t)

{
  long lVar1;
  long in_FS_OFFSET;
  timeval tv;
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  tv.tv_sec = 0;
  tv.tv_usec = t;
  select(0,(fd_set *)0x0,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


int my_toupper(int c)

{
  __int32_t **pp_Var1;
  
  switch(c) {
  case 0x27:
    c = 0x22;
    break;
  default:
    if (c + 0x80U < 0x180) {
      pp_Var1 = __ctype_toupper_loc();
      c = (*pp_Var1)[c];
    }
    break;
  case 0x2c:
    c = 0x3c;
    break;
  case 0x2d:
    c = 0x5f;
    break;
  case 0x2e:
    c = 0x3e;
    break;
  case 0x2f:
    c = 0x3f;
    break;
  case 0x30:
    c = 0x29;
    break;
  case 0x31:
    c = 0x21;
    break;
  case 0x32:
    c = 0x40;
    break;
  case 0x33:
    c = 0x23;
    break;
  case 0x34:
    c = 0x24;
    break;
  case 0x35:
    c = 0x25;
    break;
  case 0x36:
    c = 0x5e;
    break;
  case 0x37:
    c = 0x26;
    break;
  case 0x38:
    c = 0x2a;
    break;
  case 0x39:
    c = 0x28;
    break;
  case 0x3b:
    c = 0x3a;
    break;
  case 0x3d:
    c = 0x2b;
    break;
  case 0x5b:
    c = 0x7b;
    break;
  case 0x5c:
    c = 0x7c;
    break;
  case 0x5d:
    c = 0x7d;
    break;
  case 0x60:
    c = 0x7e;
  }
  return c;
}


it * new_obj(uint id,u_char type,int ttl,int sprite,u_char pos,int status,my_double x,my_double y,
            my_double xspeed,my_double yspeed,void *data)

{
  object_list *poVar1;
  object_list *poVar2;
  ulonglong uVar3;
  object_list *poVar4;
  
  poVar4 = last_obj;
  poVar1 = (object_list *)malloc(0x48);
  poVar2 = poVar4;
  if (poVar1 == (object_list *)0x0) {
    error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
    poVar2 = last_obj;
  }
  poVar4->next = poVar1;
  poVar4 = poVar2->next;
  if (poVar4 != (object_list *)0x0) {
    poVar4->prev = poVar2;
    (poVar4->member).type = type;
    (poVar4->member).data = data;
    last_obj = poVar4;
    *(char *)&(poVar4->member).field_0x11 = (char)id;
    *(char *)&(poVar4->member).field_0x13 = (char)(id >> 0x10);
    poVar4->next = (object_list *)0x0;
    *(short *)&(poVar4->member).field_0x14 = (short)ttl;
    *(short *)&(poVar4->member).field_0x16 = (short)sprite;
    *(ushort *)&(poVar4->member).field_0x18 = (ushort)pos;
    *(char *)&(poVar4->member).field_0x12 = (char)(id >> 8);
    *(short *)&(poVar4->member).field_0x1a = (short)status;
    (poVar4->member).update_counter = '\0';
    *(ulong *)&poVar4->member = CONCAT44(y,x);
    *(ulong *)&(poVar4->member).xspeed = CONCAT44(yspeed,xspeed);
    uVar3 = get_time();
    poVar2 = last_obj;
    (poVar4->member).last_updated = uVar3;
    add_to_table(poVar2);
    poVar4 = (object_list *)&last_obj->member;
  }
  return (it *)poVar4;
}


int _next_anim_left(int pos,int status,int ttl)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = pos;
  if (8 < pos) {
    if (pos < 0x1d) {
      uVar2 = pos - 0x14;
    }
    else {
      uVar2 = pos - 0x38;
      if (pos < 0x26) {
        uVar2 = pos - 0x1d;
      }
    }
  }
  if ((status & 0x100U) != 0) {
    return (int)((uVar2 + 1 & 7) + 0x38);
  }
  uVar1 = status & 0x10;
  if (uVar1 != 0) {
    uVar1 = (uint)((int)(weapon[current_weapon].cadence + 3) < ttl) * 9 + 0x14;
  }
  return (int)(uVar1 + 1 + (uVar2 & 7));
}


int _next_anim_right(int pos,int status,int ttl)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 10;
  if (((0x12 < pos) && (iVar1 = 0x26, 0x2e < pos)) && (iVar1 = 0x2f, 0x37 < pos)) {
    iVar1 = 0;
  }
  if ((status & 0x100U) != 0) {
    return (int)(((pos - iVar1) + 1U & 7) + 0x40);
  }
  iVar2 = 10;
  if ((status & 0x10U) != 0) {
    iVar2 = (uint)((int)(weapon[current_weapon].cadence + 3) < ttl) * 9 + 0x26;
  }
  return (int)(iVar2 + 1 + (pos - iVar1 & 7U));
}


void parse_command_line(int argc,char **argv)

{
  int iVar1;
  
  while( true ) {
    iVar1 = getopt(argc,argv,"3hl:s:");
    if (iVar1 == 0x3f) break;
    if (iVar1 < 0x40) {
      if (iVar1 == 0x33) {
        TRI_D_ON = 1;
      }
      else {
        if (iVar1 == 0x3a) break;
        if (iVar1 == -1) {
          return;
        }
      }
    }
    else {
      if (iVar1 == 0x68) {
        puts("0verkill client.\n(c)2000 Brainsoft\nUsage: 0verkill [-h] [-3] [-s <width>x<height>]")
        ;
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
      if (iVar1 == 0x73) {
        parse_dimensions(optarg);
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void parse_dimensions(char *txt)

{
  char *pcVar1;
  ulong uVar2;
  char cVar3;
  long in_FS_OFFSET;
  char *e;
  long local_20;
  
  cVar3 = *txt;
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  pcVar1 = txt;
  if ((cVar3 != 'x') && (cVar3 != '\0')) {
    do {
      cVar3 = pcVar1[1];
      pcVar1 = pcVar1 + 1;
      if (cVar3 == '\0') goto LAB_00107fc3;
    } while (cVar3 != 'x');
  }
  if (cVar3 != '\0') {
    *pcVar1 = '\0';
    if ((*txt != '\0') && (pcVar1[1] != '\0')) {
      uVar2 = strtoul(txt,&e,10);
      SCREEN_X = (int)uVar2;
      if (*e == '\0') {
        uVar2 = strtoul(pcVar1 + 1,&e,10);
        SCREEN_Y = (int)uVar2;
        if (*e == '\0') {
          set_size = '\x01';
          if (local_20 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
            __stack_chk_fail();
          }
          set_size = '\x01';
          return;
        }
      }
    }
    fwrite("Error: Decimal number expected.\n",1,0x20,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
LAB_00107fc3:
  fwrite("Error: Expected dimensions in form WIDTHxHEIGHT.\n",1,0x31,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}


/* WARNING: Could not reconcile some variable overlaps */

void play(void)

{
  char **ppcVar1;
  long lVar2;
  ulong uVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  ulonglong uVar9;
  int iVar10;
  msgline_type *pmVar11;
  uint *puVar12;
  uint *puVar13;
  ulong t;
  int iVar14;
  long lVar15;
  long in_FS_OFFSET;
  bool bVar16;
  u_char packet_1;
  u_char packet [3];
  char string [80];
  
  lVar2 = *(long *)(in_FS_OFFSET + 0x28);
  uVar9 = get_time();
  bVar5 = false;
  t = uVar9 + 0x7a12;
  bVar4 = false;
  bVar16 = false;
  uVar9 = get_time();
  if (0x2faf08 < uVar9 - t) goto LAB_0010838b;
LAB_00108100:
  iVar6 = read_data();
  do {
    if (iVar6 != 0) goto LAB_001083a8;
    clear_screen();
    iVar10 = 0;
    iVar14 = 0;
    lVar15 = 0;
    update_game();
    draw_scene();
    if (-1 < last_message) {
      do {
        uVar3 = msg_line[lVar15].time;
        while (t < uVar3) {
          iVar10 = iVar10 + 1;
          iVar14 = iVar14 + 1;
          if (last_message < iVar10) goto LAB_0010816b;
          lVar15 = (long)iVar14;
          uVar3 = msg_line[lVar15].time;
        }
        scroll_messages();
        iVar10 = iVar10 + 1;
      } while (iVar10 <= last_message);
LAB_0010816b:
      if (-1 < last_message) {
        pmVar11 = msg_line;
        do {
          ppcVar1 = &pmVar11->msg;
          iVar14 = iVar6 + 1;
          pmVar11 = pmVar11 + 1;
          print2screen(0,iVar6,'\v',*ppcVar1);
          iVar6 = iVar14;
        } while (iVar14 <= last_message);
      }
    }
    if (bVar16) {
      iVar6 = read_str_online(SCREEN_Y + -3,string,"> ",0x4e);
      puVar13 = (uint *)string;
      if (iVar6 == 1) {
        do {
          puVar12 = puVar13;
          uVar7 = *puVar12 + 0xfefefeff & ~*puVar12;
          uVar8 = uVar7 & 0x80808080;
          puVar13 = puVar12 + 1;
        } while (uVar8 == 0);
        packet_7196[0] = 0xd;
        bVar16 = (uVar7 & 0x8080) == 0;
        if (bVar16) {
          uVar8 = uVar8 >> 0x10;
        }
        if (bVar16) {
          puVar13 = (uint *)((long)puVar12 + 6);
        }
        iVar6 = (((int)puVar13 + -3) - (uint)CARRY1((byte)uVar8,(byte)uVar8)) - (int)string;
        __memcpy_chk(0x118441,string,iVar6 + 1);
        send_packet(packet_7196,iVar6 + 2,(sockaddr *)&server,my_id,0);
        bVar16 = false;
      }
      else {
        bVar16 = iVar6 != 2;
      }
    }
    if (bVar4) {
      print_help_window();
    }
    if ((bVar5) && (active_players != 0)) {
      print_hall_of_fame();
    }
    draw_board();
    if (TRI_D_ON != 0) {
      tri_d = 1;
      blit_screen('\x01');
      tri_d = 0;
    }
    blit_screen('\x01');
    creep = '\0';
    if (bVar16) {
      iVar6 = c_was_pressed(0x138);
      if (iVar6 != 0) {
        autorun = autorun ^ 1;
      }
      iVar6 = c_was_pressed(0x146);
      if (iVar6 != 0) {
LAB_0010878c:
        send_quit();
        error_message[0] = '\0';
LAB_001083a8:
        if (lVar2 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        return;
      }
      iVar6 = c_pressed(0x148);
      if (iVar6 != 0) {
        packet_1 = '\x03';
        send_packet(&packet_1,1,(sockaddr *)&server,my_id,0);
      }
LAB_00108238:
      keyboard_status = 0;
      bVar16 = true;
    }
    else {
      c_update_kbd();
      iVar6 = c_was_pressed(0x72);
      if (iVar6 != 0) {
        c_cls();
        redraw_screen();
      }
      iVar6 = c_was_pressed(0x68);
      if (iVar6 != 0) {
        bVar4 = (bool)(bVar4 ^ 1);
      }
      iVar6 = c_was_pressed(0x12e);
      if (iVar6 != 0) {
        if (bVar5) {
          bVar5 = false;
        }
        else {
          packet_1 = '\x13';
          send_packet(&packet_1,1,(sockaddr *)&server,my_id,0);
          bVar5 = true;
        }
      }
      iVar6 = c_was_pressed(0x138);
      if ((iVar6 != 0) || (iVar6 = c_was_pressed(0x61), iVar6 != 0)) {
        autorun = autorun ^ 1;
      }
      iVar6 = c_was_pressed(0x146);
      if ((iVar6 != 0) || (iVar6 = c_was_pressed(0x71), iVar6 != 0)) goto LAB_0010878c;
      iVar6 = c_pressed(0x148);
      if (iVar6 != 0) {
        packet_1 = '\x03';
        send_packet(&packet_1,1,(sockaddr *)&server,my_id,0);
      }
      iVar6 = c_was_pressed(0x12f);
      if (iVar6 != 0) {
        string._0_16_ = (undefined  [16])0x0;
        string._16_16_ = (undefined  [16])0x0;
        string._32_16_ = (undefined  [16])0x0;
        string._48_16_ = (undefined  [16])0x0;
        string._64_16_ = (undefined  [16])0x0;
        goto LAB_00108238;
      }
      keyboard_status = 0;
      iVar6 = c_was_pressed(0x20);
      if (iVar6 != 0) {
        packet_1 = '\x14';
        send_packet(&packet_1,1,(sockaddr *)&server,my_id,0);
      }
      iVar6 = c_was_pressed(99);
      if (iVar6 != 0) {
        autocreep = autocreep ^ 1;
      }
    }
    iVar6 = c_pressed(0x15b);
    if ((iVar6 != 0) || (autocreep != '\0')) {
      keyboard_status._0_4_ = CONCAT13(1,keyboard_status._0_3_);
      keyboard_status = keyboard_status & 0xffffffff00000000 | (ulong)keyboard_status._0_4_;
    }
    iVar6 = c_pressed(0x15a);
    if (iVar6 != 0) {
      keyboard_status._0_3_ = CONCAT12(1,keyboard_status._0_2_);
      keyboard_status = keyboard_status & 0xffffffffff000000 | (ulong)keyboard_status._0_3_;
    }
    iVar6 = c_pressed(0x133);
    if ((iVar6 == 0) && (iVar6 = c_pressed(0x132), iVar6 == 0)) {
      if (bVar16) goto LAB_0010829f;
      iVar6 = c_pressed(0x7a);
      if (iVar6 != 0) {
        keyboard_status._0_6_ = CONCAT15(1,keyboard_status._0_5_);
        keyboard_status = keyboard_status & 0xffff000000000000 | (ulong)keyboard_status._0_6_;
      }
LAB_001085a8:
      iVar6 = c_pressed(100);
      if (iVar6 != 0) {
        keyboard_status = CONCAT17(1,keyboard_status._0_7_);
      }
      iVar6 = c_was_pressed(0x31);
      if (iVar6 != 0) {
        keyboard_status._0_7_ = CONCAT16(1,keyboard_status._0_6_);
        keyboard_status = keyboard_status & 0xff00000000000000 | (ulong)keyboard_status._0_7_;
      }
      iVar6 = c_was_pressed(0x32);
      if (iVar6 != 0) {
        keyboard_status._0_7_ = CONCAT16(2,keyboard_status._0_6_);
        keyboard_status = keyboard_status & 0xff00000000000000 | (ulong)keyboard_status._0_7_;
      }
      iVar6 = c_was_pressed(0x33);
      if (iVar6 != 0) {
        keyboard_status._0_7_ = CONCAT16(3,keyboard_status._0_6_);
        keyboard_status = keyboard_status & 0xff00000000000000 | (ulong)keyboard_status._0_7_;
      }
      iVar6 = c_was_pressed(0x34);
      if (iVar6 != 0) {
        keyboard_status._0_7_ = CONCAT16(4,keyboard_status._0_6_);
        keyboard_status = keyboard_status & 0xff00000000000000 | (ulong)keyboard_status._0_7_;
      }
      iVar6 = c_was_pressed(0x35);
      if (iVar6 == 0) goto LAB_0010829f;
      keyboard_status._0_7_ = CONCAT16(5,keyboard_status._0_6_);
      keyboard_status = keyboard_status & 0xff00000000000000 | (ulong)keyboard_status._0_7_;
      iVar6 = c_pressed(0x135);
      if (iVar6 == 0) goto LAB_00108640;
LAB_001082b1:
      keyboard_status._0_5_ = CONCAT14(1,keyboard_status._0_4_);
      keyboard_status = keyboard_status & 0xffffff0000000000 | (ulong)keyboard_status._0_5_;
    }
    else {
      keyboard_status._0_6_ = CONCAT15(1,keyboard_status._0_5_);
      keyboard_status = keyboard_status & 0xffff000000000000 | (ulong)keyboard_status._0_6_;
      if (!bVar16) goto LAB_001085a8;
LAB_0010829f:
      iVar6 = c_pressed(0x135);
      if (iVar6 != 0) goto LAB_001082b1;
LAB_00108640:
      iVar6 = c_pressed(0x130);
      if ((iVar6 != 0) || (autorun != '\0')) goto LAB_001082b1;
    }
    iVar6 = c_pressed(0x158);
    if (iVar6 != 0) {
      keyboard_status._0_2_ = CONCAT11(1,keyboard_status.right);
      keyboard_status = keyboard_status & 0xffffffffffff0000 | (ulong)keyboard_status._0_2_;
    }
    iVar6 = c_pressed(0x159);
    if (iVar6 != 0) {
      keyboard_status = CONCAT71(keyboard_status._1_7_,1);
      keyboard_status.right = 1;
    }
    t = t + 0x7a12;
    packet[0] = '\x12';
    packet[1] = keyboard_status.left * '\x02' | keyboard_status.jump << 2 |
                keyboard_status.creep << 3 | keyboard_status.speed << 4 | keyboard_status.fire << 5
                | keyboard_status.down_ladder << 6 | keyboard_status.right;
    packet[2] = keyboard_status.weapon;
    send_packet(packet,3,(sockaddr *)&server,my_id,0);
    sleep_until(t);
    uVar9 = get_time();
    if (uVar9 - t < 0x2faf09) goto LAB_00108100;
LAB_0010838b:
    t = get_time();
    iVar6 = read_data();
  } while( true );
}


void print2screen(int x,int y,u_char color,char *message)

{
  int iVar1;
  int iVar2;
  size_t sVar3;
  long lVar4;
  int iVar5;
  
  if ((y < SCREEN_Y) && (x < SCREEN_X)) {
    iVar5 = SCREEN_X * SCREEN_Y;
    iVar1 = y * SCREEN_X + x;
    sVar3 = strlen(message);
    lVar4 = (long)iVar1;
    iVar2 = (int)sVar3;
    if (iVar5 < iVar1 + (int)sVar3) {
      iVar2 = iVar5 - iVar1;
    }
    sVar3 = SEXT48(iVar2);
    memset(screen_a + lVar4,color & 0xf,sVar3);
    memcpy(screen + lVar4,message,sVar3);
    if (TRI_D_ON != 0) {
      memset(screen2_a + lVar4,color & 0xf,sVar3);
      memcpy(screen2 + lVar4,message,sVar3);
      return;
    }
  }
  return;
}


void print_hall_of_fame(void)

{
  if (active_players == 0) {
    return;
  }
  print_hall_of_fame();
  return;
}


void print_hall_of_fame(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  anon_struct_conflicte7f *message;
  u_char color;
  long in_FS_OFFSET;
  bool bVar8;
  char txt [32];
  undefined8 local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  __sprintf_chk(txt,1,0x20,"TOP %d PLAYERS",10);
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  draw_frame(SCREEN_X + -0x2b >> 1,(SCREEN_Y + -2) - (iVar6 + 5) >> 1,0x29,iVar6 + 5,'\x0f');
  puVar5 = (uint *)txt;
  do {
    puVar4 = puVar5;
    uVar1 = *puVar4 + 0xfefefeff & ~*puVar4;
    uVar2 = uVar1 & 0x80808080;
    puVar5 = puVar4 + 1;
  } while (uVar2 == 0);
  bVar8 = (uVar1 & 0x8080) == 0;
  if (bVar8) {
    uVar2 = uVar2 >> 0x10;
  }
  if (bVar8) {
    puVar5 = (uint *)((long)puVar4 + 6);
  }
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  print2screen((SCREEN_X + -0x2b >> 1) + 1 +
               (int)((long)&local_30 +
                     (1 - ((long)puVar5 + (-3 - (ulong)CARRY1((byte)uVar2,(byte)uVar2)))) >> 1),
               ((SCREEN_Y - iVar6) + -7 >> 1) + 1,'\v',txt);
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  print2screen((SCREEN_X + -0x2b >> 1) + 2,((SCREEN_Y - iVar6) + -7 >> 1) + 3,'\x0f',"NAME");
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  print2screen((SCREEN_X + -0x2b >> 1) + 0x1c,((SCREEN_Y - iVar6) + -7 >> 1) + 3,'\x0f',"FRAGS");
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  print2screen((SCREEN_X + -0x2b >> 1) + 0x23,((SCREEN_Y - iVar6) + -7 >> 1) + 3,'\x0f',"DEATHS");
  if (0 < active_players) {
    message = top_players;
    iVar6 = 0;
    do {
      iVar3 = message->color - 1;
      iVar7 = active_players;
      if (10 < active_players) {
        iVar7 = 10;
      }
      color = (char)iVar3 + (char)(iVar3 / 0xf) * -0xf + '\x01';
      print2screen((SCREEN_X + -0x2b >> 1) + 2,iVar6 + 4 + ((SCREEN_Y - iVar7) + -7 >> 1),color,
                   (char *)message);
      __sprintf_chk(txt,1,0x20,&DAT_0011203c);
      iVar7 = 10;
      if (active_players < 0xb) {
        iVar7 = active_players;
      }
      print2screen((SCREEN_X + -0x2b >> 1) + 0x1c,iVar6 + 4 + ((SCREEN_Y - iVar7) + -7 >> 1),color,
                   txt);
      __sprintf_chk(txt,1,0x20,&DAT_0011203c);
      iVar7 = 10;
      if (active_players < 0xb) {
        iVar7 = active_players;
      }
      message = message + 1;
      iVar3 = iVar6 + 4;
      iVar6 = iVar6 + 1;
      print2screen((SCREEN_X + -0x2b >> 1) + 0x23,iVar3 + ((SCREEN_Y - iVar7) + -7 >> 1),color,txt);
    } while ((iVar6 != 10) && (iVar6 < active_players));
  }
  __sprintf_chk(txt,1,0x20,"%d");
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  print2screen((SCREEN_X + -0x2b >> 1) + 2,((SCREEN_Y + -2) - (iVar6 + 5) >> 1) + iVar6 + 5,'\v',
               "Players in the game:");
  iVar6 = 10;
  if (active_players < 0xb) {
    iVar6 = active_players;
  }
  print2screen((SCREEN_X + -0x2b >> 1) + 0x17,((SCREEN_Y + -2) - (iVar6 + 5) >> 1) + iVar6 + 5,'\a',
               txt);
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void print_help(void)

{
  puts("0verkill client.\n(c)2000 Brainsoft\nUsage: 0verkill [-h] [-3] [-s <width>x<height>]");
  return;
}


void print_messages(void)

{
  char **ppcVar1;
  int y;
  int iVar2;
  msgline_type *pmVar3;
  
  if (-1 < last_message) {
    pmVar3 = msg_line;
    y = 0;
    do {
      ppcVar1 = &pmVar3->msg;
      iVar2 = y + 1;
      pmVar3 = pmVar3 + 1;
      print2screen(0,y,'\v',*ppcVar1);
      y = iVar2;
    } while (iVar2 <= last_message);
    return;
  }
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int process_packet(u_char *packet,int l)

{
  long lVar1;
  uint id;
  u_char type;
  ushort uVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  my_double mVar7;
  int iVar8;
  my_double mVar9;
  my_double y;
  my_double x;
  int iVar10;
  u_char *puVar11;
  object_list *poVar12;
  ulong uVar13;
  long lVar14;
  size_t sVar15;
  long lVar16;
  anon_struct_conflicte7f *__dest;
  ushort *puVar17;
  ushort *puVar18;
  u_char *puVar19;
  long in_FS_OFFSET;
  byte bVar20;
  double dVar21;
  double dVar22;
  undefined auVar23 [16];
  double local_168;
  double local_160 [2];
  char p;
  u_char txt [256];
  
  bVar20 = 0;
  lVar3 = *(long *)(in_FS_OFFSET + 0x28);
  iVar4 = l;
  switch(*packet) {
  case '\x03':
    if (1 < l) {
      iVar4 = -1;
      __snprintf_chk(error_message,0x400,1,0x400,"Game terminated by %s. Press ENTER.",packet + 1);
      break;
    }
    error_message._24_4_ = 0x5245544e;
    error_message._16_8_ = 0x4520737365725020;
    error_message._28_2_ = 0x2e;
    error_message._0_4_ = s_Game_terminated__00112820._0_4_;
    error_message._4_4_ = s_Game_terminated__00112820._4_4_;
    error_message._8_4_ = s_Game_terminated__00112820._8_4_;
    error_message._12_4_ = s_Game_terminated__00112820._12_4_;
    goto LAB_00105e36;
  case '\x04':
    if (0x1b < l) {
      mVar7 = get_float(packet + 0x13);
      mVar9 = get_float(packet + 0xf);
      y = get_float(packet + 0xb);
      x = get_float(packet + 7);
      iVar5 = get_int16(packet + 0x17);
      iVar8 = get_int16(packet + 5);
      iVar10 = get_int16(packet + 0x1a);
      type = packet[0x19];
      uVar6 = get_int(packet + 1);
      iVar4 = 0x1c;
      new_obj(uVar6,type,iVar10,iVar8,'\0',iVar5,x,y,mVar9,mVar7,(void *)0x0);
    }
    break;
  case '\x05':
    if (6 < l) {
      uVar6 = get_int(packet + 1);
      poVar12 = find_in_table(uVar6);
      if (poVar12 != (object_list *)0x0) {
        iVar4 = get_int16(packet + 5);
        *(ushort *)&(poVar12->member).field_0x1a = (ushort)iVar4;
        if (((poVar12->member).type == '\0') && (((ushort)iVar4 & 0x30) == 0x30)) {
          *(ushort *)&(poVar12->member).field_0x14 = weapon[current_weapon].cadence + 8;
        }
      }
      iVar4 = 7;
    }
    break;
  case '\x06':
    if (0x19 < l) {
      uVar6 = get_int(packet + 1);
      poVar12 = find_in_table(uVar6);
      if ((poVar12 != (object_list *)0x0) &&
         ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
        (poVar12->member).update_counter = packet[5];
        mVar7 = get_float(packet + 6);
        (poVar12->member).x = mVar7;
        mVar7 = get_float(packet + 10);
        (poVar12->member).y = mVar7;
        mVar7 = get_float(packet + 0xe);
        (poVar12->member).xspeed = mVar7;
        mVar7 = get_float(packet + 0x12);
        (poVar12->member).yspeed = mVar7;
        iVar4 = get_int16(packet + 0x16);
        (poVar12->member).data = (void *)0x0;
        *(short *)&(poVar12->member).field_0x1a = (short)iVar4;
        iVar4 = get_int16(packet + 0x18);
        *(short *)&(poVar12->member).field_0x14 = (short)iVar4;
        if (((poVar12->member).type == '\0') &&
           ((*(ushort *)&(poVar12->member).field_0x1a & 0x30) == 0x30)) {
          *(ushort *)&(poVar12->member).field_0x14 = weapon[current_weapon].cadence + 8;
        }
      }
      iVar4 = 0x1a;
    }
    break;
  case '\b':
    if (4 < l) {
      iVar4 = get_int(packet + 1);
      delete_obj((long)iVar4);
      iVar4 = 5;
    }
    break;
  case '\t':
    error_message._0_4_ = error_message._0_4_ & 0xffffff00;
    iVar4 = -1;
    break;
  case '\r':
    if (1 < l) {
      add_message((char *)(packet + 1));
      sVar15 = strlen((char *)(packet + 1));
      iVar4 = (int)sVar15 + 2;
    }
    break;
  case '\x0e':
    if (0x16 < l) {
      health = packet[1];
      armor = packet[2];
      puVar17 = ammo;
      puVar11 = packet + 3;
      do {
        puVar18 = puVar17 + 1;
        iVar4 = get_int16(puVar11);
        *puVar17 = (ushort)iVar4;
        puVar17 = puVar18;
        puVar11 = puVar11 + 2;
      } while (puVar18 != (ushort *)&autorun);
      frags = get_int(packet + 0xd);
      deaths = get_int(packet + 0x11);
      current_weapon = packet[0x15];
      weapons = packet[0x16];
      iVar4 = 0x17;
    }
    break;
  case '\x0f':
    if (7 < l) {
      uVar6 = get_int(packet + 1);
      poVar12 = find_in_table(uVar6);
      if (poVar12 != (object_list *)0x0) {
        uVar2 = *(ushort *)&(poVar12->member).field_0x1a;
        *(ushort *)&(poVar12->member).field_0x1a = uVar2 | 0x80;
        (poVar12->member).data =
             (void *)(long)(int)((uint)packet[7] * 0x100 + (uint)packet[5] * 0x10000 +
                                (uint)packet[6]);
        if (((poVar12->member).type == '\0') && ((uVar2 & 0x30) == 0x30)) {
          *(ushort *)&(poVar12->member).field_0x14 = weapon[current_weapon].cadence + 8;
        }
      }
      iVar4 = 8;
    }
    break;
  case '\x13':
    if (5 < l) {
      active_players = get_int(packet + 1);
      if (packet[5] == '\0') {
        iVar4 = 6;
      }
      else {
        iVar5 = 0;
        iVar4 = 6;
        __dest = top_players;
        do {
          lVar16 = (long)iVar4;
          iVar5 = iVar5 + 1;
          iVar8 = get_int(packet + lVar16);
          __dest->frags = iVar8;
          iVar8 = get_int(packet + lVar16 + 4);
          __dest->deaths = iVar8;
          __dest->color = packet[lVar16 + 8];
          sVar15 = strlen((char *)(packet + lVar16 + 9));
          iVar4 = iVar4 + 10 + (int)sVar15;
          memcpy(__dest,packet + lVar16 + 9,(long)((int)sVar15 + 1));
          if ((int)(uint)packet[5] <= iVar5) break;
          __dest = __dest + 1;
        } while (iVar5 != 10);
      }
    }
    break;
  case '\x15':
    if (1 < l) {
      iVar5 = 1;
      do {
        iVar4 = process_packet(packet + iVar5,l - iVar5);
        iVar5 = iVar5 + iVar4;
        if (l <= iVar5) break;
      } while (iVar5 < 0x11c);
    }
    break;
  case '\x16':
    if (0x15 < l) {
      uVar6 = get_int(packet + 1);
      poVar12 = find_in_table(uVar6);
      if (poVar12 == (object_list *)0x0) {
        iVar4 = 0x16;
      }
      else {
        iVar4 = 0x16;
        if ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80) {
          (poVar12->member).update_counter = packet[5];
          mVar7 = get_float(packet + 6);
          (poVar12->member).x = mVar7;
          mVar7 = get_float(packet + 10);
          (poVar12->member).y = mVar7;
          mVar7 = get_float(packet + 0xe);
          (poVar12->member).xspeed = mVar7;
          mVar7 = get_float(packet + 0x12);
          (poVar12->member).yspeed = mVar7;
        }
      }
    }
    break;
  case '\x17':
    if (l < 0xe) break;
    uVar6 = get_int(packet + 1);
    poVar12 = find_in_table(uVar6);
    if ((poVar12 != (object_list *)0x0) &&
       ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
      (poVar12->member).update_counter = packet[5];
      mVar7 = get_float(packet + 6);
      (poVar12->member).xspeed = mVar7;
      mVar7 = get_float(packet + 10);
      (poVar12->member).yspeed = mVar7;
    }
    goto LAB_0010588e;
  case '\x18':
    if (l < 0xe) break;
    uVar6 = get_int(packet + 1);
    poVar12 = find_in_table(uVar6);
    if ((poVar12 != (object_list *)0x0) &&
       ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
      (poVar12->member).update_counter = packet[5];
      mVar7 = get_float(packet + 6);
      (poVar12->member).x = mVar7;
      mVar7 = get_float(packet + 10);
      (poVar12->member).y = mVar7;
      iVar4 = 0xe;
      break;
    }
LAB_0010588e:
    iVar4 = 0xe;
    break;
  case '\x19':
    if (l < 0x10) break;
    uVar6 = get_int(packet + 1);
    poVar12 = find_in_table(uVar6);
    if ((poVar12 != (object_list *)0x0) &&
       ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
      (poVar12->member).update_counter = packet[5];
      mVar7 = get_float(packet + 6);
      (poVar12->member).xspeed = mVar7;
      mVar7 = get_float(packet + 10);
      (poVar12->member).yspeed = mVar7;
LAB_001057b5:
      iVar4 = get_int16(packet + 0xe);
      *(ushort *)&(poVar12->member).field_0x1a = (ushort)iVar4;
      if (((poVar12->member).type == '\0') && (((ushort)iVar4 & 0x30) == 0x30)) {
        *(ushort *)&(poVar12->member).field_0x14 = weapon[current_weapon].cadence + 8;
      }
    }
    goto LAB_001057f0;
  case '\x1a':
    if (l < 0x10) break;
    uVar6 = get_int(packet + 1);
    poVar12 = find_in_table(uVar6);
    if ((poVar12 != (object_list *)0x0) &&
       ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
      (poVar12->member).update_counter = packet[5];
      mVar7 = get_float(packet + 6);
      (poVar12->member).x = mVar7;
      mVar7 = get_float(packet + 10);
      (poVar12->member).y = mVar7;
      goto LAB_001057b5;
    }
LAB_001057f0:
    iVar4 = 0x10;
    break;
  case '\x1b':
    if (l < 0x12) break;
    uVar6 = get_int(packet + 1);
    poVar12 = find_in_table(uVar6);
    if ((poVar12 != (object_list *)0x0) &&
       ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
      (poVar12->member).update_counter = packet[5];
      mVar7 = get_float(packet + 6);
      (poVar12->member).xspeed = mVar7;
      mVar7 = get_float(packet + 10);
      (poVar12->member).yspeed = mVar7;
LAB_001056b5:
      iVar4 = get_int16(packet + 0xe);
      *(short *)&(poVar12->member).field_0x1a = (short)iVar4;
      iVar4 = get_int16(packet + 0x10);
      *(short *)&(poVar12->member).field_0x14 = (short)iVar4;
      if (((poVar12->member).type == '\0') &&
         ((*(ushort *)&(poVar12->member).field_0x1a & 0x30) == 0x30)) {
        *(ushort *)&(poVar12->member).field_0x14 = weapon[current_weapon].cadence + 8;
      }
    }
    goto LAB_00105700;
  case '\x1c':
    if (l < 0x12) break;
    uVar6 = get_int(packet + 1);
    poVar12 = find_in_table(uVar6);
    if ((poVar12 != (object_list *)0x0) &&
       ((int)((uint)packet[5] - (uint)(poVar12->member).update_counter) < 0x80)) {
      (poVar12->member).update_counter = packet[5];
      mVar7 = get_float(packet + 6);
      (poVar12->member).x = mVar7;
      mVar7 = get_float(packet + 10);
      (poVar12->member).y = mVar7;
      goto LAB_001056b5;
    }
LAB_00105700:
    iVar4 = 0x12;
    break;
  case '\x1d':
    if (8 < l) {
      iVar5 = get_int(packet + 1);
      uVar6 = get_int(packet + 5);
      poVar12 = find_in_table(uVar6);
      iVar4 = 9;
      if (poVar12 != (object_list *)0x0) {
        lVar16 = 0x400;
        auVar23 = (undefined  [16])0x0;
        iVar4 = 0;
        dVar21 = DAT_00112840;
        while( true ) {
          dVar22 = SUB168(auVar23,0) * _DAT_00112838;
          dVar21 = dVar21 * _DAT_00112838;
          iVar8 = (poVar12->member).yspeed;
          id = iVar4 + iVar5;
          mVar7 = (poVar12->member).y;
          iVar4 = iVar4 + 1;
          mVar9 = (poVar12->member).x;
          uVar13 = weapon[4].speed * lVar16;
          lVar16 = lVar16 + 0x400;
          lVar1 = (ulong)((uint)(uVar13 >> 6) & 0x3fff0) + 0x19000;
          iVar10 = (poVar12->member).xspeed;
          lVar14 = random();
          new_obj(id,'\x10',100,shrapnel_sprite[lVar14 % 10],'\0',4,mVar9,mVar7,
                  (int)((int)dVar21 * lVar1 >> 10) + iVar10,iVar8 + (int)((int)dVar22 * lVar1 >> 10)
                  ,(void *)0x0);
          if (iVar4 == 0x28) break;
          sincos((((double)iVar4 + (double)iVar4) * _DAT_00112848) / _DAT_00112850,local_160,
                 &local_168);
          auVar23 = ZEXT816((ulong)local_160[0]);
          dVar21 = local_168;
        }
        delete_obj((ulong)uVar6);
        iVar4 = 9;
      }
    }
    break;
  case '\x1e':
    c_bell();
    iVar4 = 1;
    break;
  case '\x1f':
    if (l < 0x26) break;
    iVar4 = get_int(packet + 1);
    if (level_number == iVar4) {
level_changed:
      p = ' ';
      send_packet((u_char *)&p,1,(sockaddr *)&server,my_id,0);
      iVar4 = 0x26;
    }
    else {
      level_number = iVar4;
      __snprintf_chk(txt,0x100,1,0x100,"Trying to change level to number %d",iVar4);
      add_message((char *)txt);
      puVar11 = load_level(level_number);
      if (puVar11 == (u_char *)0x0) {
        __snprintf_chk(error_message,0x400,1,0x400,
                       "Cannot find level number %d. Game terminated. Press ENTER.",level_number);
        send_quit();
      }
      else {
        __snprintf_chk(txt,0x100,1,0x100,"Changing level to \"%s\"",puVar11);
        if (puVar11 != (u_char *)0xffffffffffffffff) {
          free(puVar11);
        }
        add_message((char *)txt);
        puVar11 = md5_level(level_number);
        iVar4 = strcmp((char *)puVar11,(char *)(packet + 5));
        if (iVar4 == 0) {
          if (puVar11 != (u_char *)0xffffffffffffffff) {
            free(puVar11);
          }
          change_level();
          goto level_changed;
        }
        mem_free(puVar11);
        lVar16 = 0x43;
        puVar11 = (u_char *)"Invalid MD5 sum. Can\'t change level. Game terminated. Press ENTER.";
        puVar19 = error_message;
        while (lVar16 != 0) {
          lVar16 = lVar16 + -1;
          *puVar19 = *puVar11;
          puVar11 = puVar11 + (ulong)bVar20 * -2 + 1;
          puVar19 = puVar19 + (ulong)bVar20 * -2 + 1;
        }
        add_message("Invalid MD5 sum. Can\'t change level. Exiting...");
        send_quit();
      }
LAB_00105e36:
      iVar4 = -1;
    }
  }
  if (lVar3 == *(long *)(in_FS_OFFSET + 0x28)) {
    return iVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void put_float(undefined4 *param_1,undefined4 param_2)

{
  *param_1 = param_2;
  return;
}


void put_int(u_char *p,int num)

{
  *(int *)p = num;
  return;
}


void put_int16(u_char *p,int num)

{
  *(short *)p = (short)num;
  return;
}


void put_line(int xs,int ys,u_char *scrn,u_char *attr,int x,int y,line *l,u_char type,u_char plastic)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  bool bVar6;
  
  if ((((y < ys) && (-1 < y)) && (iVar5 = l->len, 0 < iVar5)) && (x < xs)) {
    iVar4 = y * xs + x;
    uVar2 = 0;
    if (plastic == '\0') {
      do {
        iVar1 = (int)uVar2;
        if ((l->attr[uVar2] != '\0') && (-1 < x + iVar1)) {
          lVar3 = (long)(iVar4 + iVar1);
          scrn[lVar3] = l->bmp[uVar2];
          attr[lVar3] = l->attr[uVar2] | type;
          iVar5 = l->len;
        }
      } while ((iVar1 + 1 < iVar5) && (bVar6 = uVar2 != (uint)(~x + xs), uVar2 = uVar2 + 1, bVar6));
    }
    else {
      do {
        iVar1 = (int)uVar2;
        if (l->attr[uVar2] != '\0') {
          lVar3 = (long)(iVar4 + iVar1);
          if ((((attr[lVar3] & 0xf0) - 0x30 & 0xe0) != 0) && (-1 < x + iVar1)) {
            scrn[lVar3] = l->bmp[uVar2];
            attr[lVar3] = l->attr[uVar2] | type;
            iVar5 = l->len;
          }
        }
        if (iVar5 <= iVar1 + 1) {
          return;
        }
        bVar6 = uVar2 != (uint)(~x + xs);
        uVar2 = uVar2 + 1;
      } while (bVar6);
    }
  }
  return;
}


void put_long_long(u_char *p,ulonglong num)

{
  *(ulonglong *)p = num;
  return;
}


void _put_sprite(int xs,int ys,u_char *scrn,u_char *attr,int x,int y,pos *p,u_char type,u_char plastic)

{
  line *plVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  ulong uVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  bool bVar9;
  
  iVar2 = p->n;
  if (0 < iVar2) {
    lVar8 = 0;
    do {
      iVar4 = y + (int)lVar8 + p->yo;
      if ((iVar4 < ys) && (-1 < iVar4)) {
        plVar1 = p->lines + lVar8;
        iVar6 = plVar1->len;
        if ((0 < iVar6) && (iVar3 = x + p->xo, iVar3 < xs)) {
          iVar4 = iVar4 * xs;
          if (plastic == '\0') {
            uVar5 = 0;
            do {
              iVar2 = (int)uVar5;
              if ((plVar1->attr[uVar5] != '\0') && (-1 < iVar3 + iVar2)) {
                lVar7 = (long)(iVar4 + iVar3 + iVar2);
                scrn[lVar7] = plVar1->bmp[uVar5];
                attr[lVar7] = plVar1->attr[uVar5] | type;
                iVar6 = plVar1->len;
              }
            } while ((iVar2 + 1 < iVar6) &&
                    (bVar9 = (uint)((xs + -1) - iVar3) != uVar5, uVar5 = uVar5 + 1, bVar9));
          }
          else {
            iVar2 = iVar4 + iVar3;
            lVar7 = 0;
            do {
              if (plVar1->attr[lVar7] != '\0') {
                if ((((attr[iVar2] & 0xf0) - 0x30 & 0xe0) != 0) && (-1 < iVar3 + (int)lVar7)) {
                  scrn[iVar2] = plVar1->bmp[lVar7];
                  attr[iVar2] = plVar1->attr[lVar7] | type;
                  iVar6 = plVar1->len;
                }
              }
              if (iVar6 <= (int)lVar7 + 1) break;
              iVar2 = iVar2 + 1;
              lVar7 = lVar7 + 1;
            } while (iVar2 != iVar4 + xs);
          }
          iVar2 = p->n;
        }
      }
      lVar8 = lVar8 + 1;
    } while ((int)lVar8 < iVar2);
  }
  return;
}


void put_sprite(int x,int y,pos *p,u_char plastic)

{
  int iVar1;
  line *plVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  u_char *puVar7;
  ulong uVar8;
  long lVar9;
  int iVar10;
  int iVar11;
  long lVar12;
  u_char *puVar13;
  bool bVar14;
  
  iVar4 = SCREEN_X;
  iVar3 = SCREEN_Y;
  if ((TRI_D_ON == 0) || (puVar7 = screen2_a, puVar13 = screen2, tri_d == 0)) {
    puVar7 = screen_a;
    puVar13 = screen;
  }
  lVar9 = 0;
  iVar6 = p->n;
  iVar1 = SCREEN_X + -1;
  if (0 < iVar6) {
    do {
      iVar5 = y + (int)lVar9 + p->yo;
      if ((iVar5 < iVar3) && (-1 < iVar5)) {
        plVar2 = p->lines + lVar9;
        iVar11 = plVar2->len;
        if ((0 < iVar11) && (iVar10 = x + p->xo, iVar10 < iVar4)) {
          iVar5 = iVar5 * iVar4;
          if (plastic == '\0') {
            uVar8 = 0;
            do {
              iVar6 = (int)uVar8;
              if ((plVar2->attr[uVar8] != '\0') && (-1 < iVar10 + iVar6)) {
                lVar12 = (long)(iVar5 + iVar10 + iVar6);
                puVar13[lVar12] = plVar2->bmp[uVar8];
                puVar7[lVar12] = plVar2->attr[uVar8];
                iVar11 = plVar2->len;
              }
            } while ((iVar6 + 1 < iVar11) &&
                    (bVar14 = (uint)(iVar1 - iVar10) != uVar8, uVar8 = uVar8 + 1, bVar14));
          }
          else {
            iVar6 = iVar10 + iVar5;
            lVar12 = 0;
            do {
              if (plVar2->attr[lVar12] != '\0') {
                if ((((puVar7[iVar6] & 0xf0) - 0x30 & 0xe0) != 0) && (-1 < iVar10 + (int)lVar12)) {
                  puVar13[iVar6] = plVar2->bmp[lVar12];
                  puVar7[iVar6] = plVar2->attr[lVar12];
                  iVar11 = plVar2->len;
                }
              }
              if (iVar11 <= (int)lVar12 + 1) break;
              iVar6 = iVar6 + 1;
              lVar12 = lVar12 + 1;
            } while (iVar6 != iVar5 + iVar4);
          }
          iVar6 = p->n;
        }
      }
      lVar9 = lVar9 + 1;
    } while ((int)lVar9 < iVar6);
  }
  return;
}


int read_data(void)

{
  byte bVar1;
  int iVar2;
  long lVar3;
  fd_set *pfVar4;
  long in_FS_OFFSET;
  int a;
  timeval tv;
  sockaddr_in client;
  fd_set rfds;
  long local_20;
  
  lVar3 = 0x10;
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  a = 0x10;
  tv = (timeval)0x0;
  pfVar4 = (fd_set *)&rfds;
  while (lVar3 != 0) {
    lVar3 = lVar3 + -1;
    pfVar4->fds_bits[0] = 0;
    pfVar4 = (fd_set *)(pfVar4->fds_bits + 1);
  }
  lVar3 = __fdelt_chk(0);
  bVar1 = (byte)(fd >> 0x37);
  rfds.__fds_bits[lVar3] =
       rfds.__fds_bits[lVar3] | 1 << (((char)fd + (bVar1 >> 2) & 0x3f) - (bVar1 >> 2) & 0x3f);
  while ((iVar2 = select(fd + 1,(fd_set *)&rfds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv),
         iVar2 != 0 &&
         (iVar2 = recv_packet(read_data::packet,0x11c,(sockaddr *)&client,&a,1,my_id,(int *)0x0),
         -1 < iVar2))) {
    iVar2 = process_packet(read_data::packet,iVar2);
    if (iVar2 < 0) {
      iVar2 = 1;
LAB_00105fda:
      if (local_20 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return iVar2;
    }
  }
  iVar2 = 0;
  goto LAB_00105fda;
}


int read_num_online(int y,char *pointer,char *message,int max_len)

{
  long lVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  size_t sVar5;
  size_t sVar6;
  long lVar7;
  uint uVar8;
  ulong uVar9;
  int iVar10;
  size_t __n;
  
  sVar5 = strlen(message);
  sVar6 = strlen(pointer);
  iVar2 = (int)sVar5;
  sVar5 = SEXT48(iVar2);
  iVar10 = (int)sVar6;
  __n = SEXT48(iVar10);
  uVar9 = sVar6 & 0xffffffff;
  lVar7 = (long)(y * SCREEN_X);
  lVar1 = sVar5 + lVar7;
  if (TRI_D_ON != 0) {
    memcpy(screen2 + lVar7,message,sVar5);
    memcpy(screen2 + lVar1,pointer,__n);
    memset(screen2_a + lVar7,0xf,sVar5);
    memset(screen2_a + lVar1,7,__n);
    memset(screen2_a + lVar1 + __n,0,(long)((SCREEN_X - iVar10) - iVar2));
  }
  memcpy(screen + lVar7,message,sVar5);
  memcpy(screen + lVar1,pointer,__n);
  memset(screen_a + lVar7,0xf,sVar5);
  memset(screen_a + lVar1,7,__n);
  memset(screen_a + lVar1 + __n,0,(long)((SCREEN_X - iVar10) - iVar2));
  c_update_kbd();
  iVar2 = c_was_pressed(300);
  if (iVar2 == 0) {
    iVar2 = c_was_pressed(0x12f);
    if (iVar2 == 0) {
      iVar3 = c_pressed(0x12d);
      if ((iVar3 != 0) && (iVar10 != 0)) {
        uVar9 = (ulong)(iVar10 - 1U);
        pointer[(int)(iVar10 - 1U)] = '\0';
      }
      iVar10 = 0x30;
      iVar3 = (int)uVar9;
      while (iVar3 < max_len) {
        iVar4 = c_was_pressed(iVar10);
        while (iVar3 = (int)uVar9, iVar4 != 0) {
          uVar8 = iVar3 + 1;
          uVar9 = (ulong)uVar8;
          pointer[iVar3] = (char)iVar10;
          iVar10 = iVar10 + 1;
          pointer[(int)uVar8] = '\0';
          if (max_len <= (int)uVar8) {
            return 0;
          }
          if (0x39 < iVar10) {
            return 0;
          }
          iVar4 = c_was_pressed(iVar10);
        }
        iVar10 = iVar10 + 1;
        if (0x39 < iVar10) {
          return 0;
        }
      }
    }
    else {
      iVar2 = 1;
    }
  }
  else {
    *pointer = '\0';
    iVar2 = 2;
  }
  return iVar2;
}


int read_str_online(int y,char *pointer,char *message,int max_len)

{
  long lVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  size_t sVar7;
  size_t sVar8;
  long lVar9;
  ulong uVar10;
  size_t __n;
  bool bVar11;
  
  sVar7 = strlen(message);
  sVar8 = strlen(pointer);
  iVar3 = (int)sVar7;
  sVar7 = SEXT48(iVar3);
  iVar5 = (int)sVar8;
  __n = SEXT48(iVar5);
  uVar10 = sVar8 & 0xffffffff;
  lVar9 = (long)(y * SCREEN_X);
  lVar1 = lVar9 + sVar7;
  if (TRI_D_ON != 0) {
    memcpy(screen2 + lVar9,message,sVar7);
    memcpy(screen2 + lVar1,pointer,__n);
    memset(screen2_a + lVar9,0xf,sVar7);
    memset(screen2_a + lVar1,7,__n);
    memset(screen2_a + lVar1 + __n,0,(long)((SCREEN_X - iVar5) - iVar3));
  }
  memcpy(screen + lVar9,message,sVar7);
  memcpy(screen + lVar1,pointer,__n);
  memset(screen_a + lVar9,0xf,sVar7);
  memset(screen_a + lVar1,7,__n);
  memset(screen_a + lVar1 + __n,0,(long)((SCREEN_X - iVar5) - iVar3));
  c_update_kbd();
  iVar3 = c_was_pressed(300);
  if (iVar3 == 0) {
    iVar3 = c_was_pressed(0x12f);
    if (iVar3 == 0) {
      iVar4 = c_pressed(0x12d);
      if ((iVar4 != 0) && (iVar5 != 0)) {
        uVar10 = (ulong)(iVar5 - 1U);
        pointer[(int)(iVar5 - 1U)] = '\0';
      }
      bVar11 = true;
      iVar5 = c_pressed(0x135);
      if (iVar5 == 0) {
        iVar5 = c_pressed(0x130);
        bVar11 = iVar5 != 0;
      }
      iVar5 = 0x20;
      do {
        iVar4 = (int)uVar10;
        if (max_len <= iVar4) {
          return 0;
        }
        iVar6 = c_was_pressed(iVar5);
        if (iVar6 != 0) {
          cVar2 = (char)iVar5;
          if (bVar11) {
            iVar6 = my_toupper(iVar5);
            cVar2 = (char)iVar6;
          }
          uVar10 = (ulong)(iVar4 + 1U);
          pointer[iVar4] = cVar2;
          pointer[(int)(iVar4 + 1U)] = '\0';
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < 0x80);
    }
    else {
      iVar3 = 1;
    }
  }
  else {
    *pointer = '\0';
    iVar3 = 2;
  }
  return iVar3;
}


void redraw_screen(void)

{
  memset(screen_a_old,0,(long)(SCREEN_X * SCREEN_Y));
  memset(screen_old,0,(long)(SCREEN_X * SCREEN_Y));
  if (TRI_D_ON == 0) {
    clear_screen();
    return;
  }
  memset(screen2_a_old,0,(long)(SCREEN_X * SCREEN_Y));
  memset(screen2_old,0,(long)(SCREEN_X * SCREEN_Y));
  clear_screen();
  return;
}


void reinit_area(void)

{
  memset(area,0x20,500000);
  memset(area_a,0,500000);
  return;
}


object_list * remove_from_table(uint id)

{
  object_list *poVar1;
  byte bVar2;
  long lVar3;
  ulong uVar4;
  object_list *__ptr;
  ulong uVar5;
  object_list **__dest;
  
  uVar5 = (ulong)(id & 0x7fff);
  bVar2 = table[uVar5].count;
  if (bVar2 != 0) {
    lVar3 = 0;
    __dest = table[uVar5].pointer;
    do {
      poVar1 = *__dest;
      if (*(uint *)&(poVar1->member).type >> 8 == id) {
        memmove(__dest,table[uVar5].pointer + lVar3 + 1,
                (long)(int)(((uint)bVar2 - (int)lVar3) + -1) << 3);
        __ptr = (object_list *)table[uVar5].pointer;
        bVar2 = table[uVar5].count - 1;
        table[uVar5].count = bVar2;
        uVar4 = (ulong)bVar2;
        if (__ptr == (object_list *)0xffffffffffffffff) {
          if (uVar4 != 0) {
            __ptr = (object_list *)malloc(uVar4 * 8);
            if (__ptr == (object_list *)0x0) {
              error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
            }
          }
        }
        else {
          if (__ptr == (object_list *)0x0) {
            errline = 0x56;
            errfile = "error.h";
            int_error((u_char *)"mem_realloc(NULL, %d)");
          }
          else {
            if (uVar4 == 0) {
              free(__ptr);
              __ptr = (object_list *)0xffffffffffffffff;
            }
            else {
              __ptr = (object_list *)realloc(__ptr,uVar4 * 8);
              if (__ptr == (object_list *)0x0) {
                error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
              }
            }
          }
        }
        table[uVar5].pointer = (object_list **)__ptr;
        return poVar1;
      }
      lVar3 = lVar3 + 1;
      __dest = __dest + 1;
    } while ((int)lVar3 < (int)(uint)bVar2);
  }
  return (object_list *)0x0;
}


void reset_keyboard(void)

{
  keyboard_status = 0;
  return;
}


void resize_screen(void)

{
  u_char *puVar1;
  u_char *puVar2;
  size_t sVar3;
  
  c_get_size(&SCREEN_X,&SCREEN_Y);
  sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
  if (screen == (u_char *)0xffffffffffffffff) {
    if (sVar3 == 0) {
      if (screen_a != (u_char *)0xffffffffffffffff) {
        if (screen_a != (u_char *)0x0) goto LAB_0010bfee;
        goto LAB_0010c4a0;
      }
LAB_0010c060:
      if (screen_old == (u_char *)0xffffffffffffffff) goto LAB_0010c0f0;
      if (screen_old != (u_char *)0x0) goto LAB_0010c07a;
      sVar3 = 0;
      goto LAB_0010c4e0;
    }
    puVar1 = (u_char *)malloc(sVar3);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen = (u_char *)0x0;
      goto LAB_0010c2a0;
    }
LAB_0010bcbc:
    screen = puVar1;
    if (screen_a != (u_char *)0xffffffffffffffff) goto LAB_0010bcd4;
LAB_0010bf91:
    if (sVar3 == 0) goto LAB_0010c060;
    puVar1 = (u_char *)malloc(sVar3);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      screen_a = (u_char *)0x0;
      goto LAB_0010c2a0;
    }
LAB_0010bd04:
    screen_a = puVar1;
    if (screen_old != (u_char *)0xffffffffffffffff) goto LAB_0010bd1c;
LAB_0010c01f:
    if (sVar3 != 0) {
      puVar1 = (u_char *)malloc(sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      goto LAB_0010bd4c;
    }
LAB_0010c0f0:
    if (screen_a_old != (u_char *)0xffffffffffffffff) {
      if (screen_a_old != (u_char *)0x0) goto LAB_0010c10a;
      sVar3 = 0;
      goto LAB_0010c520;
    }
LAB_0010c228:
    sVar3 = 0;
    puVar2 = (u_char *)0xffffffffffffffff;
    puVar1 = screen_a_old;
  }
  else {
    if (screen == (u_char *)0x0) {
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)");
      screen = (u_char *)0x0;
      goto LAB_0010c2a0;
    }
    if (sVar3 != 0) {
      puVar1 = (u_char *)realloc(screen,sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
        screen = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
      goto LAB_0010bcbc;
    }
    free(screen);
    screen = (u_char *)0xffffffffffffffff;
    sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
    if (screen_a == (u_char *)0xffffffffffffffff) goto LAB_0010bf91;
LAB_0010bcd4:
    if (screen_a == (u_char *)0x0) {
LAB_0010c4a0:
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)");
      screen_a = (u_char *)0x0;
      goto LAB_0010c2a0;
    }
    if (sVar3 != 0) {
      puVar1 = (u_char *)realloc(screen_a,sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
        screen_a = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
      goto LAB_0010bd04;
    }
LAB_0010bfee:
    free(screen_a);
    screen_a = (u_char *)0xffffffffffffffff;
    sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
    if (screen_old == (u_char *)0xffffffffffffffff) goto LAB_0010c01f;
LAB_0010bd1c:
    if (screen_old == (u_char *)0x0) {
LAB_0010c4e0:
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)",sVar3);
      screen_old = (u_char *)0x0;
      goto LAB_0010c2a0;
    }
    if (sVar3 == 0) {
LAB_0010c07a:
      free(screen_old);
      screen_old = (u_char *)0xffffffffffffffff;
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
      if (screen_a_old != (u_char *)0xffffffffffffffff) goto LAB_0010bd64;
LAB_0010c0ab:
      if (sVar3 == 0) goto LAB_0010c228;
      puVar2 = (u_char *)malloc(sVar3);
      puVar1 = puVar2;
      if (puVar2 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen_a_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
    }
    else {
      puVar1 = (u_char *)realloc(screen_old,sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
        screen_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
LAB_0010bd4c:
      screen_old = puVar1;
      if (screen_a_old == (u_char *)0xffffffffffffffff) goto LAB_0010c0ab;
LAB_0010bd64:
      if (screen_a_old == (u_char *)0x0) {
LAB_0010c520:
        errline = 0x56;
        errfile = "error.h";
        int_error((u_char *)"mem_realloc(NULL, %d)",sVar3);
        screen_a_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      if (sVar3 == 0) {
LAB_0010c10a:
        free(screen_a_old);
        screen_a_old = (u_char *)0xffffffffffffffff;
        puVar2 = (u_char *)0xffffffffffffffff;
        sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
        puVar1 = screen_a_old;
      }
      else {
        puVar2 = (u_char *)realloc(screen_a_old,sVar3);
        if (puVar2 == (u_char *)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          screen_a_old = (u_char *)0x0;
          goto LAB_0010c2a0;
        }
        sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
        puVar1 = puVar2;
      }
    }
  }
  screen_a_old = puVar1;
  memset(puVar2,0,sVar3);
  memset(screen_old,0,(long)(SCREEN_X * SCREEN_Y));
  if (TRI_D_ON == 0) goto LAB_0010bf2f;
  sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
  if (screen2 == (u_char *)0xffffffffffffffff) {
    if (sVar3 != 0) {
      puVar1 = (u_char *)malloc(sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2 = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      goto LAB_0010be2c;
    }
    if (screen2_a == (u_char *)0xffffffffffffffff) goto LAB_0010c3d0;
    if (screen2_a != (u_char *)0x0) goto LAB_0010c180;
LAB_0010c268:
    errline = 0x56;
    errfile = "error.h";
    int_error((u_char *)"mem_realloc(NULL, %d)");
    screen2_a = (u_char *)0x0;
    goto LAB_0010c2a0;
  }
  if (screen2 == (u_char *)0x0) {
    errline = 0x56;
    errfile = "error.h";
    int_error((u_char *)"mem_realloc(NULL, %d)");
    screen2 = (u_char *)0x0;
    goto LAB_0010c2a0;
  }
  if (sVar3 == 0) {
    free(screen2);
    screen2 = (u_char *)0xffffffffffffffff;
    sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
    puVar1 = screen2;
  }
  else {
    puVar1 = (u_char *)realloc(screen2,sVar3);
    if (puVar1 == (u_char *)0x0) {
      error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
      screen2 = (u_char *)0x0;
      goto LAB_0010c2a0;
    }
    sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
  }
LAB_0010be2c:
  screen2 = puVar1;
  if (screen2_a == (u_char *)0xffffffffffffffff) {
    if (sVar3 != 0) {
      puVar1 = (u_char *)malloc(sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2_a = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      goto LAB_0010be74;
    }
LAB_0010c3d0:
    if (screen2_old != (u_char *)0xffffffffffffffff) {
      if (screen2_old != (u_char *)0x0) goto LAB_0010c200;
      sVar3 = 0;
      goto LAB_0010c3f0;
    }
LAB_0010c440:
    if (screen2_a_old != (u_char *)0xffffffffffffffff) {
      if (screen2_a_old != (u_char *)0x0) {
LAB_0010c1d0:
        free(screen2_a_old);
        screen2_a_old = (u_char *)0xffffffffffffffff;
        puVar2 = (u_char *)0xffffffffffffffff;
        sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
        puVar1 = screen2_a_old;
        goto LAB_0010bf07;
      }
      sVar3 = 0;
LAB_0010c460:
      errline = 0x56;
      errfile = "error.h";
      int_error((u_char *)"mem_realloc(NULL, %d)",sVar3);
      screen2_a_old = (u_char *)0x0;
LAB_0010c2a0:
      fwrite("Not enough memory!\n",1,0x13,stderr);
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
LAB_0010c430:
    sVar3 = 0;
    puVar2 = (u_char *)0xffffffffffffffff;
    puVar1 = screen2_a_old;
  }
  else {
    if (screen2_a == (u_char *)0x0) goto LAB_0010c268;
    if (sVar3 == 0) {
LAB_0010c180:
      free(screen2_a);
      screen2_a = (u_char *)0xffffffffffffffff;
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
      puVar1 = screen2_a;
    }
    else {
      puVar1 = (u_char *)realloc(screen2_a,sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
        screen2_a = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
    }
LAB_0010be74:
    screen2_a = puVar1;
    if (screen2_old == (u_char *)0xffffffffffffffff) {
      if (sVar3 == 0) goto LAB_0010c440;
      puVar1 = (u_char *)malloc(sVar3);
      if (puVar1 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
    }
    else {
      if (screen2_old == (u_char *)0x0) {
LAB_0010c3f0:
        errline = 0x56;
        errfile = "error.h";
        int_error((u_char *)"mem_realloc(NULL, %d)",sVar3);
        screen2_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      if (sVar3 == 0) {
LAB_0010c200:
        free(screen2_old);
        screen2_old = (u_char *)0xffffffffffffffff;
        sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
        puVar1 = screen2_old;
      }
      else {
        puVar1 = (u_char *)realloc(screen2_old,sVar3);
        if (puVar1 == (u_char *)0x0) {
          error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          screen2_old = (u_char *)0x0;
          goto LAB_0010c2a0;
        }
        sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
      }
    }
    screen2_old = puVar1;
    if (screen2_a_old == (u_char *)0xffffffffffffffff) {
      if (sVar3 == 0) goto LAB_0010c430;
      puVar2 = (u_char *)malloc(sVar3);
      puVar1 = puVar2;
      if (puVar2 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        screen2_a_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
    }
    else {
      if (screen2_a_old == (u_char *)0x0) goto LAB_0010c460;
      if (sVar3 == 0) goto LAB_0010c1d0;
      puVar2 = (u_char *)realloc(screen2_a_old,sVar3);
      if (puVar2 == (u_char *)0x0) {
        error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
        screen2_a_old = (u_char *)0x0;
        goto LAB_0010c2a0;
      }
      sVar3 = SEXT48(SCREEN_X * SCREEN_Y);
      puVar1 = puVar2;
    }
  }
LAB_0010bf07:
  screen2_a_old = puVar1;
  memset(puVar2,0,sVar3);
  memset(screen2_old,0,(long)(SCREEN_X * SCREEN_Y));
LAB_0010bf2f:
  SCREEN_XOFFSET = SCREEN_X + -0xf >> 1;
  SCREEN_YOFFSET = SCREEN_Y + -0xd >> 1;
  clear_screen();
  return;
}


void save_cfg(char *host,char *name,int color)

{
  long lVar1;
  char *pcVar2;
  FILE *__stream;
  long in_FS_OFFSET;
  u_char txt [256];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  pcVar2 = getenv("HOME");
  __sprintf_chk(txt,1,0x100,"%s/%s",pcVar2,".0verkill");
  __stream = fopen((char *)txt,"w");
  if (__stream != (FILE *)0x0) {
    __fprintf_chk(__stream,1,"%s\n%s\n%d\n",host,name,color);
    fclose(__stream);
  }
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void scroll_messages(void)

{
  if (-1 < last_message) {
    scroll_messages();
    return;
  }
  return;
}


void scroll_messages(void)

{
  char *__s;
  size_t sVar1;
  char *pcVar2;
  long lVar3;
  msgline_type *pmVar4;
  int iVar5;
  
  if (0 < last_message) {
    pmVar4 = msg_line;
    iVar5 = 0;
    do {
      pcVar2 = pmVar4[1].msg;
      iVar5 = iVar5 + 1;
      pmVar4->time = pmVar4[1].time;
      sVar1 = strlen(pcVar2);
      pcVar2 = pmVar4->msg;
      if (pcVar2 == (char *)0xffffffffffffffff) {
        pcVar2 = (char *)malloc(sVar1 + 1);
        if (pcVar2 == (char *)0x0) {
          error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
        }
      }
      else {
        if (pcVar2 == (char *)0x0) {
          errfile = "error.h";
          errline = 0x56;
          int_error((u_char *)"mem_realloc(NULL, %d)");
        }
        else {
          pcVar2 = (char *)realloc(pcVar2,sVar1 + 1);
          if (pcVar2 == (char *)0x0) {
            error((u_char *)"ERROR: out of memory (realloc returned NULL)\n");
          }
        }
      }
      __s = pmVar4[1].msg;
      pmVar4->msg = pcVar2;
      pmVar4 = pmVar4 + 1;
      sVar1 = strlen(__s);
      memcpy(pcVar2,__s,sVar1 + 1);
    } while (iVar5 < last_message);
  }
  lVar3 = (long)last_message;
  pcVar2 = msg_line[lVar3].msg;
  if (pcVar2 != (char *)0xffffffffffffffff) {
    if (pcVar2 == (char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
      lVar3 = (long)last_message;
    }
    else {
      free(pcVar2);
      lVar3 = (long)last_message;
    }
  }
  msg_line[lVar3].msg = (char *)0x0;
  last_message = last_message + -1;
  return;
}


int select_color(int value,int max)

{
  bool bVar1;
  int iVar2;
  
  iVar2 = 10;
  if (value < max >> 1) {
    bVar1 = max >> 2 <= value;
    iVar2 = bVar1 + 9 + (uint)bVar1;
  }
  return iVar2;
}


void send_info_request(void)

{
  long in_FS_OFFSET;
  u_char packet;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  packet = '\x13';
  send_packet(&packet,1,(sockaddr *)&server,my_id,0);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void send_keyboard(void)

{
  long lVar1;
  long in_FS_OFFSET;
  u_char packet [3];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  packet[0] = '\x12';
  packet[1] = keyboard_status.left * '\x02' | keyboard_status.jump << 2 | keyboard_status.right |
              keyboard_status.creep << 3 | keyboard_status.speed << 4 | keyboard_status.fire << 5 |
              keyboard_status.down_ladder << 6;
  packet[2] = keyboard_status.weapon;
  send_packet(packet,3,(sockaddr *)&server,my_id,0);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void send_message(char *msg)

{
  size_t sVar1;
  
  sVar1 = strlen(msg);
  packet_7196[0] = 0xd;
  __memcpy_chk(0x118441,msg,(long)((int)sVar1 + 1));
  send_packet(packet_7196,(int)sVar1 + 2,(sockaddr *)&server,my_id,0);
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void send_quit(void)

{
  byte bVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  fd_set *pfVar5;
  long in_FS_OFFSET;
  u_char p;
  int a;
  timeval tv;
  fd_set rfds;
  long local_40;
  
  lVar3 = 0x10;
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  a = 0x10;
  tv.tv_sec = _DAT_00112800;
  tv.tv_usec = _UNK_00112808;
  pfVar5 = (fd_set *)&rfds;
  while (lVar3 != 0) {
    lVar3 = lVar3 + -1;
    pfVar5->fds_bits[0] = 0;
    pfVar5 = (fd_set *)(pfVar5->fds_bits + 1);
  }
  iVar4 = 0;
  lVar3 = __fdelt_chk(_DAT_00112800,(long)fd);
  bVar1 = (byte)(fd >> 0x37);
  rfds.__fds_bits[lVar3] =
       rfds.__fds_bits[lVar3] | 1 << (((char)fd + (bVar1 >> 2) & 0x3f) - (bVar1 >> 2) & 0x3f);
  do {
    while( true ) {
      p = '\a';
      iVar4 = iVar4 + 1;
      send_packet(&p,1,(sockaddr *)&server,my_id,0);
      iVar2 = select(fd + 1,(fd_set *)&rfds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv);
      if (iVar2 == 0) break;
      recv_packet(&p,1,(sockaddr *)&server,&a,1,my_id,(int *)0x0);
      if ((p == '\t') || (0x20 < iVar4)) goto LAB_00103e36;
    }
  } while (iVar4 < 0x21);
  recv_packet(&p,1,(sockaddr *)&server,&a,1,my_id,(int *)0x0);
LAB_00103e36:
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


void send_reenter_game(void)

{
  long in_FS_OFFSET;
  u_char packet;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  packet = '\x14';
  send_packet(&packet,1,(sockaddr *)&server,my_id,0);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void show_window(int x,int y)

{
  long lVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  
  iVar5 = SCREEN_X + x;
  if ((0 < iVar5) && (0 < SCREEN_Y + y)) {
    iVar4 = -x;
    if (-1 < x) {
      iVar4 = 0;
    }
    if (iVar5 < 1000) {
      iVar5 = 1000;
    }
    iVar5 = iVar5 + -1000;
    if (tri_d == 0) {
      if (y < 0) {
        iVar7 = -y;
        if (SCREEN_Y == iVar7 || SBORROW4(SCREEN_Y,iVar7) != SCREEN_Y + y < 0) {
          return;
        }
      }
      else {
        if (SCREEN_Y < 1) {
          return;
        }
        iVar7 = tri_d;
        if (499 < y) {
          return;
        }
      }
      lVar3 = (long)x + (long)(y * 1000) + (long)iVar4 + (long)(iVar7 * 1000);
      do {
        iVar11 = iVar7 + 1;
        lVar1 = (long)(iVar7 * SCREEN_X) + (long)iVar4;
        memcpy(screen + lVar1,area + lVar3,(long)((SCREEN_X - iVar4) - iVar5));
        memcpy(screen_a + lVar1,area_a + lVar3,(long)((SCREEN_X - iVar4) - iVar5));
        if (SCREEN_Y <= iVar11) {
          return;
        }
        lVar3 = lVar3 + 1000;
        iVar7 = iVar11;
      } while (iVar11 + y < 500);
    }
    else {
      if (tri_d == 1) {
        memset(screen2_a,0,(long)(SCREEN_X * SCREEN_Y));
        memset(screen2,0,(long)(SCREEN_X * SCREEN_Y));
        if (y < 0) {
          iVar7 = -y;
          if (SCREEN_Y <= iVar7) {
            return;
          }
        }
        else {
          if (SCREEN_Y < 1) {
            return;
          }
          if (499 < y) {
            return;
          }
          iVar7 = 0;
        }
        iVar8 = y * 1000 + x + iVar4 + iVar7 * 1000;
        iVar11 = SCREEN_Y;
        iVar9 = SCREEN_X;
        do {
          iVar6 = iVar9 * iVar7;
          if (0 < (iVar9 - iVar4) - iVar5) {
            lVar3 = 0;
            do {
              while( true ) {
                lVar1 = iVar8 + lVar3;
                iVar11 = (int)lVar3;
                if (((area_a[lVar1] & 0xf0) - 0x30 & 0xe0) != 0) break;
                iVar10 = 1;
LAB_0010a559:
                if (iVar4 + iVar11 + iVar10 < iVar9) {
                  lVar2 = (long)(iVar11 + iVar6 + iVar4 + iVar10);
                  screen2[lVar2] = area[lVar1];
                  screen2_a[lVar2] = area_a[lVar1];
                  iVar9 = SCREEN_X;
                }
LAB_0010a5a0:
                lVar3 = lVar3 + 1;
                iVar11 = SCREEN_Y;
                if ((iVar9 - iVar4) - iVar5 <= (int)lVar3) goto LAB_0010a616;
              }
              if ((area_a[lVar1] & 0xf0) != 0) {
                iVar10 = 0;
                if ((area[lVar1] & 0xdf) != 0) goto LAB_0010a559;
                goto LAB_0010a5a0;
              }
              if ((area[lVar1] & 0xdf) == 0) goto LAB_0010a5a0;
              iVar10 = -1;
              if (iVar4 + iVar11 != 0) goto LAB_0010a559;
              lVar3 = lVar3 + 1;
              iVar11 = SCREEN_Y;
            } while ((int)lVar3 < (iVar9 - iVar4) - iVar5);
          }
LAB_0010a616:
          iVar7 = iVar7 + 1;
        } while ((iVar7 < iVar11) && (iVar8 = iVar8 + 1000, iVar7 + y < 500));
      }
    }
  }
  return;
}


void shut_down(int x)

{
  char *__ptr;
  long lVar1;
  int iVar2;
  msgline_type *pmVar3;
  
  c_shutdown();
  free_sprites(0);
  free_area();
  if (hero != (it *)0x0) {
    delete_obj((ulong)(*(uint *)&hero->type >> 8));
  }
  while (objects.next != (object_list *)0x0) {
    delete_obj((ulong)(*(uint *)&((objects.next)->member).type >> 8));
  }
  lVar1 = (long)last_message;
  iVar2 = 0;
  pmVar3 = msg_line;
  if (-1 < last_message) {
    do {
      __ptr = pmVar3->msg;
      if (__ptr != (char *)0xffffffffffffffff) {
        if (__ptr == (char *)0x0) {
          errfile = "error.h";
          errline = 0x4c;
          int_error((u_char *)"mem_free(NULL)");
          lVar1 = (long)last_message;
        }
        else {
          free(__ptr);
          lVar1 = (long)last_message;
        }
      }
      iVar2 = iVar2 + 1;
      pmVar3 = pmVar3 + 1;
    } while (iVar2 <= (int)lVar1);
  }
  msg_line[lVar1].msg = (char *)0x0;
  shutdown_sprites();
  check_memory_leaks();
  if (x == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  exit(0);
}


void shutdown_blit(void)

{
  if (screen != (u_char *)0xffffffffffffffff) {
    if (screen == (u_char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
    }
    else {
      free(screen);
    }
  }
  if (screen_a != (u_char *)0xffffffffffffffff) {
    if (screen_a == (u_char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
    }
    else {
      free(screen_a);
    }
  }
  if (screen_old != (u_char *)0xffffffffffffffff) {
    if (screen_old == (u_char *)0x0) {
      errline = 0x4c;
      errfile = "error.h";
      int_error((u_char *)"mem_free(NULL)");
    }
    else {
      free(screen_old);
    }
  }
  if (screen_a_old != (u_char *)0xffffffffffffffff) {
    if (screen_a_old != (u_char *)0x0) {
      free(screen_a_old);
      return;
    }
    errline = 0x4c;
    errfile = "error.h";
    int_error((u_char *)"mem_free(NULL)");
    return;
  }
  return;
}


void shut_down(void)

{
  char *__ptr;
  long lVar1;
  int iVar2;
  msgline_type *pmVar3;
  
  c_shutdown();
  free_sprites(0);
  free_area();
  if (hero != (it *)0x0) {
    delete_obj((ulong)(*(uint *)&hero->type >> 8));
  }
  while (objects.next != (object_list *)0x0) {
    delete_obj((ulong)(*(uint *)&((objects.next)->member).type >> 8));
  }
  lVar1 = (long)last_message;
  iVar2 = 0;
  pmVar3 = msg_line;
  if (-1 < last_message) {
    do {
      __ptr = pmVar3->msg;
      if (__ptr != (char *)0xffffffffffffffff) {
        if (__ptr == (char *)0x0) {
          errfile = "error.h";
          errline = 0x4c;
          int_error((u_char *)"mem_free(NULL)");
          lVar1 = (long)last_message;
        }
        else {
          free(__ptr);
          lVar1 = (long)last_message;
        }
      }
      iVar2 = iVar2 + 1;
      pmVar3 = pmVar3 + 1;
    } while (iVar2 <= (int)lVar1);
  }
  msg_line[lVar1].msg = (char *)0x0;
  shutdown_sprites();
  check_memory_leaks();
  return;
}


void shutdown_sprites(void)

{
  shutdown_blit();
  return;
}


void signal_handler(int signum)

{
  if (connected != 0) {
    send_quit();
  }
  shut_down();
  psignal(signum,"Exiting on signal");
                    /* WARNING: Subroutine does not return */
  exit(1);
}


void sigwinch_handler(int signum)

{
  undefined4 in_register_0000003c;
  
  resize_screen(CONCAT44(in_register_0000003c,signum));
  c_cls();
  c_refresh();
  signal(0x1c,sigwinch_handler);
  return;
}


void _skip_ws(char **txt)

{
  char cVar1;
  char *pcVar2;
  
  cVar1 = **txt;
  if (((byte)(cVar1 - 9U) < 2) || (cVar1 == ' ')) {
    pcVar2 = *txt + 1;
    do {
      do {
        *txt = pcVar2;
        cVar1 = *pcVar2;
        pcVar2 = pcVar2 + 1;
      } while ((byte)(cVar1 - 9U) < 2);
    } while (cVar1 == ' ');
  }
  return;
}


void sleep_until(ulonglong t)

{
  long lVar1;
  ulong uVar2;
  long in_FS_OFFSET;
  timezone tz;
  timeval tv;
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  tz = (timezone)0x0;
  gettimeofday((timeval *)&tv,(__timezone_ptr_t)&tz);
  uVar2 = tv.tv_sec * 1000000 + tv.tv_usec;
  if (uVar2 < t) {
    tv.tv_usec = t - uVar2;
    tv.tv_sec = 0;
    select(0,(fd_set *)0x0,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv);
  }
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void test_shift(int k)

{
  switch(k) {
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x28:
  case 0x29:
  case 0x2a:
  case 0x2b:
  case 0x3a:
  case 0x3c:
  case 0x3e:
  case 0x3f:
  case 0x40:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4a:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x4e:
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
  case 0x58:
  case 0x59:
  case 0x5a:
  case 0x5e:
  case 0x5f:
  case 0x7b:
  case 0x7c:
  case 0x7d:
  case 0x7e:
    shift_pressed = 1;
  }
  return;
}


void update_anim(it *obj)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  int iVar5;
  
  uVar1 = *(ushort *)&obj->field_0x1a;
  uVar3 = uVar1 >> 1 & 3;
  if ((uVar1 & 1) == 0) {
    if (uVar3 != 1) {
      if (uVar3 != 2) {
        if ((uVar1 >> 1 & 3) != 0) {
          return;
        }
        *(ushort *)&obj->field_0x18 = (-(ushort)((uVar1 & 0x100) == 0) & 0xffc1) + 0x48;
        return;
      }
      if ((uVar1 & 0x10) != 0) {
        iVar5 = (int)*(short *)&obj->field_0x14;
        if ((uVar1 & 0x200) == 0) {
          *(ushort *)&obj->field_0x18 =
               (ushort)((int)(weapon[current_weapon].cadence + 3) < iVar5) * 9 + 0x26;
          goto LAB_00104a79;
        }
        if ((int)((weapon[4].cadence >> 1) + 7) < iVar5) {
          *(undefined2 *)&obj->field_0x18 = 0x4c;
          goto LAB_00104a79;
        }
        if ((int)((weapon[4].cadence >> 2) + 7) < iVar5) {
          *(undefined2 *)&obj->field_0x18 = 0x4d;
          goto LAB_00104a79;
        }
        if (7 < *(short *)&obj->field_0x14) {
          *(undefined2 *)&obj->field_0x18 = 0x4e;
          goto LAB_00104a79;
        }
      }
      *(undefined2 *)&obj->field_0x18 = 10;
LAB_00104a79:
      if ((uVar1 & 0x100) == 0) {
        return;
      }
      *(undefined2 *)&obj->field_0x18 = 0x40;
      return;
    }
    if ((uVar1 & 0x10) != 0) {
      iVar5 = (int)*(short *)&obj->field_0x14;
      if ((uVar1 & 0x200) == 0) {
        *(ushort *)&obj->field_0x18 =
             (ushort)((int)(weapon[current_weapon].cadence + 3) < iVar5) * 9 + 0x14;
        goto LAB_00104a98;
      }
      if ((int)((weapon[4].cadence >> 1) + 7) < iVar5) {
        *(undefined2 *)&obj->field_0x18 = 0x49;
        goto LAB_00104a98;
      }
      if ((int)((weapon[4].cadence >> 2) + 7) < iVar5) {
        *(undefined2 *)&obj->field_0x18 = 0x4a;
        goto LAB_00104a98;
      }
      if (7 < *(short *)&obj->field_0x14) {
        *(undefined2 *)&obj->field_0x18 = 0x4b;
        goto LAB_00104a98;
      }
    }
    *(undefined2 *)&obj->field_0x18 = 0;
LAB_00104a98:
    if ((uVar1 & 0x100) == 0) {
      return;
    }
    *(undefined2 *)&obj->field_0x18 = 0x38;
    return;
  }
  if (uVar3 == 1) {
    uVar2 = *(ushort *)&obj->field_0x18;
    uVar3 = uVar2;
    if (8 < uVar2) {
      if (uVar2 < 0x1d) {
        uVar3 = uVar2 - 0x14;
      }
      else {
        uVar3 = uVar2 - 0x38;
        if (uVar2 < 0x26) {
          uVar3 = uVar2 - 0x1d;
        }
      }
    }
    if ((uVar1 & 0x100) != 0) {
      sVar4 = (uVar3 + 1 & 7) + 0x38;
      goto LAB_00104946;
    }
    sVar4 = 0;
    if ((uVar1 & 0x10) != 0) {
      sVar4 = (ushort)((int)(weapon[current_weapon].cadence + 3) < (int)*(short *)&obj->field_0x14)
              * 9 + 0x14;
    }
  }
  else {
    if (uVar3 != 2) {
      return;
    }
    uVar3 = *(ushort *)&obj->field_0x18;
    sVar4 = 10;
    if (((0x12 < uVar3) && (sVar4 = 0x26, 0x2e < uVar3)) && (sVar4 = 0x2f, 0x37 < uVar3)) {
      sVar4 = 0;
    }
    uVar3 = uVar3 - sVar4;
    if ((uVar1 & 0x100) != 0) {
      sVar4 = (uVar3 + 1 & 7) + 0x40;
      goto LAB_00104946;
    }
    sVar4 = 10;
    if ((uVar1 & 0x10) != 0) {
      sVar4 = (ushort)((int)(weapon[current_weapon].cadence + 3) < (int)*(short *)&obj->field_0x14)
              * 9 + 0x26;
    }
  }
  sVar4 = sVar4 + 1 + (uVar3 & 7);
LAB_00104946:
  *(short *)&obj->field_0x18 = sVar4;
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void update_game(void)

{
  ushort *puVar1;
  byte bVar2;
  int iVar3;
  undefined8 uVar4;
  char cVar5;
  object_list *poVar6;
  ulonglong uVar7;
  ulong uVar8;
  short sVar9;
  u_char uVar10;
  object_list *poVar11;
  object_list *poVar12;
  ushort uVar13;
  it *obj;
  long lVar14;
  int iVar15;
  int iVar16;
  long lVar17;
  long in_FS_OFFSET;
  my_double x1;
  u_char stop_x;
  u_char stop_y;
  u_char sy;
  int w;
  int h;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  if (objects.next != (object_list *)0x0) {
    poVar6 = objects.next;
    poVar11 = &objects;
    do {
      bVar2 = (poVar6->member).type;
      uVar8 = (ulong)bVar2;
      poVar12 = poVar6;
      if ((bVar2 == 0xc) ||
         (uVar13 = *(ushort *)&(poVar6->member).field_0x1a, (uVar13 & 0x400) != 0))
      goto LAB_0010465c;
      sVar9 = *(short *)&(poVar6->member).field_0x14;
      if ((sVar9 < 1) ||
         (sVar9 = sVar9 + -1, *(short *)&(poVar6->member).field_0x14 = sVar9, sVar9 != 0)) {
LAB_001044a0:
        if ((obj_attr[uVar8].maintainer & 1) == 0) goto LAB_0010465c;
        if ((uVar13 & 8) == 0) {
          (poVar6->member).xspeed =
               (my_double)((long)obj_attr[uVar8].slow_down_x * (long)(poVar6->member).xspeed >> 10);
        }
        if (obj_attr[uVar8].fall != '\0') {
          uVar13 = uVar13 | 8;
          *(ushort *)&(poVar6->member).field_0x1a = uVar13;
          iVar15 = (poVar6->member).yspeed + 0x2880;
          if (0x2d000 < iVar15) {
            iVar15 = 0x2d000;
          }
          (poVar6->member).yspeed = iVar15;
        }
        get_dimensions((uint)bVar2,(uint)uVar13,
                       sprites[*(ushort *)&(poVar6->member).field_0x16].positions,&w,&h);
        iVar15 = (poVar11->next->member).x;
        iVar16 = (poVar11->next->member).y;
        uVar7 = get_time();
        poVar6 = poVar11->next;
        lVar17 = (long)(int)(((double)(uVar7 - (poVar6->member).last_updated) / ram0x00112830) *
                            _DAT_00112838);
        update_position(&poVar6->member,
                        (int)((poVar6->member).xspeed * lVar17 >> 10) + (poVar6->member).x,
                        (int)((poVar6->member).yspeed * lVar17 >> 10) + (poVar6->member).y,w,h,
                        &stop_x,&stop_y);
        poVar12 = poVar11->next;
        (poVar12->member).last_updated = uVar7;
        if (stop_x == '\0') {
          iVar15 = (poVar12->member).xspeed;
LAB_001046a1:
          uVar8 = (ulong)(poVar12->member).type;
          uVar10 = '\0';
        }
        else {
          bVar2 = (poVar12->member).type;
          uVar8 = (ulong)bVar2;
          lVar14 = (long)(poVar12->member).xspeed;
          if ((bVar2 == 0) && ((*(byte *)&(poVar12->member).field_0x1b & 1) == 0)) {
            iVar3 = (poVar12->member).x;
            obj = &poVar12->member;
            iVar16 = iVar16 + -0x400;
            uVar4 = *(undefined8 *)obj;
            (poVar12->member).y = iVar16;
            obj->x = iVar15;
            update_position(obj,iVar15 + (int)(lVar17 * lVar14 >> 10),
                            (int)((poVar12->member).yspeed * lVar17 >> 10) + iVar16,w,h,(u_char *)0x0
                            ,&sy);
            poVar12 = poVar11->next;
            iVar15 = (poVar12->member).xspeed;
            if (iVar15 < 1) {
              if ((iVar15 != 0) && (iVar3 <= (poVar12->member).x)) goto LAB_001047a3;
            }
            else {
              if ((poVar12->member).x == iVar3 || (poVar12->member).x < iVar3) {
LAB_001047a3:
                *(undefined8 *)&poVar12->member = uVar4;
                uVar8 = (ulong)(poVar12->member).type;
                uVar10 = stop_x;
                if (stop_x != '\0') {
                  lVar14 = (long)iVar15;
                  goto LAB_001045ce;
                }
                goto LAB_001045e9;
              }
            }
            stop_y = sy;
            stop_x = '\0';
            goto LAB_001046a1;
          }
LAB_001045ce:
          iVar15 = -(int)(obj_attr[uVar8].bounce_x * lVar14 >> 10);
          (poVar12->member).xspeed = iVar15;
          uVar10 = stop_x;
        }
LAB_001045e9:
        if (iVar15 + 0x1ccbU < 0x3997) {
          puVar1 = (ushort *)&(poVar12->member).field_0x1a;
          *puVar1 = *puVar1 & 0xfffe;
          (poVar12->member).xspeed = 0;
        }
        cVar5 = (char)uVar8;
        if (stop_y == '\0') {
          if ((cVar5 == '\x01' || cVar5 == '\x10') && (uVar10 != '\0')) goto LAB_001046c0;
        }
        else {
          iVar15 = (int)((long)obj_attr[uVar8].bounce_y * (long)(poVar12->member).yspeed >> 10);
          if (0x1ccbU - iVar15 < 0x3997) {
            (poVar12->member).yspeed = 0;
            if (stop_y == '\x01') {
              puVar1 = (ushort *)&(poVar12->member).field_0x1a;
              *puVar1 = *puVar1 & 0xfff7;
            }
          }
          else {
            (poVar12->member).yspeed = -iVar15;
          }
          if (cVar5 == '\x01' || cVar5 == '\x10') goto LAB_001046c0;
        }
      }
      else {
        if (bVar2 == 0) {
          uVar13 = uVar13 & 0xffef;
          *(ushort *)&(poVar6->member).field_0x1a = uVar13;
          goto LAB_001044a0;
        }
        if (bVar2 == 0xe) goto LAB_001044a0;
LAB_001046c0:
        poVar6 = poVar11->prev;
        delete_obj((ulong)(*(uint *)&(poVar6->next->next->member).type >> 8));
        poVar12 = poVar6->next;
      }
LAB_0010465c:
      poVar6 = poVar12->next;
      poVar11 = poVar12;
    } while (poVar6 != (object_list *)0x0);
  }
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


void update_messages(ulonglong time)

{
  int iVar1;
  int iVar2;
  long lVar3;
  
  if (last_message < 0) {
    return;
  }
  lVar3 = 0;
  iVar2 = 0;
  iVar1 = 0;
  do {
    if (time <= msg_line[lVar3].time && msg_line[lVar3].time != time) {
      do {
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + 1;
        if (last_message < iVar1) {
          return;
        }
        lVar3 = (long)iVar2;
      } while (time <= msg_line[lVar3].time && msg_line[lVar3].time != time);
    }
    scroll_messages();
    iVar1 = iVar1 + 1;
  } while (iVar1 <= last_message);
  return;
}


void update_position(it *obj,my_double new_x,my_double new_y,int width,int height,u_char *fx,
                    u_char *fy)

{
  my_double mVar1;
  int iVar2;
  int iVar3;
  byte down_ladder;
  int iVar4;
  
  down_ladder = 0;
  if (obj->type == '\0') {
    down_ladder = (byte)((ushort)*(undefined2 *)&obj->field_0x1a >> 0xb) & 1;
  }
  iVar3 = obj->y >> 10;
  iVar2 = height + -1 + (obj->y + 0x3ff >> 10);
  if (obj->xspeed < 1) {
    iVar2 = can_go_x(obj->x,new_x,iVar3,iVar2,fx);
    obj->x = iVar2;
  }
  else {
    iVar4 = (width + -1) * 0x400;
    mVar1 = can_go_x(obj->x + iVar4,new_x + iVar4,iVar3,iVar2,fx);
    iVar2 = mVar1 + (width + -1) * -0x400;
    obj->x = iVar2;
  }
  iVar3 = width + -1 + (iVar2 + 0x3ff >> 10);
  if (0 < obj->yspeed) {
    iVar4 = (height + -1) * 0x400;
    mVar1 = can_go_y(obj->y + iVar4,iVar4 + new_y,iVar2 >> 10,iVar3,fy,down_ladder);
    obj->y = mVar1 + (height + -1) * -0x400;
    return;
  }
  mVar1 = can_go_y(obj->y,new_y,iVar2 >> 10,iVar3,fy,down_ladder);
  obj->y = mVar1;
  return;
}


void wait_for_enter(void)

{
  int iVar1;
  
  c_update_kbd();
  while( true ) {
    iVar1 = c_was_pressed(0x12f);
    if (iVar1 != 0) break;
    c_wait_for_key();
    c_update_kbd();
  }
  return;
}

ulong crc32(u_char *buf,uint len)

{
  byte *pbVar1;
  ulong uVar2;
  byte *pbVar3;
  
  uVar2 = 0xffffffff;
  if (7 < len) {
    pbVar1 = buf + (ulong)(len - 8 >> 3) * 8 + 8;
    pbVar3 = buf;
    do {
      buf = pbVar3 + 8;
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)*pbVar3 ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[1] ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[2] ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[3] ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[4] ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[5] ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[6] ^ (uint)uVar2) & 0xff];
      uVar2 = uVar2 >> 8 ^ crc_table[((uint)pbVar3[7] ^ (uint)uVar2) & 0xff];
      pbVar3 = buf;
    } while (buf != pbVar1);
    len = len & 7;
  }
  if ((((len != 0) && (uVar2 = uVar2 >> 8 ^ crc_table[((uint)*buf ^ (uint)uVar2) & 0xff], len != 1))
      && (uVar2 = uVar2 >> 8 ^ crc_table[((uint)buf[1] ^ (uint)uVar2) & 0xff], len != 2)) &&
     (((uVar2 = uVar2 >> 8 ^ crc_table[((uint)buf[2] ^ (uint)uVar2) & 0xff], len != 3 &&
       (uVar2 = uVar2 >> 8 ^ crc_table[((uint)buf[3] ^ (uint)uVar2) & 0xff], len != 4)) &&
      ((uVar2 = uVar2 >> 8 ^ crc_table[((uint)buf[4] ^ (uint)uVar2) & 0xff], len != 5 &&
       (uVar2 = uVar2 >> 8 ^ crc_table[((uint)buf[5] ^ (uint)uVar2) & 0xff], len != 6)))))) {
    uVar2 = uVar2 >> 8 ^ crc_table[((uint)buf[6] ^ (uint)uVar2) & 0xff];
  }
  return uVar2 ^ 0xffffffff;
}

int kbd_is_pressed(int key)

{
  if (keyboard_type == 0) {
    if (0x141 < key - 0x20U) {
      return (int)(uint)keyboard[keyboard_type];
    }
    return (int)(uint)keyboard[(char)CSWTCH.32[key - 0x20U]];
  }
  if (keyboard_type == 1) {
    if ((key != 0x135) && (key != 0x130)) {
      return (int)(uint)(current_key == key);
    }
    return shift_pressed;
  }
  return 0;
}


int kbd_update(void)

{
  byte bVar1;
  int iVar2;
  ssize_t sVar3;
  long lVar4;
  byte *pbVar5;
  fd_set *pfVar6;
  undefined8 *puVar7;
  int iVar8;
  long in_FS_OFFSET;
  byte bVar9;
  timeval tv;
  fd_set fds;
  u_char buffer [256];
  long local_20;
  
  iVar8 = keyboard_type;
  bVar9 = 0;
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  if (keyboard_type == 0) {
    old_keyboard._0_4_ = keyboard._0_4_;
    old_keyboard._4_4_ = keyboard._4_4_;
    old_keyboard._8_4_ = keyboard._8_4_;
    old_keyboard._12_4_ = keyboard._12_4_;
    old_keyboard._16_4_ = keyboard._16_4_;
    old_keyboard._20_4_ = keyboard._20_4_;
    old_keyboard._24_4_ = keyboard._24_4_;
    old_keyboard._28_4_ = keyboard._28_4_;
    old_keyboard._32_4_ = keyboard._32_4_;
    old_keyboard._36_4_ = keyboard._36_4_;
    old_keyboard._40_4_ = keyboard._40_4_;
    old_keyboard._44_4_ = keyboard._44_4_;
    old_keyboard._48_4_ = keyboard._48_4_;
    old_keyboard._52_4_ = keyboard._52_4_;
    old_keyboard._56_4_ = keyboard._56_4_;
    old_keyboard._60_4_ = keyboard._60_4_;
    old_keyboard._64_4_ = keyboard._64_4_;
    old_keyboard._68_4_ = keyboard._68_4_;
    old_keyboard._72_4_ = keyboard._72_4_;
    old_keyboard._76_4_ = keyboard._76_4_;
    old_keyboard._80_4_ = keyboard._80_4_;
    old_keyboard._84_4_ = keyboard._84_4_;
    old_keyboard._88_4_ = keyboard._88_4_;
    old_keyboard._92_4_ = keyboard._92_4_;
    old_keyboard._96_4_ = keyboard._96_4_;
    old_keyboard._100_4_ = keyboard._100_4_;
    old_keyboard._104_4_ = keyboard._104_4_;
    old_keyboard._108_4_ = keyboard._108_4_;
    old_keyboard._112_4_ = keyboard._112_4_;
    old_keyboard._116_4_ = keyboard._116_4_;
    old_keyboard._120_4_ = keyboard._120_4_;
    old_keyboard._124_4_ = keyboard._124_4_;
    sVar3 = read(0,buffer,0x100);
    if (0 < (int)sVar3) {
      while( true ) {
        pbVar5 = buffer;
        do {
          bVar1 = *pbVar5;
          pbVar5 = pbVar5 + 1;
          keyboard[bVar1 & 0x7f] = (byte)~bVar1 >> 7;
        } while (pbVar5 != buffer + (ulong)((int)sVar3 - 1) + 1);
        if ((int)sVar3 != 0x100) break;
        sVar3 = read(0,buffer,0x100);
        if ((int)sVar3 < 1) {
LAB_00110218:
          iVar8 = 1;
          goto LAB_001100fb;
        }
      }
      if (((byte)((byte)((uint)keyboard._28_4_ >> 8) | keyboard[97]) != 0) && (keyboard[46] != '\0')
         ) {
        raise(2);
      }
      if ((byte)((byte)keyboard._56_4_ | keyboard[100]) != 0) {
        if (keyboard[59] == '\0') {
          if (keyboard[60] == '\0') {
            if (keyboard[61] == '\0') {
              if (keyboard[62] == '\0') {
                if (keyboard[63] == '\0') {
                  if (keyboard[64] == '\0') {
                    if (keyboard[65] == '\0') {
                      if (keyboard[66] == '\0') {
                        if (keyboard[67] == '\0') {
                          iVar8 = 0x44;
                          if (keyboard[68] == '\0') goto LAB_001102e1;
                        }
                        else {
                          iVar8 = 0x43;
                        }
                      }
                      else {
                        iVar8 = 0x42;
                      }
                    }
                    else {
                      iVar8 = 0x41;
                    }
                  }
                  else {
                    iVar8 = 0x40;
                  }
                }
                else {
                  iVar8 = 0x3f;
                }
              }
              else {
                iVar8 = 0x3e;
              }
            }
            else {
              iVar8 = 0x3d;
            }
          }
          else {
            iVar8 = 0x3c;
          }
        }
        else {
          iVar8 = 0x3b;
        }
        lVar4 = 0x10;
        puVar7 = (undefined8 *)keyboard;
        while (lVar4 != 0) {
          lVar4 = lVar4 + -1;
          *puVar7 = 0;
          puVar7 = puVar7 + (ulong)bVar9 * -2 + 1;
        }
        lVar4 = 0x10;
        puVar7 = (undefined8 *)old_keyboard;
        while (lVar4 != 0) {
          lVar4 = lVar4 + -1;
          *puVar7 = 0;
          puVar7 = puVar7 + (ulong)bVar9 * -2 + 1;
        }
        ioctl(0,0x5606,(ulong)(iVar8 - 0x3a));
LAB_001102e1:
        if (keyboard[87] == '\0') {
          if (keyboard[88] != '\0') {
            lVar4 = 0x10;
            puVar7 = (undefined8 *)keyboard;
            while (lVar4 != 0) {
              lVar4 = lVar4 + -1;
              *puVar7 = 0;
              puVar7 = puVar7 + (ulong)bVar9 * -2 + 1;
            }
            lVar4 = 0x10;
            iVar8 = 1;
            puVar7 = (undefined8 *)old_keyboard;
            while (lVar4 != 0) {
              lVar4 = lVar4 + -1;
              *puVar7 = 0;
              puVar7 = puVar7 + (ulong)bVar9 * -2 + 1;
            }
            ioctl(0,0x5606,0xc);
            goto LAB_001100fb;
          }
        }
        else {
          lVar4 = 0x10;
          puVar7 = (undefined8 *)keyboard;
          while (lVar4 != 0) {
            lVar4 = lVar4 + -1;
            *puVar7 = 0;
            puVar7 = puVar7 + (ulong)bVar9 * -2 + 1;
          }
          lVar4 = 0x10;
          puVar7 = (undefined8 *)old_keyboard;
          while (lVar4 != 0) {
            lVar4 = lVar4 + -1;
            *puVar7 = 0;
            puVar7 = puVar7 + (ulong)bVar9 * -2 + 1;
          }
          ioctl(0,0x5606,0xb);
        }
      }
      goto LAB_00110218;
    }
  }
  else {
    if (keyboard_type == 1) {
      lVar4 = 0x10;
      current_key = 0;
      pfVar6 = (fd_set *)&fds;
      while (lVar4 != 0) {
        lVar4 = lVar4 + -1;
        pfVar6->fds_bits[0] = 0;
        pfVar6 = (fd_set *)(pfVar6->fds_bits + 1);
      }
      fds.__fds_bits[0] = fds.__fds_bits[0] | 1;
      tv = (timeval)0x0;
      iVar2 = select(1,(fd_set *)&fds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)&tv);
      if (iVar2 != 0) {
        current_key = getkey();
        goto LAB_001100fb;
      }
    }
  }
  iVar8 = 0;
LAB_001100fb:
  if (local_20 == *(long *)(in_FS_OFFSET + 0x28)) {
    return iVar8;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

void kbd_wait_for_key(void)

{
  long lVar1;
  fd_set *pfVar2;
  long in_FS_OFFSET;
  fd_set fds;
  long local_10;
  
  lVar1 = 0x10;
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  pfVar2 = &fds;
  while (lVar1 != 0) {
    lVar1 = lVar1 + -1;
    *(undefined8 *)pfVar2 = 0;
    pfVar2 = (fd_set *)((long)pfVar2 + 8);
  }
  fds.__fds_bits[0] = fds.__fds_bits[0] | 1;
  select(1,(fd_set *)&fds,(fd_set *)0x0,(fd_set *)0x0,(timeval *)0x0);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

int kbd_was_pressed(int key)

{
  uint uVar1;
  long lVar2;
  
  if (keyboard_type == 0) {
    lVar2 = 0;
    uVar1 = keyboard_type;
    if (key - 0x20U < 0x142) {
      lVar2 = (long)(char)CSWTCH.32[key - 0x20U];
      if (old_keyboard[lVar2] == '\0') goto LAB_001104e2;
    }
    else {
      if (old_keyboard[0] == '\0') {
LAB_001104e2:
        return (int)(uint)(keyboard[lVar2] != '\0');
      }
    }
  }
  else {
    if (keyboard_type != 1) {
      return 0;
    }
    if ((key == 0x135) || (key == 0x130)) {
      return shift_pressed;
    }
    uVar1 = (uint)(current_key == key);
  }
  return (int)uVar1;
}

void print_help_window(void)

{
  draw_frame(SCREEN_X + -0x2b >> 1,SCREEN_Y + -0x15 >> 1,0x29,0x13,'\x0e');
  print2screen((SCREEN_X + -0x2b >> 1) + 0x12,(SCREEN_Y + -0x15 >> 1) + 1,'\v',"HELP");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 3,'\n',"LEFT,RIGHT");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 3,'\a',"move");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 4,'\n',"UP");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 4,'\a',"jump");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 5,'\n',"SHIFT+DIRECTION");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 5,'\a',"run");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 6,'\n',"CAPSLOCK,A");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 6,'\a',"toggle autorun");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 7,'\n',"DOWN+DIRECTION");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 7,'\a',"creep");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 8,'\n',"C");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 8,'\a',"toggle creep");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 9,'\n',"Z,CTRL");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 9,'\a',"fire");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 10,'\n',"1,2,3,4,5");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 10,'\a',"change weapon");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0xb,'\n',"ENTER");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0xb,'\a',"chat");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0xc,'\n',"F10,Q");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0xc,'\a',"quit");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0xd,'\n',"F12");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0xd,'\a',"abort game");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0xe,'\n',"R");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0xe,'\a',"redraw screen");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0xf,'\n',"H");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0xf,'\a',"toggle help");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0x10,'\n',"TAB");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0x10,'\a',
               "toggle top players");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0x11,'\n',"SPACE");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0x11,'\a',
               "respawn dead player");
  print2screen((SCREEN_X + -0x2b >> 1) + 3,(SCREEN_Y + -0x15 >> 1) + 0x12,'\n',"D");
  print2screen((SCREEN_X + -0x2b >> 1) + 0x15,(SCREEN_Y + -0x15 >> 1) + 0x12,'\a',
               "climb down ladders");
  return;
}

int recv_packet(u_char *packet,int max_len,sockaddr *addr,int *addr_len,int sender_server,
               int recipient,int *sender)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  ssize_t sVar9;
  ulong uVar10;
  int iVar11;
  byte *__src;
  uint len;
  size_t __size;
  byte *__buf;
  
  __size = SEXT48(max_len + 0xc);
  if (__size == 0) {
    __src = (byte *)0xb;
    __buf = (byte *)0xffffffffffffffff;
  }
  else {
    __buf = (byte *)malloc(__size);
    if (__buf == (byte *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      return 0xffffffff;
    }
    __src = __buf + 0xc;
  }
  sVar9 = recvfrom(fd,__buf,__size,0,(sockaddr *)addr,(socklen_t *)addr_len);
  memcpy(packet,__src,(long)max_len);
  bVar1 = __buf[1];
  bVar2 = *__buf;
  bVar3 = __buf[2];
  bVar4 = __buf[3];
  iVar11 = (uint)__buf[7] * 0x1000000 +
           (uint)__buf[4] + (uint)__buf[5] * 0x100 + (uint)__buf[6] * 0x10000;
  if (sender != (int *)0x0) {
    *sender = iVar11;
  }
  bVar5 = __buf[9];
  bVar6 = __buf[8];
  bVar7 = __buf[10];
  bVar8 = __buf[0xb];
  if (__buf != (byte *)0xffffffffffffffff) {
    free(__buf);
  }
  if ((int)sVar9 != -1) {
    len = (int)sVar9 - 0xc;
    uVar10 = crc32(packet,len);
    if ((uVar10 == (long)(int)((uint)bVar4 * 0x1000000 +
                              (uint)bVar2 + (uint)bVar1 * 0x100 + (uint)bVar3 * 0x10000)) &&
       ((uint)bVar7 * 0x10000 + (uint)bVar6 + (uint)bVar5 * 0x100 + (uint)bVar8 * 0x1000000 ==
        recipient)) {
      if (sender_server == 0) {
        return len;
      }
      if (iVar11 == 0) {
        return len;
      }
    }
  }
  return (int)0xffffffff;
}


int remap_in(int k)

{
  int iVar1;
  
  iVar1 = 0;
  if (k - 0x20U < 0x142) {
    iVar1 = (int)(char)CSWTCH.32[k - 0x20U];
  }
  return iVar1;
}


int remap_out(int k)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  long in_FS_OFFSET;
  int remap_table [128];
  long local_10;
  
  lVar1 = 0x40;
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  puVar2 = &DAT_00112f40;
  puVar3 = (undefined8 *)remap_table;
  while (lVar1 != 0) {
    lVar1 = lVar1 + -1;
    *puVar3 = *puVar2;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
  }
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return remap_table[k];
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


void send_packet(u_char *packet,int len,sockaddr *addr,int sender,int recipient)

{
  int __fd;
  ulong uVar1;
  undefined4 *__buf;
  undefined4 *__dest;
  size_t __size;
  
  uVar1 = crc32(packet,len);
  __size = SEXT48(len + 0xc);
  if (__size == 0) {
    __dest = (undefined4 *)0xb;
    __buf = (undefined4 *)0xffffffffffffffff;
  }
  else {
    __buf = (undefined4 *)malloc(__size);
    if (__buf == (undefined4 *)0x0) {
      error((u_char *)"ERROR: out of memory (malloc returned NULL)\n");
      return;
    }
    __dest = __buf + 3;
  }
  memcpy(__dest,packet,(long)len);
  *__buf = (int)uVar1;
  __fd = fd;
  __buf[1] = sender;
  __buf[2] = recipient;
  sendto(__fd,__buf,__size,0,(sockaddr *)addr,0x10);
  if (__buf != (undefined4 *)0xffffffffffffffff) {
    free(__buf);
    return;
  }
  return;
}

/* WARNING: Could not reconcile some variable overlaps */

int main(int argc,char **argv)

{
  object_list *poVar1;
  char *__ptr;
  int iVar2;
  int iVar3;
  object_list *poVar4;
  long lVar5;
  uint uVar6;
  msgline_type *pmVar7;
  int *num;
  ushort port;
  int color;
  char name [25];
  char host [65];
  u_char txt [256];
  
  host._0_16_ = (undefined  [16])0x0;
  host._16_16_ = (undefined  [16])0x0;
  host._32_16_ = (undefined  [16])0x0;
  host._48_16_ = (undefined  [16])0x0;
  name._0_16_ = (undefined  [16])0x0;
  port = 0x1a0a;
  color = 1;
  last_message = -1;
  autorun = '\0';
  autocreep = '\0';
  host[64] = '\0';
  name._16_8_ = 0;
  name[24] = '\0';
  load_cfg(host,name,&color);
  last_obj = &objects;
  error_message[0] = '\0';
  parse_command_line(argc,argv);
  hash_table_init();
  if (set_size == '\0') {
    c_get_size(&SCREEN_X,&SCREEN_Y);
  }
  init_sprites();
  init_area();
  load_sprites((u_char *)"data/sprites.dat");
  level_sprites_start = n_sprites;
  iVar2 = find_sprite("hit",&hit_sprite);
  if (iVar2 != 0) {
    fwrite("Error: Can\'t find sprite \"hit\".\n",1,0x20,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  iVar2 = find_sprite((u_char *)"title",&title_sprite);
  if (iVar2 != 0) {
    fwrite("Error: Can\'t find sprite \"title\".\n",1,0x22,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  iVar2 = find_sprite((u_char *)"bulge",&bulge_sprite);
  num = shrapnel_sprite;
  if (iVar2 == 0) {
    do {
      iVar2 = iVar2 + 1;
      __sprintf_chk(txt,1,0x100,"shrapnel%d",iVar2);
      iVar3 = find_sprite(txt,num);
      if (iVar3 != 0) {
        __fprintf_chk(stderr,1,"Can\'t find sprite \"%s\".\n",txt);
                    /* WARNING: Subroutine does not return */
        exit(1);
      }
      num = num + 1;
    } while (iVar2 != 10);
    signal(2,signal_handler);
    signal(0xf,signal_handler);
    signal(4,signal_handler);
    signal(6,signal_handler);
    signal(8,signal_handler);
    signal(0xb,signal_handler);
    signal(3,signal_handler);
    signal(7,signal_handler);
    signal(0x1c,sigwinch_handler);
    c_init(SCREEN_X,SCREEN_Y);
    c_cls();
    c_cursor(1);
    c_refresh();
    do {
      level_number = -1;
      connected = 0;
      menu_screen(host,name,&port,&color);
      connected = 1;
      play();
      poVar1 = &objects;
      while (poVar4 = poVar1, poVar1 = poVar4->next, poVar1 != (object_list *)0x0) {
        uVar6 = *(uint *)&(poVar1->member).type >> 8;
        if (*(uint *)&hero->type >> 8 != uVar6) {
          delete_obj((ulong)uVar6);
          poVar1 = poVar4;
        }
      }
      lVar5 = (long)last_message;
      if (-1 < last_message) {
        pmVar7 = msg_line;
        iVar2 = 0;
        do {
          __ptr = pmVar7->msg;
          if (__ptr != (char *)0xffffffffffffffff) {
            if (__ptr == (char *)0x0) {
              errline = 0x4c;
              errfile = "error.h";
              int_error((u_char *)"mem_free(NULL)");
              lVar5 = (long)last_message;
            }
            else {
              free(__ptr);
              lVar5 = (long)last_message;
            }
          }
          iVar2 = iVar2 + 1;
          pmVar7 = pmVar7 + 1;
        } while (iVar2 <= (int)lVar5);
      }
      uVar6 = *(uint *)&hero->type;
      last_message = -1;
      msg_line[lVar5].msg = (char *)0x0;
      delete_obj((ulong)(uVar6 >> 8));
    } while( true );
  }
  fwrite("Error: Can\'t find sprite \"bulge\".\n",1,0x22,stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}

