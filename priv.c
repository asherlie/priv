/* TODO: this should be a curses program */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <sys/ioctl.h>

#define DB 0

void clear_screen(){
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      for(int i = 0; i < w.ws_row; ++i)puts("");
}

int main(int a, char** b){
      if(a < 2)return 0;

      struct termios def, raw;
      tcgetattr(0, &raw);
      tcgetattr(0, &def);
      cfmakeraw(&raw);

      /* enter raw terminal mode */
      tcsetattr(0, TCSANOW, &raw);

      FILE* out_fp, * decoy_fp;

      /* TODO: use all args after first as input for decoy */
      /* if only one is give, it repeats after running out of chars */
      decoy_fp = (a > 2) ? fopen(b[2], "r") : NULL;

      out_fp = fopen(b[1], "a");

      /* we keep track of this for when we need to switch back to decoy mode
       * after printing written text
       */
      int n_decoy = 0, out_sz = 0, out_cap = 50;
      char c, dc, * out_txt = calloc(out_cap, 1);


      while((c = fgetc(stdin)) != EOF){
            /*printf("%i\n", c);*/

            if(c == 'D'){
                  clear_screen();
            }

            if(c == 3 || c == 4)break;
            if(DB)fputc(c, stdout);
            else if(decoy_fp){
                  if((dc = fgetc(decoy_fp)) == EOF){
                        ++n_decoy;
                        fseek(decoy_fp, 0L, SEEK_SET);
                  }
                  if(dc == '\n')fputc('\r', stdout);
                  fputc(dc, stdout);
            }
            /* backspace */
            if(c == 127)fprintf(out_fp, "<DEL>");
            fputc(c, out_fp);
      }
      /*fputc(0, out_fp);*/
      fputc('\n', out_fp);

      /* exit raw mode */
      tcsetattr(0, TCSANOW, &def);

      fclose(out_fp);
}
