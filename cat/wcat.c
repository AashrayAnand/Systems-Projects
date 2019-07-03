#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int i;
    FILE* fp;
    char ch;
    for(i = 1; i < argc; i++){
      fp = fopen(argv[i], "r");
      // case of issue opening file
      if(fp == NULL){
        printf("wcat: cannot open file\n");
        return 1;
      }
      // case that file has been opened, iterate
      // through file and output contents
      while((ch = fgetc(fp)) != EOF){
        printf("%c", ch);
      }

      fclose(fp);
    }

    return 0;
}
