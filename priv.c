#include <stdio.h>
#include <termios.h>

#define DB 1

int main(int a, char** b){
      if(a < 2)return 0;

      struct termios def, raw;
      tcgetattr(0, &raw);
      tcgetattr(0, &def);
      cfmakeraw(&raw);

      /* enter raw terminal mode */
      tcsetattr(0, TCSANOW, &raw);

      FILE* fp = fopen(b[1], "a");

      char c;
      while((c = fgetc(stdin)) != EOF){
            if(DB)fputc(c, stdout);
            fputc(c, fp);
            if(c == 'q')break;
      }

      /* exit raw mode */
      tcsetattr(0, TCSANOW, &def);

      fclose(fp);
}
