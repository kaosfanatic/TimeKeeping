#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;


string weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class TimeRecord{
  
  public:
  
 
  
  void setStart(int sTime){start = sTime;}
  void setEnd(int eTime){
    end = eTime;
    duration = end - start;
  }
  int getDuration(){ return duration; }
  
  
  private:
  
  int start;
  int end;
  unsigned int duration;
  unsigned short int dayOfWeek;
  unsigned short int year;
  

  
};

void checkInput(string &input, bool * running, bool * end ){
  
  stringstream convert(input);
  int result;
  
  if( !(convert >> result) )
    result = 0;
  
  //cout<<result<<endl;
  
  if(result == 1){
    *running = true;
    //Initialize record, store starting time.
    return;
  }
  
  if(result == 2){
    *end = true;
    return;
  }
  
}


int main(int argc, char *argv[]){
  
  string choice;
  TimeRecord *record = new TimeRecord();
  record->setStart((int) time(0));
  bool end = false;
  bool running = false;
 
  //cout << "Year: "<< 1900 + ltm->tm_year<<endl;
  //cout << "Day of Week: "<< weekday[ltm->tm_wday]<<endl;
  
  //cout<<argc<<"  "<<argv[1]<<endl;
  
  
  while(!end){
    if(!running){
      cout<<"1. Start\n2. Quit\n\nEntry: ";
      getline(cin, choice);
      checkInput(choice, &running, &end);
    }
    else{
      cout<<"Press enter to stop... ";
      getline(cin, choice);
      end = true;
    }
  }
  
  /*record->setEnd(time(0));
  
  fstream timeLogs;
  
  timeLogs.open("logs.txt");
  
  
  
  timeLogs << "Duration: "<<record->getDuration();
  
  
  timeLogs.close();*/
  return 0;
}