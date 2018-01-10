#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;


string weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class TimeRecord{
  
  public:
  
  TimeRecord(){
    start = 0;
    end = 0;
    duration = -1;
    dayOfWeek = 0;
    year = 1900;
  }
  
  void setStart(int sTime){start = sTime;}
  void setEnd(int eTime){
    end = eTime;
    duration = end - start;
  }
  int getDuration(){ return duration; }
  
  
  private:
  
  int start;
  int end;
  int duration;
  unsigned short int dayOfWeek;
  unsigned short int year;
  unsigned short int month;
  

  
};

int checkInput(string input, bool * valid ){
  
  stringstream convert(input);
  int result;
  
  if( !(convert >> result) )
    result = 0;
  
  //cout<<result<<endl;
  
  if(result == 1 || result == 2){
    *valid = true;
    return result;
  }
  else{
    cout<<"\nInvalid Input.  Try again.\n";
    return -1;
  }

}

void writeLog(TimeRecord record){
  
  ofstream logFile;
  
  logFile.open("logs.bin", ios::out | ios::app | ios::binary);
  
  if(logFile.is_open()){
    
    logFile.write((char*) &record, sizeof(record));
  }
  else
    cout<<"Error: File not opened.\n";
  
  
}


int main(int argc, char *argv[]){
  
  string choice;
  int convChoice;
  TimeRecord *record = new TimeRecord();
  record->setStart((int) time(0));
  bool valid = false;
 
  //cout << "Year: "<< 1900 + ltm->tm_year<<endl;
  //cout << "Day of Week: "<< weekday[ltm->tm_wday]<<endl;
  
  //cout<<argc<<"  "<<argv[1]<<endl;
  
  
  while(!valid){
    cout<<"1. Start\n2. Quit\n\nEntry: ";
    getline(cin, choice);
    convChoice = checkInput(choice, &valid);
  }
  
  switch(convChoice){
    
    case 1:
      record->setStart((int) time(0));
      break;
    case 2:
      delete record;
      break;
    
    
  }
  
  writeLog(*record);
  
  return 0;
}