#include <stdio.h>
#include <termios.h>

#define DB 0

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

      char c, dc;
      while((c = fgetc(stdin)) != EOF){
            if(DB)fputc(c, stdout);
            else if(decoy_fp){
                  if((dc = fgetc(decoy_fp)) == EOF)fseek(decoy_fp, 0L, SEEK_SET);
                  fputc(dc, stdout);
            }
            fputc(c, out_fp);
            if(c == 'q')break;
      }
      fputc(0, out_fp);
      fputc('\n', out_fp);

      /* exit raw mode */
      tcsetattr(0, TCSANOW, &def);

      fclose(out_fp);
}
