#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define lli long long int
#define False 0
#define True 1
#define logLocation "/var/log/netlog"
/*
* Takes log file at logLocation formatted as
* timestamp,uptime,upbytes,downbytes
* sums the data and displays in human readable format.
* If called with an arg, it prints a block suitable for i3bar
* Author: Juf
*/

//Set up vars
lli timestamp = 0;
double uptime = 0.0;
lli upTotal = 0;
lli downTotal = 0;
int sessions = 0;
int invalidLines = 0;
time_t beginningDate = 0;

char * calculateSize(uint64_t size);

void printAll();

int main ( int argc, char *argv[] ) {

  //Open file, if failed, exit status 1
  FILE* file = fopen(logLocation,"r");

  if(!file) {
    fprintf(stderr, "Couldn't open netlog\n");
    exit(1);
  }
  
  // Temp variables to store data while looping through file.
  double uptimeTmp = 0.0;
  lli upTmp = 0;
  lli downTmp = 0;

  //Loop through file until end and add up data
  while(!feof(file)){
    //Only sum up data is it follows the aforementioned format.
    if(fscanf(file, "%lli,%lf,%lli,%lli", &timestamp, &uptimeTmp, &upTmp, &downTmp) == 4){
      if(beginningDate == 0) beginningDate = (time_t) timestamp;
      uptime += uptimeTmp;
      upTotal += upTmp;
      downTotal += downTmp;
      sessions++;
    }else{
      // If we're not at the EOF return an error stating line number
      // increment error counter and move cursor to next line.
      if(feof(file)) break;
      invalidLines++;
      fprintf(stderr, "Log file contains invalid line at line number: %d\n", invalidLines + sessions);
      int tmp;
      while(tmp = fgetc(file) != '\n' && !feof(file));
    }
  }

  fclose(file);

  if(argc == 1){
    printAll();
  }else{
    printf("%s ⇅ %s", calculateSize(upTotal), calculateSize(downTotal));
  }

  return 0;
}

void printAll(){
  printf("Log Started at: %s", ctime(&beginningDate));
  //Print Up/Down
  printf("Usage: %s ⇅ %s\nOver %d sessions, ", \
  calculateSize(upTotal), calculateSize(downTotal), sessions);

  //Calculate and print human readable uptime (apporx, doesn't account for leap years ect.)
  double minutes = uptime / 60;
  double hours = minutes / 60;
  double days = hours / 24;
  double weeks = days / 7;
  double months = weeks / ((365/12)/7);
  double years = months / 12;
  int notFirst = False;

  if(years >= 1){
    printf("%d years, ", abs(years));
    notFirst = True;
  }
  if(notFirst) months = (int)months % 12;
  if(months >= 1){
    printf("%d months, ", abs(months));
    notFirst = True;
  }
  if(notFirst) weeks = (int)weeks % ((365/12)/7);
  if(weeks >= 1){
    printf("%d weeks, ", abs(weeks));
    notFirst = True;
  }
  if(notFirst) days = (int)days % 7;
  if(days >= 1){
    printf("%d days, ", abs(days));
    notFirst = True;
  }
  if(notFirst) hours = (int)hours % 24;
  if(hours >= 1){
    printf("%d hours, ", abs(hours));
    notFirst = True;
  }
  if(notFirst) minutes = (int)minutes % 60;
  if(minutes >= 1){
    printf("%d minutes, ", abs(minutes));
    notFirst = True;
  }
  if(notFirst) uptime = (int)uptime %60;
  if(uptime >= 1){
    printf("%d seconds.", abs(uptime));
  }
  printf("\n");
}

// Byte to human readable format by http://stackoverflow.com/users/15168/jonathan-leffler
// http://stackoverflow.com/questions/3898840/converting-a-number-of-bytes-into-a-file-size-in-c/8104240
#define DIM(x) (sizeof(x)/sizeof(*(x)))

static const char     *sizes[]   = { "EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B" };
static const uint64_t  exbibytes = 1024ULL * 1024ULL * 1024ULL *
                                   1024ULL * 1024ULL * 1024ULL;

char *
calculateSize(uint64_t size)
{
    char     *result = (char *) malloc(sizeof(char) * 20);
    uint64_t  multiplier = exbibytes;
    int i;

    for (i = 0; i < DIM(sizes); i++, multiplier /= 1024)
    {
        if (size < multiplier)
            continue;
        if (size % multiplier == 0)
            sprintf(result, "%" PRIu64 " %s", size / multiplier, sizes[i]);
        else
            sprintf(result, "%.1f %s", (float) size / multiplier, sizes[i]);
        return result;
    }
    strcpy(result, "0");
    return result;
}
