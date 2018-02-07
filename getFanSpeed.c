#include <stdio.h>
// Reads the pwm state of my CPU fan and converts it to a
// percent of max speed. Prints to stdout suitable for i3bar.

int main ( void ) {


  //Open file, if it's open proceed, else exit status 1
  FILE* file = fopen("/sys/devices/platform/nct6775.656/hwmon/hwmon3/pwm2","r");

  if(!file){
    fprintf(stderr, "FileOpenError");
    return 1;
  }

  int pwmSpeed = 0;

  //Read current PWM state, if fails exit.
  if(fscanf(file, "%d", &pwmSpeed) != 1){
      fprintf(stderr, "ReadError");
      fclose(file);
      return 1;
  }

  fclose(file);

  //fanSpeed = Percent speed of the fan.
  int fanSpeed = (int) (((float)pwmSpeed/255) * 100);

  printf("%d%%", fanSpeed);

  return 0;
}
