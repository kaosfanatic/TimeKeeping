#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;


string weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class TimeRecord{
  
  public:
  
  TimeRecord(){
    
  }
  
  void setStart(time_t mtime){start = mtime;}
  void setEnd(time_t mtime){
    end = mtime;
    duration = end - start;
  }
  time_t getDuration(){ return duration; }
  
  
  private:
  
  time_t start;
  time_t end;
  time_t duration;
  time_t dayOfWeek;
  time_t year;
  

  
};




int main(int argc, char *argv[]){
  
  TimeRecord *record = new TimeRecord();
  record->setStart(time(0));
  time_t now = time(0);
  
  tm *ltm = localtime(&now);
  
  //cout << "Year: "<< 1900 + ltm->tm_year<<endl;
  //cout << "Day of Week: "<< weekday[ltm->tm_wday]<<endl;
  
  //cout<<argc<<"  "<<argv[1]<<endl;
  
  string buff;
  
  getline(cin, buff);
  
  record->setEnd(time(0));
  
  fstream timeLogs;
  
  timeLogs.open("logs.txt");
  
  
  
  
  timeLogs << "Duration: "<<record->getDuration();
  
  
  timeLogs.close();
  return 0;
}